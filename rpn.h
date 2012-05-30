#ifndef RPN_H
#define RPN_H

#include <QString>

class RPN
{
public:
	static QString convert(const QString &expression);
	static double calculate(const QString &expression);

private:
	static int priority(const QString &operation);
	static bool isNumber(const QString &variable);
};

#endif // RPN_H
