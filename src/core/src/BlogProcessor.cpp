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
 * $Id: BlogProcessor.cpp 59 2011-04-18 14:14:17Z kua $ 
 *
 * \file BlogProcessor.cpp
 * \brief CBlogProcessor implementation
 *
 * File description
 *
 * PROJ: OSLL/scblog
 * ---------------------------------------------------------------- */

#include <QDebug>
#include <QTimer>
#include <QSet>
#include "BlogProcessor.h"
#include "Id.h"
#include "Ontology.h"

namespace core
{
  CBlogProcessor::CBlogProcessor()
  {
    m_ljManager = new BlogService::CLjManager(this);

    QSharedPointer<BlogService::CLjHandler> ljHandler =
        QSharedPointer<BlogService::CLjHandler> (new BlogService::CLjHandler("http://www.livejournal.com", "osll",
                                                                             "scblog1861", m_ljManager));
    m_conferenceHandler = new SmartSpace::CConferenceHandler("X", this);
    m_scriboHandler = new SmartSpace::CScriboHandler("X", this);
    m_ljManager->setHandler(ljHandler);

    QObject::connect(m_conferenceHandler, SIGNAL(loadReportsDone(QList<QSharedPointer<core::CReport> >)), this,
                     SLOT(reciveReports(QList<QSharedPointer<core::CReport> >)));

    //QObject::connect(ljHandler.data(), SIGNAL(loadPostsDone(QList<QSharedPointer<core::CPost> >)), this,
    //                 SLOT(reciveLjPosts(QList<QSharedPointer<core::CPost> >)));
    QObject::connect(ljHandler.data(), SIGNAL(loadCommentsDone(QList<QSharedPointer<core::CComment> >)), this,
                     SLOT(reciveLjComments(QList<QSharedPointer<core::CComment> >)));
    QObject::connect(ljHandler.data(), SIGNAL(sendPostDone(core::CId, QSharedPointer<core::CPost>)), this,
                     SLOT(storePost(core::CId, QSharedPointer<core::CPost>)));
    QObject::connect(ljHandler.data(), SIGNAL(sendCommentDone(core::CId, QSharedPointer<core::CComment>)), this,
                     SLOT(storeComment(core::CId, QSharedPointer<core::CComment>)));

    QObject::connect(m_scriboHandler, SIGNAL(loadPostsDone(QList<QSharedPointer<core::CPost> >)), this,
                     SLOT(reciveSsPosts(QList<QSharedPointer<core::CPost> >)));
    QObject::connect(m_scriboHandler, SIGNAL(loadCommentsDone(QList<QSharedPointer<core::CComment> >)), this,
                     SLOT(reciveSsComments(QList<QSharedPointer<core::CComment> >)));
    QObject::connect(m_scriboHandler, SIGNAL(refreshComments()), this, SLOT(refreshComments()));
  }

  void CBlogProcessor::init()
  {
    m_conferenceHandler->loadReports();
  }

  void CBlogProcessor::reciveReports(QList<QSharedPointer<core::CReport> > posts)
  {
    foreach (QSharedPointer<core::CReport> object, posts)
        m_scriboHandler->sendBlogObject(object);

    m_scriboHandler->loadPosts(SmartSpace::ACCOUNT_NAME);
  }

  void CBlogProcessor::reciveLjComments(QList<QSharedPointer<core::CComment> > comments)
  {
    qDebug() << "CBlogProcessor::reciveLjComments";

    foreach(QSharedPointer<core::CComment> comment, comments)
        if(!m_blogObjects.contains(*comment->id()))
        {
          saveCommentToSs(comment);
          m_blogObjects.insert(*comment->id(), comment);
        }

    qDebug() << "Size" <<m_blogObjects.size();
    for(QMap<CId, QSharedPointer<IBlogObject> >::const_iterator it = m_blogObjects.begin(); it != m_blogObjects.end(); ++it)
    {
      QString s;

      QTextStream os(&s);
      os << **it;

      qDebug() << s;
    }
  }

  void CBlogProcessor::reciveSsComments(QList<QSharedPointer<core::CComment> > comments)
  {
    foreach(QSharedPointer<CComment> comment, comments)
        if(!m_blogObjects.contains(*comment->id()))
          saveCommentToLj(comment);
    //m_blogObjects.insert(comment->id(), comment);
  }

  void CBlogProcessor::reciveSsPosts(QList<QSharedPointer<core::CPost> > posts)
  {
    qDebug() << "CBlogProcessor::reciveSsPosts";

    foreach(QSharedPointer<core::CPost> post, posts)
        if(!m_blogObjects.contains(*post->id()))
        {
          m_ljManager->sendPost(post);
          //m_blogObjects.insert(post->id(), post);
        }
  }

  void CBlogProcessor::refreshComments()
  {
    qDebug() << QString("refreshComments");

    foreach(CId id, m_postIds)
      {
        QString s;

        QTextStream os(&s);
        os << *m_blogObjects[id].dynamicCast<CPost> ();

        qDebug() << s;

        m_ljManager->loadComments(m_blogObjects[id].dynamicCast<CPost> ());
      }
  }

  bool CBlogProcessor::setParent(QSharedPointer<CComment> comment, bool copyPostId = false)
  {
    if (!m_blogObjects.contains(*comment->parentId()))
      return false;

    QSharedPointer<IBlogObject> parent = m_blogObjects.value(*comment->parentId());

    if(parent->id()->isComlete())
    {
      comment->setParentId(parent->id());

      if(copyPostId)
        comment->setPostId(parent->postId());

      return true;
    }

    return false;
  }

  void CBlogProcessor::saveCommentToSs(QSharedPointer<CComment> comment)
  {
    qDebug() << " CBlogProcessor::saveCommentToSs";

    comment->generateSsId();

    if(setParent(comment))
      m_scriboHandler->sendBlogObject(comment);
  }

  void CBlogProcessor::saveCommentToLj(QSharedPointer<CComment> comment)
  {
    if(setParent(comment, true))
      m_ljManager->sendComment(comment);
  }

  void CBlogProcessor::storePost(core::CId id, QSharedPointer<core::CPost> post)
  {
    qDebug() << "CBlogProcessor::storePost";

    m_blogObjects.insert(id, post);
    m_postIds.push_back(id);
  }

  void CBlogProcessor::storeComment(core::CId id, QSharedPointer<core::CComment> comment)
  {
    m_blogObjects.insert(id, comment);
  }

} // namespace core

/* ===[ End of file $HeadURL: svn+ssh://kua@osll.spb.ru/svn/scblog/trunk/src/core/src/BlogProcessor.cpp $ ]=== */
