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

/* $Id: Ontology.h 57 2011-04-14 14:55:31Z kua $ */
/*!
 * \file Ontology.h
 * \brief Header of COntology
 * \todo add comment here
 *
 * File description
 *
 * PROJ: OSLL/scblog
 * ---------------------------------------------------------------- */

#ifndef _Ontology_H_D7E9B056_E9D3_4890_B3F4_00E0BF5E3F79_INCLUDED_
#define _Ontology_H_D7E9B056_E9D3_4890_B3F4_00E0BF5E3F79_INCLUDED_

#include <QObject>

namespace SmartSpace
{
  static const QString ANY = "sib:any";
  static const QString TYPE = "rdf:type";

  // Smart Conference Ontology
  static const QString HAS_USER = "have";
  static const QString HAS_NAME = "is";
  static const QString EMAIL = "email";
  static const QString PHONE = "phone";
  static const QString HAS_PRESENTATION = "presents";
  static const QString URL = "url";
  static const QString PRESENTATION_TITLE = "Title";
  static const QString KEYWORDS = "Keywords";

  // Scribo Ontology
  static const QString BLOG_SERVICE_NAME ="ScLj";
  static const QString ACCOUNT_NAME ="account-sclj";
  static const QString POST ="scribo_Post";
  static const QString COMMENT ="scribo_Comment";
  static const QString TITLE = "scribo_title";
  static const QString TEXT = "scribo_text";
  static const QString HAS_COMMENT = "scribo_hasComment";
  static const QString HAS_POST = "scribo_hasPost";
  
} // namespace smartspace

#endif //_Ontology_H_D7E9B056_E9D3_4890_B3F4_00E0BF5E3F79_INCLUDED_

/* ===[ End of file $HeadURL: svn+ssh://kua@osll.spb.ru/svn/scblog/trunk/src/smartspace/inc/Ontology.h $ ]=== */
