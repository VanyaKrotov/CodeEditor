#ifndef LINENUMBERAREA_H
#define LINENUMBERAREA_H

#include <QWidget>
#include "texteditor.h"

class LineNumberArea : public QWidget
 {
 public:
     LineNumberArea(TextEditor *editor) : QWidget(editor) {
         codeEditor = editor;
     }

     QSize sizeHint() const {
         return QSize(codeEditor->lineNumberAreaWidth(), 0);
     }

 protected:
     void paintEvent(QPaintEvent *event) {
         codeEditor->lineNumberAreaPaintEvent(event);
     }

 private:
     TextEditor *codeEditor;
};

#endif // LINENUMBERAREA_H
