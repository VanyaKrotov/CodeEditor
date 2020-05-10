#ifndef TABPAGE_H
#define TABPAGE_H
#include <QWidget>
#include <QSettings>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QPlainTextEdit>
#include <QFileInfo>
#include <QProgressBar>
#include <QThread>

#include "filereader.h"
#include "filewriter.h"
#include "texteditor.h"

#define CONFIGURATION_FILE "configurations.conf"
#define SAVE false
#define SAVE_AS true

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
    void saveFile(const bool saveType);
    QString getEditorContent();
    TextEditor *getTextEditor();

public slots:
    void setProgressReadFile(int value);
    void setLineFile(QString line);
    void finishedFileOpen();
    void finishedFileSave(QString pathSaved);

private:
    TextEditor *editor;
    QProgressBar *progressBar;

    QThread fileReadThread;
    FileReader fileReader;

    QThread fileWriteThread;
    FileWriter fileWriter;

signals:
    void updateTabData(TabPage * tab);
    void endSave();
};

#endif // TABPAGE_H
