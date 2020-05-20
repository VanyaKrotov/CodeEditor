#ifndef FILEREADER_H
#define FILEREADER_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QDebug>
#include <QTextEdit>
#include "texteditor.h"

class FileReader : public QObject
{
    Q_OBJECT

public:
    explicit FileReader(QObject *parent = nullptr);
    void setProps(QString filePath);

    QString filePath;

public slots:
    void readFile();

signals:
    void setStateReading(int state);
    void setLineFileReading(QString textLine);
    void endRead();

};

#endif // FILEREADER_H
