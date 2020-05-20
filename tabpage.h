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
#include "highlighter.h"

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

    bool isSaved = true;
    bool savingInProgress = false;
    QFileInfo * fileinfo;

    void openFile();
    void saveFile(const bool saveType);
    void asyncSaveFile(const bool saveType);
    QString getEditorContent();
    TextEditor *getTextEditor();

public slots:
    void setLineFile(QString line);
    void finishedFileOpen();
    void finishedFileSave(QString pathSaved);
    void setSaved(const bool value);

private:
    TextEditor *editor;
    Highlighter *highlighter;

    QThread fileReadThread;
    FileReader fileReader;

    QThread fileWriteThread;
    FileWriter fileWriter;

signals:
    void updateTabData(TabPage * tab);
    void endSave();
};

#endif // TABPAGE_H
