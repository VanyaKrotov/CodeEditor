#include "filereader.h"

FileReader::FileReader(QObject *parent) : QObject(parent) {

}

void FileReader::setProps(QString filePath){
    this->filePath = filePath;
}

void FileReader::readFile(){
    QFile file(filePath);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text) && (file.exists())) {
        QTextStream stream(&file);

//        stream.setCodec(QTextCodec::codecForName("UTF-8"));
        QString content = stream.readAll();

        emit setLineFileReading(content);

        file.flush();
        file.close();
    }

    emit endRead();
}
