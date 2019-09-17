/**
 *
 * Copyright (C) 2019  The Talipot developers
 *
 * Talipot is a fork of Tulip, created by David Auber
 * and the Tulip development Team from LaBRI, University of Bordeaux
 *
 * See the AUTHORS file at the top-level directory of this distribution
 * License: GNU General Public License version 3, or any later version
 * See top-level LICENSE file for more information
 *
 */

#ifndef PARENMATCHERHIGHLIGHTER_H_
#define PARENMATCHERHIGHLIGHTER_H_

#include <QSyntaxHighlighter>
#include <QVector>

class QTextDocument;

struct ParenInfo {
  char character;
  int position;

  bool operator<(const ParenInfo &info) const {
    return position < info.position;
  }
};

class ParenInfoTextBlockData : public QTextBlockUserData {

  QVector<ParenInfo> _parenInfo;

public:
  ParenInfoTextBlockData();

  QVector<ParenInfo> parens();
  void insert(const ParenInfo &parenInfo);
  void sortParenInfo();
};

class ParenMatcherHighlighter : public QSyntaxHighlighter {

  QVector<char> _leftParensToMatch;
  QVector<char> _rightParensToMatch;

public:
  explicit ParenMatcherHighlighter(QTextDocument *parent = nullptr);

protected:
  void highlightBlock(const QString &text) override;
};

#endif /* PARENMATCHERHIGHLIGHTER_H_ */
