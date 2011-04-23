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
 * $Id: Comment.cpp 63 2011-04-23 22:20:00Z kua $ 
 *
 * \file Comment.cpp
 * \brief CComment implementation
 *
 * File description
 *
 * PROJ: OSLL/scblog
 * ---------------------------------------------------------------- */

#include <QUuid>
#include <QDebug>
#include "Comment.h"
#include "Id.h"
#include "SSHandler.h"
#include "Ontology.h"

namespace core
{
  CComment::CComment()
  {
    m_parentId = QSharedPointer<CId>(new CId());
  }

  CComment::CComment(QString title, QString text):
    IBlogObject(title, text)
  {
    m_parentId = QSharedPointer<CId>(new CId());
  }

  CComment::CComment(const CComment& obj) :
    IBlogObject()
  {
    *this = obj;
  }

  CComment& CComment::operator=(const CComment& obj)
  {
    qobject_cast<IBlogObject*>(this)->operator=(obj);

    return *this;
  }

  bool CComment::operator==(const CComment& obj) const
  {
    return (title() == obj.title());
  }

  void CComment::generateSsId()
  {
    id()->setSsId(QString("comment-" + generateId()));
  }

  void CComment::setParentId(QSharedPointer<CId> id)
  {
    m_parentId = id;
  }

  QSharedPointer<CId> CComment::parentId()
  {
    return m_parentId;
  }

  QSharedPointer<CId> CComment::parentId() const
  {
    return m_parentId;
  }

  QList<Triple *> CComment::triplets() const
  {
    QList<Triple *> triplets;

    triplets.append(SmartSpace::CSSHandler::createDefaultTriple(parentId()->ssId(), SmartSpace::HAS_COMMENT, id()->ssId()));
    triplets.append(SmartSpace::CSSHandler::createDefaultTriple(id()->ssId(), SmartSpace::TYPE, SmartSpace::COMMENT));
    triplets.append(SmartSpace::CSSHandler::createDefaultTriple(id()->ssId(), SmartSpace::TITLE, title()));
    triplets.append(SmartSpace::CSSHandler::createDefaultTriple(id()->ssId(), SmartSpace::TEXT, text()));
    triplets.append(SmartSpace::CSSHandler::createDefaultTriple(id()->ssId(), SmartSpace::POSTER, SmartSpace::PERSON_NAME));

    return triplets;
  }

  QTextStream& operator<<(QTextStream& os, const CComment& comment)
  {
    os.setCodec("UTF-8");

    os << "Id: " << comment.id()->ssId() << "; ";
    os << "Title: " << comment.title() << "; ";
    os << "Text: " << comment.text() << "; ";
    os << "ditemId: " << comment.id()->ljId() << "; ";

    return os;
  }

} // namespace core

/* ===[ End of file $HeadURL: svn+ssh://kua@osll.spb.ru/svn/scblog/trunk/src/core/src/Comment.cpp $ ]=== */
