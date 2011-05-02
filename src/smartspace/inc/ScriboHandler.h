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

/* $Id: ScriboHandler.h 67 2011-05-02 20:09:17Z kua $ */
/*!
 * \file ScriboHandler.h
 * \brief Header of CScriboHandler
 * \todo add comment here
 *
 * File description
 *
 * PROJ: OSLL/scblog
 * ---------------------------------------------------------------- */

#ifndef _ScriboHandler_H_BAAEF3DD_2A17_47A6_B53E_481B31FA3BCA_INCLUDED_
#define _ScriboHandler_H_BAAEF3DD_2A17_47A6_B53E_481B31FA3BCA_INCLUDED_

#include <QSet>
#include <QQueue>
#include <QPair>
#include "SSHandler.h"

namespace SmartSpace
{
  enum ScriboObject{
    SCRIBO_POST,
    SCRIBO_COMMENT
  };

  /*!
   * Class description. May use HTML formatting
   *
   */
  class CScriboHandler: public CSSHandler
  {
    Q_OBJECT

    QMap<QString, QSharedPointer<core::IBlogObject> > m_blogObjects;
    void (CScriboHandler::*m_postProcessor)(QList<Triple*> );
    QString m_accountName;

    QQueue<QPair<QString, QList<Triple*> > > m_queryQueue;

    QSet<QString> m_relatedPersons;

    void createPredicatesHash();

    virtual void postProcess(QList<Triple *> triples);

    void emitCommentSignal(QSet<QString> readyBlogObjects);
    void emitPostSignal(QSet<QString> readyBlogObjects);

    ScriboObject defineScriboObject(QString name);

    void init();

  private slots:
    void refreshCommentsRequest();
    void sendCommentRequest();
    void processBlogObjectsList(int success);
    void processBlogObjects(int success);
    void scriboQuery();
    void updateRelatedPersonsSet(QSet<QString> personsIds);

  public slots:
    void replyToNotification(QString predicate, QString message);

  signals:
    void loadPostsDone( QList<QSharedPointer<core::CPost> > posts);
    void loadCommentsDone(QList<QSharedPointer<core::CComment> > comments);
    void refreshComments();

  public:
    CScriboHandler(QString sibUri, QObject *parent = 0);
    ~CScriboHandler();

    void subscribeRefreshComments();
    void subscribeSendComment();

    void loadComments(QString subject);
    void loadPosts(QString accountName);
    
    void sendPosts(QList<core::CPost> posts);
    void sendBlogObject(QSharedPointer<core::IBlogObject> object);
    void sendBlogObjects(QList <QSharedPointer<core::IBlogObject> > objects);
    void saveParentRelations(QSharedPointer<core::CComment> comment);

  }; // class CScriboHandler
} // namespace smartspace

#endif //_ScriboHandler_H_BAAEF3DD_2A17_47A6_B53E_481B31FA3BCA_INCLUDED_
/* ===[ End of file $HeadURL: svn+ssh://kua@osll.spb.ru/svn/scblog/trunk/src/smartspace/inc/ScriboHandler.h $ ]=== */
