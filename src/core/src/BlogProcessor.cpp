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
 * $Id: BlogProcessor.cpp 64 2011-04-24 16:27:13Z kua $ 
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
#include <QFile>
#include "BlogProcessor.h"
#include "Id.h"
#include "Ontology.h"

namespace core
{
  static const QString SERIALIZE_FILE_NAME = "scblog.dat";

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
    qDebug() << "CBlogProcessor::reciveReports";

    foreach (QSharedPointer<core::CReport> object, posts)
        m_scriboHandler->sendBlogObject(object);

    m_scriboHandler->loadPosts(SmartSpace::ACCOUNT_NAME);
  }

  bool CBlogProcessor::checkBlogObjectContains(const CId id)
  {
    QList<CId>::iterator it = m_idList.begin();
    bool flag = false;

    while((it != m_idList.end()) && (!flag))
    {
      if(*it == id)
        flag = true;

      ++it;
    }

    return flag;
  }

  CId CBlogProcessor::getFullId(const CId id)
  {
    QList<CId>::iterator it = m_idList.begin();

    while(it != m_idList.end())
    {
      if(*it == id)
        return *it;

      ++it;
    }

    return id;
  }

  void CBlogProcessor::reciveLjComments(QList<QSharedPointer<core::CComment> > comments)
  {
    qDebug() << "CBlogProcessor::reciveLjComments";

    foreach(QSharedPointer<core::CComment> comment, comments)
        if(!checkBlogObjectContains(*comment->id()))
        {
          comment->generateSsId();
          m_toSsList.push_back(comment);
        }

    saveCommentToSs();

    m_scriboHandler->replyToNotification(SmartSpace::REFRESH_COMMENTS, "ок");

    printSerializedMap();
  }

  void CBlogProcessor::reciveSsComments(QList<QSharedPointer<core::CComment> > comments)
  {
    qDebug() << "CBlogProcessor::reciveSsComments";

    foreach(QSharedPointer<CComment> comment, comments)
        if(!checkBlogObjectContains(*comment->id()))
          saveCommentToLj(comment);
  }

  void CBlogProcessor::reciveSsPosts(QList<QSharedPointer<core::CPost> > posts)
  {
    qDebug() << "CBlogProcessor::reciveSsPosts";

    foreach(QSharedPointer<core::CPost> post, posts)
        if(!checkBlogObjectContains(*post->id()))
          m_ljManager->sendPost(post);
  }

  void CBlogProcessor::refreshComments()
  {
    qDebug() << "CBlogProcessor::refreshComments";

    foreach(CId id, m_postIds)
        m_ljManager->loadComments(id);
  }

  bool CBlogProcessor::setParent(QSharedPointer<CComment> comment, bool copyPostId = false)
  {
    qDebug() << "CBlogProcessor::setParent";
    qDebug() << "parentId" << comment->parentId()->ljId() << comment->parentId()->ssId();

    if(!checkBlogObjectContains(*comment->parentId()))
      return false;

    CId parentId = getFullId(*comment->parentId());

    if(parentId.isComlete())
    {
      comment->setParentId(QSharedPointer<CId> (new CId(parentId)));

      if(copyPostId)
      {
        QString postId = parentId.postId().isEmpty() ? parentId.ljId() : parentId.postId();
        comment->id()->setPostId(postId);
      }
      return true;
    }

    return false;
  }

  void CBlogProcessor::saveCommentToSs()
  {
    qDebug() << " CBlogProcessor::saveCommentToSs";

    while(!m_toSsList.isEmpty())
    {
      QList<QSharedPointer<CComment> > temp = m_toSsList;

      for(QList<QSharedPointer<CComment> >::iterator iter = temp.begin(); iter != temp.end(); ++iter)
        if(setParent(*iter))
        {
          m_scriboHandler->sendBlogObject(*iter);
          storeBlogObject(*(*iter)->id(), *iter);
          m_toSsList.removeOne(*iter);

          QString s;

          QTextStream os(&s);
          os << **iter;
          qDebug() << s;
        }
    }
  }

  void CBlogProcessor::storeBlogObject(CId id, QSharedPointer<IBlogObject> blogObject)
  {
    m_blogObjects.insert(id, blogObject);
    m_idList.push_back(id);
    serialize();
  }

  void CBlogProcessor::saveCommentToLj(QSharedPointer<CComment> comment)
  {
    if(setParent(comment, true))
      m_ljManager->sendComment(comment);
  }

  void CBlogProcessor::storePost(core::CId id, QSharedPointer<core::CPost> post)
  {
    qDebug() << "CBlogProcessor::storePost";

    storeBlogObject(id, post);
    m_postIds.push_back(id);
  }

  void CBlogProcessor::storeComment(core::CId id, QSharedPointer<core::CComment> comment)
  {
    storeBlogObject(id, comment);
    m_scriboHandler->replyToNotification(SmartSpace::SEND_COMMENT, "ok");
  }

  void CBlogProcessor::printSerializedMap()
  {
    qDebug() << "Map Size" << m_blogObjects.size();
    for(QMap<CId, QSharedPointer<IBlogObject> >::iterator iter = m_blogObjects.begin(); iter != m_blogObjects.end(); ++iter)
    {
      QString s;

      QTextStream os(&s);
      os << **iter;
      qDebug() << s;
    }
  }

  void CBlogProcessor::serialize()
  {
    QFile file(SERIALIZE_FILE_NAME);
    file.remove();
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);
    out << m_idList;
    file.close();
  }

  void CBlogProcessor::deserialize()
  {
    QFile file(SERIALIZE_FILE_NAME);
    if(file.exists())
    {
      file.open(QIODevice::ReadOnly);
      QDataStream in(&file);
      in >> m_idList;
    }
    file.close();

    foreach(CId id, m_idList)
        if(id.isPost())
          m_postIds.push_back(id);
  }

} // namespace core

/* ===[ End of file $HeadURL: svn+ssh://kua@osll.spb.ru/svn/scblog/trunk/src/core/src/BlogProcessor.cpp $ ]=== */
