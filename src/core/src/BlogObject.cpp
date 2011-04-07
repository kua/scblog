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
 * $Id: BlogObject.cpp 53 2011-04-07 13:11:18Z kua $ 
 *
 * \file BlogObject.cpp
 * \brief IBlogObject implementation
 *
 * File description
 *
 * PROJ: OSLL/scblog
 * ---------------------------------------------------------------- */

#include <QUuid>
#include <QDebug>
#include "BlogObject.h"

namespace core
{
  IBlogObject::IBlogObject(QString id)
  {
    setId(id);
  }

  IBlogObject::IBlogObject(QString title, QString text, QString id) :
    m_title(title), m_text(text)
  {
    setId(id);
  }

  IBlogObject::IBlogObject(const IBlogObject& obj) :
    QObject(), m_id(obj.id()), m_title(obj.title()), m_text(obj.text()), m_ditemid(obj.id())
  {
  }

  IBlogObject& IBlogObject::operator=(const IBlogObject& obj)
  {
    m_title = obj.title();
    m_text = obj.text();
    m_id = obj.id();

    return *this;
  }

  bool IBlogObject::operator==(const IBlogObject& obj)
  {
    return (m_title == obj.title());
  }

  void IBlogObject::setTitle(QString title)
  {
    m_title = title;
  }

  void IBlogObject::setText(QString text)
  {
    m_text = text;
  }

  void IBlogObject::setDitemId(QString ditemid)
  {
    m_ditemid = ditemid;
  }

  void IBlogObject::setId(QString id)
  {
    m_id = id.isEmpty() ? generateId() : id;
  }

  QString IBlogObject::id() const
  {
    return m_id;
  }

  QString IBlogObject::text() const
  {
    return m_text;
  }

  QString IBlogObject::title() const
  {
    return m_title;
  }

  QString IBlogObject::ditemid() const
  {
    return m_ditemid;
  }

  QString IBlogObject::generateId()
  {
    return QString::number(QUuid::createUuid().data1);
  }

  QTextStream& operator<<(QTextStream& os, const IBlogObject& blogObject)
  {
    os.setCodec("UTF-8");

    os << "Id: " << blogObject.id() << "; ";
    os << "Title: " << blogObject.title() << "; ";
    os << "Text: " << blogObject.text() << "; ";
    os << "ditemId: " << blogObject.ditemid() << "; ";

    return os;
  }

} // namespace core

/* ===[ End of file $HeadURL: svn+ssh://kua@osll.spb.ru/svn/scblog/trunk/src/core/src/BlogObject.cpp $ ]=== */
