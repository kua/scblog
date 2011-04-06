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
 * $Id: ResponseParser.cpp 50 2011-04-03 13:01:31Z kua $ 
 *
 * \file ResponseParser.cpp
 * \brief CResponseParser implementation
 *
 * File description
 *
 * PROJ: OSLL/scblog
 * ---------------------------------------------------------------- */

#include <QRegExp>
#include <QDebug>
#include "ResponseParser.h"

namespace BlogService
{

  CResponseParser::CResponseParser(QString response)
  {
    m_document.setContent(response); //! обработать false

    MEMBERS_NAMES.insert(QString("events"), BlogService::EVENTS);
    MEMBERS_NAMES.insert(QString("event"), BlogService::EVENT);
    MEMBERS_NAMES.insert(QString("subject"), BlogService::SUBJECT);
    MEMBERS_NAMES.insert(QString("url"), BlogService::URL);
    MEMBERS_NAMES.insert(QString("comments"), BlogService::COMMENTS);
    MEMBERS_NAMES.insert(QString("body"), BlogService::BODY);
  }

  QString CResponseParser::parseValue(QDomNode valueNode)
  {
    QDomNode value = valueNode.firstChild();
    QString tag = value.toElement().tagName();
    QString result = value.firstChild().toText().data();

    if(tag == "base64")
      result = tr(QByteArray::fromBase64(result.toUtf8()));

    return result;
  }

  QString CResponseParser::parseUrl(QUrl url)
  {
    QRegExp rx("/+(\\d+)+(.html)");
    int pos = rx.indexIn(url.path());

    if(pos > -1)
    {
      qDebug() << "REGEX" << rx.cap(1);
      return rx.cap(1);
    }
    return QString();
  }

  QString CResponseParser::parameter(QString parameter)
  {
    QDomElement root = m_document.documentElement();

    // if (root.tagName != "  ") return;

    QDomNode node = root.firstChild();
    while(!node.isNull())
    {
      if(node.toElement().tagName() == "member")
      {
        QDomNode nameNode = node.firstChildElement("name");
        QDomNode name = nameNode.firstChild();

        if((name.nodeType() == QDomNode::TextNode) && (name.toText().data() == parameter))
        {
          QDomNode valueNode = node.firstChildElement("value");
          return valueNode.firstChild().firstChild().toText().data();
        }

        node = node.nextSibling();
      }
      else
        node = node.firstChild();
    }

    return NULL;
  }

} // namespace BlogService

/* ===[ End of file $HeadURL: svn+ssh://kua@osll.spb.ru/svn/scblog/trunk/src/blogservice/src/ResponseParser.cpp $ ]=== */
