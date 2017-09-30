#include <iostream>
#include <string>
#include <stack>
#include <cassert>

using namespace std;

int evaluate(string infix, const bool values[], string& postfix, bool& result)
{
	postfix = "";
	stack<char> operators;
	for (int j = 0; j < infix.size(); j++) //Deletes all the space characters and shortens the string.
	{
		if (infix[j] == ' ')
		{
			for (int l = j; l < infix.size() - 1; l++)
			{
				infix[l] = infix[l + 1];
			}
			infix.resize(infix.size() - 1);;
			j--;
		}
	}
	int paren1counter = 0; //Makes sure all the parentheses close.
	int paren2counter = 0;
	for (int k = 0; k < infix.size(); k++)
	{
		if (infix[k] == '|' || infix[k] == '&') //If there is '|' or '&', there must be a digit before and after it.
		{
			int j = k + 1;
			while (j <= infix.size())
			{
				if (j == infix.size())
					return 1;
				if (isdigit(infix[j]) != 0)
					break;
				if (infix[j] == ')' || infix[j] == '|' || infix[j] == '&') //After the operator, there must not be these before a digit.
					return 1;
				j++;
			}
			int m = k - 1;
			while (m >= -1)
			{
				if (m == -1)
					return 1;
				if (isdigit(infix[m]) != 0)
					break;
				if (infix[m] == '!' || infix[j] == '|' || infix[j] == '&' || infix[m] == '(') //Before the operator, there must not be these before a digit.
					return 1;
				m++;
			}
		}
		switch (infix[k])
		{
		case '0':	case '1':	case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9': //If a digit add it on the postfix
			postfix += infix[k];
			break;
		case '(': //If a start parentheses, push onto the stack.
			operators.push(infix[k]);
			paren1counter++;
			break;
		case ')':
			while (operators.top() != '(') //If end parentheses, add onto postfix the stack until the open parentheses is reached.
			{
				postfix += operators.top();
				operators.pop();
			}
			operators.pop();
			paren2counter++;
			break;
		case '&':
		case '|':
		case '!':
			while (operators.empty() == false && operators.top() != '(' && infix[k] != '!'
				&& (infix[k] == '|' || (infix[k] == '&' && operators.top() != '|')))
				//If there is an operator, the stack mustn't be empty and the top value must not be '(' or of higher precedent.
			{
				postfix += operators.top();
				operators.pop();
			}
			operators.push(infix[k]);
			break;
		default:
			return 1;
		}
	}
	if (paren1counter != paren2counter) //Check the parentheses count.
		return 1;
	while (operators.empty() == false) //Add all remaining items from stack to postfix.
	{
		postfix += operators.top();
		operators.pop();
	}
	stack<char> operand;
	for (int k = 0; k < postfix.size(); k++) //For every character of the string do an operation
	{
		if (isdigit(postfix[k])) //Add digits's t/f value onto the operand stack.
		{
			int digit = (postfix[k] - '0');
			if (values[digit] != 0)
				operand.push('t');
			else
				operand.push('f');
		}
		else if (postfix[k] == '!') //Switch the top value of the stack to the opposite value (t->f or f->t)
		{
			char m = operand.top();
			operand.pop();
			if (m == 't')
				operand.push('f');
			else
				operand.push('t');
		}
		else //If an operator (| or &) pop off the top two values and operate on them.
		{
			char operand2;
			char operand1;
			operand2 = operand.top();
			operand.pop();
			operand1 = operand.top();
			operand.pop();
			if (postfix[k] == '|') //For |, you need at one true to return true.
			{
				if (operand1 == 't' || operand2 == 't')
					operand.push('t');
				else
					operand.push('f');
			}
			else if (postfix[k] == '&') //For &, you need 2 true to be true.
			{
				if (operand1 == 't' && operand2 == 't')
					operand.push('t');
				else
					operand.push('f');
			}
		}
	}
	if (operand.empty()) //If it is empty, then it is an invalid infix.
		return 1;
	char top = operand.top();
	operand.pop();
	if (!operand.empty()) //after removing 1 value, if it is not empty then it is an invalid infix.
		return 1;
	if (top == 't') //If the value is t, then the result is true, otherwise false.
	{
		result = true;
	}
	else
	{
		result = false;
	}
	return 0;
}
int main()
{
	bool ba[10] = {
		//  0      1      2      3      4      5      6      7      8      9
		true,  true,  true,  false, false, false, true,  false, true,  false
	};
	string pf;
	bool answer;
	assert(evaluate("2| 3", ba, pf, answer) == 0 && pf == "23|" &&  answer);
	assert(evaluate("8|", ba, pf, answer) == 1);
	assert(evaluate("4 5", ba, pf, answer) == 1);
	assert(evaluate("01", ba, pf, answer) == 1);
	assert(evaluate("()", ba, pf, answer) == 1);
	assert(evaluate("2(9|8)", ba, pf, answer) == 1);
	assert(evaluate("2(&8)", ba, pf, answer) == 1);
	assert(evaluate("(6&(7|7)", ba, pf, answer) == 1);
	assert(evaluate("", ba, pf, answer) == 1);
	assert(evaluate("4  |  !3 & (0&3) ", ba, pf, answer) == 0
		&& pf == "43!03&&|" && !answer);
	assert(evaluate(" 9  ", ba, pf, answer) == 0 && pf == "9" && !answer);
	ba[2] = false;
	ba[9] = true;
	assert(evaluate("((9))", ba, pf, answer) == 0 && pf == "9"  &&  answer);
	assert(evaluate("2| 3", ba, pf, answer) == 0 && pf == "23|" && !answer);
	cout << "Passed all tests" << endl;
}
/*int main()
{
	string infix = "1&!(9|1&1|9) | !!!(9&1&9) ";
	string postfix;
	bool values[] = { true , true,  true  ,false, false, false, true,  false, true,  false };
	bool result;
	evaluate(infix, values, postfix, result);
	if (result)
		cerr << "true" << endl;
}*/