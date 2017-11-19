/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */

#include <algorithm>

#include "tulip/ParenMatcherHighlighter.h"

ParenInfoTextBlockData::ParenInfoTextBlockData() {}

QVector<ParenInfo> ParenInfoTextBlockData::parens() {
  return _parenInfo;
}

void ParenInfoTextBlockData::insert(const ParenInfo &parenInfo) {
  _parenInfo.append(parenInfo);
}

void ParenInfoTextBlockData::sortParenInfo() {
  std::sort(_parenInfo.begin(), _parenInfo.end());
}

ParenMatcherHighlighter::ParenMatcherHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent) {
  _leftParensToMatch.append('(');
  _leftParensToMatch.append('[');
  _leftParensToMatch.append('{');
  _rightParensToMatch.append(')');
  _rightParensToMatch.append(']');
  _rightParensToMatch.append('}');
}

void ParenMatcherHighlighter::highlightBlock(const QString &text) {
  ParenInfoTextBlockData *data = new ParenInfoTextBlockData;

  QString modifiedText = text;
  QRegExp dblQuotesRegexp("\"[^\"]*\"");
  QRegExp simpleQuotesRegexp("'[^']*'");

  int pos = dblQuotesRegexp.indexIn(modifiedText);

  while (pos != -1) {
    for (int i = pos; i < pos + dblQuotesRegexp.matchedLength(); ++i) {
      modifiedText[i] = ' ';
    }

    pos = dblQuotesRegexp.indexIn(modifiedText, pos + dblQuotesRegexp.matchedLength());
  }

  pos = simpleQuotesRegexp.indexIn(modifiedText);

  while (pos != -1) {
    for (int i = pos; i < pos + simpleQuotesRegexp.matchedLength(); ++i) {
      modifiedText[i] = ' ';
    }

    pos = simpleQuotesRegexp.indexIn(modifiedText, pos + simpleQuotesRegexp.matchedLength());
  }

  for (char i : _leftParensToMatch) {
    int leftPos = modifiedText.indexOf(i);

    while (leftPos != -1) {
      ParenInfo info;
      info.character = i;
      info.position = currentBlock().position() + leftPos;
      data->insert(info);
      leftPos = modifiedText.indexOf(i, leftPos + 1);
    }
  }

  for (char i : _rightParensToMatch) {
    int rightPos = modifiedText.indexOf(i);

    while (rightPos != -1) {
      ParenInfo info;
      info.character = i;
      info.position = currentBlock().position() + rightPos;
      data->insert(info);
      rightPos = modifiedText.indexOf(i, rightPos + 1);
    }
  }

  data->sortParenInfo();
  setCurrentBlockUserData(data);
}
