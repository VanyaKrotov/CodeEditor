#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "tabwidget.h"
#include <QSettings>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QVariant>

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



private slots:
    void on_openFile_triggered();

    void on_closeTab_triggered();

    void on_closeEverythingTabs_triggered();

    void on_closeWindow_triggered();

    void on_newFile_triggered();

    void on_openDir_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
