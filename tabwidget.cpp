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
    if(tabs.isEmpty()) {
       return nullptr;
    }

    if(!tabs[tabIndex]->isSaved || tabs[tabIndex]->getEditorContent().length()){
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

    QString pathClosedFile = tabs[tabIndex]->fileinfo ? tabs[tabIndex]->fileinfo->filePath() : nullptr;

    try {
        removeTab(tabIndex);
        tabs.removeAt(tabIndex);
    } catch (...) {
        return nullptr;
    }

    return pathClosedFile;
}

void TabWidget::closeAllTabs(){
    int closedTabIndex = count() - 1;

    while (closedTabIndex != -1) {
        closeTab(closedTabIndex);

        closedTabIndex--;
    }
}

TabPage * TabWidget::createTab(QString filePath){
    QFileInfo * fileInfo = nullptr;
    QString titleTab = "Undefined";

    if(!filePath.isEmpty()) {
        fileInfo = new QFileInfo(filePath);
        titleTab = fileInfo->fileName();
    }

    TabPage * tabPage = new TabPage(fileInfo, this);

    tabs.append(tabPage);
    addTab(tabPage, titleTab);

    setCurrentIndex(tabs.length() - 1);

    return tabPage;
}

TabPage * TabWidget::getCurrentTab(){
    if(!tabs.length()){
        return nullptr;
    }

    return tabs[currentIndex()];
}
