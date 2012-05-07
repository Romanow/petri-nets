#include "textbrowser.h"

#include <QFile>

TextBrowser::TextBrowser(QWidget * parent) : QTextEdit(parent)
{
	syntaxHighlighter = new SyntaxHighlighter(document());
	setWordWrapMode(QTextOption::NoWrap);
}

TextBrowser::~TextBrowser()
{
	delete syntaxHighlighter;
}

bool TextBrowser::openFile(const QString &fileName)
{
	bool result;
	QFile file(fileName);
	if ((result = file.open(QIODevice::ReadOnly | QIODevice::Text)))
	{
		QString data = file.readAll();
		file.close();

		setPlainText(data);
	}

	return result;
}
