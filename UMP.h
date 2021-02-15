#pragma once
#include <string>
#include <vector>
#include <iostream>

class UMP : public ::std::vector<unsigned int> {
public:
	UMP(unsigned long long a = 0);
	// UMP(::std::string str);

	static int cmp(const UMP &a, const UMP &b); // 1 for greater, 0 for equal and -1 for less
	bool operator==(const UMP &b) const;
	bool operator!=(const UMP &b) const;
	bool operator>(const UMP &b) const;
	bool operator<(const UMP &b) const;
	bool operator>=(const UMP &b) const;
	bool operator<=(const UMP &b) const;

	UMP operator<<(unsigned int a) const;
	UMP operator>>(unsigned int a) const;
	UMP operator&(const UMP &a) const;
	UMP operator|(const UMP &a) const;
	UMP operator~() const;

	UMP operator+(const UMP &b) const;
	UMP &operator+=(const UMP &b);
	UMP operator-(const UMP &b) const;
	UMP &operator-=(const UMP &b);
	friend UMP operator*(const UMP &a, const unsigned int b);
	friend UMP operator*(const unsigned int a, const UMP &b);
	UMP operator*(const UMP &_b) const;
	UMP &operator*=(const unsigned int b);
	UMP &operator*=(const UMP &_b);

	friend std::pair<UMP, UMP> div(UMP a, UMP b); // first for quotient and second for remainder
	friend UMP operator/(const UMP &a, const UMP &b);
	UMP &operator/=(const unsigned int b);
	friend UMP operator%(const UMP &a, const UMP &b);
	UMP &operator%=(const UMP &_b);

public:
	friend std::ostream &operator<<(std::ostream &out, UMP a);
	friend std::istream &operator>>(std::istream &in, UMP &a);
protected:
	UMP &check();
};
