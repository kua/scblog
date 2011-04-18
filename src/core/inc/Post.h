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

/* $Id: Post.h 59 2011-04-18 14:14:17Z kua $ */
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

#include <QTextStream>
#include "triple.h"
#include "BlogObject.h"
#include "Comment.h"

namespace core
{
  /*!
   * Class description. May use HTML formatting
   *
   */
  class CPost: public IBlogObject
  {
    Q_OBJECT

  public:

    CPost();
    CPost(const CPost& obj);
    CPost(QString title, QString text);
    ~CPost(){}
    CPost& operator=(const CPost& obj);
    bool operator==(const CPost& obj) const;

    virtual QSharedPointer<CId> parentId();
    virtual void generateSsId();
    virtual QList<Triple *> triplets() const;

    friend QTextStream& operator<<(QTextStream& os, const CPost& post);
  }; // class CPost
} // namespace core

inline uint qHash(const core::CPost& post)
{
   return qHash(post.title());
}

#endif //_Post_H_0FB03DCF_B900_4D9B_9B00_362B2E2A23D9_INCLUDED_
/* ===[ End of file $HeadURL: svn+ssh://kua@osll.spb.ru/svn/scblog/trunk/src/core/inc/Post.h $ ]=== */
