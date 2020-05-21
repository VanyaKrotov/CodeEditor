#include "highlighter.h"

Highlighter::Highlighter(QTextDocument *parent): QSyntaxHighlighter(parent) {
    /*HighlightingRule rule;

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
            <<  "\\bpublic\\b"
            <<  "\\bprotected\\b"
            <<  "\\busing\\b"
            <<  "\\bnamespace\\b"
            <<  "\\bstd\\b"
            <<  "\\breturn\\b";

    foreach(const QString &pattern, keywordPatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }


    rule.pattern = QRegExp("#[A-Za-z0-9]+");
    rule.format = QTextCharFormat();
    rule.format.setForeground(QColor("#cfb15f"));

    highlightingRules.append(rule);

    rule.pattern = QRegExp("\\b[0-9]+\\b");
    rule.format = QTextCharFormat();
    rule.format.setForeground(QColor("#a1727a"));

    highlightingRules.append(rule);
    // //[^\n]*
    rule.pattern = QRegExp("/\\*[^\n]*");
    rule.format = QTextCharFormat();
    rule.format.setForeground(QColor("#50a156"));

    highlightingRules.append(rule);*/
    HighlightingRule rule;

    keywordFormat.setForeground(QColor("#5B69FF"));
    QStringList keywordPatterns;

    keywordPatterns << "\\bchar\\b" << "\\bclass\\b" << "\\bconst\\b"
                 << "\\bdouble\\b" << "\\benum\\b" << "\\bexplicit\\b"
                 << "\\bfriend\\b" << "\\binline\\b" << "\\bint\\b"
                 << "\\blong\\b" << "\\bnamespace\\b" << "\\boperator\\b"
                 << "\\bprivate\\b" << "\\bprotected\\b" << "\\bpublic\\b"
                 << "\\bshort\\b" << "\\bsignals\\b" << "\\bsigned\\b"
                 << "\\bslots\\b" << "\\bstatic\\b" << "\\bstruct\\b"
                 << "\\btemplate\\b" << "\\btypedef\\b" << "\\btypename\\b"
                 << "\\bunion\\b" << "\\bunsigned\\b" << "\\bvirtual\\b"
                 << "\\bvoid\\b" << "\\bvolatile\\b" << "\\breturn\\b";

    foreach (const QString &pattern, keywordPatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    classFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegExp("\\bQ[A-Za-z]+\\b");
    rule.format = classFormat;
    highlightingRules.append(rule);

    singleLineCommentFormat.setForeground(QColor("#50a156"));
    rule.pattern = QRegExp("//[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(QColor("#50a156"));

    quotationFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegExp("\".*\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    functionFormat.setForeground(QColor("#678087"));
    rule.pattern = QRegExp("\\b[A-Za-z0-9_]+\\s*(?=\\()");
    rule.format = functionFormat;
    highlightingRules.append(rule);

    rule.pattern = QRegExp("\\b[0-9]+");
    rule.format = QTextCharFormat();
    rule.format.setForeground(QColor("#a1727a"));

    commentStartExpression = QRegExp("/\\*");
    commentEndExpression = QRegExp("\\*/");
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

    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = commentStartExpression.indexIn(text);

    while (startIndex >= 0) {
        int endIndex = commentEndExpression.indexIn(text, startIndex);
        int commentLength;

        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                 + commentEndExpression.matchedLength();
        }

        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = commentStartExpression.indexIn(text, startIndex + commentLength);
    }
}
