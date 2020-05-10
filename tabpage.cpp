#include "tabpage.h"

TabPage::TabPage(QFileInfo *fileInfo, QWidget *parent) : QWidget(parent) {
    setLayout(new QVBoxLayout(this));
    layout()->setMargin(0);

    editor = new TextEditor(&isSaved, this);

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

    connect(&fileWriteThread, &QThread::started, &fileWriter, &FileWriter::writeFile);
    connect(&fileWriter, &FileWriter::endWrite, &fileWriteThread, &QThread::terminate);
    connect(&fileWriter, &FileWriter::endWrite, this, &TabPage::finishedFileSave);
    connect(&fileWriter, &FileWriter::endWrite, this, &TabPage::endSave);


    fileReader.moveToThread(&fileReadThread);
    fileWriter.moveToThread(&fileWriteThread);
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

TextEditor *TabPage::getTextEditor() {
    return editor;
}

void TabPage::openFile(){
    editor->setDisabled(true);

    fileReader.setProps(fileinfo->filePath());
    fileReadThread.start();
}

void TabPage::saveFile(const bool saveType) {
    QString filePath;
    QSettings settings(CONFIGURATION_FILE, QSettings::IniFormat);
    QString lastOpenedDir = settings.value("LastOpenedDir").toString();

    if(saveType == SAVE_AS || !fileinfo) {
        filePath = QFileDialog::getSaveFileName(this,
                                                QObject::tr("Укажите путь и имя файла"),
                                                lastOpenedDir + "/" + editor->toPlainText().split(" ")[0],
                                                QObject::tr("Файл (*.txt);;Все файлы (*.*)"));
    } else {
        filePath = fileinfo->filePath();
    }

    if(filePath.isEmpty()) {
        return;
    }

    editor->setDisabled(true);

    fileWriter.setProps(editor, filePath);
    fileWriteThread.start();
}

void TabPage::setLineFile(QString line){
    editor->setPlainText(line);
}

void TabPage::setProgressReadFile(int value){
    progressBar->setValue(value);
}

void TabPage::finishedFileOpen(){
    editor->setDisabled(false);
    editor->moveCursor(QTextCursor::Start);
    progressBar->hide();
}

void TabPage::finishedFileSave(QString pathSaved){
    editor->setDisabled(false);
    editor->setFocus();

    fileinfo = new QFileInfo(pathSaved);
    isSaved = true;

    emit updateTabData(this);
}
