#include "cuint.h"
#include <algorithm>
#include <stack>
#include <string>
using namespace std;

static const CUINT zero(0), one(1);

CUINT::CUINT(unsigned long long a) {
	assign(2, 0);
	(*this)[0] = a;
	(*this)[1] = a >> 32;
	check();
}

int CUINT::cmp(const CUINT &a, const CUINT &b) {
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

bool CUINT::operator==(const CUINT &b) const {
	int res = cmp(*this, b);
	if (res == 0) {
		return true;
	}
	return false;
}

bool CUINT::operator!=(const CUINT &b) const {
	return !(*this == b);
}

bool CUINT::operator>(const CUINT &b) const {
	int res = cmp(*this, b);
	if (res == 1) {
		return true;
	}
	return false;
}

bool CUINT::operator<(const CUINT &b) const {
	int res = cmp(*this, b);
	if (res == -1) {
		return true;
	}
	return false;
}

bool CUINT::operator>=(const CUINT &b) const {
	return !(*this < b);
}

bool CUINT::operator<=(const CUINT &b) const {
	return !(*this > b);
}

CUINT CUINT::operator<<(unsigned int a) const {
	if (a >= size() * 32) { 
		return zero;
	}
	CUINT ans(*this);
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

CUINT &CUINT::operator<<=(unsigned int a) {
	return *this = *this << a;
}

CUINT CUINT::operator>>(unsigned int a) const {
	if (a >= size() * 32) { 
		return zero;
	}
	CUINT ans(*this);
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

CUINT &CUINT::operator>>=(unsigned int a) {
	return *this = *this >> a;
}

CUINT CUINT::operator&(const CUINT &a) const {
	const CUINT *pa, *pb;
	if (size() > a.size()) {
		pa = this;
		pb = &a;
	}
	else {
		pa = &a;
		pb = this;
	}
	const CUINT &_a = *pa, &_b = *pb;
	CUINT ret;
	ret.resize(_a.size());
	for (int i = 0; i < _b.size(); ++i) {
		ret[i] = _a[i] & _b[i];
	}
	return ret;
}

CUINT &CUINT::operator&=(const CUINT &a) {
	return *this = *this & a;
}

CUINT CUINT::operator|(const CUINT &a) const {
	const CUINT *pa, *pb;
	if (size() > a.size()) {
		pa = this;
		pb = &a;
	}
	else {
		pa = &a;
		pb = this;
	}
	const CUINT &_a = *pa, &_b = *pb;
	CUINT ret;
	ret.resize(_a.size());
	for (int i = 0; i < _b.size(); ++i) {
		ret[i] = _a[i] | _b[i];
	}
	for (int i = _b.size(); i < _a.size(); ++i) {
		ret[i] = _a[i];
	}
	return ret;
}

CUINT &CUINT::operator|=(const CUINT &a) {
	return *this = *this | a;
}

CUINT CUINT::operator~() const {
	CUINT ret;
	ret.resize(size());
	for (int i = 0; i < size(); ++i) {
		ret[i] = ~(*this)[i];
	}
	return ret;
}

CUINT CUINT::operator+(const CUINT &b) const {
	if (size() < b.size()) {
		return b + (*this);
	}
	CUINT a;
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

CUINT &CUINT::operator+=(const CUINT &b) {
	CUINT &a = *this;
	return a = a + b;
}

CUINT CUINT::operator-(const CUINT &b) const {
	int cmpres = cmp(*this, b);
	const CUINT *pa = nullptr, *pb = nullptr; // require *pa > *pb
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
	CUINT _a = (*pa), _b = (*pb);
	int asize = _a.size(), bsize = _b.size();
	// for (int i = 0; i < bsize; ++i) {
	// 	_b[i] = ~_b[i];
	// }
	_b = ~_b;
#warning _b++ required
	_b = _b + one;
	CUINT ret;
	ret.resize(pa->size() + 1);
	union {
		unsigned long long ull;
		struct {
			unsigned int lower, higher;
		} split;
	} res;
	// unsigned long long res;
	for (int i = 0; i < _b.size(); ++i) {
		res.ull = pa->at(i) + (unsigned long long)_b.at(i) + ret[i];
		ret[i] = res.split.lower;
		ret[i + 1] = res.split.higher;
	}
	for (int i = _b.size(); i < pa->size(); ++i) {
		res.ull = pa->at(i) + ((unsigned long long)(-1)) + (unsigned long long)ret[i];
		ret[i] = res.split.lower;
		ret[i + 1] = res.split.higher;
	}
	// ret.check();
	auto endi = ret.end();
	--endi;
	ret.erase(endi, ret.end());
	return ret.check();
}

CUINT &CUINT::operator-=(const CUINT &b) {
	CUINT &a = *this;
	return a = a - b;
}

CUINT operator*(const CUINT &a, const unsigned int _b) {
	unsigned long long b = _b;
	CUINT ans;
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

CUINT operator*(const unsigned int a, const CUINT &b) {
	return b * a;
}

CUINT CUINT::operator*(const CUINT &_b) const {
	const CUINT *a, *b;
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
	CUINT a1, a2(a->at(0)), b1, b2(b->at(0));
	a1.assign(a->begin(), a->end() - 1);
	b1.assign(b->begin(), b->end() - 1);
	CUINT x= a1 * b1, y = a2 * b2, z = (a1 + a2) * (b1 + b2) - (x + y);
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
	// CUINT x1, x2, x3;
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

CUINT& CUINT::operator*=(const unsigned int b) {
	return *this = *this * b;
}

CUINT &CUINT::operator*=(const CUINT &_b) {
	return *this = *this * _b;
}

std::pair<CUINT, CUINT> CUINT::div(const CUINT &a, const CUINT &b) {
	switch (CUINT::cmp(a, b)) {
	case -1: {
		return make_pair(zero, a);
	}
	case 0: {
		return make_pair(one, zero);
	}
	}
	if (a.size() == 1) {
		return make_pair(a[0] / b[0], a[0] % b[0]);
	}
	unsigned int d = a.size() - b.size();
	unsigned int a1 = a[a.size() - 1], a2 = a[a.size() - 2], b1 = b[b.size() - 1];
	CUINT ans;
	if (a1 > b1) {
		unsigned int ans1 = a1 / b1;
		ans.assign(d + 1, 0);
		ans[d] = ans1;
		auto res = CUINT::div(a - b * ans, b);
		return make_pair(ans + res.first, res.second);
	}
	else {
		if (d == 0) {
			return make_pair(one, a - b);
		}
		else {
			unsigned int ans1 = (((unsigned long long)a1 << 32) + a2) / (b1 + 1);
			ans.assign(d, 0);
			ans[d - 1] = ans1;
			auto res = CUINT::div(a - b * ans, b);
			return make_pair(ans + res.first, res.second);
		}
	}
/*
	unsigned int d = a.size() - b.size();
	if (d < 0) {
		return make_pair(0, a);
	}
	if (d == 0) {
		switch (CUINT::cmp(a, b)) {
		case -1: {
			return make_pair(zero, a);
		}
		case 0: {
			return make_pair(one, zero);
		}
		}
	}
	if (a.size() == 1) {
		return make_pair(a[0] / b[0], a[0] % b[0]);
	}
	CUINT ans;
	unsigned int a1 = a[a.size() - 1], a2 = a[a.size() - 2], b1 = b[b.size() - 1];
	if (a1 > b1) {
		unsigned int ans1 = a1 / b1;
		ans.assign(d + 1, 0);
		ans[d] = ans1;
		auto res = CUINT::div(a - b * ans, b);
		return make_pair(ans + res.first, res.second);
	}
	else {
		unsigned int ans1 = ((unsigned long long)a1 >> 32 + a2) / (b1 + 1);
		ans.assign(d, 0);
		ans[d - 1] = ans1;
		auto res = CUINT::div(a - b * ans, b);
		return make_pair(ans + res.first, res.second);
	}
*/
/*
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
*/
}

CUINT operator/(const CUINT &a, const CUINT &b) {
	return CUINT::div(a, b).first;
}

CUINT &CUINT::operator/=(const unsigned int b) {
	return *this = *this / b;
}

CUINT operator%(const CUINT &a, const CUINT &b) {
	return CUINT::div(a, b).second;
}

CUINT &CUINT::operator%=(const CUINT &b) {
	return *this = *this % b;
}

std::ostream &operator<<(std::ostream &out, CUINT a) {
	stack<char> s;
	while (a != 0) {
		pair<CUINT, CUINT> res = CUINT::div(a, 10);
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

std::istream &operator>>(std::istream &in, CUINT &a) {
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

CUINT &CUINT::check() {
//	while (this->at(size() - 1) == 0 && size() != 1) {
//		resize(size() - 1);
//	}
	auto iter = end();
	--iter;
	while (*iter == 0 && iter != begin()) {
		--iter;
	}
	++iter;
    	erase(iter, end());
	return *this;
}

