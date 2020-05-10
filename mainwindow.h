#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QVariant>
#include <QSplitter>

#include "tabwidget.h"


#define NAME_COLUMN 0
#define PATH_COLUMN 1
#define CONFIGURATION_FILE "configurations.conf"
#define SAVE false
#define SAVE_AS true


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    // Поля для хранения динамических виджетов
    QList<TabWidget*> tabWidgets;
    QList<QTreeWidget*> treeWidgets;

    // Методы для работы с динамическими виджетами
    QTreeWidget * createTreeWidgets();
    QTabWidget * createTabWidgets();

    void readDirInTreeWidget(const QString dirPath);

    // Методы для работы с настройками
    QVariant getSettings(const QString key);
    void setSettings(const QString key, const QVariant value);

    //

    void setStateStatusBar(bool state);
    void setStateSideBar(bool state);

private slots:
    void on_openFile_triggered();
    void on_closeTab_triggered();
    void on_closeEverythingTabs_triggered();
    void on_closeWindow_triggered();
    void on_newFile_triggered();
    void on_openDir_triggered();
    void on_changeSideBar_toggled(bool value);
    void on_changeStatusBar_toggled(bool value);
    void on_changeTextWrap_triggered(bool value);
    void on_save_triggered();
    void on_openClosedFile_triggered();
    void on_exit_triggered();

    void setStatusBarColAndRow(const int col, const int row, const int select);

private:
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
