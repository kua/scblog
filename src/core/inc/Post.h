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

/* $Id: Post.h 52 2011-04-05 14:04:15Z kua $ */
/*!
 * \file Post.h
 * \brief Header of CPost
 * \todo add comment here
 *
 * File description
 *
 * PROJ: OSLL/scblog
 * ---------------------------------------------------------------- */

#ifndef _Post_H_0FB03DCF_B900_4D9B_9B00_362B2E2A23D9_INCLUDED_
#define _Post_H_0FB03DCF_B900_4D9B_9B00_362B2E2A23D9_INCLUDED_

#include <QObject>
#include <QUrl>
#include <QMap>
#include <QTextStream>
#include "triple.h"
#include "BlogObject.h"

namespace core
{
  /*!
   * Class description. May use HTML formatting
   *
   */
  class CPost: public IBlogObject
  {
    Q_OBJECT

    QString m_userId;
    QString m_presentationId;
    QString m_timeSlot;

    QMap<QString, QString> m_userData;
    QMap<QString, QString> m_presentationData;

    void generateTitle();
    void generateText();

    virtual QString generateId();

  public:

    CPost(QString id = QString());
    CPost(QString title, QString text);
    CPost(const CPost& obj);
    ~CPost(){}
    CPost& operator=(const CPost& obj);
    bool operator==(const CPost& obj) const;

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
    QList<Triple *> triplets() const;

    friend QTextStream& operator<<(QTextStream& os, const CPost& post);
  }; // class CPost
} // namespace core

static uint qHash(const core::CPost& post)
{
   return qHash(post.title());
}

#endif //_Post_H_0FB03DCF_B900_4D9B_9B00_362B2E2A23D9_INCLUDED_
/* ===[ End of file $HeadURL: svn+ssh://kua@osll.spb.ru/svn/scblog/trunk/src/core/inc/Post.h $ ]=== */
