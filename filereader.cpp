#include "filereader.h"

FileReader::FileReader(QObject *parent) : QObject(parent)
{

}

void FileReader::setProps(QString filePath, QTextEdit *textEdit){
    this->filePath = filePath;
    this->textEdit = textEdit;
}

void FileReader::readFile(){

    qDebug() << filePath << endl;

    int i = 0;
    int maxCount = 70000;
    while(i++ < maxCount) {
        qDebug() << i << endl;

        emit setStateReading((int) (i / (maxCount / 100)));
    }

    emit endRead();

//    QFile file(filePath);
//    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
//    {
//        return;
//    }
//    QTextStream stream(&file);

//    stream.setCodec(QTextCodec::codecForName("UTF-8"));


////    this->codeedit->setPlainText(stream.readAll());
////    this->fileinfo = new QFileInfo(path);
////    this->type = fileinfo->fileName().split('.').last();
////    this->IsSaved = true;
//    file.flush();
//    file.close();
}
