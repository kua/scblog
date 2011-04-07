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

/* $Id: ScriboHandler.h 53 2011-04-07 13:11:18Z kua $ */
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

    QMap<QString, core::IBlogObject*> m_blogObjects;
    void (CScriboHandler::*m_postProcessor)(QList<Triple*> );
    QString m_accountName;
    ScriboObject m_scriboObject;

    void createPredicatesHash();

    virtual void postProcess(QList<Triple *> triples);

    void processBlogObjectsList(QList<Triple *> triple);
    void processBlogObjects(QList<Triple *> triple);

    void emitCommentSignal();
    void emitPostSignal();

  signals:
    void loadPostsDone(QSet<core::CPost> posts);
    void loadCommentsDone(QSet<core::CComment> comments);

  public:
    CScriboHandler(QString sibUri, QString accountName, QObject *parent = 0);
    ~CScriboHandler(){};

    void loadComments(QString postId);
    void loadPosts();
    
  }; // class CScriboHandler
} // namespace smartspace

#endif //_ScriboHandler_H_BAAEF3DD_2A17_47A6_B53E_481B31FA3BCA_INCLUDED_
/* ===[ End of file $HeadURL: svn+ssh://kua@osll.spb.ru/svn/scblog/trunk/src/smartspace/inc/ScriboHandler.h $ ]=== */
