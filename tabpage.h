#ifndef TABPAGE_H
#define TABPAGE_H
#include <QWidget>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QFileInfo>
#include <QProgressBar>
#include <QThread>

#include "filereader.h"


class TabPage : public QWidget
{
  Q_OBJECT

public:
    TabPage( QFileInfo *fileInfo, QWidget *parent = nullptr);
    TabPage( QWidget *parent = nullptr);
    ~TabPage();

    bool isSaved;
    QFileInfo * fileinfo;

    void openFile();
    QString getEditorContent();

public slots:
    void setProgressReadFile(int value);
    void setLineFile(QString line);
    void finishedFileOpen();

private:
    QTextEdit *editor;
    QProgressBar *progressBar;
    QThread fileReadThread;
    FileReader fileReader;
};

#endif // TABPAGE_H
