#include "tabwidget.h"
#include <QDebug>

TabWidget::TabWidget(QWidget *parent): QTabWidget(parent) {
    setTabsClosable(true);
    setMovable(true);
    setFocus();

    connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab_trigger(int)));
    connect(tabBar(), &QTabBar::tabMoved, this, &TabWidget::tabMoved);
}

TabWidget::~TabWidget(){
    if(!tabs.length()) {
       return;
    }

    for(TabPage * tabPage: tabs){
        tabPage->destroyed();
    }
}

void TabWidget::closeTab_trigger(const int &tabIndex) {
    QString closedFilePath = closeTab(tabIndex);

    if(!closedFilePath.isEmpty()) {
        QSettings settings(CONFIGURATION_FILE, QSettings::IniFormat);

        settings.setValue("LastClosedFile", closedFilePath);
    }
}

void TabWidget::removeTabInWidget() {

}

void TabWidget::updateTabData(TabPage *tab) {
    setTabText(tabs.indexOf(tab), tab->fileinfo->fileName());
}

void TabWidget::tabMoved(const int from, const int to) {
    tabs.swapItemsAt(from, to);
}

void TabWidget::setStatusBarDataSlot(const int col, const int row, const int select) {
    emit setStatusBarData(col, row, select);
}

QString TabWidget::closeTab(const int &tabIndex){
    if(tabs.isEmpty()) {
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
              tabs[tabIndex]->saveFile(SAVE);
            break;
          case QMessageBox::Discard:
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

        delete tabs[tabIndex];
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

void TabWidget::openFile(QString path) {
    if(path.isEmpty()) {
        return;
    }

    QFileInfo *fileinfo = new QFileInfo(path);

    TabPage *tabPage = new TabPage(fileinfo, this);

    tabPage->openFile();

    connect(tabPage, &TabPage::updateTabData, this, &TabWidget::updateTabData);

    tabs.append(tabPage);
    addTab(tabPage, fileinfo->fileName());

    setCurrentIndex(tabs.length() - 1);
}

void TabWidget::setActiveThisWidget() {
    emit setActive(this);
}


TabPage * TabWidget::createTab(QString filePath){
    QFileInfo * fileInfo = nullptr;
    QString titleTab = "Undefined";

    if(!filePath.isEmpty()) {
        fileInfo = new QFileInfo(filePath);
        titleTab = fileInfo->fileName();
    }

    TabPage * tabPage = new TabPage(fileInfo, this);

    connect(tabPage, &TabPage::updateTabData, this, &TabWidget::updateTabData);

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


