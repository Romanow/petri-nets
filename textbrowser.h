#ifndef TEXTBROWSER_H
#define TEXTBROWSER_H

#include <QTextEdit>

#include "syntaxhighlighter.h"

class TextBrowser : public QTextEdit
{
    Q_OBJECT

public:
	TextBrowser(QWidget * parent = 0);
	~TextBrowser();

	bool openFile(const QString &fileName);

private:
	SyntaxHighlighter * syntaxHighlighter;
};

#endif // TEXTBROWSER_H
