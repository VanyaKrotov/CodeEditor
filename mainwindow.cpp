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

    headerItem->setToolTip(0, dirPath);
    headerItem->setText(0, dirInfo.fileName());
    headerItem->setText(1, dirPath);

    treeWidgets.last()->setHeaderItem(headerItem);

    std::function<void(QTreeWidgetItem*)> addChild = [&addChild](QTreeWidgetItem* parent){
        QDir rootDir(parent->text(1));
        rootDir.setFilter(QDir::NoDotAndDotDot | QDir::NoSymLinks | QDir::Dirs | QDir::Files);
        rootDir.setSorting(QDir::Type);

        QFileInfoList fileInfoList = rootDir.entryInfoList();

        foreach(QFileInfo fileInfo, fileInfoList){
            QTreeWidgetItem * item = new QTreeWidgetItem();

            item->setText(0, fileInfo.fileName());
            item->setText(1, fileInfo.filePath());
            item->setToolTip(0, fileInfo.filePath());

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

    setSettings("LastOpenedDir", openedDirPath);

    createTreeWidgets();

    readDirInTreeWidget(openedDirPath);
}

void MainWindow::on_openFile_triggered() {
    QString filePath =
            QFileDialog::getOpenFileName(this,
                                         QObject::tr("Выберите документ"),
                                         QDir::rootPath(), QObject::tr("Все файлы (*.*)"));

    if(tabWidgets.isEmpty()) {
        createTabWidgets();
    }

    tabWidgets[0]->createTab(filePath);
}

void MainWindow::on_closeTab_triggered() {
    const int closedTabIndex = tabWidgets[0]->currentIndex();

    tabWidgets[0]->closeTab(closedTabIndex);
}

void MainWindow::on_closeEverythingTabs_triggered() {
    tabWidgets[0]->closeAllTabs();
}

void MainWindow::on_closeWindow_triggered() {
    hide();
}

void MainWindow::on_newFile_triggered() {
   tabWidgets[0]->createNewTab();
}
