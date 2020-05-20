#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <QPlainTextEdit>
#include <QSettings>
#include <QDebug>

#define CONFIGURATION_FILE "configurations.conf"


struct CursorState{
    int col;
    int row;
    int selected;
};

class TextEditor: public QPlainTextEdit {

    Q_OBJECT

public:
    TextEditor(QWidget *parent = nullptr);
    ~TextEditor();
    void setConnect(QObject *to);

protected:
    void focusInEvent(QFocusEvent *e);
    void focusOutEvent(QFocusEvent *e);

private:
    bool *isSaved;

    CursorState getCursorState();

public slots:
    void onChangeCursor();
    void onTextChanged();

signals:
    void changeCursorPosition(const int col, const int row, const int select);
    void inFocus();
    void outFocus();
    void setSaved(const bool value);
};

#endif // TEXTEDITOR_H
