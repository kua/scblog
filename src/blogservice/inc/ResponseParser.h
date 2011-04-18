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

/* $Id: ResponseParser.h 59 2011-04-18 14:14:17Z kua $ */
/*!
 * \file ResponseParser.h
 * \brief Header of CResponseParser
 * \todo add comment here
 *
 * File description
 *
 * PROJ: OSLL/scblog
 * ---------------------------------------------------------------- */

#ifndef _ResponseParser_H_5B6D8F01_4DBC_4B0A_8665_87D766AE8633_INCLUDED_
#define _ResponseParser_H_5B6D8F01_4DBC_4B0A_8665_87D766AE8633_INCLUDED_

#include <QObject>
#include <QDomDocument>
#include <QUrl>
#include <QSet>
#include <QMap>
#include <QSharedPointer>
#include "Post.h"
#include "Comment.h"

namespace BlogService
{
  enum NAMES
  {
    EVENTS,
    EVENT,
    SUBJECT,
    URL,
    COMMENTS,
    BODY,
    ID,
    POST_DATE,
    COMMENT_DATE,
    CHILD_COMMENT
  };

  /*!
   * Class description. May use HTML formatting
   *
   */
  class CResponseParser: public QObject
  {
  Q_OBJECT

    QMap<QString, NAMES> MEMBERS_NAMES;

    QDomDocument m_document;

    QString parseValue(QDomNode valueNode);

  public:
    CResponseParser(QString& response);

    template<class Element>
    QList<QSharedPointer<Element> > parseElements()
    {
      QDomElement root = m_document.documentElement();
      QDomNode node = root.firstChild();

      return parseElements<Element>(node);
    }

    template<class Element>
    QList<QSharedPointer<Element> > parseElements(QDomNode node)
    {
      QList<QSharedPointer<Element> > elements;
      QSharedPointer<Element> element = QSharedPointer<Element> (new Element);

      QDomNode eventsSeparator;
      QString id;

      while(!node.isNull())
      {
        if(node.toElement().tagName() == "data")
          eventsSeparator = node.firstChild();

        if(node.toElement().tagName() == "member")
        {
          QDomNode value = node.firstChildElement("value");
          QDomNode name = node.firstChildElement("name").firstChild();

          if(name.nodeType() == QDomNode::TextNode)
          {
            QString memberName = name.toText().data();

            if(MEMBERS_NAMES.contains(memberName))
              switch(MEMBERS_NAMES.value(memberName))
              {
                case COMMENTS:
                case EVENTS:
                  node = node.firstChild();
                  break;
                case BODY:
                case EVENT:
                  element->setText(parseValue(value));
                  break;
                case SUBJECT:
                  element->setTitle(parseValue(value));
                  break;
                case URL:
                  element->id()->setLjId(parseUrl(QUrl(parseValue(value))));
                  break;
                case ID:
                  id = parseValue(value);
                  element->id()->setLjId(id);
                  break;
                case POST_DATE:
                case COMMENT_DATE:
                  elements.push_back(element);
                  element = QSharedPointer<Element> (new Element);
                  node = eventsSeparator;
                  eventsSeparator = eventsSeparator.nextSibling();
                  break;
                case CHILD_COMMENT:
                 {
                  QList<QSharedPointer<Element> > childElements = parseElements<Element>(value);
                  foreach(QSharedPointer<Element> childElement, childElements)
                    if (!childElement->parentId()->isLjIdSet())
                      childElement->parentId()->setLjId(id);
                  elements.append(childElements);
                 }
                 break;
                default:
                  break;
              }
          }
          node = node.nextSibling();
        }
        else
          node = node.firstChild();
      }
      return elements;
    }
    ;

    QString parameter(QString parameter);
    static QString parseUrl(QUrl url);

  }; // class CResponseParser

} // namespace BlogService

#endif //_ResponseParser_H_5B6D8F01_4DBC_4B0A_8665_87D766AE8633_INCLUDED_
/* ===[ End of file $HeadURL: svn+ssh://kua@osll.spb.ru/svn/scblog/trunk/src/blogservice/inc/ResponseParser.h $ ]=== */
