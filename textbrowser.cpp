#include "textbrowser.h"

TextBrowser::TextBrowser(QWidget * parent) : QTextEdit(parent)
{
	syntaxHighlighter = new SyntaxHighlighter(document());
	setWordWrapMode(QTextOption::NoWrap);
}

TextBrowser::~TextBrowser()
{
	delete syntaxHighlighter;
}
