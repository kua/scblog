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
 * $Id: SSHandler.cpp 58 2011-04-16 19:11:21Z kua $ 
 *
 * \file SSHandler.cpp
 * \brief CSSHandler implementation
 *
 * File description
 *
 * PROJ: OSLL/scblog
 * ---------------------------------------------------------------- */

#include <QDebug>
#include <QTimer>
#include <ostream>
#include "triple.h"
#include "SSHandler.h"
#include "Ontology.h"

namespace SmartSpace
{
  CSSHandler::CSSHandler(QString sibUri, QObject *parent) :
    QObject(parent)
  {
    m_node = QSharedPointer<QWhiteBoardNode> (new QWhiteBoardNode);
    join(sibUri);

  }

  void CSSHandler::join(QString sibUri)
  {
    int result = m_node->join(sibUri);
    m_node->joinCompleteRcvd(result);

    qDebug() << ((result >= 0) ? "join success" : "join failed");
  }

  void CSSHandler::insert(Triple* triple)
  {
    QList<Triple*> list;
    list.push_back(triple);

    insert(list);
  }

  void CSSHandler::insert(QList<Triple*> triplets)
  {
    int result = m_node->insert(triplets);

    qDebug() << ((result >= 0) ? "insert success" : "insert failed");
  }

  void CSSHandler::remove(QList<Triple*> list)
  {
    int result = m_node->remove(list);

    qDebug() << ((result >= 0) ? "remove success" : "remove failed");
  }

  Triple* CSSHandler::createDefaultTriple(QString subject, QString predicat, QString object)
  {
    return new Triple(TripleElement(subject, TripleElement::ElementTypeURI), TripleElement(predicat),
                      TripleElement(object, TripleElement::ElementTypeURI));
  }

  Triple* CSSHandler::createLiteralTriple(QString subject, QString predicat, QString object)
  {
    return new Triple(TripleElement(subject, TripleElement::ElementTypeURI), TripleElement(predicat),
                      TripleElement(object, TripleElement::ElementTypeLiteral));
  }

  void CSSHandler::query(Triple* triple)
  {
    m_queryList.clear();
    m_queryList.push_back(triple);

    query();
  }

  void CSSHandler::query()
  {
    QSharedPointer<TemplateQuery> query = QSharedPointer<TemplateQuery>(new TemplateQuery(m_node.data()));
    query->setObjectName("query");

    if(!m_queries.contains(query->objectName()))
      m_queries[query->objectName()] = query;

    for(QList<Triple *>::iterator it = m_queryList.begin(); it != m_queryList.end(); ++it)
    {
      qDebug() << (*it)->subject().node();
      qDebug() << (*it)->predicate().node();
      qDebug() << (*it)->object().node();
    }

    connect(query.data(), SIGNAL(finished(int)), this, SLOT(queryDone(int)));
    query->query(m_queryList);

    while(m_queryList.count())
      delete m_queryList.takeFirst();
  }

  void CSSHandler::queryDone(int success)
  {
    if(m_queries.contains(sender()->objectName()))
    {
      QSharedPointer<TemplateQuery> templateQuery = m_queries[sender()->objectName()];

      if(success == 0)
      {
        QList<Triple *> results = templateQuery->results();

        QList<Triple *>::iterator it;

        for(it = results.begin(); it != results.end(); ++it)
        {
          qDebug() << (*it)->subject().node();
          qDebug() << (*it)->predicate().node();
          qDebug() << (*it)->object().node();
        }

        postProcess(results);

        m_queries.remove(sender()->objectName());
      }
    }
  }

  QSharedPointer<TemplateSubscription> CSSHandler::getSubscription(QString name)
  {
    if(m_subscriptions.contains(name))
      return m_subscriptions[name];

    return QSharedPointer<TemplateSubscription>();
  }

  QSharedPointer<TemplateQuery> CSSHandler::getQuery(QString name)
  {
    if(m_queries.contains(name))
      return m_queries[name];

    return QSharedPointer<TemplateQuery>();
  }

  QSharedPointer<TemplateSubscription> CSSHandler::creatreSubscription(QString name)
  {
    QSharedPointer<TemplateSubscription> subscription = QSharedPointer<TemplateSubscription>(new TemplateSubscription(m_node.data()));

    subscription->setObjectName(name);

    if(!m_subscriptions.contains(subscription->objectName()))
      m_subscriptions[subscription->objectName()] = subscription;

    return subscription;
  }

  QSharedPointer<TemplateQuery> CSSHandler::creatreQuery(QString name)
  {
    QSharedPointer<TemplateQuery> query = QSharedPointer<TemplateQuery>(new TemplateQuery(m_node.data()));

    query->setObjectName(name);

    if(!m_queries.contains(query->objectName()))
      m_queries[query->objectName()] = query;

    return query;
  }

  void CSSHandler::deleteQuery(QString name)
  {
    if(m_queries.contains(name))
      m_queries.remove(name);
  }
} // namespace SmartSpace

/* ===[ End of file $HeadURL: svn+ssh://kua@osll.spb.ru/svn/scblog/trunk/src/smartspace/src/SSHandler.cpp $ ]=== */
