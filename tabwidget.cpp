#include "tabwidget.h"
#include <QDebug>

TabWidget::TabWidget(QWidget *parent): QTabWidget(parent) {
    setTabsClosable(true);
    setMovable(true);

    connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab_trigger(int)));
}

TabWidget::~TabWidget(){
    if(!tabs.length()) {
       return;
    }
}

void TabWidget::closeTab_trigger(const int &tabIndex) {
    QString closedFilePath = closeTab(tabIndex);

    qDebug() << "close file with" << closedFilePath << endl;
}

QString TabWidget::closeTab(const int &tabIndex){
    if(!tabs.length()) {
       return nullptr;
    }

    if(!tabs[tabIndex]->isSaved){
        int response =
                QMessageBox::warning(this,
                                     tr("Сохранение файла"),
                                     QString("Файл \""+ tabText(tabIndex) +"\" был модифицирован.\nСохранить изменения?"),
                                     QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel, QMessageBox::Save);
        switch (response) {
          case QMessageBox::Save:
            qDebug() << "save" << endl;
//              tabs[tabIndex]->SaveFile("");
              break;
          case QMessageBox::Discard:
            qDebug() << "Discard" << endl;
              break;
          case QMessageBox::Cancel:
                return nullptr;
          default:
              break;
        }
    }

    QString closedFilePath = tabs[tabIndex]->fileinfo->filePath();

    try {
        removeTab(tabIndex);
        tabs.removeAt(tabIndex);
    } catch (...) {
        return nullptr;
    }

    return closedFilePath;
}

void TabWidget::closeAllTabs(){
    int closedTabIndex = count() - 1;

    while (closedTabIndex != -1) {
        closeTab(closedTabIndex);

        closedTabIndex--;
    }
}

void TabWidget::createTab(QString &filePath){

    QFileInfo *fileInfo = new QFileInfo(filePath);

    TabPage * tabPage = new TabPage(fileInfo, this);

    tabs.append(tabPage);
    addTab(tabPage, fileInfo->fileName());
}

void TabWidget::createNewTab(){
    TabPage * tabPage = new TabPage(nullptr, this);

    tabs.append(tabPage);
    addTab(tabPage, "Новый файл");
}

TabPage * TabWidget::getCurrentTab(){
    if(!tabs.length()){
        return nullptr;
    }

    return tabs[currentIndex()];
}
