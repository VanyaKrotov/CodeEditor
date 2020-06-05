#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <QPlainTextEdit>
#include <QSettings>
#include <QDebug>
#include <QPainter>
#include <QTextBlock>
#include <QColor>

#include "cursorinfomessage.h"

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

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

protected:
    void focusInEvent(QFocusEvent *e);
    void focusOutEvent(QFocusEvent *e);
    void resizeEvent(QResizeEvent *e);

private:
    bool *isSaved;
    QWidget *lineNumberArea;

    CursorState getCursorState();

public slots:
    void onChangeCursor();
    void onTextChanged();

    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &, int);

signals:
    void changeCursorPosition(CursorInfoMessage cursorState);
    void inFocus();
    void outFocus();
    void setSaved(const bool value);
};

#endif // TEXTEDITOR_H
