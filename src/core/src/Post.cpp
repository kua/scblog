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
 * $Id: Post.cpp 66 2011-04-26 19:07:12Z kua $ 
 *
 * \file Post.cpp
 * \brief Post implementation
 *
 * File description
 *
 * PROJ: OSLL/scblog
 * ---------------------------------------------------------------- */

#include <QDebug>
#include "Post.h"
#include "SSHandler.h"
#include "Ontology.h"

namespace core
{
  CPost::CPost(const CPost& obj):IBlogObject()
  {
    *this = obj;
  }

  CPost::CPost():IBlogObject()
  {
  }

  CPost::CPost(QString title, QString text):IBlogObject(title,text)
  {
  }

  CPost& CPost::operator=(const CPost& obj)
  {
    qobject_cast<IBlogObject*>(this)->operator=(obj);

    return *this;
  }

  bool CPost::operator==(const CPost& obj) const
  {
    return (title() == obj.title());
  }

  QSharedPointer<CId> CPost::parentId()
  {
    return QSharedPointer<CId> (new CId());
  }

  void CPost::generateSsId()
  {
    id()->setSsId(QString("post-" + generateId()));
  }

  QList<Triple *> CPost::triplets() const
  {
    QList<Triple *> triplets;

    triplets.append(SmartSpace::CSSHandler::createDefaultTriple(SmartSpace::ACCOUNT_NAME, SmartSpace::HAS_POST, id()->ssId()));
    triplets.append(SmartSpace::CSSHandler::createDefaultTriple(id()->ssId(), SmartSpace::TYPE, SmartSpace::POST));
    triplets.append(SmartSpace::CSSHandler::createDefaultTriple(id()->ssId(), SmartSpace::TITLE, encodeString(title().toUtf8())));
    triplets.append(SmartSpace::CSSHandler::createDefaultTriple(id()->ssId(), SmartSpace::TEXT, encodeString(text().toUtf8())));

    return triplets;
  }

  QTextStream& operator<<(QTextStream& os, const CPost& post)
  {
    os.setCodec("UTF-8");

    os << "Id: " << post.id()->ssId() << "; ";
    os << "Title: " << post.title() << "; ";
    os << "Text: " << post.text() << "; ";
    os << "ditemid: " << post.id()->ljId() << "; ";

    return os;
  }
} // namespace core

/* ===[ End of file $HeadURL: svn+ssh://kua@osll.spb.ru/svn/scblog/trunk/src/core/src/Post.cpp $ ]=== */
