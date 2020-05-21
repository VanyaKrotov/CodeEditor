#include "texteditor.h"
#include "linenumberarea.h"

TextEditor::TextEditor(QWidget *parent): QPlainTextEdit(parent) {
    setFocus();

    QSettings settings(CONFIGURATION_FILE, QSettings::IniFormat);

    const int cursorWidth = settings.value("CursorWidth").toInt();
    const int sumbolWidth = fontMetrics().horizontalAdvance(QLatin1Char('9'));
    const int tabWidth = sumbolWidth * settings.value("TabWidth").toInt();
    lineNumberArea = new LineNumberArea(this);

    setTabStopWidth(tabWidth);
    setCursorWidth(cursorWidth);

    connect(this, &TextEditor::cursorPositionChanged, this, &TextEditor::onChangeCursor);
    connect(this, &TextEditor::textChanged, this, &TextEditor::onTextChanged);

    connect(this, SIGNAL(changeCursorPosition(int, int, int)), parent->parent(), SLOT(setStatusBarDataSlot(int, int, int)));
    connect(this, SIGNAL(inFocus()), parent->parent(), SLOT(setActiveThisWidget()));
    connect(this, SIGNAL(setSaved(const bool)), parent, SLOT(setSaved(const bool)));

     connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
     connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
     connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

     updateLineNumberAreaWidth(0);
     highlightCurrentLine();

    emit changeCursorPosition(0, 0, 0);
}

TextEditor::~TextEditor() {
    delete lineNumberArea;
}

void TextEditor::lineNumberAreaPaintEvent(QPaintEvent *event) {
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), Qt::transparent);

    painter.setPen(QColor(255, 255, 255, 10));
    painter.drawLine(event->rect().topRight(), event->rect().bottomRight());

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            const int lighterText = block.blockNumber() == textCursor().blockNumber() ? 150 : 100;
            QString number = QString::number(blockNumber + 1);

            painter.setPen(QColor(Qt::darkGray).lighter(lighterText));
            painter.drawText(0, top, lineNumberArea->width() - 3, fontMetrics().height(), Qt::AlignCenter, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
     }
}

int TextEditor::lineNumberAreaWidth() {
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 20 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;

    return space;
}

void TextEditor::focusInEvent(QFocusEvent *e) {
    onChangeCursor();
    emit inFocus();

    QPlainTextEdit::focusInEvent(e);
}

void TextEditor::focusOutEvent(QFocusEvent *e) {
    emit changeCursorPosition(0, 0, 0);
    emit outFocus();

    QPlainTextEdit::focusOutEvent(e);
}

void TextEditor::resizeEvent(QResizeEvent *e) {
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

CursorState TextEditor::getCursorState() {
    CursorState cursorState = CursorState();
    auto &[col, row, selected] = cursorState;

    col = textCursor().blockNumber() + 1;
    row = textCursor().positionInBlock() + 1;
    selected = textCursor().selectedText().length();

    return cursorState;
}

void TextEditor::onChangeCursor() {
    auto [col, row, selected] = getCursorState();

    emit changeCursorPosition(col, row, selected);
}

void TextEditor::onTextChanged() {
    emit setSaved(false);
}

void TextEditor::updateLineNumberAreaWidth(int newBlockCount) {
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void TextEditor::highlightCurrentLine() {
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(255, 255, 255, 14);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

void TextEditor::updateLineNumberArea(const QRect &rect, int dy) {
    if (dy)
         lineNumberArea->scroll(0, dy);
     else
         lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

     if (rect.contains(viewport()->rect()))
         updateLineNumberAreaWidth(0);
}


