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
 * $Id: LjManager.cpp 66 2011-04-26 19:07:12Z kua $ 
 *
 * \file LjManager.cpp
 * \brief CLjManager implementation
 *
 * File description
 *
 * PROJ: OSLL/scblog
 * ---------------------------------------------------------------- */

#include <QDebug>
#include "LjManager.h"

namespace BlogService
{
  CLjManager::CLjManager(QObject *parent) :
    QObject(parent)
  {
  }

  void CLjManager::setHandler(QSharedPointer<CLjHandler> handler)
  {
    m_ljHandler = handler;
    connect(m_ljHandler.data(), SIGNAL(transactionDone()), this, SLOT(performTask()));
  }

  void CLjManager::performTask()
  {
    if (m_ljHandler->isReady() && !m_taskQueue.isEmpty())
    {
      qDebug() << "CLjManager::performTask::m_taskQueue.size()" << m_taskQueue.size();

      void (BlogService::CLjHandler::*function)() = m_taskQueue.dequeue();
      (m_ljHandler.data()->*function)();
    }
  }

  void CLjManager::login()
  {
    m_taskQueue.enqueue(&BlogService::CLjHandler::login);

    performTask();
  }

  void CLjManager::loadComments(core::CId postId)
  {
    m_ljHandler->addPostToInputBuffer(postId);
    m_taskQueue.enqueue(&BlogService::CLjHandler::loadComments);

    performTask();
  }

  void CLjManager::loadPosts()
  {
    m_taskQueue.enqueue(&BlogService::CLjHandler::loadPosts);

    performTask();
  }

  void CLjManager::sendPost(QSharedPointer<core::CPost> post)
  {
    m_ljHandler->addPostToOutputBuffer(post);
    m_taskQueue.enqueue(&BlogService::CLjHandler::sendPost);

    performTask();
  }

  void CLjManager::sendComment(QSharedPointer<core::CComment> comment)
  {
    m_ljHandler->addCommentToOutputBuffer(comment);
    m_taskQueue.enqueue(&BlogService::CLjHandler::sendComment);

    performTask();
  }

  bool CLjManager::isWait()
  {
    return m_taskQueue.isEmpty();
  }

  bool CLjManager::isLoadComment()
  {
    return m_taskQueue.contains(&BlogService::CLjHandler::loadComments);
  }
} // namespace BlogService

/* ===[ End of file $HeadURL: svn+ssh://kua@osll.spb.ru/svn/scblog/trunk/src/blogservice/src/LjManager.cpp $ ]=== */
