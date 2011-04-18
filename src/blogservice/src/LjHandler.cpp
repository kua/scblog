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
 * $Id: LjHandler.cpp 59 2011-04-18 14:14:17Z kua $ 
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
#include <QThread>
#include <QTime>
#include <QDate>
#include "LjHandler.h"
#include "RequestCreator.h"
#include "ResponseParser.h"

namespace BlogService
{
  CLjHandler::CLjHandler(const QString& serviceUrl, const QString& userName, const QString& password, QObject *parent) :
    QObject(parent), m_userName(userName), m_password(password)
  {
    m_networkManager = new QNetworkAccessManager();
    m_url.setUrl(serviceUrl + "/interface/xmlrpc");

    m_postProcessor = NULL;

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

  bool CLjHandler::isReady()
  {
    return (m_postProcessor == NULL);
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

  void CLjHandler::loadComments()
  {
    m_postProcessor = &CLjHandler::getComments;

    getChallenge();
  }

  void CLjHandler::sendPost()
  {
    if (m_postsOutputBuffer.empty())
      return;

    m_postProcessor = &CLjHandler::sendPost;

    getChallenge();
  }

  void CLjHandler::sendComment()
  {
    m_postProcessor = &CLjHandler::sendComment;

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
    QMap<QString, QString> parameters = initParametersMap(response);

    parameters.insert("selecttype", "lastn");
    parameters.insert("howmany", "50");
    parameters.insert("truncate", "20");
    parameters.insert("noprops", "1");
    parameters.insert("lineendings", "unix");

    QString request = CRequestCreator::createRequest("LJ.XMLRPC.getevents", parameters);

    sendRequest(request.toUtf8());

    m_postProcessor = &CLjHandler::processPosts;
  }

  void CLjHandler::sendPost(QString response)
  {
    if(m_postsOutputBuffer.empty())
      return;

    QMap<QString, QString> parameters = initParametersMap(response);
    QSharedPointer<core::CPost> post = m_postsOutputBuffer.first();

    QDateTime now = QDateTime::currentDateTime();
    QDate date = now.date();
    QTime time = now.time();

    parameters.insert("event", post->text());
    parameters.insert("subject", post->title());
    parameters.insert("year", QString::number(date.year()));
    parameters.insert("mon", QString::number(date.month()));
    parameters.insert("day", QString::number(date.day()));
    parameters.insert("hour", QString::number(time.hour()));
    parameters.insert("min", QString::number(time.minute()));

    QString request = CRequestCreator::createRequest("LJ.XMLRPC.postevent", parameters);

    m_postProcessor = &CLjHandler::checkSendingPost;

    sendRequest(request.toUtf8());
  }

  void CLjHandler::checkSendingPost(QString response)
  {
    CResponseParser parser(response);

    QString dtalkid = parser.parameter("url");

    if (!dtalkid.isNull())
    {
      QSharedPointer<core::CPost> post = m_postsOutputBuffer.dequeue();
      post->id()->setLjId(CResponseParser::parseUrl(QUrl(dtalkid)));

      emit sendPostDone(*post->id(), post);
    }

    m_postProcessor = NULL;

    emit transactionDone();
  }

  void CLjHandler::checkSendingComment(QString response)
  {
    CResponseParser parser(response);

    QString dtalkid = parser.parameter("dtalkid");
    QString status = parser.parameter("status");

    if (status== "OK")
    {
      QSharedPointer<core::CComment> comment = m_commentsOutputBuffer.dequeue();
      comment->id()->setLjId(dtalkid);
      emit sendCommentDone(*comment->id(), comment);
    }

    m_postProcessor = NULL;

    emit transactionDone();
  }

  void CLjHandler::sendComment(QString response)
  {
    if(m_commentsOutputBuffer.empty())
      return;

    QMap<QString, QString> parameters = initParametersMap(response);
    QSharedPointer<core::CComment> comment = m_commentsOutputBuffer.first();

    QString journal = m_userName.replace('-', '_');

    parameters.insert("body", comment->text());
    parameters.insert("subject", comment->title());
    parameters.insert("ditemid", comment->postId());
    parameters.insert("journal", journal);
    parameters.insert("parent", comment->parentId()->ljId());

    QString request = CRequestCreator::createRequest("LJ.XMLRPC.addcomment", parameters);

    m_postProcessor = &CLjHandler::checkSendingComment;

    sendRequest(request.toUtf8());
  }

  void CLjHandler::processPosts(QString response)
  {
    CResponseParser parser(response);
    QList<QSharedPointer<core::CPost> > posts = parser.parseElements<core::CPost> ();

    for(QList<QSharedPointer<core::CPost> >::const_iterator it = posts.begin(); it != posts.end(); ++it)
    {
      QString s;

      QTextStream os(&s);
      os << **it;

      qDebug() << s;
    }

    emit loadPostsDone(posts);

    m_postProcessor = NULL;

    emit transactionDone();
  }

  void CLjHandler::getComments(QString response)
  {
    m_postProcessor = &CLjHandler::processComments;

    QString journal = m_userName.replace('-', '_');
    QMap<QString, QString> parameters = initParametersMap(response);

    qDebug() << "LjID" << m_postsInputBuffer.first()->id()->ljId();

    parameters.insert("ditemid", m_postsInputBuffer.first()->id()->ljId());
    parameters.insert("journal", journal);
    parameters.insert("page", "1");

    QString request = CRequestCreator::createRequest("LJ.XMLRPC.getcomments", parameters);

    sendRequest(request.toUtf8());
  }

  void CLjHandler::processComments(QString response)
  {
    qDebug() << "process comments";
    CResponseParser parser(response);
    QList<QSharedPointer<core::CComment> > comments = parser.parseElements<core::CComment> ();
    QString postId = m_postsInputBuffer.dequeue()->id()->ljId();

    foreach(QSharedPointer<core::CComment> comment, comments)
    {
      comment->setPostId(postId);

      if (!comment->parentId()->isLjIdSet())
        comment->parentId()->setLjId(postId);
    }

    qDebug() << "Comments size" << comments.size();

    for(QList<QSharedPointer<core::CComment> >::const_iterator it = comments.begin(); it != comments.end(); ++it)
    {
      QString s;

      QTextStream os(&s);
      os << **it;

      qDebug() << s;
    }

    m_postProcessor = NULL;

    emit loadCommentsDone(comments);
    emit transactionDone();
  }

  void CLjHandler::addPostToOutputBuffer(QSharedPointer<core::CPost> post)
  {
    m_postsOutputBuffer.enqueue(post);
  }

  void CLjHandler::addPostToInputBuffer(QSharedPointer<core::CPost> post)
  {
    m_postsInputBuffer.enqueue(post);
  }

  void CLjHandler::addCommentToOutputBuffer(QSharedPointer<core::CComment> comment)
  {
    m_commentsOutputBuffer.enqueue(comment);
  }
} // namespace BlogService

/* ===[ End of file $HeadURL: svn+ssh://kua@osll.spb.ru/svn/scblog/trunk/src/blogservice/src/LjHandler.cpp $ ]=== */
