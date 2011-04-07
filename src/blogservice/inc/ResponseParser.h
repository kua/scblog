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

/* $Id: ResponseParser.h 53 2011-04-07 13:11:18Z kua $ */
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
    BODY
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
    QString parseUrl(QUrl url);

  public:
    CResponseParser(QString response);

    template<class Element>
    QList<Element> parseElements()
    {
      QList<Element> elements;
      Element element;

      QDomElement root = m_document.documentElement();
      QDomNode node = root.firstChild();

      QDomNode eventsSeparator;

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
                  element.setText(parseValue(value));

                  elements.push_back(element);
                  element = Element();
                  node = eventsSeparator;
                  eventsSeparator = eventsSeparator.nextSibling();
                  break;
                case SUBJECT:
                  element.setTitle(parseValue(value));
                  break;
                case URL:
                  element.setDitemId(parseUrl(QUrl(parseValue(value))));
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

  }; // class CResponseParser

} // namespace BlogService

#endif //_ResponseParser_H_5B6D8F01_4DBC_4B0A_8665_87D766AE8633_INCLUDED_
/* ===[ End of file $HeadURL: svn+ssh://kua@osll.spb.ru/svn/scblog/trunk/src/blogservice/inc/ResponseParser.h $ ]=== */
