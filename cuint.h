#pragma once
#include <string>
#include <vector>
#include <iostream>

class CUINT : public ::std::vector<unsigned int> {
public:
	CUINT(unsigned long long a = 0);
	// CUINT(::std::string str);

	static int cmp(const CUINT &a, const CUINT &b); // 1 for greater, 0 for equal and -1 for less
	bool operator==(const CUINT &b) const;
	bool operator!=(const CUINT &b) const;
	bool operator>(const CUINT &b) const;
	bool operator<(const CUINT &b) const;
	bool operator>=(const CUINT &b) const;
	bool operator<=(const CUINT &b) const;

	CUINT operator<<(unsigned int a) const;
	CUINT &operator<<=(unsigned int a);
	CUINT operator>>(unsigned int a) const;
	CUINT &operator>>=(unsigned int a);
	CUINT operator&(const CUINT &a) const;
	CUINT &operator&=(const CUINT &a);
	CUINT operator|(const CUINT &a) const;
	CUINT &operator|=(const CUINT &a);
	CUINT operator~() const;

	CUINT operator+(const CUINT &b) const;
	CUINT &operator+=(const CUINT &b);
	CUINT operator-(const CUINT &b) const;
	CUINT &operator-=(const CUINT &b);
	friend CUINT operator*(const CUINT &a, const unsigned int b);
	friend CUINT operator*(const unsigned int a, const CUINT &b);
	CUINT operator*(const CUINT &_b) const;
	CUINT &operator*=(const unsigned int b);
	CUINT &operator*=(const CUINT &_b);

	static std::pair<CUINT, CUINT> div(const CUINT &a, const CUINT &b); // first for quotient and second for remainder
	friend CUINT operator/(const CUINT &a, const CUINT &b);
	CUINT &operator/=(const unsigned int b);
	friend CUINT operator%(const CUINT &a, const CUINT &b);
	CUINT &operator%=(const CUINT &_b);

public:
	friend std::ostream &operator<<(std::ostream &out, CUINT a);
	friend std::istream &operator>>(std::istream &in, CUINT &a);
protected:
	CUINT &check();
};

