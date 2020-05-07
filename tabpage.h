#ifndef TABPAGE_H
#define TABPAGE_H
#include <QWidget>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QFileInfo>

class TabPage : public QWidget
{
  Q_OBJECT

public:
    TabPage( QFileInfo *fileInfo, QWidget *parent = nullptr);
    ~TabPage();
    bool isSaved;
    QFileInfo * fileinfo;

private:
    QTextEdit *editor;
    QPixmap *pixmap;
};

#endif // TABPAGE_H
