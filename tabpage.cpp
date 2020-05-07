#include "tabpage.h"

TabPage::TabPage(QFileInfo *fileInfo, QWidget *parent) : QWidget(parent)
{
    setLayout(new QHBoxLayout(this));
    layout()->setMargin(0);

    editor = new QTextEdit(this);
    editor->setObjectName("cod" + this->parent()->objectName());

    this->layout()->addWidget(editor);

    isSaved = true;
    this->fileinfo = fileInfo;
}

TabPage::~TabPage(){
    if(editor)
        delete editor;

    delete this->fileinfo;
}


