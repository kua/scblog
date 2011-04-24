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
 * $Id: RequestCreator.cpp 64 2011-04-24 16:27:13Z kua $ 
 *
 * \file RequestCreator.cpp
 * \brief CRequestCreator implementation
 *
 * File description
 *
 * PROJ: OSLL/scblog
 * ---------------------------------------------------------------- */

#include <QDebug>
#include <QDomDocument>
#include "RequestCreator.h"

namespace BlogService
{
  QString CRequestCreator::createRequest(QString method, QMap<QString, QString> parameters)
  {
    QDomDocument doc;

    static const int indent = 4;

    QDomElement methodCall = doc.createElement("methodCall");
    QDomElement methodName = doc.createElement("methodName");
    QDomElement params = doc.createElement("params");
    QDomElement param = doc.createElement("param");
    QDomElement value = doc.createElement("value");
    QDomElement xmlStruct = doc.createElement("struct");

    QDomText methodTextName = doc.createTextNode(method);

    doc.appendChild(methodCall);
    methodCall.appendChild(methodName);
    methodName.appendChild(methodTextName);
    methodCall.appendChild(params);
    params.appendChild(param);
    param.appendChild(value);
    value.appendChild(xmlStruct);

    for(QMap<QString, QString>::const_iterator it = parameters.begin(); it != parameters.end(); ++it)
    {
      QDomElement member = doc.createElement("member");
      QDomElement name = doc.createElement("name");
      QDomElement value = doc.createElement("value");
      QDomElement xmlString = doc.createElement("string");

      xmlStruct.appendChild(member);
      member.appendChild(name);

      QDomText parameterName = doc.createTextNode(it.key());
      name.appendChild(parameterName);

      member.appendChild(value);
      value.appendChild(xmlString);

      QDomText parameterValue = doc.createTextNode(it.value());
      xmlString.appendChild(parameterValue);
    }
    QString result;

    QTextStream os(&result);
    doc.save(os, indent);

    return result;
  }
} // namespace BlogService

/* ===[ End of file $HeadURL: svn+ssh://kua@osll.spb.ru/svn/scblog/trunk/src/blogservice/src/RequestCreator.cpp $ ]=== */
