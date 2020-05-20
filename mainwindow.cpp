#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDir>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this); 

    if(!getSettings("SettingsIsInstall").toBool()) {
        setDefaultSettings();
    }

    const bool isShowStatusBar = getSettings("IsShowStatusBar").toBool();
    const bool isShowSideBar = getSettings("IsShowSideBar").toBool();
    const bool isTextWrap = getSettings("IsTextWrap").toBool();
    const bool onAutoSave = getSettings("OnAutoSave").toBool();

    const int x = getSettings("WindowX").toInt();
    const int y = getSettings("WindowY").toInt();

    const int windowWidth = getSettings("WindowWidth").toInt();
    const int windowHeight = getSettings("WindowHeight").toInt();

    setGeometry(x, y, windowWidth, windowHeight);

    ui->changeStatusBar->setChecked(isShowStatusBar);
    ui->changeSideBar->setChecked(isShowSideBar);
    ui->changeTextWrap->setChecked(isTextWrap);
    ui->autoSave->setChecked(onAutoSave);

    setStateStatusBar(isShowStatusBar);
    setStateSideBar(isShowSideBar);

    const int sideBarWidth = getSettings("SideBarWidth").toInt();
    if(sideBarWidth) {
        const int windowWidth = width();
        int restWidth = windowWidth - sideBarWidth;

        ui->golSplitter->setSizes(QList<int>({sideBarWidth, restWidth}));
    }
}

MainWindow::~MainWindow() {
    setSettings("SideBarWidth", ui->optionsTabWidget->width());
    setSettings("WindowWidth", width());
    setSettings("WindowHeight", height());
    setSettings("WindowX", x());
    setSettings("WindowY", y());

    delete ui;
}

TabPage *MainWindow::checkOpenedFileInWidgets(QString filePath) {
    for(TabWidget * tabWidget: tabWidgets){
        for(TabPage * tabPage: tabWidget->tabs){
            if(tabPage->fileinfo && tabPage->fileinfo->filePath() == filePath){
                return tabPage;
            }
        }
    }

    return nullptr;
}

void MainWindow::openFileInWidgets(QString filePath) {
    if(filePath.isEmpty()) {
        return;
    }

    if(tabWidgets.isEmpty()) {
        createTabWidgets();
    }

    TabPage *tabWithOpenFile = checkOpenedFileInWidgets(filePath);

    if(tabWithOpenFile) {
        activeTabWidget->setCurrentWidget(tabWithOpenFile);

        return;
    }

    activeTabWidget->createTab(filePath)->openFile();
}

void MainWindow::saveEverythingFiles() {
    if(!tabWidgets.isEmpty()) {
        for(TabWidget * tabWidget: tabWidgets){
            for(TabPage * tabPage: tabWidget->tabs){
                if(!tabPage->isSaved){
                    activeTabWidget = tabWidget;
                    tabWidget->setCurrentWidget(tabPage);
                    tabPage->saveFile(SAVE);
                }
            }
        }
    }
}

QString MainWindow::getLastClosedFile() {
    QList<QVariant> listClosedFiles = getSettings("LastOpenedTab").toList();
    if(listClosedFiles.isEmpty()) {
        return nullptr;
    }

    QString filePath = listClosedFiles.last().toString();
    listClosedFiles.removeLast();

    setSettings("LastOpenedTab", listClosedFiles);

    return filePath;
}

void MainWindow::setLastClosedFile(QString filePath) {
    QList<QVariant> listClosedFiles = getSettings("LastOpenedTab").toList();

    listClosedFiles.append(filePath);

    setSettings("LastOpenedTab", listClosedFiles);
}

bool MainWindow::event(QEvent * e) {
    switch(e->type()) {
        case QEvent::WindowActivate :
            // todo сделать проверку файлов на изменения

           break;

        case QEvent::WindowDeactivate :
            if(getSettings("OnAutoSave").toBool() && activeTabWidget) {
                if(activeTabWidget->getCurrentTab()->fileinfo) {
                    activeTabWidget->getCurrentTab()->saveFile(SAVE);
                }
            }

            break;
        default:
            break;
    }

    return QMainWindow::event(e) ;
}

// Методы для работы с динамическими виджетами
QTreeWidget * MainWindow::createTreeWidgets(){
    QTreeWidget * treeWidget = new QTreeWidget(ui->forderViewGroups);

    treeWidget->headerItem()->setHidden(true);

    ui->treeWidgetsSplitter->addWidget(treeWidget);

//    connect(treeWidget, &QTreeWidget::setActive, this, &MainWindow::setActiveTabWidget); // todo переделать на отдельный класс

    treeWidgets.append(treeWidget);

    return treeWidget;
}

TabWidget * MainWindow::createTabWidgets(){
    TabWidget *tabWidget = new TabWidget(ui->textTabGroup);

    ui->tabWidgetsSplitter->addWidget(tabWidget);
    tabWidget->setFocus();

    tabWidgets.append(tabWidget);

    connect(tabWidget, &TabWidget::setStatusBarData, this, &MainWindow::setStatusBarColAndRow);
    connect(tabWidget, &TabWidget::setActive, this, &MainWindow::setActiveTabWidget);

    setActiveTabWidget(tabWidget);

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
        rootDir.setSorting(QDir::DirsFirst | QDir::Type);

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
    QSettings settings(CONFIGURATION_FILE, QSettings::IniFormat);

    return settings.value(key);
}

void MainWindow::setSettings(const QString key, const QVariant value){
    QSettings settings(CONFIGURATION_FILE, QSettings::IniFormat);

    settings.setValue(key, value);
}

//

void MainWindow::setStateStatusBar(bool state){
     setSettings("IsShowStatusBar", state);

    if(state) {
        ui->statusBarBox->show();
    } else {
        ui->statusBarBox->hide();
    }
}

void MainWindow::setStateSideBar(bool state){
    setSettings("IsShowSideBar", state);

    if(state) {
        ui->optionsTabWidget->show();
    } else {
        ui->optionsTabWidget->hide();
    }
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

    openFileInWidgets(filePath);
}

void MainWindow::on_closeTab_triggered() {
    if(activeTabWidget) {
        const int closedTabIndex = activeTabWidget->currentIndex();

        if(activeTabWidget->getCurrentTab()->savingInProgress) {
           return;
        }

        QString pathClosedFile = activeTabWidget->closeTab(closedTabIndex);

        if(!pathClosedFile.isEmpty()) {
            setLastClosedFile(pathClosedFile);
        }
    } else {
        exit(0);
    }
}

void MainWindow::on_closeEverythingTabs_triggered() {
    if(activeTabWidget) {
        activeTabWidget->closeAllTabs();
    }
}

void MainWindow::on_closeWindow_triggered() {
    hide();
}

void MainWindow::on_newFile_triggered() {
    if(tabWidgets.isEmpty()) {
        createTabWidgets();
    }

    for(TabPage * tabPage: activeTabWidget->tabs){
        if(!tabPage->fileinfo){
             activeTabWidget->setCurrentWidget(tabPage);

             return;
        }
    }

    if(activeTabWidget) {
        activeTabWidget->createTab("")->getTextEditor()->setFocus();
    }
}

void MainWindow::on_changeSideBar_toggled(bool value) {
    setStateSideBar(value);
}

void MainWindow::on_changeStatusBar_toggled(bool value) {
    setStateStatusBar(value);
}

void MainWindow::on_changeTextWrap_triggered(bool value) {
    setSettings("IsTextWrap", value);
}

void MainWindow::on_save_triggered() {
    if(activeTabWidget) {
        activeTabWidget->getCurrentTab()->asyncSaveFile(SAVE);
    }
}

void MainWindow::on_openClosedFile_triggered() {
    const QString filePath = getLastClosedFile();

    openFileInWidgets(filePath);
}

void MainWindow::on_exit_triggered() {
    saveEverythingFiles();

    exit(0);
}

void MainWindow::setActiveTabWidget(TabWidget *activeWidget) {
    activeTabWidget = activeWidget;
}

void MainWindow::setActiveTreeWidget(QTreeWidget *activeWidget) {
    activeTreeWidget = activeWidget;
}

void MainWindow::setStatusBarColAndRow(const int col, const int row, const int select) {
    QString buttonLabel =
            "Строка: " + QString::number(col) +
            ", Столбец: " +  QString::number(row) +
            (select ? " (выделено: " + QString::number(select) + ")" : "");

    ui->cursorStateButton->setText(buttonLabel);
}

// метод для установки дефолтных настроек
void MainWindow::setDefaultSettings() {
   QSettings settings(CONFIGURATION_FILE, QSettings::IniFormat);

   settings.setValue("SettingsIsInstall", true);

   settings.setValue("IsTextWrap", false);
   settings.setValue("IsShowSideBar", true);
   settings.setValue("IsShowStatusBar", true);
   settings.setValue("OnAutoSave", true);

   settings.setValue("TabWidth", 4);
   settings.setValue("CursorWidth", 2);
   settings.setValue("WindowWidth", 900);
   settings.setValue("WindowHeight", 500);
   settings.setValue("WindowX", 50);
   settings.setValue("WindowY", 50);

   settings.setValue("SideBarWidth", 300);
}

void MainWindow::on_saveAs_triggered() {
    if(activeTabWidget) {
        activeTabWidget->getCurrentTab()->saveFile(SAVE_AS);
    }
}

void MainWindow::on_autoSave_triggered(bool checked) {
    setSettings("OnAutoSave", checked);
}

void MainWindow::on_saveEverything_triggered() {
    saveEverythingFiles();
}
