/*
 * Copyright © 2011 Yury Korolev yury.king@gmail.com
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior written
 *    permission.
 *
 * The advertising clause requiring mention in adverts must never be included.
 */

/*! ---------------------------------------------------------------
 * $Id: LjHandler.cpp 53 2011-04-07 13:11:18Z kua $ 
 *
 * \file LjHandler.cpp
 * \brief CLjHandler implementation
 *
 * File description
 *
 * PROJ: OSLL/scblog
 * ---------------------------------------------------------------- */

#include <QDebug>
#include <QNetworkReply>
#include <QCryptographicHash>
#include <QNetworkCookieJar>
#include <QSharedPointer>
#include <QDateTime>
#include <QTime>
#include <QDate>
#include "LjHandler.h"
#include "RequestCreator.h"
#include "ResponseParser.h"

namespace BlogService
{
  CLjHandler::CLjHandler(QString serviceUrl, QString userName, QString password, QObject *parent) :
    QObject(parent), m_userName(userName), m_password(password)
  {
    m_networkManager = new QNetworkAccessManager(this);
    m_url.setUrl(serviceUrl + "/interface/xmlrpc");

    connect(m_networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(httpDone(QNetworkReply*)));
  }

  void CLjHandler::sendRequest(QByteArray textRequest)
  {
    qDebug() << "Reguest\r\n " << textRequest;

    QNetworkRequest postRequest;

    postRequest.setUrl(m_url);
    postRequest.setRawHeader("Host", m_url.host().toUtf8());
    postRequest.setRawHeader("Content-type", "text/xml");

    m_networkManager->post(postRequest, textRequest);
  }

  void CLjHandler::httpDone(QNetworkReply* reply)
  {
    QString replyText = QString::fromUtf8((reply->readAll()));

    qDebug() << "Response\r\n " << replyText;

    if(m_postProcessor != NULL)
      (this->*m_postProcessor)(replyText);
  }

  void CLjHandler::login()
  {
    m_postProcessor = &CLjHandler::loginChallenge;

    getChallenge();
  }

  void CLjHandler::loadPosts()
  {
    m_postProcessor = &CLjHandler::getPosts;

    getChallenge();
  }

  void CLjHandler::loadComments(core::CPost post)
  {
    m_postProcessor = &CLjHandler::getComments;
    m_post = post;

    getChallenge();
  }

  void CLjHandler::sendPost(core::CPost post)
  {
    m_postProcessor = &CLjHandler::sendPost;
    m_postsBuffer.push_back(post);

    getChallenge();
  }

  void CLjHandler::sendComment(core::CComment comment)
  {
    m_postProcessor = &CLjHandler::sendComment;
    m_commentsBuffer.push_back(comment);

    getChallenge();
  }

  void CLjHandler::getChallenge()
  {
    QString request = CRequestCreator::createRequest("LJ.XMLRPC.getchallenge", QMap<QString, QString> ());

    sendRequest(request.toUtf8());
  }

  QString CLjHandler::computeMD5(QString input)
  {
    QByteArray hash = QCryptographicHash::hash(input.toUtf8(), QCryptographicHash::Md5);

    return QString(hash.toHex());
  }

  QString CLjHandler::getAuthResponse(QString password, QString challenge)
  {
    QString hpass = computeMD5(password);

    QString constr = challenge + hpass;
    QString authResponse = computeMD5(constr);

    return authResponse;
  }

  void CLjHandler::loginChallenge(QString response)
  {
    QMap<QString, QString> parameters = initParametersMap(response);

    QString request = CRequestCreator::createRequest("LJ.XMLRPC.login", parameters);

    sendRequest(request.toUtf8());

    m_postProcessor = NULL;
  }

  void CLjHandler::sessionGenerate(QString response)
  {
    QMap<QString, QString> parameters = initParametersMap(response);

    parameters.insert("expiration", "long");

    QString request = CRequestCreator::createRequest("LJ.XMLRPC.sessiongenerate", parameters);

    sendRequest(request.toUtf8());

    m_postProcessor = &CLjHandler::loginWithCookies;
  }

  void CLjHandler::loginWithCookies(QString response)
  {
    CResponseParser parser(response);
    QString ljsession = parser.parameter("ljsession");

    qDebug() << "LjSession: " << ljsession;

    QNetworkCookie cookie(QByteArray("ljsession"), ljsession.toUtf8());
    QList<QNetworkCookie> cookieList;
    cookieList.push_back(cookie);

    QNetworkCookieJar* cookieJar = m_networkManager->cookieJar();
    cookieJar->setCookiesFromUrl(cookieList, QUrl("http://www.livejournal.com/"));
    m_networkManager->setCookieJar(cookieJar);

    QMap<QString, QString> parameters;
    parameters.insert("username", m_userName);
    parameters.insert("auth_method", "cookie");
    parameters.insert("ver", "1");

    QString request = CRequestCreator::createRequest("LJ.XMLRPC.login", parameters);

    sendRequest(request.toUtf8());

    m_postProcessor = NULL;
  }

  QMap<QString, QString> CLjHandler::initParametersMap(QString challengeResponse)
  {
    CResponseParser parser(challengeResponse);
    QString challenge = parser.parameter("challenge");
    QMap<QString, QString> parameters;

    if(!challenge.isNull())
    {
      QString auth_response = getAuthResponse(m_password, challenge);

      parameters.insert("username", m_userName);
      parameters.insert("auth_method", "challenge");
      parameters.insert("auth_challenge", challenge);
      parameters.insert("auth_response", auth_response);
      parameters.insert("ver", "1");
    }

    return parameters;
  }

  void CLjHandler::getPosts(QString response)
  {
     m_postProcessor = &CLjHandler::processPosts;

     QMap<QString, QString> parameters = initParametersMap(response);

     parameters.insert("selecttype", "lastn");
     parameters.insert("howmany", "50");
     parameters.insert("truncate", "20");
     parameters.insert("noprops", "1");
     parameters.insert("lineendings", "unix");

     QString request = CRequestCreator::createRequest("LJ.XMLRPC.getevents", parameters);

     sendRequest(request.toUtf8());
  }

  void CLjHandler::sendPost(QString response)
  {
    m_postProcessor = NULL;

    if (m_postsBuffer.empty())
      return;

    QMap<QString, QString> parameters = initParametersMap(response);
    core::CPost post = m_postsBuffer.first();

    QDateTime now = QDateTime::currentDateTime();
    QDate date = now.date();
    QTime time = now.time();

    parameters.insert("event", post.text());
    parameters.insert("subject", post.title());
    parameters.insert("year", QString::number(date.year()));
    parameters.insert("mon", QString::number(date.month()));
    parameters.insert("day", QString::number(date.day()));
    parameters.insert("hour", QString::number(time.hour()));
    parameters.insert("min", QString::number(time.minute()));

    QString request = CRequestCreator::createRequest("LJ.XMLRPC.postevent", parameters);

    sendRequest(request.toUtf8());
  }

  void CLjHandler::sendComment(QString response)
  {
    m_postProcessor = NULL;

    if (m_commentsBuffer.empty())
      return;

    QMap<QString, QString> parameters = initParametersMap(response);
    core::CComment comment = m_commentsBuffer.first();

    QString journal = m_userName.replace('-', '_');

    parameters.insert("body", comment.text());
    parameters.insert("subject", comment.title());
    parameters.insert("ditemid", comment.ditemid());
    parameters.insert("journal", journal);
    parameters.insert("parenttalkid", "0");

    QString request = CRequestCreator::createRequest("LJ.XMLRPC.addcomment", parameters);

    sendRequest(request.toUtf8());
  }

  void CLjHandler::processPosts(QString response)
  {
    m_postProcessor = NULL;

    CResponseParser parser(response);
    QSet<core::CPost> posts = parser.parseElements< core::CPost >().toSet();

    for(QSet<core::CPost>::const_iterator it = posts.begin(); it != posts.end(); ++it)
    {
      QString s;

      QTextStream os(&s);
      os << *it;

      qDebug() << s;
    }

    emit loadPostsDone(posts);
  }

  void CLjHandler::getComments(QString response)
  {
    m_postProcessor = &CLjHandler::processComments;

    QString journal = m_userName.replace('-', '_');
    QMap<QString, QString> parameters = initParametersMap(response);

    parameters.insert("ditemid", m_post.ditemid());
    parameters.insert("journal", journal);
    parameters.insert("page", "1");

    QString request = CRequestCreator::createRequest("LJ.XMLRPC.getcomments", parameters);

    sendRequest(request.toUtf8());
  }

  void CLjHandler::processComments(QString response)
  {
    m_postProcessor = NULL;

    qDebug() << "process comments";
    CResponseParser parser(response);
    QSet<core::CComment> comments = parser.parseElements< core::CComment >().toSet();

    qDebug() << "Comments size" <<  comments.size();

    for(QSet<core::CComment>::const_iterator it = comments.begin(); it != comments.end(); ++it)
    {
      QString s;

      QTextStream os(&s);
      os << *it;

      qDebug() << s;
    }
  }


} // namespace BlogService

/* ===[ End of file $HeadURL: svn+ssh://kua@osll.spb.ru/svn/scblog/trunk/src/blogservice/src/LjHandler.cpp $ ]=== */
