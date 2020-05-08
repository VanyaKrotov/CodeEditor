#include "tabpage.h"

TabPage::TabPage(QFileInfo *fileInfo, QWidget *parent) : QWidget(parent) {
    setLayout(new QVBoxLayout(this));
    layout()->setMargin(0);

    editor = new QTextEdit(this);
    editor->setObjectName("cod" + this->parent()->objectName());
    editor->setFocus();

    progressBar = new QProgressBar(this);
    progressBar->setTextVisible(false);
    progressBar->setValue(0);

    this->layout()->addWidget(progressBar);
    this->layout()->addWidget(editor);

    isSaved = true;
    this->fileinfo = fileInfo;

    connect(&fileReadThread, &QThread::started, &fileReader, &FileReader::readFile);
    connect(&fileReader, &FileReader::endRead, &fileReadThread, &QThread::terminate);
    connect(&fileReader, &FileReader::endRead, this, &TabPage::finishedFileOpen);
    connect(&fileReader, &FileReader::setStateReading, this, &TabPage::setProgressReadFile);
    connect(&fileReader, &FileReader::setLineFileReading, this, &TabPage::setLineFile);

    fileReader.moveToThread(&fileReadThread);
}

TabPage::~TabPage(){
    if(editor) {
        editor->deleteLater();
    }

    fileReader.deleteLater();
    fileReadThread.deleteLater();
    progressBar->deleteLater();

    delete fileinfo;
}

QString TabPage::getEditorContent(){
    return editor->toPlainText();
}

void TabPage::openFile(){
    editor->hide();

    fileReader.setProps(fileinfo->filePath());
    fileReadThread.start();
}

void TabPage::setLineFile(QString line){
    editor->append(line);
}

void TabPage::setProgressReadFile(int value){
    progressBar->setValue(value);
}

void TabPage::finishedFileOpen(){
    editor->show();
    editor->moveCursor(QTextCursor::Start);

    progressBar->hide();
}

