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
 * $Id: Post.cpp 52 2011-04-05 14:04:15Z kua $ 
 *
 * \file Post.cpp
 * \brief Post implementation
 *
 * File description
 *
 * PROJ: OSLL/scblog
 * ---------------------------------------------------------------- */

#include <QHashNode>
#include <QUuid>
#include <QDebug>
#include "Post.h"
#include "SSHandler.h"

namespace core
{
  CPost::CPost(const CPost& obj):IBlogObject()
  {
    *this = obj;
  }

  CPost::CPost(QString id):IBlogObject(id)
  {
  }

  CPost::CPost(QString title, QString text):IBlogObject(title,text)
  {
  }

  CPost& CPost::operator=(const CPost& obj)
  {
    qobject_cast<IBlogObject*>(this)->operator=(obj);

    m_timeSlot = obj.timeSlot();
    m_userId = obj.userId();
    m_presentationId = obj.presentationId();

    m_userData = obj.userData();
    m_presentationData = obj.presentationData();

    return *this;
  }

  bool CPost::operator==(const CPost& obj) const
  {
    return (title() == obj.title());
  }

  void CPost::setUserId(QString& userId)
  {
    m_userId = userId;
  }

  void CPost::setPresentationId(QString presentationId)
  {
    m_presentationId = presentationId;
  }

  void CPost::putUserData(QString key, QString value)
  {
    m_userData.insert(key, value);
  }

  void CPost::putPresentationData(QString key, QString value)
  {
    m_presentationData.insert(key, value);
  }

  void CPost::setTimeSlot(QString timeSlot)
  {
    m_timeSlot = timeSlot;
  }

  QString CPost::userId() const
  {
    return m_userId;
  }

  QString CPost::presentationId() const
  {
    return m_presentationId;
  }

  QString CPost::timeSlot() const
  {
    return m_timeSlot;
  }

  QMap<QString, QString> CPost::userData() const
  {
    return m_userData;
  }

  QMap<QString, QString> CPost::presentationData() const
  {
    return m_presentationData;
  }

  void CPost::generateTitle()
  {
   setTitle(m_presentationData["Title"]);
  }

  void CPost::generateText()
  {
    QString text;

    text.append("Speaker is " + m_userData["is"]);
    text.append(" (email: " + m_userData["email"]);
    text.append("; phone: " + m_userData["phone"] + ";).");
    text.append(" Presentation is available at: " + m_presentationData["url"]);
    text.append(" Keywords: " + m_presentationData["Keywords"]);

    setText(text);
  }

  QString CPost::generateId()
  {
    return QString("post-" + IBlogObject::generateId());
  }

  void CPost::generatePostData()
  {
    generateTitle();
    generateText();
    generateId();
  }

  QList<Triple *> CPost::triplets() const
  {
    QList<Triple *> triplets;

    triplets.append(SmartSpace::CSSHandler::createDefaultTriple(id(), "rdf:type", "scribo_Post"));
    triplets.append(SmartSpace::CSSHandler::createDefaultTriple(id(), "scribo_title", title()));
    triplets.append(SmartSpace::CSSHandler::createDefaultTriple(id(), "scribo_text", text()));

    return triplets;
  }

  QTextStream& operator<<(QTextStream& os, const CPost& post)
  {
    os.setCodec("UTF-8");

    os << "UserId: " << post.userId() << "; ";
    os << "Title: " << post.title() << "; ";
    os << "Text: " << post.text() << "; ";
    os << "ditemid: " << post.ditemid() << "; ";

    QMap<QString, QString> userData = post.userData();
    QMap<QString, QString> presentationData = post.presentationData();

    for(QMap<QString, QString>::const_iterator it = userData.begin(); it != userData.end(); ++it)
      os << it.key() << ": " << it.value() << "; ";

    for(QMap<QString, QString>::const_iterator it = presentationData.begin(); it != presentationData.end(); ++it)
      os << it.key() << ": " << it.value() << "; ";

    return os;
  }
} // namespace core

/* ===[ End of file $HeadURL: svn+ssh://kua@osll.spb.ru/svn/scblog/trunk/src/core/src/Post.cpp $ ]=== */