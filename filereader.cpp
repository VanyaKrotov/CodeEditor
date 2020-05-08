#include "filereader.h"

FileReader::FileReader(QObject *parent) : QObject(parent)
{

}

void FileReader::setProps(QString filePath){
    this->filePath = filePath;
}

void FileReader::readFile(){
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text) && (file.exists()))
    {
        QTextStream stream(&file);

        stream.setCodec(QTextCodec::codecForName("UTF-8"));

        const int maxCount = file.size() / 2 / 100;
        int counReadSimbols = 0;

        while(!stream.atEnd()){
            QString line = stream.readLine();
            counReadSimbols += line.length();

            emit setLineFileReading(line);
            emit setStateReading(counReadSimbols / maxCount);
        }

        file.flush();
        file.close();
    }

    emit endRead();
}
