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
 * $Id: Report.cpp 60 2011-04-21 16:42:47Z kua $ 
 *
 * \file Report.cpp
 * \brief CReport implementation
 *
 * File description
 *
 * PROJ: OSLL/scblog
 * ---------------------------------------------------------------- */

#include <QHashNode>
#include <QUuid>
#include <QDebug>
#include "Ontology.h"
#include "Post.h"
#include "SSHandler.h"
#include "Report.h"

namespace core
{
  CReport::CReport(const CReport& obj):CPost()
  {
    *this = obj;
  }

  CReport::CReport():CPost()
  {
  }

  CReport& CReport::operator=(const CReport& obj)
  {
    qobject_cast<CPost*>(this)->operator=(obj);

    m_timeSlot = obj.timeSlot();
    m_userId = obj.userId();
    m_presentationId = obj.presentationId();

    m_userData = obj.userData();
    m_presentationData = obj.presentationData();

    return *this;
  }

  bool CReport::operator==(const CReport& obj) const
  {
    return (title() == obj.title());
  }

  void CReport::setUserId(QString& userId)
  {
    m_userId = userId;
  }

  void CReport::setPresentationId(QString presentationId)
  {
    m_presentationId = presentationId;
  }

  void CReport::putUserData(QString key, QString value)
  {
    m_userData.insert(key, value);
  }

  void CReport::putPresentationData(QString key, QString value)
  {
    m_presentationData.insert(key, value);
  }

  void CReport::setTimeSlot(QString timeSlot)
  {
    m_timeSlot = timeSlot;
  }

  QString CReport::userId() const
  {
    return m_userId;
  }

  QString CReport::presentationId() const
  {
    return m_presentationId;
  }

  QString CReport::timeSlot() const
  {
    return m_timeSlot;
  }

  QMap<QString, QString> CReport::userData() const
  {
    return m_userData;
  }

  QMap<QString, QString> CReport::presentationData() const
  {
    return m_presentationData;
  }

  void CReport::generateTitle()
  {
   setTitle(m_presentationData["Title"]);
  }

  void CReport::generateText()
  {
    QString text;

    text.append("Speaker is " + m_userData["is"]);
    text.append(" (email: " + m_userData["email"]);
    text.append("; phone: " + m_userData["phone"] + ";).");
    text.append(" Presentation is available at: " + m_presentationData["url"]);
    text.append(" Keywords: " + m_presentationData["Keywords"]);

    setText(text);
  }

  void CReport::generatePostData()
  {
    generateTitle();
    generateText();
    CPost::generateSsId();
  }

  QList<Triple *> CReport::triplets() const
  {
    QList<Triple *> triples = CPost::triplets();
    //triples.append(SmartSpace::CSSHandler::createDefaultTriple(id()->ssId(), SmartSpace::DESCRIBE, userId()));

    return triples;
  }

  QList<Triple *> CReport::scTriplets() const
  {
    QList<Triple *> triples = SmartSpace::CSSHandler::createTripleList(SmartSpace::CSSHandler::createDefaultTriple(id()->ssId(), SmartSpace::DESCRIBE, userId()));

    return triples;
  }

  QTextStream& operator<<(QTextStream& os, const CReport& post)
  {
    os.setCodec("UTF-8");

    os << "UserId: " << post.userId() << "; ";
    os << "Id: " << post.id()->ssId() << "; ";
    os << "Title: " << post.title() << "; ";
    os << "Text: " << post.text() << "; ";
    os << "ditemid: " << post.id()->ljId() << "; ";

    QMap<QString, QString> userData = post.userData();
    QMap<QString, QString> presentationData = post.presentationData();

    for(QMap<QString, QString>::const_iterator it = userData.begin(); it != userData.end(); ++it)
      os << it.key() << ": " << it.value() << "; ";

    for(QMap<QString, QString>::const_iterator it = presentationData.begin(); it != presentationData.end(); ++it)
      os << it.key() << ": " << it.value() << "; ";

    return os;
  }
} // namespace core


/* ===[ End of file $HeadURL: svn+ssh://kua@osll.spb.ru/svn/scblog/trunk/src/core/src/Report.cpp $ ]=== */
