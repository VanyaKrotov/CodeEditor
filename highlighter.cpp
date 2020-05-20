#include "highlighter.h"

Highlighter::Highlighter(QTextDocument *parent): QSyntaxHighlighter(parent) {
    HighlightingRule rule;

    keywordFormat.setForeground(QColor("#5B69FF"));
//    keywordFormat.setFontWeight(QFont::Bold);

    QStringList keywordPatterns;

    keywordPatterns
            << "\\bchar\\b"
            << "\\bclass\\b"
            << "\\bconst\\b"
            <<  "\\bint\\b"
            <<  "\\bdouble\\b"
            <<  "\\benum\\b"
            <<  "\\bstatic\\b"
            <<  "\\bprivate\\b"
            <<  "\\bpublic\\b";

    foreach(const QString &pattern, keywordPatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }
}

void Highlighter::highlightBlock(const QString &text) {
    foreach(const HighlightingRule &rule, highlightingRules) {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);

        while(index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }
}
