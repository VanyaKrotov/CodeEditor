#ifndef FILEREADER_H
#define FILEREADER_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QDebug>
#include <QTextEdit>

class FileReader : public QObject
{
    Q_OBJECT

public:
    explicit FileReader(QObject *parent = nullptr);
    void setProps(QString filePath, QTextEdit *textEdit);

    QString filePath;
    QTextEdit *textEdit;

public slots:
    void readFile();

signals:
    void setStateReading(int state);
    void endRead();

};

#endif // FILEREADER_H
