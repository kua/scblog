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

/* $Id: ConferenceHandler.h 67 2011-05-02 20:09:17Z kua $ */
/*!
 * \file ConferenceHandler.h
 * \brief Header of CConferenceHandler
 * \todo add comment here
 *
 * File description
 *
 * PROJ: OSLL/scblog
 * ---------------------------------------------------------------- */

#ifndef _ConferenceHandler_H_505E0F8A_426D_49DD_A37E_CE29D1B3C0B2_INCLUDED_
#define _ConferenceHandler_H_505E0F8A_426D_49DD_A37E_CE29D1B3C0B2_INCLUDED_

#include <QSet>
#include "SSHandler.h"
#include "Report.h"

namespace SmartSpace
{
  /*!
   * Class description. May use HTML formatting
   *
   */
  class CConferenceHandler: public CSSHandler
  {
    Q_OBJECT

    QMap<QString, QSharedPointer<core::CReport> > m_posts;

    QMap<QString, QString> m_presentationsCache;
    QSet<QString> m_reports;
    QSet<QString> m_reportsBuffer;
    void (CConferenceHandler::*m_postProcessor)(QList<Triple*> );

    virtual void postProcess(QList<Triple *> triples);
    bool isReady();

    void processTimeSlots(QList<Triple *> triple);
    void processUserProfile(QList<Triple *> triple);
    void processPresentation(QList<Triple *> triple);
    void recieveReports(QList<Triple *> triple);

  private slots:
    void refreshReportsRequest();
    void checkExistingReports();
    void checkReportsBuffer();

  signals:
    void transactionDone();

    void loadReportsDone(QList<QSharedPointer<core::CReport> > posts);

  public:
    CConferenceHandler(QObject *parent = 0) : CSSHandler(parent) {};
    CConferenceHandler(QString sibUri, QObject *parent = 0);
    ~CConferenceHandler();
    
    void subscribeToScheduleChanges();
    void loadReports();
    void saveReport(QSharedPointer<core::CReport> report);

  }; // class CConferenceHandler
} // namespace smartspace

#endif //_ConferenceHandler_H_505E0F8A_426D_49DD_A37E_CE29D1B3C0B2_INCLUDED_
/* ===[ End of file $HeadURL: svn+ssh://kua@osll.spb.ru/svn/scblog/trunk/src/smartspace/inc/ConferenceHandler.h $ ]=== */
