#pragma once

#include <vector>
#include <string>
#include <iostream>

const unsigned int MaxNumber = 999999999;
const unsigned int MaxLength = 9;

class BigInteger
{
public:
	//constructors
	BigInteger(int x);
	BigInteger();
	//operations
	friend BigInteger operator+(const BigInteger& a, const BigInteger& x);
	BigInteger& operator+=(const BigInteger& x);
	friend BigInteger operator-(const BigInteger& a, const BigInteger& x);
	BigInteger& operator-=(const BigInteger& x);
	friend BigInteger operator*(const BigInteger& a, const BigInteger& x);
	BigInteger& operator*=(const BigInteger& x);
	friend BigInteger operator/(const BigInteger& a, const BigInteger& x);
	BigInteger& operator/=(const BigInteger& x);
	friend BigInteger operator%(const BigInteger& a, const BigInteger& x);
	BigInteger& operator%=(const BigInteger& x);
	//unar operations
	BigInteger operator-() const;
	BigInteger& operator++();
	BigInteger operator++(int x);
	BigInteger& operator--();
	BigInteger operator--(int x);
	//bool
	friend bool operator==(const BigInteger& a, const BigInteger& x);
	friend bool operator!=(const BigInteger& a, const BigInteger& x);
	friend bool operator<(const BigInteger& a, const BigInteger& x);
	friend bool operator<=(const BigInteger& a, const BigInteger& x);
	friend bool operator>(const BigInteger& a, const BigInteger& x);
	friend bool operator>=(const BigInteger& a, const BigInteger& x);
	explicit operator bool();
	//streams
	friend std::istream& operator>>(std::istream& os, BigInteger& x);
	friend std::ostream& operator<<(std::ostream& os, const BigInteger& x);
	//miscellaneous
	std::string toString() const;
private:
	std::vector<unsigned int> number;
	int sign;
	BigInteger operator>>(unsigned int x) const;
	BigInteger mult(unsigned int x) const;
	void summ(const BigInteger& x);
	void sub(const BigInteger& x);
	void set_sign();
};
BigInteger::BigInteger(int x):
	number(0)
{
	if (x < 0)
	{
		x = -x;
		sign = -1;
	}
	else
		sign = 1;
	number.push_back(x % (MaxNumber + 1));
	x /= (MaxNumber + 1);
	while (x > 0)
	{
		number.push_back(x % (MaxNumber + 1));
		x /= (MaxNumber + 1);
	}
}
BigInteger::BigInteger():
	number(1, 0),
	sign(1)
{
}
BigInteger operator+(const BigInteger& a, const BigInteger& x)
{
	if (a.sign != x.sign)
		return a - (-x);
	BigInteger temp_a(a);
	temp_a.summ(x);
	return temp_a;
}
BigInteger& BigInteger::operator+=(const BigInteger& x)
{
	*this = *this + x;
	return *this;
}
BigInteger operator-(const BigInteger& a, const BigInteger& x)
{
	if (a.sign != x.sign)
	{
		BigInteger temp_a(a);
		temp_a.sign *= -1;
		temp_a.summ(x);
		temp_a.sign = a.sign;
		temp_a.set_sign();
		return temp_a;
	}
	BigInteger temp_a(a), temp_x(x);
	temp_a.sign = 1;
	temp_x.sign = 1;
	if (temp_a < temp_x)
	{
		temp_x.sub(temp_a);
		temp_x.sign = a.sign * (-1);
		temp_x.set_sign();
		return temp_x;
	}
	else
	{
		temp_a.sub(temp_x);
		temp_a.sign = a.sign;
		temp_a.set_sign();
		return temp_a;
	}
}
BigInteger& BigInteger::operator-=(const BigInteger& x)
{
	*this = *this - x;
	return *this;
}
BigInteger operator*(const BigInteger& a, const BigInteger& x)
{
	BigInteger result(0);
	for (unsigned int i = 0; i < x.number.size(); i++)
	{
		result += a.mult(x.number[i]) >> i;
	}
	result.sign = a.sign * x.sign;
	while ((result.number[result.number.size() - 1] == 0) && (result.number.size() > 1))
		result.number.pop_back();
	result.set_sign();
	return result;
}
BigInteger& BigInteger::operator*=(const BigInteger& x)
{
	*this = *this * x;
	return *this;
}
BigInteger operator/(const BigInteger& a, const BigInteger& x)
{
	if ((x.number.size() == 1) && (x.number[0] == 0))
		return 0;
	BigInteger result(0);
	while (result.number.size() < a.number.size())
		result.number.push_back(0);
	result.sign = a.sign * x.sign;
	BigInteger current = a;
	current.sign = 1;
	BigInteger temp_x = x;
	temp_x.sign = 1;
	for (int i = current.number.size() - temp_x.number.size(); (current >= temp_x) && (i >= 0); i--)
	{
		unsigned int max = MaxNumber;
		unsigned int min = 0;
		while (max - min > 1)
		{
			unsigned int median = (min + max) / 2;
			BigInteger check = (temp_x.mult(median) >> i);
			if (current >= check)
				min = median;
			else
				max = median;
		}
		BigInteger check = (temp_x.mult(max) >> i);
		if (current >= check)
		{
			result.number[i] = max;
			current -= check;
		}
		else
		{
			result.number[i] = min;
			current -= (temp_x.mult(min) >> i);
		}
	}
	while ((result.number[result.number.size() - 1] == 0) && (result.number.size() > 1))
		result.number.pop_back();
	result.set_sign();
	return result;
}
BigInteger& BigInteger::operator/=(const BigInteger& x)
{
	*this = *this / x;
	return *this;
}
BigInteger operator%(const BigInteger& a, const BigInteger& x)
{
	BigInteger temp_a(a);
	temp_a /= x;
	temp_a *= x;
	return a - temp_a;
}
BigInteger& BigInteger::operator%=(const BigInteger& x)
{
	*this = *this % x;
	return *this;
}
BigInteger BigInteger::operator-() const
{
	BigInteger result = *this;
	if ((result.number.size() == 1) && (result.number[0] == 0))
		result.sign = 1;
	else
		result.sign *= -1;
	return result;
}
BigInteger& BigInteger::operator++()
{
	*this += 1;
	return *this;
}
BigInteger BigInteger::operator++(int x)
{
	BigInteger result = *this;
	x = 0;
	*this += 1 - x;
	return result;
}
BigInteger& BigInteger::operator--()
{
	*this -= 1;
	return *this;
}
BigInteger BigInteger::operator--(int x)
{
	BigInteger result = *this;
	x = 0;
	*this -= 1 - x;
	return result;
}
bool operator==(const BigInteger& a, const BigInteger& x)
{
	if (a.number.size() != x.number.size())
		return false;
	if (a.sign != x.sign)
		return false;
	bool result = true;
	for (unsigned int i = 0; (i < a.number.size()) && result; i++)
	{
		result = a.number[i] == x.number[i];
	}
	return result;
}
bool operator!=(const BigInteger& a, const BigInteger& x)
{
	return !(a == x);
}
bool operator<(const BigInteger& a, const BigInteger& x)
{
	if (a.sign != x.sign)
		return a.sign < x.sign;
	if (a.sign == -1)
		return (-a) > (-x);
	if (a.number.size() != x.number.size())
		return a.number.size() < x.number.size();
	int i = a.number.size() - 1;
	for (i = a.number.size() - 1; (i > 0) && (a.number[i] == x.number[i]); i--);
	return a.number[i] < x.number[i];
	return false;
}
bool operator<=(const BigInteger& a, const BigInteger& x)
{
	return !(a > x);
}
bool operator>(const BigInteger& a, const BigInteger& x)
{
	if (a.sign != x.sign)
		return a.sign > x.sign;
	if (a.sign == -1)
		return (-a) < (-x);
	if (a.number.size() != x.number.size())
		return a.number.size() > x.number.size();
	int i = a.number.size() - 1;
	for (i = a.number.size() - 1; (i > 0) && (a.number[i] == x.number[i]); i--);
	return a.number[i] > x.number[i];
	return false;
}
bool operator>=(const BigInteger& a, const BigInteger& x)
{
	return !(a < x);
}
BigInteger::operator bool()
{
	if (number.size() == 1)
		return bool(number[0]);
	return true;
}
std::istream& operator>>(std::istream& os, BigInteger& x)
{
	x = 0;
	std::string s;
	os >> s;
	unsigned int i = 0;
	int sign = 1;
	if (s[0] == '-')
	{
		sign = -1;
		i++;
	}
	for (; i < s.length(); i++)
	{
		x = (x * 10) + (s[i] - '0');
	}
	x.sign = sign;
	return os;
}
std::ostream& operator<<(std::ostream& os, const BigInteger& x)
{
	std::string s = x.toString();
	for (unsigned int i = 0; i < s.length(); i++)
		os << s[i];
	return os;
}
std::string BigInteger::toString() const
{
	if (this->number.size() == 0)
		return "";
	std::string result = "";
	if (sign == -1)
		result += '-';
	result += std::to_string(number[number.size() - 1]);
	for (int i = number.size() - 2; i >= 0; i--)
	{
		std::string next = std::to_string(number[i]);
		while (next.length() < MaxLength)
			next = '0' + next;
		result += next;
	}
	return result;
}
BigInteger BigInteger::operator>>(unsigned int x) const
{
	BigInteger result(0);
	result.number.clear();
	result.sign = sign;
	for (unsigned int i = 0; i < x; i++)
		result.number.push_back(0);
	for (unsigned int i = 0; i < number.size(); i++)
		result.number.push_back(number[i]);
	return result;
}
BigInteger BigInteger::mult(unsigned int x) const
{
	BigInteger result(0);
	result.number.clear();
	result.sign = sign;
	unsigned long long int current = 0;
	unsigned int remained = 0;
	for (unsigned int i = 0; i < number.size() + 1; i++)
	{
		current = 0;
		if (i < number.size())
		{
			current += number[i];
			current *= x;
		}
		current += remained;
		result.number.push_back(static_cast<unsigned int>(current % (MaxNumber + 1)));
		remained = static_cast<unsigned int>(current / (MaxNumber + 1));
	}
	while ((result.number[result.number.size() - 1] == 0) && (result.number.size() > 1))
		result.number.pop_back();
	result.set_sign();
	return result;
}
void BigInteger::summ(const BigInteger& x)
{
	unsigned int maxlength = 1;
	if (x.number.size() > number.size())
		maxlength = x.number.size();
	else
		maxlength = number.size();
	while (number.size() < maxlength + 1)
		number.push_back(0);
	unsigned long long int current = 0;
	unsigned int remained = 0;
	for (unsigned int i = 0; i < maxlength + 1; i++)
	{
		current = 0;
		if (i < number.size())
			current += number[i];
		if (i < x.number.size())
			current += x.number[i];
		current += remained;
		number[i] = static_cast<unsigned int>(current % (MaxNumber + 1));
		remained = static_cast<unsigned int>(current / (MaxNumber + 1));
	}
	while ((number[number.size() - 1] == 0) && (number.size() > 1))
		number.pop_back();
	set_sign();
}
void BigInteger::sub(const BigInteger& x)
{
	unsigned int maxlength = 1;
	if (x.number.size() > number.size())
		maxlength = x.number.size();
	else
		maxlength = number.size();
	while (number.size() < maxlength + 1)
		number.push_back(0);
	long long int current = 0;
	int remained = 0;
	for (unsigned int i = 0; i < maxlength + 1; i++)
	{
		current = 0;
		if (i < number.size())
			current += number[i];
		if (i < x.number.size())
			current -= x.number[i];
		current -= remained;
		if (current < 0)
		{
			number[i] = static_cast<unsigned int>(MaxNumber + 1 + current);
			remained = 1;
		}
		else
		{
			number[i] = static_cast<unsigned int>(current);
			remained = 0;
		}
	}
	while ((number[number.size() - 1] == 0) && (number.size() > 1))
		number.pop_back();
	set_sign();
}
void BigInteger::set_sign()
{
	if ((number.size() == 1) && (number[0] == 0))
		sign = 1;
}

