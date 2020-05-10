#ifndef WRITEFILE_H
#define WRITEFILE_H

#include <QObject>
#include <QPlainTextEdit>
#include <QDebug>
#include <QTextStream>
#include <QTextCodec>
#include <QFileInfo>
#include <QFile>

#include "texteditor.h"

class FileWriter: public QObject
{
    Q_OBJECT
public:
    FileWriter(QObject *parent = nullptr);
    void setProps(TextEditor * textEditor, QString path);

public slots:
    void writeFile();

signals:
    void endWrite(QString path);

private:
    TextEditor *textEditor;
    QString path;
};

#endif // WRITEFILE_H
