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

/* $Id: Report.h 60 2011-04-21 16:42:47Z kua $ */
/*!
 * \file Report.h
 * \brief Header of CReport
 * \todo add comment here
 *
 * File description
 *
 * PROJ: OSLL/scblog
 * ---------------------------------------------------------------- */

#ifndef _Report_H_3C856C8E_327F_4BE5_AF99_C2166BE3519E_INCLUDED_
#define _Report_H_3C856C8E_327F_4BE5_AF99_C2166BE3519E_INCLUDED_

#include <QMap>
#include <QString>
#include "Post.h"

namespace core
{
 /*!
   * Class description. May use HTML formatting
   *
   */
  class CReport:
    public CPost
  {
    Q_OBJECT

    QString m_userId;
    QString m_presentationId;
    QString m_timeSlot;

    QMap<QString, QString> m_userData;
    QMap<QString, QString> m_presentationData;

    void generateTitle();
    void generateText();

  public:

    CReport();
    CReport(const CReport& obj);
    ~CReport(){}
    CReport& operator=(const CReport& obj);
    bool operator==(const CReport& obj) const;

    void setUserId(QString& userId);
    void setPresentationId(QString presentationId);

    void putUserData(QString key, QString value);
    void putPresentationData(QString key, QString value);

    void setTimeSlot(QString timeSlot);

    QMap<QString, QString> userData() const;
    QMap<QString, QString> presentationData() const;

    void generatePostData();

    QString userId() const;
    QString presentationId() const;
    QString timeSlot() const;

    QList<Triple *> scTriplets() const;
    virtual QList<Triple *> triplets() const;

    friend QTextStream& operator<<(QTextStream& os, const CReport& post);
  }; // class CReport
  
} // namespace core

#endif //_Report_H_3C856C8E_327F_4BE5_AF99_C2166BE3519E_INCLUDED_

/* ===[ End of file $HeadURL: svn+ssh://kua@osll.spb.ru/svn/scblog/trunk/src/core/inc/Report.h $ ]=== */
