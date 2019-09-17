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

#ifndef FORM_POST_H
#define FORM_POST_H

#include <QObject>
#include <QStringList>

class QNetworkReply;

class FormPost : public QObject {
  Q_OBJECT
public:
  FormPost();

  QString userAgent();
  void setUserAgent(QString agent);

  QString referer();
  void setReferer(QString ref);

  QString encoding();
  void setEncoding(QString enc);

  void addField(QString name, QString value);

  void addFile(QString fieldName, QByteArray file, QString name, QString mime);
  void addFile(QString fieldName, QString fileName, QString mime);

  QNetworkReply *postData(QString url);
  QByteArray response();

private:
  QByteArray _data;
  QString _encoding;
  QString _userAgent;
  QString _referer;
  QStringList _fieldNames;
  QStringList _fieldValues;
  QStringList _fileFieldNames;
  QStringList _fileNames;
  QStringList _fileMimes;
  QList<QByteArray> files;
  QByteArray strToEnc(QString s);

private slots:
  void readData(QNetworkReply *r);
};

#endif // FORM_POST_H
