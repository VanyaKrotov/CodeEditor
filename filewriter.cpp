#include "filewriter.h"

FileWriter::FileWriter(QObject *parent) : QObject(parent) {

}

void FileWriter::setProps(TextEditor *textEditor, QString path) {
    this->textEditor = textEditor;
    this->path = path;
}

void FileWriter::writeFile() {
    QFile file(path);

    if(file.open(QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream writeStream(&file);

        writeStream.setCodec(QTextCodec::codecForName("UTF-8"));

        writeStream << textEditor->toPlainText();

        file.flush();
        file.close();
    }

    emit endWrite(path);
}
