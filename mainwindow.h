#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "tabwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QList<TabWidget*> tabWidgets;

private slots:
    void on_openFile_triggered();

    void on_closeTab_triggered();

    void on_closeEverythingTabs_triggered();

    void on_closeWindow_triggered();

    void on_newFile_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
