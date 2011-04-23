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
 * $Id: ScriboHandler.cpp 62 2011-04-23 19:53:07Z kua $ 
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
  static const QString PROFILE_QUERY = "['seq',['inv','"+ACCOUNT_TYPE+"'],['inv','"+PERSON_INFORMATION+"']]";

  static const QString LOAD_POSTS = QString("LoadPosts");
  static const QString LOAD_COMMENTS = QString("LoadComments");

  static QHash<QString, void(core::IBlogObject::*)(QString)> PREDICATES;

  CScriboHandler::CScriboHandler(QString sibUri, QObject *parent) :
    CSSHandler(sibUri, parent)
  {
    createPredicatesHash();
    subscribeRefreshComments();
    subscribeSendComment();

    connect(this,SIGNAL(loadProfilesDone(QSet<QString>)),this,SLOT(updateRelatedPersonsSet(QSet<QString>)));
    //connect(this,SIGNAL(requestPersonsSignal()), this,SLOT(reguestRelatedPersons()),Qt::QueuedConnection);
  }

  void CScriboHandler::postProcess(QList<Triple *> triples)
  {
    if(m_postProcessor != NULL)
      (this->*m_postProcessor)(triples);
  }

  void CScriboHandler::createPredicatesHash()
  {
    PREDICATES.insert(TEXT, &core::IBlogObject::setText);
    PREDICATES.insert(TITLE, &core::IBlogObject::setTitle);
  }

  void CScriboHandler::loadComments(QString subject, QString predicate)
  {
    QSharedPointer<TemplateQuery> query = creatreQuery(QString(LOAD_COMMENTS + subject));

    connect(query.data(), SIGNAL(finished(int)), this, SLOT(processBlogObjectsList(int)));

    QList<Triple *> list = createTripleList(createDefaultTriple(subject, predicate, ANY));

    m_queryQueue.enqueue(qMakePair(query->objectName(),list));
    QTimer::singleShot(1,this, SLOT(scriboQuery()));
  }

  void CScriboHandler::loadPosts(QString accountName)
  {
    QSharedPointer<TemplateQuery> query = creatreQuery(QString(LOAD_POSTS + accountName));

    connect(query.data(), SIGNAL(finished(int)), this, SLOT(processBlogObjectsList(int)));

    Triple *triple = createDefaultTriple(accountName, HAS_POST, ANY);

    QList<Triple *> list;
    list.append(triple);

    query->query(list);

    while(list.count())
      delete list.takeFirst();
  }

  void CScriboHandler::processBlogObjectsList(int success)
  {
    qDebug() << "processBlogObjectsList";

    QString queryName = sender()->objectName();
    QSharedPointer<TemplateQuery> query = getQuery(queryName);

    if(!query.isNull() && (success == 0))
    {
      QList<Triple *> results = query->results();
      ScriboObject scriboObject = defineScriboObject(queryName);
      QList<Triple *> queryList;

      for(QList<Triple *>::iterator it = results.begin(); it != results.end(); ++it)
      {
        QString id = (*it)->object().node();
        QSharedPointer<core::IBlogObject> obj;

        switch(scriboObject)
        {
          case SCRIBO_COMMENT:
            obj = QSharedPointer<core::IBlogObject> (new core::CComment());
            queryList.push_back(createDefaultTriple(ANY, HAS_COMMENT, id));
            break;
          case SCRIBO_POST:
            obj = QSharedPointer<core::IBlogObject> (new core::CPost());
            break;
          default:
            break;
        }

        obj->id()->setSsId(id);

        m_blogObjects.insert(id, obj);

        queryList.push_back(createDefaultTriple(id, TITLE, ANY));
        queryList.push_back(createDefaultTriple(id, TEXT, ANY));
      }

      disconnect(query.data(), SIGNAL(finished(int)), this, SLOT(processBlogObjectsList(int)));
      connect(query.data(), SIGNAL(finished(int)), this, SLOT(processBlogObjects(int)));

      m_queryQueue.enqueue(qMakePair(queryName,queryList));
      QTimer::singleShot(1,this, SLOT(scriboQuery()));
    }
  }

  void CScriboHandler::scriboQuery()
  {
    QPair<QString, QList<Triple*> > pair = m_queryQueue.dequeue();

    QSharedPointer<TemplateQuery> query = getQuery(pair.first);
    QList<Triple*> triples = pair.second;
    query->query(triples);

    while(triples.count())
      delete triples.takeFirst();
  }

  void CScriboHandler::processBlogObjects(int success)
  {
    qDebug() << "processBlogObjects";

    QString queryName = sender()->objectName();
    QSharedPointer<TemplateQuery> query = getQuery(queryName);

    if(!query.isNull() && (success == 0))
    {
      QList<Triple *> results = query->results();

      ScriboObject scriboObject = defineScriboObject(queryName);
      QSet<QString> readyBlogObjects;

      for(QList<Triple *>::iterator it = results.begin(); it != results.end(); ++it)
      {
        if ((*it)->predicate().node() == HAS_COMMENT)
          m_blogObjects[(*it)->object().node()]->parentId()->setSsId((*it)->subject().node());
        else
        {
          QString id = (*it)->subject().node();
          readyBlogObjects.insert(id);

          void (core::IBlogObject::*function)(QString) = PREDICATES[(*it)->predicate().node()];
          (m_blogObjects[id].data()->*function)((*it)->object().node());
        }
      }

      switch(scriboObject)
      {
        case SCRIBO_COMMENT:
          emitCommentSignal(readyBlogObjects);
          break;
        case SCRIBO_POST:
          emitPostSignal(readyBlogObjects);
          break;
        default:
          break;
      }

      deleteQuery(queryName);
    }
  }

  ScriboObject CScriboHandler::defineScriboObject(QString name)
  {
    if(name.contains(LOAD_POSTS))
      return SCRIBO_POST;

    return SCRIBO_COMMENT;
  }

  void CScriboHandler::emitPostSignal(QSet<QString> readyBlogObjects)
  {
    QList<QSharedPointer<core::CPost> > posts;

    for(QSet<QString>::const_iterator it = readyBlogObjects.begin(); it != readyBlogObjects.end(); ++it)
    {
      QString s;
      QTextStream os(&s);
      os << *m_blogObjects[*it];
      qDebug() << s;

      posts.push_back(m_blogObjects.value(*it).dynamicCast<core::CPost> ());

      if (m_blogObjects.contains(*it))
        m_blogObjects.remove(*it);
    }
    emit loadPostsDone(posts);
  }

  void CScriboHandler::emitCommentSignal(QSet<QString> readyBlogObjects)
  {
    QList<QSharedPointer<core::CComment> > comments;

    for(QSet<QString>::const_iterator it = readyBlogObjects.begin(); it != readyBlogObjects.end(); ++it)
    {
      QString s;
      QTextStream os(&s);
      os << *m_blogObjects[*it];
      qDebug() << s;

      comments.push_back(m_blogObjects.value(*it).dynamicCast<core::CComment> ());
      m_blogObjects.remove(*it);
    }

    emit loadCommentsDone(comments);
  }

  void CScriboHandler::sendPosts(QList<core::CPost> posts)
  {
    foreach(core::CPost post, posts)
        insert(post.triplets());
  }

  void CScriboHandler::sendBlogObject(QSharedPointer<core::IBlogObject> object)
  {
    insert(object->triplets());
  }

  void CScriboHandler::sendBlogObjects(QList<QSharedPointer<core::IBlogObject> > objects)
  {
    foreach (QSharedPointer<core::IBlogObject> object, objects)
        sendBlogObject(object);
  }

  void CScriboHandler::subscribeRefreshComments()
  {
    QSharedPointer<TemplateSubscription> subscription = creatreSubscription(REFRESH_COMMENTS);

    connect(subscription.data(), SIGNAL(indication()), this, SLOT(refreshCommentsRequest()) );

    Triple *triple = createDefaultTriple(BLOG_SERVICE_NAME, REFRESH_COMMENTS, ACCOUNT_NAME);

    QList<Triple *> list;
    list.append(triple);

    subscription->subscribe(list);

    while(list.count())
      delete list.takeFirst();
  }

  void CScriboHandler::subscribeSendComment()
  {
    QSharedPointer<TemplateSubscription> subscription = creatreSubscription(SEND_COMMENT);

    connect(subscription.data(), SIGNAL(indication()), this, SLOT(sendCommentRequest()));

    QList<Triple *> list = createTripleList(createDefaultTriple(BLOG_SERVICE_NAME, SEND_COMMENT, ANY));

    subscription->subscribe(list);

    while(list.count())
      delete list.takeFirst();
  }

  void CScriboHandler::sendCommentRequest()
  {
    qDebug() << "CScriboHandler::sendCommentRequest";

    QSharedPointer<TemplateSubscription> subscription = getSubscription(sender()->objectName());

    if(!subscription.isNull())
    {
      wqlQuery(ACCOUNT_NAME,PROFILE_QUERY, SLOT(processProfileIds(int)));

      qDebug() << "sad";

      QList<Triple*> result = subscription->results();

      foreach(Triple* triple, result)
      {
        QString notificationId = triple->object().node();
        loadComments(notificationId, COMMENT_ID);
      }

      remove(subscription->results());
    }
  }

  void CScriboHandler::refreshCommentsRequest()
  {
    qDebug() << "CScriboHandler::refreshCommentsRequest";

    QSharedPointer<TemplateSubscription> subscription = getSubscription(sender()->objectName());

    if(!subscription.isNull())
    {
      wqlQuery(ACCOUNT_NAME,PROFILE_QUERY, SLOT(processProfileIds(int)));

      remove(subscription->results());

      emit refreshComments();
    }
  }

  void CScriboHandler::updateRelatedPersonsSet(QSet<QString> personsIds)
  {
    m_relatedPersons = personsIds;
  }

  void CScriboHandler::replyToNotification(QString predicate, QString message)
  {
    QList<Triple*> triples;

    foreach(QString person, m_relatedPersons)
    {
      QString notification = "Notification" + person;

      triples.push_back(createDefaultTriple(notification,predicate,message));
    }

    insert(triples);
  }

} // namespace smartspace

/* ===[ End of file $HeadURL: svn+ssh://kua@osll.spb.ru/svn/scblog/trunk/src/smartspace/src/ScriboHandler.cpp $ ]=== */
