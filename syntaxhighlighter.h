#ifndef SYNTAXHIGHLIGHTER_H
#define SYNTAXHIGHLIGHTER_H

#include <QSyntaxHighlighter>

class SyntaxHighlighter : public QSyntaxHighlighter
{
public:
	SyntaxHighlighter(QTextDocument * parent);

protected:
	virtual void highlightBlock(const QString &text);

private:
	struct HighlightingRule
	{
		QRegExp pattern;
		QTextCharFormat format;
	};
	QVector<HighlightingRule> highlightingRules;

	QRegExp commentStartExpression;
	QRegExp commentEndExpression;

	QTextCharFormat tagFormat;
	QTextCharFormat attributeFormat;
	QTextCharFormat attributeContentFormat;
	QTextCharFormat commentFormat;
};

#endif // SYNTAXHIGHLIGHTER_H
