#include "UMP.h"
#include <algorithm>
#include <stack>
#include <string>
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
	union {
		unsigned long long ull;
		struct {
			unsigned int lower, higher;
		} split;
	} res;
	// unsigned long long res;
	for (int i = 0; i < b.size(); ++i) {
		res.ull = at(i) + (unsigned long long)b[i] + a[i];
		a[i] = res.split.lower;
		a[i + 1] = res.split.higher;
	}
	for (int i = b.size(); i < size(); ++i) {
		res.ull = at(i) + (unsigned long long)a[i];
		a[i] = res.split.lower;
		a[i + 1] = res.split.higher;
	}
	return a.check();
}

UMP &UMP::operator+=(const UMP &b) {
	UMP &a = *this;
	return a = a + b;
}

UMP UMP::operator<<(unsigned int a) const {
	UMP ans(*this);
	if (a / 32 != 0) {
		ans.insert(ans.begin(), a / 32, 0);
		a = a % 32;
	}
	if (a != 0) {
		ans.resize(ans.size() + 1);
		for (int i = ans.size() - 1; i > 0; --i) {
			ans[i] |= (ans[i - 1] >> (32 - a));
			ans[i - 1] <<= a;
		}
	}
	return ans.check();
}

UMP UMP::operator>>(unsigned int a) const {
	UMP ans(*this);
	if (a >= 32) {
		ans.erase(ans.begin(), ans.begin() + (a / 32 - 1));
		a = a % 32;
	}
	if (a != 0) {
		for (int i = 0; i < ans.size() - 1; ++i) {
			ans[i] >>= a;
			ans[i] |= (ans[i + 1] << (32 - a));
		}
		ans[ans.size() - 1] >>= a;
	}
	return ans.check();
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
	UMP _a = (*pa);
	int asize = _a.size();
	for (int i = 0; i < asize; ++i) {
		_a[i] = ~_a[i];
	}
#warning _b++ required
	_a = _a + one;
	UMP ret;
	ret = _a + *pb;
	int retsize = ret.size();
	for (int i = 0; i < retsize; ++i) {
		ret[i] = ~ret[i];
	}
	ret += 1;
	// _b.resize(bsize);
	// UMP ret;

	// ret.resize(pa->size() + 1);
	// unsigned long long res;
	// for (int i = 0; i < _b.size(); ++i) {
	// 	res = pa->at(i) + (unsigned long long)_b.at(i) + ret[i];
	// 	ret[i] = (unsigned int)res;
	// 	ret[i + 1] = res >> (8 * sizeof(unsigned int));
	// }
	// for (int i = _b.size(); i < pa->size(); ++i) {
	// 	res = pa->at(i) + ((unsigned long long)(-1)) + (unsigned long long)ret[i];
	// 	ret[i] = (unsigned int)res;
	// 	ret[i + 1] = res >> (8 * sizeof(unsigned int));
	// }
	return ret.check();
}

UMP &UMP::operator-=(const UMP &b) {
	UMP &a = *this;
	return a = a - b;
}

UMP operator*(const UMP &a, const unsigned int _b) {
	unsigned long long b = _b;
	UMP ans;
	ans.assign(a.size() + 1,0);
	unsigned long long res = 0;
	for (int i = 0; i < a.size(); ++i) {
		res += b * a.at(i);
		ans[i] = (unsigned int)res;
		res >>= 32;
	}
	ans[a.size()] = res;
	return ans.check();
}

UMP operator*(const unsigned int a, const UMP &b) {
	return b * a;
}

UMP UMP::operator*(const UMP &_b) const {
	const UMP *a, *b;
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
	UMP a1, a2(a->at(0)), b1, b2(b->at(0));
	a1.assign(a->begin(), a->end() - 1);
	b1.assign(b->begin(), b->end() - 1);
	UMP x= a1 * b1, y = a2 * b2, z = (a1 + a2) * (b1 + b2) - (x + y);
	x.insert(x.begin(), 2, 0);
	y.insert(y.begin(), 0);
	return x + y + z;
	// int c = a->size() / 2;
	// vector<unsigned int>::const_iterator iter1 = a->begin() + c;
	// a1.assign(iter1, a->end());
	// a2.assign(a->begin(), iter1);
	// if (a->size() >= 2 * b->size()) {
	// 	b1 = zero;
	// 	b2 = *b;
	// }
	// else {
	// 	vector<unsigned int>::const_iterator iter2 = b->begin() + c;
	// 	b1.assign(iter2, b->end());
	// 	b2.assign(b->begin(), iter2);
	// }
	// UMP x1, x2, x3;
	// x1 = a1 * b1;
	// x3 = a2 * b2;
	// x2 = (a1 + a2) * (b1 + b2) - x1 - x3;
	// for (int i = 0; i < c; ++i) {
	// 	x1.push_back(0);
	// 	x1.push_back(0);
	// 	x2.push_back(0);
	// }
	// return x1 + x2 + x3;
}

UMP& UMP::operator*=(const unsigned int b) {
	return *this = *this * b;
}

UMP &UMP::operator*=(const UMP &_b) {
	return *this = *this * _b;
}

std::pair<UMP, UMP> div(UMP a, UMP b) {
	if (a.size() < b.size()) {
		return make_pair(0, a);
	}
	UMP ans;
	unsigned int d;
	{
		int _a, _b;
		unsigned int tmp1 = a.back();
		for (_a = 0; _a < 32 && tmp1 != 0; ++_a, tmp1 >>= 1) {}
		unsigned int tmp2 = b.back();
		for (_b = 0; _b < 32 && tmp2 != 0; ++_b, tmp2 >>= 1) {}
		if (_a - _b < 0 && a.size() == b.size()) {
			return make_pair(0, a);
		}
		d = (a.size() - b.size()) * 32 + _a - _b;
		b = b << d;
	}
	for (int i = 0; i <= d; ++i, b = b >> 1) {
		ans = ans << 1;
		if (a >= b) {
			ans += 1;
			a -= b;
		}
	}
	return make_pair(ans, a);
}

UMP operator/(const UMP &a, const UMP &b) {
	return div(a, b).first;
}

UMP operator%(const UMP &a, const UMP &b) {
	return div(a, b).second;
}

std::ostream &operator<<(std::ostream &out, UMP a) {
	stack<char> s;
	while (a != 0) {
		pair<UMP, UMP> res = div(a, 10);
		s.push(res.second[0] + '0');
		a = res.first;
	}
	if (s.size() == 0) {
		s.push('0');
	}
	while (!s.empty()) {
		out << s.top();
		s.pop();
	}
	return out;
}

std::istream &operator>>(std::istream &in, UMP &a) {
	a = zero;
	string str;
	in >> str;
	int i = 0;
	if (str[0] == '-') {
		i = 1;
	}
	for (; i < str.size(); ++i) {
		if (str[i] < '0' || str[i] > '9') {
			a = zero;
			return in;
		}
		else {
			a *= 10;
			a += str[i] - '0';
		}
	}
	return in;
}

UMP &UMP::check() {
	while (this->at(size() - 1) == 0 && size() != 1) {
		resize(size() - 1);
	}
	return *this;
}
