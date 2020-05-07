#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDir>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);

    TabWidget *tabWidget = new TabWidget(ui->textTabGroup);
    ui->textTabGroup->layout()->addWidget(tabWidget);

    tabWidgets.append(tabWidget);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_openFile_triggered()
{
    QString filePath =
            QFileDialog::getOpenFileName(this,
                                         QObject::tr("Выберите документ"),
                                         QDir::rootPath(), QObject::tr("Все файлы (*.*)"));

    tabWidgets[0]->createTab(filePath);
}

void MainWindow::on_closeTab_triggered()
{
    const int closedTabIndex = tabWidgets[0]->currentIndex();

    tabWidgets[0]->closeTab(closedTabIndex);
}

void MainWindow::on_closeEverythingTabs_triggered()
{
    tabWidgets[0]->closeAllTabs();
}

void MainWindow::on_closeWindow_triggered()
{
    hide();
}

void MainWindow::on_newFile_triggered()
{
   tabWidgets[0]->createNewTab();
}
