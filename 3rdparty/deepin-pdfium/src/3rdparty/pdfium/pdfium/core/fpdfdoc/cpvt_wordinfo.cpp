// Copyright 2016 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "core/fpdfdoc/cpvt_wordinfo.h"

#include "core/fxcrt/fx_codepage.h"

CPVT_WordInfo::CPVT_WordInfo()
    : Word(0),
      nCharset(FX_CHARSET_ANSI),
      fWordX(0.0f),
      fWordY(0.0f),
      fWordTail(0.0f),
      nFontIndex(-1) {}

CPVT_WordInfo::CPVT_WordInfo(uint16_t word, int32_t charset, int32_t fontIndex)
    : Word(word),
      nCharset(charset),
      fWordX(0.0f),
      fWordY(0.0f),
      fWordTail(0.0f),
      nFontIndex(fontIndex) {}

CPVT_WordInfo::CPVT_WordInfo(const CPVT_WordInfo& word)
    : Word(0),
      nCharset(FX_CHARSET_ANSI),
      fWordX(0.0f),
      fWordY(0.0f),
      fWordTail(0.0f),
      nFontIndex(-1) {
  operator=(word);
}

CPVT_WordInfo::~CPVT_WordInfo() = default;

CPVT_WordInfo& CPVT_WordInfo::operator=(const CPVT_WordInfo& word) {
  if (this == &word)
    return *this;

  Word = word.Word;
  nCharset = word.nCharset;
  nFontIndex = word.nFontIndex;
  fWordX = word.fWordX;
  fWordY = word.fWordY;
  fWordTail = word.fWordTail;
  return *this;
}
