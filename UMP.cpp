#include "UMP.h"
#include <algorithm>
using namespace std;

static const UMP zero(0), one(1);

UMP::UMP(unsigned long long a) {
	push_back(a);
	push_back(a >> (8 * sizeof(unsigned int)));
	check();
}

int UMP::cmp(const UMP &a, const UMP &b) {
	if (a.size() > b.size()) {
		return 1;
	}
	else if (a.size() < b.size()) {
		return -1;
	}
	else {
		for (int i = a.size() - 1; i >= 0; --i) {
			if (a[i] > b[i]) {
				return 1;
			}
			else if (a[i] < b[i]) {
				return -1;
			}
		}
		return 0;
	}
}

bool UMP::operator==(const UMP &b) const {
	int res = cmp(*this, b);
	if (res == 0) {
		return true;
	}
	return false;
}

bool UMP::operator!=(const UMP &b) const {
	return !(*this == b);
}

bool UMP::operator>(const UMP &b) const {
	int res = cmp(*this, b);
	if (res == 1) {
		return true;
	}
	return false;
}

bool UMP::operator<(const UMP &b) const {
	int res = cmp(*this, b);
	if (res == -1) {
		return true;
	}
	return false;
}

bool UMP::operator>=(const UMP &b) const {
	return !(*this < b);
}

bool UMP::operator<=(const UMP &b) const {
	return !(*this > b);
}

UMP UMP::operator+(const UMP &b) const {
	if (size() < b.size()) {
		return b + (*this);
	}
	UMP a;
	a.resize(size() + 1);
	unsigned long long res;
	for (int i = 0; i < b.size(); ++i) {
		res = at(i) + (unsigned long long)b[i] + a[i];
		a[i] = (unsigned int)res;
		a[i + 1] = res >> (8 * sizeof(unsigned int));
	}
	for (int i = b.size(); i < size(); ++i) {
		res = at(i) + (unsigned long long)a[i];
		a[i] = (unsigned int)res;
		a[i + 1] = res >> (8 * sizeof(unsigned int));
	}
	return a.check();
}

UMP &UMP::operator+=(const UMP &b) {
	UMP &a = *this;
	return a = a + b;
}

UMP UMP::operator-(const UMP &b) const {
	int cmpres = cmp(*this, b);
	const UMP *pa = nullptr, *pb = nullptr; // require *pa > *pb
	if (cmpres == 0) {
		return zero;
	}
	else if (cmpres == -1) {
		pa = &b;
		pb = this;
	}
	else {
		pa = this;
		pb = &b;
	}
	UMP _b = (*pb);
	int bsize = _b.size();
	for (int i = 0; i < bsize; ++i) {
		_b[i] = ~_b[i];
	}
#warning _b++ required
	_b = _b + one;
	_b.resize(bsize);
	UMP ret;

	ret.resize(pa->size() + 1);
	unsigned long long res;
	for (int i = 0; i < _b.size(); ++i) {
		res = pa->at(i) + (unsigned long long)_b.at(i) + ret[i];
		ret[i] = (unsigned int)res;
		ret[i + 1] = res >> (8 * sizeof(unsigned int));
	}
	for (int i = _b.size(); i < pa->size(); ++i) {
		res = pa->at(i) + ((unsigned long long)(-1)) + (unsigned long long)ret[i];
		ret[i] = (unsigned int)res;
		ret[i + 1] = res >> (8 * sizeof(unsigned int));
	}
	return ret.check();
}

UMP &UMP::operator-=(const UMP &b) {
	UMP &a = *this;
	return a = a - b;
}

UMP &UMP::check() {
	while (this->at(size() - 1) == 0 && size() != 1) {
		resize(size() - 1);
	}
	return *this;
}
