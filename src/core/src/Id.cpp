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
 * $Id: Id.cpp 59 2011-04-18 14:14:17Z kua $ 
 *
 * \file Id.cpp
 * \brief CId implementation
 *
 * File description
 *
 * PROJ: OSLL/scblog
 * ---------------------------------------------------------------- */

#include <QDebug>
#include "Id.h"

namespace core
{
  CId::CId()
  {
  }

  CId::CId(QString& ssId, QString& ljId) :
    m_ssId(ssId), m_ljId(ljId)
  {
  }

  CId::CId(const CId& obj) :
    QObject(), m_ssId(obj.ssId()), m_ljId(obj.ljId())
  {
  }

  CId& CId::operator=(const CId& obj)
  {
    m_ssId = obj.ssId();
    m_ljId = obj.ljId();

    return *this;
  }

  bool CId::operator==(const CId& obj)
  {
    return ((!ssId().isEmpty() && (ssId() == obj.ssId()))
        || ( !ljId().isEmpty() && (ljId() == obj.ljId())));
  }

  void CId::setLjId(QString ljId)
  {
    m_ljId = ljId;
  }

  void CId::setSsId(QString ssId)
  {
    m_ssId = ssId;
  }

  QString CId::ssId() const
  {
    return m_ssId;
  }

  QString CId::ljId() const
  {
    return m_ljId;
  }

  bool CId::isLjIdSet() const
  {
    return (!m_ljId.isEmpty());
  }

  bool CId::isSsIdSet() const
  {
    return (!m_ssId.isEmpty());
  }

  bool CId::isComlete() const
  {
    return (isLjIdSet() && isSsIdSet());
  }

} // namespace core

/* ===[ End of file $HeadURL: svn+ssh://kua@osll.spb.ru/svn/scblog/trunk/src/core/src/Id.cpp $ ]=== */
