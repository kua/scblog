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

/* $Id: BlogProcessor.h 67 2011-05-02 20:09:17Z kua $ */
/*!
 * \file BlogProcessor.h
 * \brief Header of CBlogProcessor
 * \todo add comment here
 *
 * File description
 *
 * PROJ: OSLL/scblog
 * ---------------------------------------------------------------- */

#ifndef _BlogProcessor_H_6C2A1FA7_62E3_4C80_A293_D3F9910D483E_INCLUDED_
#define _BlogProcessor_H_6C2A1FA7_62E3_4C80_A293_D3F9910D483E_INCLUDED_

#include <QList>
#include <QTimer>
#include <QSharedPointer>
#include "LjManager.h"
#include "ScriboHandler.h"
#include "ConferenceHandler.h"
#include "Post.h"
#include "Comment.h"

namespace core
{
  #define BIT(a) (1 << (a))

  enum BloggingObjects{
     REPORTS,
     LJ_POSTS,
     SCRIBO_POSTS
  };

  /*!
   * Class description. May use HTML formatting
   *
   */
  class CBlogProcessor: public QObject
  {
    Q_OBJECT

    BlogService::CLjManager* m_ljManager;
    SmartSpace::CConferenceHandler* m_conferenceHandler;
    SmartSpace::CScriboHandler* m_scriboHandler;

    QTimer* m_timer;

    QMap<CId, QSharedPointer<IBlogObject> > m_blogObjects;
    QList<CId> m_postIds;

    QList<QSharedPointer<CComment> > m_toSsList;

    QList<CId> m_idList;

    int m_postsCounter;

    QMap<QString, QSharedPointer<core::CPost> > m_existingLjPosts;

    bool setParent(QSharedPointer<CComment> comment, bool copyPostId);
    void saveCommentToSs();
    void saveCommentToLj(QSharedPointer<CComment> comment);
    bool checkBlogObjectContains(const CId id);
    void printSerializedMap();
    CId getFullId(const CId id);
    void storeBlogObject(CId id, QSharedPointer<IBlogObject> blogObject);
    void loadExistingLjPosts();

  private slots:

    void reciveReports(QList<QSharedPointer<core::CReport> >);
    void reciveLjComments(QList<QSharedPointer<core::CComment> >);
    void reciveSsPosts(QList<QSharedPointer<core::CPost> >);
    void reciveSsComments(QList<QSharedPointer<core::CComment> >comment);
    void refreshComments();
    void storePost(core::CId id, QSharedPointer<core::CPost> post);
    void storeComment(core::CId id, QSharedPointer<core::CComment> comment);
    void refreshByTimer();
    void reciveLjPosts(QList<QSharedPointer<core::CPost> > posts);

  public:
    CBlogProcessor();
    ~CBlogProcessor(){};
    
    void init();
    void deserialize();
    void serialize();
  }; // class CBlogProcessor
} // namespace core

#endif //_BlogProcessor_H_6C2A1FA7_62E3_4C80_A293_D3F9910D483E_INCLUDED_

/* ===[ End of file $HeadURL: svn+ssh://kua@osll.spb.ru/svn/scblog/trunk/src/core/inc/BlogProcessor.h $ ]=== */
