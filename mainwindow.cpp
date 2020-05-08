#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDir>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}


// Методы для работы с динамическими виджетами
QTreeWidget * MainWindow::createTreeWidgets(){
    QTreeWidget * treeWidget = new QTreeWidget(ui->forderViewGroups);

    treeWidget->headerItem()->setHidden(true);

    ui->forderViewGroups->layout()->addWidget(treeWidget);

    treeWidgets.append(treeWidget);

    return treeWidget;
}

QTabWidget * MainWindow::createTabWidgets(){
    TabWidget *tabWidget = new TabWidget(ui->textTabGroup);

    ui->textTabGroup->layout()->addWidget(tabWidget);

    tabWidgets.append(tabWidget);

    return tabWidget;
}


void MainWindow::readDirInTreeWidget(const QString dirPath) {
    QFileInfo dirInfo = QFileInfo(dirPath);

    QTreeWidgetItem * headerItem = new QTreeWidgetItem(treeWidgets.last());

    headerItem->setToolTip(NAME_COLUMN, dirPath);
    headerItem->setText(NAME_COLUMN, dirInfo.fileName());
    headerItem->setText(PATH_COLUMN, dirPath);

    treeWidgets.last()->setHeaderItem(headerItem);

    std::function<void(QTreeWidgetItem*)> addChild = [&addChild](QTreeWidgetItem* parent){
        QDir rootDir(parent->text(PATH_COLUMN));
        rootDir.setFilter(QDir::NoDotAndDotDot | QDir::NoSymLinks | QDir::Dirs | QDir::Files);
        rootDir.setSorting(QDir::Type);

        QFileInfoList fileInfoList = rootDir.entryInfoList();

        foreach(QFileInfo fileInfo, fileInfoList){
            QTreeWidgetItem * item = new QTreeWidgetItem();

            item->setText(NAME_COLUMN, fileInfo.fileName());
            item->setText(PATH_COLUMN, fileInfo.filePath());
            item->setToolTip(NAME_COLUMN, fileInfo.filePath());

            if(fileInfo.isDir()){
                addChild(item);
            }

            parent->addChild(item);
        }
    };

    addChild(headerItem);

    treeWidgets.last()->setColumnHidden(1, true);
}


// Методы для работы с настройками
QVariant MainWindow::getSettings(const QString key){
    QSettings settings("configurations.conf", QSettings::IniFormat);

    return settings.value(key);
}

void MainWindow::setSettings(const QString key, const QVariant value){
    QSettings settings("configurations.conf", QSettings::IniFormat);

    settings.setValue(key, value);
}


// Слоты
void MainWindow::on_openDir_triggered() {
    QString lastOpenedDir = getSettings("LastOpenedDir").toString();

    QString openedDirPath =
            QFileDialog::getExistingDirectory(
                this, "Выберите папку",
                lastOpenedDir,
                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if(openedDirPath.isEmpty()) {
        return;
    }

    for(QTreeWidget * treeWidget: treeWidgets){
        if(treeWidget->headerItem()->text(PATH_COLUMN) == openedDirPath) {
            return;
        }
    }

    setSettings("LastOpenedDir", openedDirPath);

    createTreeWidgets();

    readDirInTreeWidget(openedDirPath);
}

void MainWindow::on_openFile_triggered() {
    QString lastOpenedDir = getSettings("LastOpenedDir").toString();
    QString filePath =
            QFileDialog::getOpenFileName(this,
                                         QObject::tr("Выберите документ"),
                                         lastOpenedDir,
                                         QObject::tr("Все файлы (*.*)"));

    if(filePath.isEmpty()) {
        return;
    }


    for(TabWidget * tabWidget: tabWidgets){
        for(TabPage * tabPage: tabWidget->tabs){
            if(tabPage->fileinfo && tabPage->fileinfo->filePath() == filePath){
                 tabWidgets[0]->setCurrentWidget(tabPage);

                 return;
            }
        }
    }

    if(tabWidgets.isEmpty()) {
        createTabWidgets();
    }

    tabWidgets[0]->createTab(filePath)->openFile();
}

void MainWindow::on_closeTab_triggered() {
    const int closedTabIndex = tabWidgets[0]->currentIndex();

    QString pathClosedFile = tabWidgets[0]->closeTab(closedTabIndex);

    if(!pathClosedFile.isEmpty()) {
        setSettings("LastClosedFile", pathClosedFile);
    }
}

void MainWindow::on_closeEverythingTabs_triggered() {
    tabWidgets[0]->closeAllTabs();
}

void MainWindow::on_closeWindow_triggered() {
    hide();
}

void MainWindow::on_newFile_triggered() {
    if(tabWidgets.isEmpty()) {
        createTabWidgets();
    }

    for(TabPage * tabPage: tabWidgets[0]->tabs){
        if(!tabPage->fileinfo){
             tabWidgets[0]->setCurrentWidget(tabPage);

             return;
        }
    }

    tabWidgets[0]->createTab("");
}


