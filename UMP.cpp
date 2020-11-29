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

UMP UMP::operator*(const unsigned int b) const {
	UMP ans;
	ans.assign(size() + 1,0);
	unsigned long long res = 0;
	for (int i = 0; i < size(); ++i) {
		res += b * at(i);
		ans[i] = (unsigned int)res;
		res >>= 32;
	}
	ans[size()] = res;
	return ans.check();
}

UMP UMP::operator*(const UMP &_b) const {
	const UMP *a, *b;
	UMP a1, a2, b1, b2;
	if (size() < _b.size()) {
		a = &_b;
		b = this;
	}
	else {
		a = this;
		b = &_b;
	}
	if (b->size() == 1) {
		return (*a) * (b->at(0));
	}
	int c = a->size() / 2;
	vector<unsigned int>::const_iterator iter1 = a->begin() + c;
	a1.assign(iter1, a->end());
	a2.assign(a->begin(), iter1);
	if (a->size() >= 2 * b->size()) {
		b1 = zero;
		b2 = *b;
	}
	else {
		vector<unsigned int>::const_iterator iter2 = b->begin() + c;
		b1.assign(iter2, b->end());
		b2.assign(b->begin(), iter2);
	}
	UMP x1, x2, x3;
	x1 = a1 * b1;
	x3 = a2 * b2;
	x2 = (a1 + a2) * (b1 + b2) - x1 - x3;
	for (int i = 0; i < c; ++i) {
		x1.push_back(0);
		x1.push_back(0);
		x2.push_back(0);
	}
	return x1 + x2 + x3;
}

UMP &UMP::check() {
	while (this->at(size() - 1) == 0 && size() != 1) {
		resize(size() - 1);
	}
	return *this;
}
