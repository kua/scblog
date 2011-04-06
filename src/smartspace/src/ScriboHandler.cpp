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
 * $Id: ScriboHandler.cpp 52 2011-04-05 14:04:15Z kua $ 
 *
 * \file ScriboHandler.cpp
 * \brief CScriboHandler implementation
 *
 * File description
 *
 * PROJ: OSLL/scblog
 * ---------------------------------------------------------------- */

#include <QDebug>
#include <QTimer>
#include "Ontology.h"
#include "ScriboHandler.h"
#include "BlogObject.h"

namespace SmartSpace
{
  static QHash<QString, void(core::IBlogObject::*)(QString)> PREDICATES;

  CScriboHandler::CScriboHandler(QString sibUri, QString accountName, QObject *parent) :
    CSSHandler(sibUri, parent), m_accountName(accountName)
  {
    createPredicatesHash();
    m_factory = &CScriboHandler::createPost;
  }

  void CScriboHandler::postProcess(QList<Triple *> triples)
  {
    if(m_postProcessor != NULL)
    {
      (this->*m_postProcessor)(triples);

      QTimer::singleShot(10, this, SLOT(query()));
    }
  }

  void CScriboHandler::createPredicatesHash()
  {
    PREDICATES.insert(TEXT, &core::IBlogObject::setText);
    PREDICATES.insert(TITLE, &core::IBlogObject::setTitle);
  }

  void CScriboHandler::loadComments(QString postId)
  {
    m_postProcessor = &CScriboHandler::processBlogObjectsList;
    m_factory = &CScriboHandler::createComment;

    query(createDefaultTriple(postId, HAS_COMMENT, ANY));
  }

  void CScriboHandler::loadPosts()
  {
    m_postProcessor = &CScriboHandler::processBlogObjectsList;
    m_factory = &CScriboHandler::createPost;

    query(createDefaultTriple(m_accountName, HAS_POST, ANY));
  }

  core::IBlogObject* CScriboHandler::createComment(QString id)
  {
    return new core::CComment(id);
  }

  core::IBlogObject* CScriboHandler::createPost(QString id)
  {
    return new core::CPost(id);
  }

  void CScriboHandler::processBlogObjectsList(QList<Triple *> triple)
  {
    qDebug() << "processBlogObjectsList";

    m_queryList.clear();

    for(QList<Triple *>::iterator it = triple.begin(); it != triple.end(); ++it)
    {
      QString id = (*it)->object().node();

      m_blogObjects.insert(id, (this->*m_factory)(id));

      m_queryList.push_back(createDefaultTriple(id, TITLE, ANY));
      m_queryList.push_back(createDefaultTriple(id, TEXT, ANY));
    }

    m_postProcessor = &CScriboHandler::processBlogObjects;
  }

  void CScriboHandler::processBlogObjects(QList<Triple *> triple)
  {
    qDebug() << "processBlogObjects";

    m_queryList.clear();

    for(QList<Triple *>::iterator it = triple.begin(); it != triple.end(); ++it)
    {
      QString id = (*it)->subject().node();

      void (core::IBlogObject::*function)(QString) = PREDICATES[(*it)->predicate().node()];
      (m_blogObjects[id]->*function)((*it)->object().node());
    }

   /* QSet<core::CPost> posts;

    for(QMap<QString, core::IBlogObject*>::const_iterator it = m_blogObjects.begin(); it != m_blogObjects.end(); ++it)
    {
      posts.insert(*qobject_cast<core::CPost*>(it.value()));

      QString s;
      QTextStream os(&s);
      os << *(it.value());
      qDebug() << s;
    }*/

    m_postProcessor = NULL;

    //emit loadPostsDone(posts);
  }

} // namespace smartspace

/* ===[ End of file $HeadURL: svn+ssh://kua@osll.spb.ru/svn/scblog/trunk/src/smartspace/src/ScriboHandler.cpp $ ]=== */
