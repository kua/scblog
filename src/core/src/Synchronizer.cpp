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
 * $Id: Synchronizer.cpp 53 2011-04-07 13:11:18Z kua $ 
 *
 * \file Synchronizer.cpp
 * \brief CSynchronizer implementation
 *
 * File description
 *
 * PROJ: OSLL/scblog
 * ---------------------------------------------------------------- */

#include <QDebug>
#include <QSet>
#include "Synchronizer.h"

namespace core
{
  CSynchronizer::CSynchronizer()
  {
    m_ljHandler = new BlogService::CLjHandler("http://www.livejournal.com","osll","scblog1861", this);
    m_conferenceHandler = new SmartSpace::CConferenceHandler("X",this);
    m_scriboHandler = new SmartSpace::CScriboHandler("X",QString("account-bs"),this);

    m_loadingFlags = 0;

    QObject::connect(m_conferenceHandler, SIGNAL(loadReportsDone(QSet<core::CPost>)), this, SLOT(reciveReports(QSet<core::CPost>)));
    QObject::connect(m_ljHandler, SIGNAL(loadPostsDone(QSet<core::CPost>)), this, SLOT(reciveLjPosts(QSet<core::CPost>)));
    QObject::connect(m_scriboHandler, SIGNAL(loadPostsDone(QSet<core::CPost>)), this, SLOT(reciveSsPosts(QSet<core::CPost>)));
  }

  void CSynchronizer::requestPostSynchronization()
  {
    m_conferenceHandler->loadReports();
    m_ljHandler->loadPosts();
    m_scriboHandler->loadPosts();
  }

  void CSynchronizer::reciveReports(QSet<core::CPost> posts)
  {
    m_reports = posts;

    m_loadingFlags |= BIT(REPORTS);

    synchronizePosts();
  }

  void CSynchronizer::reciveLjPosts(QSet<core::CPost> posts)
  {
    m_ljPosts = posts;

    m_loadingFlags |= BIT(LJ_POSTS);

    synchronizePosts();
  }

  void CSynchronizer::reciveSsPosts(QSet<core::CPost> posts)
  {
    qDebug() << "recieve Scribo Posts";

    for(QSet<CPost>::const_iterator it = posts.begin(); it != posts.end(); ++it)
    {
      QString s;

      QTextStream os(&s);
      os << *it;

      qDebug() << s;
    }

    m_ssPosts = posts;

    m_loadingFlags |= BIT(SCRIBO_POSTS);

    synchronizePosts();
  }

  void CSynchronizer::synchronizePosts()
  {
    if (m_loadingFlags & BIT(REPORTS) & BIT(LJ_POSTS) & BIT(SCRIBO_POSTS))
    {
      QSet<CPost> postsToLj = m_reports;
      postsToLj.subtract(m_ljPosts);
      QSet<CPost> postsToSs = m_reports;
      postsToSs.subtract(m_ssPosts);
      //m_ljHandler->sendPosts(postsToLj);
      //m_scriboHandler->sendPosts(postsToSs);
    }
  }

} // namespace core

/* ===[ End of file $HeadURL: svn+ssh://kua@osll.spb.ru/svn/scblog/trunk/src/core/src/Synchronizer.cpp $ ]=== */
