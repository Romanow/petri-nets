#include "rpn.h"

#include <QStack>
#include <QStringList>

int RPN::priority(const QString &operation)
{
	switch (operation[0].toAscii())
	{
	case '*':
	case '/':
		return 3;

	case '+':
	case '-':
	case '>':
	case '<':
	case '=':
		return 2;

	case '(':
		return 1;
	}
}

QString RPN::convert(const QString &expression)
{
	QString result;
	QStack<QString> stack;
	for (int i = 0; i < expression.length(); ++i)
	{
		if (!expression[i].isSpace())
		{
			if (expression[i].isLetterOrNumber())
			{
				QString variable;
				while (i < expression.length() && (expression[i].isLetterOrNumber() || expression[i] == '.'))
					variable += expression[i++];

				result += variable + " ";
			}
			else if (expression[i] == '(')
				stack.push(expression[i]);
			else if (expression[i] == ')')
			{
				if (!stack.isEmpty())
				{
					while (stack.top()[0] != '(')
						result += stack.pop() + " ";
					stack.pop();
				}
			}
			else if (expression[i] == '+' || expression[i] == '-' ||
					 expression[i] == '*' || expression[i] == '/')
			{
				while (!stack.isEmpty() && priority(stack.top()) >= priority(expression[i]))
					result += stack.pop() + " ";

				stack.push(expression[i]);
			}
			else if (expression[i] == '=' || expression[i] == '>' || expression[i] == '<' || expression[i] == '!')
			{
				QString operation = expression[i];
				if (i + 1 < expression.length() && expression[i + 1] == '=')
					operation += expression[++i];

				while (!stack.isEmpty() && priority(stack.top()) >= priority(operation))
					result += stack.pop() + " ";

				stack.push(operation);
			}
		}
	}

	while (!stack.isEmpty())
		result += stack.pop() + " ";

	return result;
}

bool RPN::isNumber(const QString &variable)
{
	bool flag;
	variable.toDouble(&flag);
	return flag;
}

double RPN::calculate(const QString &expression)
{
	QStack<double> stack;
	foreach (QString str, expression.split(' ', QString::SkipEmptyParts))
	{
		if (isNumber(str))
		{
			stack.push(str.toDouble());
		}
		else
		{
			double v2 = stack.pop();
			double v1 = stack.pop();
			double result;
			switch (str[0].toAscii())
			{
			case '+':
				result = v1 + v2;
				break;

			case '-':
				result = v1 - v2;
				break;

			case '*':
				result = v1 * v2;
				break;

			case '/':
				result = v1 / v2;
				break;

			case '>':
				if (str.length() > 1 && str[1] == '=')
					result = v1 >= v2;
				else
					result = v1 > v2;
				break;

			case '<':
				if (str.length() > 1 && str[1] == '=')
					result = v1 <= v2;
				else
					result = v1 < v2;
				break;

			case '=':
				if (str.length() > 1 && str[1] == '=')
					result = v1 == v2;
				break;
			}

			stack.push(result);
		}
	}

	return stack.pop();
}
