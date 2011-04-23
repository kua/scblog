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

/* $Id: LjManager.h 62 2011-04-23 19:53:07Z kua $ */
/*!
 * \file LjManager.h
 * \brief Header of CLjManager
 * \todo add comment here
 * \fixme move to separate thread
 *
 * File description
 *
 * PROJ: OSLL/scblog
 * ---------------------------------------------------------------- */

#ifndef _LjManager_H_C9B3F09C_021C_4531_BD07_7517F3CDC30B_INCLUDED_
#define _LjManager_H_C9B3F09C_021C_4531_BD07_7517F3CDC30B_INCLUDED_

#include <QSharedPointer>
#include "LjHandler.h"
#include "Post.h"
#include "Comment.h"

namespace BlogService
{
  /*!
   * Class description. May use HTML formatting
   *
   */
  class CLjManager: public QObject
  {
    Q_OBJECT

    QQueue<void(BlogService::CLjHandler::*)()> m_taskQueue;

    QSharedPointer<CLjHandler> m_ljHandler;

  private slots:
    void performTask();

  public:
    CLjManager(QObject *parent = 0);

    void login();
    void loadComments(core::CId postId);
    void loadPosts();
    void sendPost(QSharedPointer<core::CPost> post);
    void sendComment(QSharedPointer<core::CComment> comment);

    void setHandler(QSharedPointer<CLjHandler> handler);

  }; // class CLjManager

} // namespace BlogService

#endif //_LjManager_H_C9B3F09C_021C_4531_BD07_7517F3CDC30B_INCLUDED_
/* ===[ End of file $HeadURL: svn+ssh://kua@osll.spb.ru/svn/scblog/trunk/src/blogservice/inc/LjManager.h $ ]=== */
