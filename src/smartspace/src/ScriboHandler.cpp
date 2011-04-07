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
 * $Id: ScriboHandler.cpp 53 2011-04-07 13:11:18Z kua $ 
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
    m_scriboObject = SCRIBO_COMMENT;

    query(createDefaultTriple(postId, HAS_COMMENT, ANY));
  }

  void CScriboHandler::loadPosts()
  {
    m_postProcessor = &CScriboHandler::processBlogObjectsList;
    m_scriboObject = SCRIBO_POST;

    query(createDefaultTriple(m_accountName, HAS_POST, ANY));
  }

  void CScriboHandler::processBlogObjectsList(QList<Triple *> triple)
  {
    qDebug() << "processBlogObjectsList";

    m_queryList.clear();

    for(QList<Triple *>::iterator it = triple.begin(); it != triple.end(); ++it)
    {
      QString id = (*it)->object().node();
      core::IBlogObject* obj;

      switch (m_scriboObject) {
        case SCRIBO_COMMENT:
          obj = new core::CComment(id);
          break;
        case SCRIBO_POST:
          obj = new core::CPost(id);
          break;
        default:
          break;
      }

      m_blogObjects.insert(id, obj);

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

    switch (m_scriboObject) {
      case SCRIBO_COMMENT:
        emitCommentSignal();
        break;
      case SCRIBO_POST:
        emitPostSignal();
        break;
      default:
        break;
    }

    m_postProcessor = NULL;
  }

  void CScriboHandler::emitPostSignal()
  {
    QSet<core::CPost> posts;

    for(QMap<QString, core::IBlogObject*>::const_iterator it = m_blogObjects.begin(); it != m_blogObjects.end(); ++it)
    {
      QString s;
      QTextStream os(&s);
      os << *it.value();
      qDebug() << s;

      posts.insert(*qobject_cast<core::CPost*>(it.value()));
    }

    emit loadPostsDone(posts);
  }

  void CScriboHandler::emitCommentSignal()
  {
    QSet<core::CComment> comments;

    for(QMap<QString, core::IBlogObject*>::const_iterator it = m_blogObjects.begin(); it != m_blogObjects.end(); ++it)
    {
      QString s;
      QTextStream os(&s);
      os << *it.value();
      qDebug() << s;

      comments.insert(*qobject_cast<core::CComment*>(it.value()));
    }

    emit loadCommentsDone(comments);
  }
} // namespace smartspace

/* ===[ End of file $HeadURL: svn+ssh://kua@osll.spb.ru/svn/scblog/trunk/src/smartspace/src/ScriboHandler.cpp $ ]=== */
