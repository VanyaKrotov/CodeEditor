#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QTabWidget>
#include <QMessageBox>
#include <QTabBar>

#include "tabpage.h"
#include "texteditor.h"
#include "cursorinfomessage.h"

#define CONFIGURATION_FILE "configurations.conf"
#define SAVE false
#define SAVE_AS true

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
    void openFile(QString path);

signals:
    void setStatusBarData(CursorInfoMessage cursorState);
    void setActive(TabWidget * activeWidget);

public slots:
    void closeTab_trigger(const int &tabIndex);
    void removeTabInWidget();
    void updateTabData(TabPage * tab);
    void tabMoved(const int from, const int to);
    void setStatusBarDataSlot(CursorInfoMessage cursorState);
    void setActiveThisWidget();
};

#endif // TABWIDGET_H
