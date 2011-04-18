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

/* $Id: Id.h 59 2011-04-18 14:14:17Z kua $ */
/*!
 * \file Id.h
 * \brief Header of CId
 * \todo add comment here
 *
 * File description
 *
 * PROJ: OSLL/scblog
 * ---------------------------------------------------------------- */

#ifndef _Id_H_D596BCAC_5880_4100_899C_1967EB9D33B0_INCLUDED_
#define _Id_H_D596BCAC_5880_4100_899C_1967EB9D33B0_INCLUDED_

#include <QObject>
#include <QDebug>

namespace core
{
  /*!
   * Class description. May use HTML formatting
   *
   */
  class CId: public QObject
  {
  Q_OBJECT

    QString m_ssId;
    QString m_ljId;

  public:
    CId();
    CId(QString& ssId, QString& ljId);
    CId(const CId& obj);

    CId& operator=(const CId& obj);
    bool operator==(const CId& obj);

    void setLjId(QString ljId);
    void setSsId(QString ssId);

    QString ssId() const;
    QString ljId() const;

    bool isLjIdSet() const;
    bool isSsIdSet() const;
    bool isComlete() const;
  }; // class CId
} // namespace core

inline bool operator<(const core::CId& first, const core::CId& second)
{
  bool equals = (((!first.ssId().isEmpty()) && (first.ssId() == second.ssId()))
      || ((!first.ljId().isEmpty()) && (first.ljId() == second.ljId())));

  if (equals)
    return false;

  return (QString(first.ssId() + first.ljId()) < QString(second.ssId() + second.ljId()));
}

#endif //_Id_H_D596BCAC_5880_4100_899C_1967EB9D33B0_INCLUDED_
/* ===[ End of file $HeadURL: svn+ssh://kua@osll.spb.ru/svn/scblog/trunk/src/core/inc/Id.h $ ]=== */
