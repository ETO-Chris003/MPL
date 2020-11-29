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

	UMP operator+(const UMP &b) const;
	UMP &operator+=(const UMP &b);
	UMP operator-(const UMP &b) const;
	UMP &operator-=(const UMP &b);
	UMP operator*(const unsigned int b) const;
	UMP operator*(const UMP &_b) const;

protected:
	UMP &check();
};