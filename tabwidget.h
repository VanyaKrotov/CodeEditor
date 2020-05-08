#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QTabWidget>
#include <QMessageBox>
#include "tabpage.h"


class TabWidget: public QTabWidget
{
    Q_OBJECT

public:
    TabWidget(QWidget *parent = nullptr);
    ~TabWidget();

    TabPage * getCurrentTab();
    QString closeTab(const int &tabIndex);
    QList<TabPage*> tabs;

    TabPage * createTab(QString filePath);
    void closeAllTabs();


private:

public slots:
    void closeTab_trigger(const int &tabIndex);
};

#endif // TABWIDGET_H
