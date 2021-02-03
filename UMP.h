#pragma once
#include <string>
#include <vector>

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

	UMP operator+(const UMP &b) const;
	UMP &operator+=(const UMP &b);
	UMP operator-(const UMP &b) const;
	UMP &operator-=(const UMP &b);
	friend UMP operator*(const UMP &a, const unsigned int b);
	friend UMP operator*(const unsigned int a, const UMP &b);
	UMP operator*(const UMP &_b) const;
	UMP &operator*=(const unsigned int b);
	UMP &operator*=(const UMP &_b);
	friend UMP operator/(UMP a, UMP b);

protected:
	UMP &check();
};