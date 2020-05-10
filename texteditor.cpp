#include "texteditor.h"

TextEditor::TextEditor(bool *isSaved, QWidget *parent): QPlainTextEdit(parent) {
    this->isSaved = isSaved;

    QSettings settings(CONFIGURATION_FILE, QSettings::IniFormat);

    const int cursorWidth = settings.value("CursorWidth").toInt();
    const int sumbolWidth = fontMetrics().horizontalAdvance(QLatin1Char('9'));
    const int tabWidth = sumbolWidth * settings.value("TabWidth").toInt();

    setTabStopWidth(tabWidth ? tabWidth : sumbolWidth * 4);
    setCursorWidth(cursorWidth < 1 ? 1 : cursorWidth);

    connect(this, &TextEditor::cursorPositionChanged, this, &TextEditor::onChangeCursor);
    connect(this, &TextEditor::textChanged, this, &TextEditor::onTextChanged);

    emit changeCursorPosition(0, 0, 0);
}

TextEditor::~TextEditor() {

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
    *isSaved = false;
}

