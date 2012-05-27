#ifndef RPN_H
#define RPN_H

#include <QString>

class RPN
{
public:
	static QString convert(const QString &expression);

private:
	static int priority(const QString &operation);
};

#endif // RPN_H
