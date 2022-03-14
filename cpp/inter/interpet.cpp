#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <stack>
#include <map>

using namespace std;

enum LEXEM_TYPE {
        NUMBER,
        OPER
};

enum OPERATOR {
        LBRACKET,
        RBRACKET,
        PLUS,
        MINUS,
        MULTIPLY
};

int PRIORITY[] = {
        -1, -1,
        0, 0,
        1
};


class Lexem
{
protected:
	LEXEM_TYPE type;
public:
	Lexem();
	LEXEM_TYPE getype();
};


class Number : public Lexem
{
	int value;
public:

	//Number(int number = 0) : Lexem(NUMBER), value(number) {}
	Number();
	Number(int num);
	int getValue() const;
};


class Oper : public Lexem
{
	OPERATOR opertype;
public:
	Oper();
	Oper(char ch);
	OPERATOR getType();
	int getPriority();
	int getValue(const Number &left, const Number &right);
};


Lexem::Lexem()
{

}

Number::Number()
{

}

Oper::Oper()
{

}



Oper::Oper(char ch)
{
	switch(ch)
	{
		case '+':
		{
			opertype = PLUS;
			break;
		}
		case '-':
		{
			opertype = MINUS;
			break;
		}
		case '*':
		{
			opertype = MULTIPLY;
			break;
		}
		case '(':
		{
			opertype = LBRACKET;
			break;
		}
		case ')':
		{
			opertype = RBRACKET;
			break;
		}
	}
}

Number::Number(int value)
{
	this->value = value;
}

int Number::getValue() const
{
    return value;
}



OPERATOR Oper::getType()
{
    return opertype;
}


int Oper::getPriority()
{
    return PRIORITY[opertype];
}

int Oper::getValue(const Number & left, const Number & right)
{
	switch(opertype)
	{
		case PLUS:
		{
			return left.getValue() + right.getValue();
		}
		case MINUS:
		{
			return left.getValue() - right.getValue();
		}
		case MULTIPLY:
		{
			return left.getValue() * right.getValue();
		}
	}
	return 0;
}


LEXEM_TYPE Lexem::getype()
{
	return type;
}



vector<Lexem *> parseLexem(std::string codeline)
{
	int number = 0;
	std::vector<Lexem *> infix;
	cout << "qqq ";
	for(int i = 0; i < codeline.size(); i++)
	{
		cout << codeline[i] << " ";
		if(codeline[i] == '\t' || codeline[i] == ' ' || codeline[i] == '\n')
		{
			continue;
		}
		else if(codeline[i] >= '0' && codeline[i] <= '9')
		{
			number = number * 10 + codeline[i] - '0';
			infix.push_back(new Number(number));
			number = 0;
		}
		else
		{
			infix.push_back(new Oper(codeline[i]));
		}
	}
	cout << "123 ";
	return infix;
}


std::vector<Lexem *> buildPostfix(std::vector<Lexem *> infix)
{
	std::stack<Oper *> operators;
	std::vector<Lexem *> posix;
	cout << "55555 ";
	for(int i = 0; i < infix.size(); i++)
	{
		if(infix[i]->getype() == NUMBER)
		{
			posix.push_back(infix[i]);
		}
		else if(infix[i]->getype() == OPER)
		{
			if(operators.empty())
			{
				operators.push(static_cast<Oper *>(infix[i]));
			}
			else
			{
				if(static_cast<Oper *>(infix[i])->getType() == LBRACKET)
				{
					operators.push(static_cast<Oper *>(infix[i]));
				}
				else if(static_cast<Oper *>(infix[i])->getType() == RBRACKET)
				{
					while(static_cast<Oper *>(operators.top())->getType() != LBRACKET)
					{
						posix.push_back(operators.top());
						operators.pop();
					}
					operators.pop();
				}

				else if(!operators.empty() && (operators.top())->getPriority() >= static_cast<Oper *>(infix[i])->getPriority())
				{
				 	posix.push_back(operators.top());
					operators.pop();
					operators.push(static_cast<Oper *>(infix[i]));
				}
				else
				{
					operators.push(static_cast<Oper *>(infix[i]));
				}
			}
		}
	}
	cout << "444 ";
	while(operators.empty())
	{
		posix.push_back(operators.top());
		operators.pop();
	}
	return posix;

}


int evaluatePostfix(std::vector<Lexem *> poliz)
{
	std::vector<int> stack;
	int l, r;
	for(int i = 0; i < poliz.size(); i++)
	{
		if(poliz[i]->getype() == NUMBER)
		{
			stack.push_back(static_cast<Number *>(poliz[i])->getValue());
		}
		else if(poliz[i]->getype() == OPER)
		{
			l = stack.back();
			stack.pop_back();
			r = stack.back();
			stack.pop_back();
			stack.push_back(static_cast<Oper *>(poliz[i])->getValue(l, r));
		}
	}
	return stack.back();
}

int main()
{
	std::string codeline;
	std::vector<Lexem *> infix;
	std::vector<Lexem *> postfix;
	int value;
	while(std::getline(std::cin, codeline))
	{
		if(codeline == "exit")
		{
			return 0;
		}
		std::cout << "000 ";
		infix = parseLexem(codeline);
		postfix = buildPostfix(infix);
		value = evaluatePostfix(postfix);
		std::cout << value << std::endl;
	}
	return 0;
}
