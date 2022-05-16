#include "UnicodeString.h"

/// CONSTRUCTORS
///
UnicodeString::UnicodeString(const wchar_t* str)
{
	_append(str);
}

UnicodeString::UnicodeString() 
{
	_setCapacity(0);
	_setLength(0);
}

UnicodeString::UnicodeString(const UnicodeString& other) 
{
	_append(other._wstr, other._str_len);
}

UnicodeString::UnicodeString(UnicodeString&& other) noexcept 
{
	*this = std::move(other);
}

UnicodeString::UnicodeString(const char* str) 
{
	_append(_utf8_decode(str));
}

UnicodeString::UnicodeString(std::string& str) 
{
	_append(_utf8_decode(str.c_str()));
}

UnicodeString::UnicodeString(std::wstring& str)
{
	_append(str.c_str());
}



/// FRIEND STREAM OPERATORS
///
std::wostream& operator << (std::wostream& wout, UnicodeString& str)
{
	if (str._str_len > 0) wout << str._wstr;
	return wout;
}

std::ostream& operator << (std::ostream& out, UnicodeString& str)
{
	if (str._str_len > 0) out << _utf8_encode(str._wstr);
	return out;
}

std::istream& operator >> (std::istream& in, UnicodeString& str)
{
	str.clear();
	char c;
	do {
		c = in.get();
		if (c == '\n')
			break;
		str.push_back(c);
	} while (true);

	return in;
}

std::wistream& operator >> (std::wistream& win, UnicodeString& str)
{
	str.clear();
	wchar_t c;
	do {
		c = win.get();
		if (c == '\n')
			break;
		str.push_back(c);
	} while (true);

	return win;
}

/// CONCATENATION OPERATORS
///
UnicodeString& UnicodeString::operator += (const UnicodeString& str) 
{
	_append(str._wstr, str._str_len);
	return *this;
}

UnicodeString& UnicodeString::operator +=(const wchar_t* str) 
{
	_append(str);
	return *this;
}

UnicodeString& UnicodeString::operator +=(wchar_t str) 
{
	_append(&str, 1);
	return *this;
}

// FRIEND
UnicodeString operator + (const UnicodeString& first, const UnicodeString& second) 
{
	UnicodeString copy = first;
	copy += second;
	return copy;
}



/// CONCATENATION METHODS
///
UnicodeString& UnicodeString::append(const UnicodeString& str) 
{
	_append(str._wstr, str._str_len);
	return *this;
}

UnicodeString& UnicodeString::append(const wchar_t* str) 
{
	_append(str);
	return *this;
}

UnicodeString& UnicodeString::append(wchar_t str) 
{
	_append(&str, 1);
	return *this;
}

UnicodeString& UnicodeString::append(std::wstring str) 
{
	_append(str.c_str(), str.length());
	return *this;
}

UnicodeString& UnicodeString::append(USHORT n, wchar_t c)
{
	wchar_t* buffer = nullptr;
	_alloc_cwstring(buffer, n, c);
	_append(buffer, n);
	delete[] buffer;
	return *this;
}

UnicodeString& UnicodeString::append(const wchar_t* str, USHORT n)
{
	wchar_t* buffer = nullptr;
	_substr(buffer, str, 0, n);
	_append(buffer, n);
	delete[] buffer;
	return *this;
}

UnicodeString& UnicodeString::append(const UnicodeString& other, USHORT subpos, USHORT sublen)
{
	sublen = _getLength(other, subpos, sublen);
	wchar_t* buffer = nullptr;
	_substr(buffer, other._wstr, subpos, sublen);
	_append(buffer, sublen);
	delete[] buffer;
	return *this;
}




/// ERASE OPERATORS
///
UnicodeString& UnicodeString::operator -=(const UnicodeString& str) 
{
	_erase(0, str._str_len);
	return *this;
}

UnicodeString& UnicodeString::operator -=(const wchar_t str) 
{
	_erase(0, str);
	return *this;
}

UnicodeString& UnicodeString::operator -=(wchar_t& str) 
{
	_erase(0, str);
	return *this;
}

// FRIEND
UnicodeString operator - (const UnicodeString& first, const UnicodeString& second) 
{
	UnicodeString copy = first;
	copy -= second;
	return copy;
}



/// ERASE METHODS
///
UnicodeString& UnicodeString::erase(USHORT pos, USHORT len) 
{
	_erase(pos, len);
	return *this;
}

UnicodeString& UnicodeString::erase(USHORT pos) 
{
	_erase(pos, this->_str_len - pos);
	return *this;
}

UnicodeString& UnicodeString::explus(const wchar_t& str) 
{
	int counter = 0;
	for (int i = _str_len-1; 0 <= i ; i--) {
		if (_wstr[i] == str) {
			counter++;
			for (int j = i; j < _str_len; j++)
				_wstr[j] = _wstr[j + 1];
		}
	}
	_erase(_str_len - counter, counter);
	return *this;
}

UnicodeString& UnicodeString::del(const UnicodeString& other)
{
	USHORT n = _find(other._wstr, other._str_len, 0);
	if (n != (SHORT)-1) _erase(n, other._str_len);
	return *this;
}

UnicodeString& UnicodeString::reverse_del(const UnicodeString& other) 
{
	USHORT n = _rfind(other._wstr, other._str_len, _str_len);
	if (n != (SHORT)-1) _erase(n, other._str_len);
	return *this;
}

UnicodeString& UnicodeString::del_all(const UnicodeString& other) 
{
	USHORT n{};
	for (USHORT i = 0; i < this->_str_len - other._str_len; i++) {
		n = _rfind(other._wstr, other._str_len, _str_len);
		if (n != (SHORT)-1) _erase(n, other._str_len);
	}
	return *this;
}



/// FRIEND LOGICAL COMPARE OPERATORS
///
bool operator !=(const UnicodeString& first, const UnicodeString& second)
{
	return first._compare(second) != 0;
}

bool operator ==(const UnicodeString& first, const UnicodeString& second)
{
	return first._compare(second) == 0;
}

bool operator >=(const UnicodeString& first, const UnicodeString& second)
{
	return first._compare(second) >= 0;
}

bool operator <=(const UnicodeString& first, const UnicodeString& second)
{
	return first._compare(second) <= 0;
}

bool operator >(const UnicodeString& first, const UnicodeString& second)
{
	return first._compare(second) > 0;
}

bool operator <(const UnicodeString& first, const UnicodeString& second)
{
	return first._compare(second) < 0;
}



/// EQUALITY OPERATORS
///
UnicodeString& UnicodeString::operator= (const UnicodeString& other) 
{
	if (this != &other) {
		_setLength(0);
		_append(other._wstr, other._str_len);
	}
	return *this;
}

UnicodeString& UnicodeString::operator = (UnicodeString&& other) noexcept 
{
	if (this != &other) {
		delete[] _wstr;
		_wstr = other._wstr;
		_str_len = other._str_len;
		_str_cap = other._str_cap;
		other._wstr = nullptr;
		other._str_len = 0;
		other._str_cap = 0;
	}
	return *this;
}



/// CONVERT TO NUMS
///
const int UnicodeString::to_int()
{
	if (!_digit_find()) return 0;
	return std::stoi(this->_wstr);
}

const long UnicodeString::to_long()
{
	if (!_digit_find()) return 0;
	return std::stol(this->_wstr);
}

const long long UnicodeString::to_llong()
{
	if (!_digit_find()) return 0;
	return std::stoll(this->_wstr);
}

const unsigned long long UnicodeString::to_ullong()
{
	if (!_digit_find()) return 0;
	return std::stoull(this->_wstr);
}

const float UnicodeString::to_float()
{
	if (!_digit_find()) return 0;
	return std::stof(this->_wstr);
}

const double UnicodeString::to_double()
{
	if (!_digit_find()) return 0;
	return std::stod(this->_wstr);
}

const long double UnicodeString::to_ldouble()
{
	if (!_digit_find()) return 0;
	return std::stold(this->_wstr);
}



/// INDEX OPERATORS
///
wchar_t& UnicodeString::operator[](size_t i)
{
	if (i < 0 || _str_len <= i)
		throw;
	return *(_wstr + i);
}

const wchar_t& UnicodeString::operator[](size_t i)const 
{
	if (i < 0 || _str_len <= i)
		throw;
	return *(_wstr + i);
}



/// INDEX METHODS
///
wchar_t& UnicodeString::at(size_t i)
{
	if (i < 0 || _str_len <= i)
		throw;
	return operator[](i);
}

const wchar_t& UnicodeString::at(size_t i) const 
{
	if (i < 0 || _str_len <= i)
		throw;
	return operator[](i);
}

wchar_t& UnicodeString::front()
{ 
	return operator[](0); 
}

const wchar_t UnicodeString::front() const
{ 
	return operator[](0); 
}

wchar_t& UnicodeString::back()
{
	return operator[](_str_len - 1);
}

const wchar_t& UnicodeString::back() const
{ 
	return operator[](_str_len - 1);
}



/// REPLACE
///
UnicodeString& UnicodeString::replace(USHORT pos, size_t len, const UnicodeString& other) 
{
	_replace(pos, len, other._wstr, other._str_len);
	return *this;
}

UnicodeString& UnicodeString::replace(const UnicodeString& replacement_str, const UnicodeString& new_str)
{
	USHORT pos = _find(replacement_str._wstr, replacement_str._str_len, 0);
	if (pos != (USHORT)-1)
		_replace(pos, replacement_str._str_len, new_str._wstr, new_str._str_len);
	return *this;
}


UnicodeString& UnicodeString::replace(USHORT pos, size_t len, const UnicodeString& other, USHORT subpos, USHORT sublen) 
{
	sublen = _getLength(other, subpos, sublen);
	wchar_t* buffer = nullptr;
	_substr(buffer, other._wstr, subpos, sublen);
	_replace(pos, len, buffer, wcslen(buffer));
	delete[] buffer;
	return *this;
}

UnicodeString& UnicodeString::replace(USHORT pos, size_t len, const wchar_t* s)
{
	_replace(pos, len, s, wcslen(s));
	return *this;
}

UnicodeString& UnicodeString::replace(USHORT pos, USHORT len, const wchar_t* other, USHORT n) 
{
	_replace(pos, len, other, n);
	return *this;
}

UnicodeString& UnicodeString::replace(USHORT pos, USHORT len, USHORT n, wchar_t c)
{
	wchar_t* buffer = nullptr;
	_alloc_cwstring(buffer, n, c);
	_replace(pos, len, buffer, n);
	delete[] buffer;
	return *this;
}



/// INSERT
///
UnicodeString& UnicodeString::insert(USHORT pos, const UnicodeString& other) 
{
	_insert_str(pos, other._wstr, other._str_len);
	return *this;
}

UnicodeString& UnicodeString::insert(USHORT pos, const UnicodeString& other, USHORT subpos, USHORT sublen)
{
	sublen = _getLength(other, subpos, sublen);
	wchar_t* buffer = nullptr;
	_substr(buffer, other._wstr, subpos, sublen);
	_insert_str(pos, buffer, sublen);
	delete[] buffer;
	return *this;
}

UnicodeString& UnicodeString::insert(USHORT pos, const wchar_t* other)
{
	_insert_str(pos, other);
	return *this;
}

UnicodeString& UnicodeString::insert(USHORT pos, const wchar_t* s, USHORT n)
{
	wchar_t* buffer = nullptr;
	_substr(buffer, s, 0, n);
	_insert_str(pos, buffer, n);
	delete[] buffer;
	return *this;
}

UnicodeString& UnicodeString::insert(USHORT pos, USHORT n, wchar_t c)
{
	wchar_t* buffer = nullptr;
	_alloc_cwstring(buffer, n, c);
	_insert_str(pos, buffer, n);
	delete[] buffer;
	return *this;
}



/// FIND
///
bool UnicodeString::find(const UnicodeString& other) const
{
	return _find(other._wstr, other._str_len, 0) == (USHORT)-1 ? false : true;
}

USHORT UnicodeString::find(const UnicodeString& other, USHORT pos) const
{
	return _find(other._wstr, other._str_len, pos);
}

USHORT UnicodeString::find(const wchar_t* s, USHORT pos) const
{
	return _find(s, wcslen(s), pos);
}

USHORT UnicodeString::find(const wchar_t* s, USHORT pos, USHORT n) const
{
	return _find(s, n, pos);
}

USHORT UnicodeString::find(wchar_t c, USHORT pos) const
{
	return _find(&c, 1, pos);
}

USHORT UnicodeString::reverseFind(const UnicodeString& other, USHORT pos) const
{
	return _rfind(other._wstr, other._str_len, pos);
}

USHORT UnicodeString::reverseFind(const wchar_t* s, USHORT pos) const
{
	return  _rfind(s, wcslen(s), pos);
}

USHORT UnicodeString::reverseFind(const wchar_t* s, USHORT pos, USHORT n) const
{
	return _rfind(s, n, pos);
}

USHORT UnicodeString::reverseFind(wchar_t c, USHORT pos) const
{
	return _rfind(&c, 1, pos);
}

USHORT UnicodeString::find_first_of(const UnicodeString& other, USHORT pos) const
{
	return _find_first_of(other._wstr, other._str_len, pos, true);
}

USHORT UnicodeString::find_first_of(const wchar_t* other, USHORT pos) const
{
	return _find_first_of(other, wcslen(other), pos, true);
}

USHORT UnicodeString::find_first_of(const wchar_t* other, USHORT pos, USHORT n) const
{
	return _find_first_of(other, n, pos, true);
}

USHORT UnicodeString::find_first_of(wchar_t c, USHORT pos) const
{
	return _find_first_of(&c, 1, pos, true);
}

USHORT UnicodeString::find_last_of(const UnicodeString& other, USHORT pos) const
{
	return _find_last_of(other._wstr, other._str_len, pos, true);
}

USHORT UnicodeString::find_last_of(const wchar_t* other, USHORT pos) const
{
	return _find_last_of(other, wcslen(other), pos, true);
}

USHORT UnicodeString::find_last_of(const wchar_t* other, USHORT pos, size_t n) const
{
	return _find_last_of(other, n, pos, true);
}

USHORT UnicodeString::find_last_of(wchar_t c, USHORT pos) const
{
	return _find_last_of(&c, 1, pos, true);
}

USHORT UnicodeString::find_first_not_of(const UnicodeString& other, USHORT pos) const
{
	return _find_first_of(other._wstr, other._str_len, pos, false);
}

USHORT UnicodeString::find_first_not_of(const wchar_t* other, USHORT pos) const
{
	return _find_first_of(other, wcslen(other), pos, false);
}

USHORT UnicodeString::find_first_not_of(const wchar_t* other, USHORT pos, size_t n) const
{
	return _find_first_of(other, n, pos, false);
}

USHORT UnicodeString::find_first_not_of(wchar_t c, USHORT pos) const
{
	return _find_first_of(&c, 1, pos, false);
}

USHORT UnicodeString::find_last_not_of(const UnicodeString& other, USHORT pos) const
{
	return _find_last_of(other._wstr, other._str_len, pos, false);
}

USHORT UnicodeString::find_last_not_of(const wchar_t* other, USHORT pos) const
{
	return _find_last_of(other, wcslen(other), pos, false);
}

USHORT UnicodeString::find_last_not_of(const wchar_t* other, USHORT pos, size_t n) const
{
	return _find_last_of(other, n, pos, false);
}

USHORT UnicodeString::find_last_not_of(wchar_t c, USHORT pos) const
{
	return _find_last_of(&c, 1, pos, false);
}



/// SWAP
///
void UnicodeString::swap(UnicodeString& other)
{
	UnicodeString temp = std::move(*this);
	*this = std::move(other);
	other = std::move(temp);
}

// FRIEND
void operator <=> (UnicodeString& first, UnicodeString& second) 
{
	wchar_t* temp = new wchar_t[first._str_len+1];
	for (USHORT i = 0; i < first._str_len; i++){
		temp[i] = first._wstr[i];
	}
	first = second;
	second = temp;
	delete[] temp;
}


/// DATA
///
UnicodeString UnicodeString::substr(USHORT pos, USHORT len) const
{
	len = _getLength(*this, pos, len);
	wchar_t* buffer = nullptr;
	_substr(buffer, _wstr, pos, len);
	UnicodeString toReturn(buffer);
	delete[] buffer;
	return toReturn;
}

void UnicodeString::resize(USHORT n)
{
	if (_str_len == n) return;
	if (_str_len < n) {
		_setLength(n);
		return;
	}
	_clear_str(n);
}

void UnicodeString::resize(USHORT n, wchar_t c)
{
	if (_str_len == n) return;
	if (_str_len < n) {
		USHORT pos = _str_len;
		_setLength(n);
		_fill_str(_wstr, _str_len, pos, c);
		return;
	}
	_clear_str(n);
}

void UnicodeString::reserve(USHORT n)
{
	if (_str_cap == n) return;
	if (_str_cap < n) {
		_setCapacity(n);
		return;
	}

	if (_str_cap / 2 > _str_len) {
		_decreaseCapacity(_str_cap / 2);
	}
}

USHORT UnicodeString::capacity() const 
{ 
	return _str_cap;
}

void UnicodeString::clear() 
{
	_clear_str(0); 
}

bool UnicodeString::empty() const 
{
	return _str_len ? false : true; 
}

wchar_t* UnicodeString::c_str() 
{ 
	return _wstr; 
}

int UnicodeString::length() 
{
	return _str_len;
}

void UnicodeString::shrink_to_fit() 
{ 
	_decreaseCapacity(_str_len); 
}

UnicodeString& UnicodeString::title()
{
	this->to_lower();
	this->_to_upper(0, 1, "ALL");
	return *this;
}

UnicodeString& UnicodeString::unspaced()
{
	int counter = 0;
	for (int i = _str_len - 1; 0 <= i; i--) {
		if (_wstr[i] == ' ' && _wstr[i+1] == ' ') {
			counter++;
			for (int j = i; j < _str_len; j++)
				_wstr[j] = _wstr[j + 1];
		}
	}
	_erase(_str_len - counter, counter);
	return *this;
}

const char* UnicodeString::data() const {
	return _utf8_encode(_wstr); 
}



/// LOCALE
///
UnicodeString& UnicodeString::to_lower(size_t start, size_t end, const char* locale)
{
	_to_lower(start, end, locale);
	return *this;
}

UnicodeString& UnicodeString::to_upper(size_t start, size_t end, const char* locale)
{
	_to_upper(start, end, locale);
	return *this;
}

UnicodeString& UnicodeString::to_upper(const char* locale) {
	_to_upper(0, _str_len, locale);
	return *this;
}

UnicodeString& UnicodeString::to_lower(const char* locale) {
	to_lower(0, _str_len, locale);
	return *this;
}

UnicodeString& UnicodeString::cinFilter(int length, const char* mode, wchar_t min, wchar_t max, int minDig, int maxDig)
{
	_clear_str(0);
	int mode_len = strlen(mode), factive = 0;
	wchar_t key[1]{};
	if (mode_len > 3 && min < '0' || mode_len >3 && max > '9') {
		min = '0'; max = '9';
	}
	for (int i = 0; i <= length + 1; i++) {
	m: key[0] = _getwch();
		if (i == length + 1) {
			i--;
			goto m;
		}
		if (GetKeyState(VK_UP) < 0 || GetKeyState(VK_DOWN) < 0 || GetKeyState(VK_RIGHT) < 0 || GetKeyState(VK_LEFT) < 0)
			goto m;
		if (key[0] == VK_BACK) {
			if (i != 0) {
				if (i == factive)
					factive = 0;
				i--;
				this->_erase(_str_len - 1, 1);
				std::cout << "\b \b";
			}
			goto m;
		}
		if (key[0] == VK_RETURN && i != 0) {
			if (mode_len > 3 && (this->to_float() < minDig || this->to_float() > maxDig))
				goto m;
			return *this;
		}
		if (mode == "str" && key[0] < min || key[0] > max || mode_len > 4 && key[0] < min || key[0] > max)
			goto m;
		if (mode == "calc") {
			if (key[0] < min && key[0] != L'-' && key[0] != '.' || key[0] > max && key[0] != L'-' && key[0] != L'.')
				goto m;
			if (key[0] == L'-' && i != 0) goto m;
			if (key[0] == L'.') {
				if (factive != 0 || i == 0) goto m;
				factive = i;
			}
		}
		if (i != length) {
			*this += key[0];
			std::cout << _utf8_encode(key);
		}
	}
}



/// SOME STR TYPES CONVERT
///
UnicodeString::operator std::string() {
	return _utf8_encode(_wstr);
}

UnicodeString::operator std::wstring() {
	return _wstr;
}

/// PRIVATE
///
void UnicodeString::_to_lower(size_t start, size_t end, const char* locale) {
	start < 0 ? start = 0 : end > this->_str_len ? end = _str_len : true;
	for (USHORT i = start; i < end; i++) {
		if (locale == "ALL" || locale == "en_US" || locale == "ru_RU" || locale == "ua_UA") {
			if (_wstr[i] >= 'A' && _wstr[i] <= 'Z') _wstr[i] += 32;
			else if (locale == "ru_RU" || locale == "ua_UA" || locale == "ALL") {
				for (USHORT j = 0; j < 33; j++) {
					if (_wstr[i] == _ru_Up[j]) _wstr[i] = _ru_Down[j];
					else if (locale == "ua_UA") {
						for (USHORT k = 0; k < 4; k++)
							if (_wstr[i] == _ua_Up[k]) _wstr[i] = _ua_Down[k];
					}
				}
			}
		}

		else if (locale == "el_GR") {
			for (int j = 0; j < 24; j++)
				if (_wstr[i] == _gr_Up[j]) _wstr[i] = _gr_Down[j];
		}
	}
}

void UnicodeString::_to_upper(size_t start, size_t end, const char* locale) {
	start < 0 ? start = 0 : end > this->_str_len ? end = _str_len : true;
	for (USHORT i = start; i < end; i++) {
		if (locale == "ALL" || locale == "en_US" || locale == "ru_RU" || locale == "ua_UA") {
			if (_wstr[i] >= 'a' && _wstr[i] <= 'z') _wstr[i] -= 32;
			else if (locale == "ru_RU" || locale == "ua_UA" || locale == "ALL") {
				for (USHORT j = 0; j < 33; j++) {
					if (_wstr[i] == _ru_Down[j]) _wstr[i] = _ru_Up[j];
					else if (locale == "ua_UA") {
						for (USHORT k = 0; k < 4; k++)
							if (_wstr[i] == _ua_Down[k]) _wstr[i] = _ua_Up[k];
					}
				}
			}
		}

		else if (locale == "el_GR") {
			for (int j = 0; j < 24; j++)
				if (_wstr[i] == _gr_Down[j]) _wstr[i] = _gr_Up[j];
		}
	}
}

USHORT UnicodeString::_find(const wchar_t* other, USHORT len, USHORT pos) const
{
	USHORT toReturn = -1;
	if (pos == -1)
		pos = _str_len - 1;
	for (; pos <= _str_len - len; ++pos)
		if (operator[](pos) == *other && _find_compare(other, len, pos)) {
			toReturn = pos;
			break;
		}
	return toReturn;
}

USHORT UnicodeString::_rfind(const wchar_t* other, USHORT len, USHORT pos) const
{
	size_t toReturn = -1;
	if (pos == -1 || pos + len > _str_len + 1)
		pos = _str_len - len + 1;

	for (int i = pos; 0 <= i; --i)
		if (operator[](i) == *other && _find_compare(other, len, i)) {
			toReturn = i;
			break;
		}
	return toReturn;
}

bool UnicodeString::_find_compare(const wchar_t* other, USHORT len, USHORT pos) const 
{
	for (USHORT i = 1; i < len; ++i)
		if (operator[](pos + i) != *(other + i))
			return false;
	return true;
}

USHORT UnicodeString::_find_first_of(const wchar_t* other, USHORT len, USHORT pos, bool isTrue) const
{
	USHORT toReturn = -1;
	if (pos == -1)
		pos = _str_len - 1;

	for (; pos < _str_len; ++pos)
		if (_find_of_compare(other, len, pos, isTrue)) {
			toReturn = pos;
			break;
		}
	return toReturn;
}

USHORT UnicodeString::_find_last_of(const wchar_t* other, USHORT len, USHORT pos, bool isTrue) const 
{
	USHORT toReturn = -1;
	if (pos == -1)
		pos = _str_len - 1;

	for (int i = pos; 0 <= i; --i)
		if (_find_of_compare(other, len, i, isTrue)) {
			toReturn = i;
			break;
		}
	return toReturn;
}

bool UnicodeString::_find_of_compare(const wchar_t* other, size_t len, USHORT pos, bool isTrue) const 
{
	for (USHORT i = 0; i < len; ++i)
		if (operator[](pos) == *(other + i))
			return isTrue;
	return !isTrue;
}

void UnicodeString::_insert_str(USHORT pos, const wchar_t* other) 
{
	if (other != nullptr)
		_insert_str(pos, other, wcslen(other));
}

void UnicodeString::_insert_str(USHORT pos, const wchar_t* other, USHORT n)
{
	if (pos == _str_len) return _append(other, n);
	if (pos > _str_len) throw;
	if (n == 0) return;
	_increaseCapacity(_str_len + n);
	if (_wstr) {
		wchar_t* buffer = nullptr;
		_substr(buffer, _wstr, pos, _str_len);
		_clear_str(pos);
		_append(other, n);
		_append(buffer, wcslen(buffer));
		delete[] buffer;
	}
	else _append(other, n); // cat other to _str
}

void UnicodeString::_replace(USHORT pos, USHORT len, const wchar_t* other, USHORT n) 
{
	len = _getLength(*this, pos, len);
	wchar_t* buffer = nullptr;
	_substr(buffer, _wstr, pos + len, _str_len);
	_clear_str(pos);
	wchar_t* replace_buffer = nullptr;
	_substr(replace_buffer, other, 0, n);
	_append(replace_buffer);
	_append(buffer);

	delete[] replace_buffer;
	delete[] buffer;
}

void UnicodeString::_substr(wchar_t*& buffer, const wchar_t* other, USHORT pos, USHORT len) const
{
	if (other == nullptr) {
		throw;
	}
	_alloc_cwstring(buffer, len);
	for (int i = 0; i < len; ++i)
		buffer[i] = other[pos + i];
}

void UnicodeString::_decreaseCapacity(const size_t cap) 
{
	if (_str_cap < cap)
		return;
	if (_increaseBy > 15)
		--_increaseBy;
	_setCapacity(cap);
}

bool UnicodeString::_digit_find() 
{
	for (USHORT i = 0; i < this->_str_len; i++)
		if (this->_wstr[i] <= '9' && this->_wstr[i] >= '0') return true;
	return false;
}

bool UnicodeString::_symbol_find()
{
	for (USHORT i = 0; i < this->_str_len; i++)
		if (this->_wstr[i] > '9' || this->_wstr[i] < '0') return true;
	return false;
}

void UnicodeString::_erase(USHORT position, USHORT len)
{

	if (len > _str_len - position || _str_len < 1) return;

	len = _getLength(*this, position, len);
	for (USHORT i = position + len; i < this->_str_len; ++i)
		_wstr[i - len] = _wstr[i];
	_clear_str(_str_len - len);

}

USHORT UnicodeString::_getLength(const UnicodeString& str, USHORT pos, USHORT len) const 
{

	if (len == -1) len = _str_len - pos;
	if (pos + len > str._str_len) throw;

	return len;
}

int UnicodeString::_compare(const UnicodeString& str) const 
{

	if (this->_str_len < str._str_len) return 1;
	else if (this->_str_len > str._str_len) return -1;

	return wcscmp(this->_wstr, str._wstr);
}

void UnicodeString::_append(const wchar_t* other, USHORT n)
{
	if (!other || n == 0) // if other is null or size is 0 do nothing
		return;
	_increaseCapacity(_str_len + n);
	USHORT i = 0;
	for (; i < n; ++i)
		this->_wstr[_str_len + i] = other[i];
	this->_wstr[_str_len + i] = '\0';
	_setLength(_str_len + n);
}

void UnicodeString::_setLength(const USHORT len)
{
	if (_str_len > len)  // space already allocated
		_clear_str(len);
	else if (_str_cap < len)  // length is greater than capacity 
		_increaseCapacity(len); 	    // allocate memory, increase capacity       
	_str_len = len;
}

void UnicodeString::_clear_str(const USHORT pos) 
{
	_fill_str(_wstr, _str_len, pos, '\0');
	_str_len = pos;
}

void UnicodeString::_fill_str(wchar_t* other, const USHORT len, const USHORT pos, wchar_t c) const
{
	USHORT begin = pos;
	while (begin != len)
		other[begin++] = c;
	other[begin] = '\0';
}

void UnicodeString::_increaseCapacity(const USHORT n)
{
	if (_str_cap > n && _wstr)
		return;
	USHORT cap = _str_cap;
	while (cap <= n)
		cap += _increaseBy;
	_increaseBy++;

	_setCapacity(cap); // increase Capacity
}

void UnicodeString::_setCapacity(const USHORT cap) 
{
	if (_str_cap == cap && _wstr) 
		return;
	if (cap < _str_len)
		return;
	_str_cap = cap; 

	wchar_t* buffer = _wstr;	
	_wstr = nullptr;
	_mem_allocator(_wstr, _str_cap); // allocate memory
	if (buffer) {
		for (USHORT i = 0; i < _str_len; ++i)
			this->_wstr[i] = buffer[i];
		this->_wstr[_str_len] = '\0';
	}
	delete[] buffer;
}

void UnicodeString::_mem_allocator(wchar_t*& buffer, const USHORT n)  const
{
	if (buffer)
		throw;
	buffer = new wchar_t[n + 1]();
}

void UnicodeString::_alloc_cwstring(wchar_t*& buffer, const USHORT n)  const
{
	if (buffer)
		throw;
	buffer = new wchar_t[n + 1]();
}

void UnicodeString::_alloc_cwstring(wchar_t*& buffer, const USHORT n, wchar_t c) const
{
	_alloc_cwstring(buffer, n);
	_fill_str(buffer, n, 0, c);
}

char* _utf8_encode(const wchar_t* wstr) // wide char* to char* decoder
{
	int nbytes = 0;
	unsigned short nchars = wcslen(wstr);
	if ((nbytes = WideCharToMultiByte(CP_UTF8, 0x00000080, wstr, nchars, NULL, 0, NULL, NULL)) == 0)
		return nullptr;

	char* str = nullptr;
	if (!(str = new char[nbytes + 1]))return nullptr;

	str[nbytes] = '\0';
	if (WideCharToMultiByte(CP_UTF8, 0x00000080, wstr, nchars, str, nbytes, NULL, NULL) == 0) {
		delete[] str;
		return nullptr;
	}
	return str;
}

wchar_t* _utf8_decode(const char* str) // char* to wide char* decoder
{
	int nchars = 0;
	unsigned short nbytes = strlen(str);
	if ((nchars = MultiByteToWideChar(CP_UTF8,0x00000008, str, nbytes, NULL, 0)) == 0)
		return nullptr;

	wchar_t* wstr = nullptr;
	if (!(wstr = new wchar_t[((size_t)nchars + 1) * sizeof(wchar_t)])) 
		return nullptr;

	wstr[nchars] = L'\0';
	if (MultiByteToWideChar(CP_UTF8, 0x00000008,str, nbytes, wstr, (size_t)nchars) == 0) {
		delete[] wstr;
		return nullptr;
	}
	return wstr;
}



