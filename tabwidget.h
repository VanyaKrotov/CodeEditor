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
    void createTab(QString &filePath);
    void createNewTab();
    void closeAllTabs();
    TabPage * getCurrentTab();
    QString closeTab(const int &tabIndex);

private:
    QList<TabPage*> tabs;

public slots:
    void closeTab_trigger(const int &tabIndex);
};

#endif // TABWIDGET_H
