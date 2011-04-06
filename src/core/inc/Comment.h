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

/* $Id: Comment.h 52 2011-04-05 14:04:15Z kua $ */
/*!
 * \file Comment.h
 * \brief Header of CComment
 * \todo add comment here
 *
 * File description
 *
 * PROJ: OSLL/scblog
 * ---------------------------------------------------------------- */

#ifndef _Comment_H_31BEB193_A0F6_4E28_BABF_C9FC82CD3415_INCLUDED_
#define _Comment_H_31BEB193_A0F6_4E28_BABF_C9FC82CD3415_INCLUDED_

#include <QObject>
#include "BlogObject.h"

namespace core
{
 /*!
   * Class description. May use HTML formatting
   *
   */
  class CComment:
    public IBlogObject
  {
    Q_OBJECT

    virtual QString generateId();

  public:
    CComment(QString id = QString());
    CComment(QString title, QString text, QString id = QString());
    CComment(const CComment& obj);
    
    ~CComment() {};
    CComment& operator=(const CComment& obj);
    bool operator==(const CComment& obj) const;

    friend QTextStream& operator<< (QTextStream& os, const CComment& comment);
  }; // class CComment
} // namespace core

static uint qHash(const core::CComment& comment)
{
   return qHash(comment.title());
}

#endif //_Comment_H_31BEB193_A0F6_4E28_BABF_C9FC82CD3415_INCLUDED_

/* ===[ End of file $HeadURL: svn+ssh://kua@osll.spb.ru/svn/scblog/trunk/src/core/inc/Comment.h $ ]=== */
