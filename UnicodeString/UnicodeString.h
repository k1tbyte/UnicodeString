#pragma once

#ifndef _UNICODE_STRING_
#define _UNICODE_STRING_

#pragma execution_character_set( "utf-8" )

#include <iostream>
#include <Windows.h>

class UnicodeString {

private:
	const wchar_t
		* _ru_Up = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ",
		* _ru_Down = L"абвгдеёжзийклмнопрстуфхцчшщъыьэюя",
		* _ua_Up = L"ҐЄІЇ",
		* _ua_Down = L"ґєії",
		* _gr_Up = L"ABГΔEZHΘIKΛMNΞOПPΣTΥΦΧΨΩ",
		* _gr_Down = L"αβγδεζηθικλμνξοπρστυφχψω";
		
private:
	wchar_t* _wstr = nullptr;
	USHORT _str_len = 0;
	USHORT _str_cap = 0;
	USHORT _increaseBy = 15;

public:

	/// STREAM OPERATORS

	friend std::wostream& operator << (std::wostream& out, UnicodeString& str);

	friend std::ostream& operator << (std::ostream& out, UnicodeString& str);

	friend std::istream& operator >> (std::istream& in, UnicodeString& str);

	friend std::wistream& operator >> (std::wistream& win, UnicodeString& str);



	/// CONSTRUCTORS

	UnicodeString(const wchar_t* str);

	UnicodeString();

	UnicodeString(const UnicodeString& other);

	UnicodeString(UnicodeString&& other) noexcept;

	UnicodeString(const char* str);

	UnicodeString(std::string& str);

	UnicodeString(std::wstring& str);



	/// DESTRUCTOR

	~UnicodeString() {
		delete[] _wstr;
		_str_len = 0;
		_str_cap = 0;
		_increaseBy = 0;
	}



	/// CONCATENATION OPERATORS

	UnicodeString& operator += (const UnicodeString& str);

	UnicodeString& operator +=(const wchar_t* str);

	UnicodeString& operator +=(wchar_t str);

	friend UnicodeString operator + (const UnicodeString& first, const UnicodeString& second);


	
	/// CONCATENATION METHODS

	UnicodeString& append(const UnicodeString& str);

	UnicodeString& append(const wchar_t* str);

	UnicodeString& append(wchar_t str);

	UnicodeString& append(std::wstring str);

	UnicodeString& append(USHORT n, wchar_t c);

	UnicodeString& append(const wchar_t* str, USHORT n);

	UnicodeString& append(const UnicodeString& other, USHORT subpos, USHORT sublen);

	inline void push_back(const wchar_t str) { _append(&str, 1); }



	/// ERASE OPERATORS

	UnicodeString& operator -=(const UnicodeString& str);

	UnicodeString& operator -=(const wchar_t str);

	UnicodeString& operator -=(wchar_t& str);

	friend UnicodeString operator - (const UnicodeString& frist, const UnicodeString& second);



	/// ERASE METHODS

	UnicodeString& erase(USHORT pos, USHORT len);

	UnicodeString& erase(USHORT pos);

	UnicodeString& explus(const wchar_t& str);

	UnicodeString& del(const UnicodeString& other);

	UnicodeString& reverse_del(const UnicodeString& other);

	UnicodeString& del_all(const UnicodeString& other);

	inline void pop_back() { _erase(_str_len - 1, 1); }



	/// LOGICAL COMPARE OPERATORS

	friend bool operator !=(const UnicodeString& first, const UnicodeString& second);

	friend bool operator ==(const UnicodeString& first, const UnicodeString& second);

	friend bool operator >=(const UnicodeString& first, const UnicodeString& second);

	friend bool operator <=(const UnicodeString& first, const UnicodeString& second);

	friend bool operator >(const UnicodeString& first, const UnicodeString& second);

	friend bool operator <(const UnicodeString& first, const UnicodeString& second);



	/// EQUALITY OPERATORS

	UnicodeString& operator= (const UnicodeString& other);

	UnicodeString& operator = (UnicodeString&& other) noexcept;



	/// CONVERT TO NUMS

	const int to_int();

	const long to_long();

	const long long to_llong();

	const unsigned long long to_ullong();

	const float to_float();
	
	const double to_double();

	const long double to_ldouble();
	


	/// INDEX OPERATORS

	wchar_t& operator[](size_t i);

	const wchar_t& operator[](size_t i)const;



	/// INDEX METHODS

	wchar_t& at(size_t i);

	const wchar_t& at(size_t i) const;

	wchar_t& front();

	const wchar_t front() const;

	wchar_t& back();

	const wchar_t& back() const;



	/// REPLACE
	///
	UnicodeString& replace(USHORT pos, size_t len, const UnicodeString& other);

	UnicodeString& replace(USHORT pos, size_t len, const UnicodeString& other, USHORT subpos, USHORT sublen);

	UnicodeString& replace(USHORT pos, size_t len, const wchar_t* s);

	UnicodeString& replace(USHORT pos, USHORT len, const wchar_t* other, USHORT n);

	UnicodeString& replace(USHORT pos, USHORT len, USHORT n, wchar_t c);



	/// INSERT
	///
	UnicodeString& insert(USHORT pos, const UnicodeString& other);

	UnicodeString& insert(USHORT pos, const UnicodeString& other, USHORT subpos, USHORT sublen);

	UnicodeString& insert(USHORT pos, const wchar_t* other);

	UnicodeString& insert(USHORT pos, const wchar_t* s, USHORT n);

	UnicodeString& insert(USHORT pos, USHORT n, wchar_t c);



	/// FIND
	///
	USHORT find(const UnicodeString& other, USHORT pos) const;

	USHORT find(const wchar_t* s, USHORT pos) const;

	USHORT find(const wchar_t* s, USHORT pos, USHORT n) const;

	USHORT find(wchar_t c, USHORT pos) const;

	USHORT reverseFind(const UnicodeString& other, USHORT pos) const;

	USHORT reverseFind(const wchar_t* s, USHORT pos) const;

	USHORT reverseFind(const wchar_t* s, USHORT pos, USHORT n) const;

	USHORT reverseFind(wchar_t c, USHORT pos) const;

	USHORT find_first_of(const UnicodeString& other, USHORT pos) const;

	USHORT find_first_of(const wchar_t* other, USHORT pos) const;

	USHORT find_first_of(const wchar_t* other, USHORT pos, USHORT n) const;

	USHORT find_first_of(wchar_t c, USHORT pos) const;

	USHORT find_last_of(const UnicodeString& other, USHORT pos) const;

	USHORT find_last_of(const wchar_t* other, USHORT pos) const;

	USHORT find_last_of(const wchar_t* other, USHORT pos, size_t n) const;

	USHORT find_last_of(wchar_t c, USHORT pos) const;

	USHORT find_first_not_of(const UnicodeString& other, USHORT pos) const;

	USHORT find_first_not_of(const wchar_t* other, USHORT pos) const;

	USHORT find_first_not_of(const wchar_t* other, USHORT pos, size_t n) const;

	USHORT find_first_not_of(wchar_t c, USHORT pos) const;

	USHORT find_last_not_of(const UnicodeString& other, USHORT pos) const;

	USHORT find_last_not_of(const wchar_t* other, USHORT pos) const;

	USHORT find_last_not_of(const wchar_t* other, USHORT pos, size_t n) const;

	USHORT find_last_not_of(wchar_t c, USHORT pos) const;



	/// DATA

	UnicodeString substr(USHORT pos, USHORT len) const;

	USHORT capacity() const;

	int length();

	wchar_t* c_str();

	bool empty() const;

	void swap(UnicodeString& other);

	void resize(USHORT n);

	void resize(USHORT n, wchar_t c);

	void reserve(USHORT n);

	void clear();

	void shrink_to_fit();

	

	/// Locale

	UnicodeString& to_lower(const char* locale = "ALL");

	UnicodeString& to_upper(const char* locale = "ALL");

private:

	USHORT _find(const wchar_t* other, USHORT len, USHORT pos) const;

	USHORT _rfind(const wchar_t* other, USHORT len, USHORT pos) const;

	bool _find_compare(const wchar_t* other, USHORT len, USHORT pos) const;

	USHORT _find_first_of(const wchar_t* other, USHORT len, USHORT pos, bool isTrue) const;

	USHORT _find_last_of(const wchar_t* other, USHORT len, USHORT pos, bool isTrue) const;

	bool _find_of_compare(const wchar_t* other, size_t len, USHORT pos, bool isTrue) const;

	void _insert_str(USHORT pos, const wchar_t* other);

	void _insert_str(USHORT pos, const wchar_t* other, USHORT n);

	void _replace(USHORT pos, USHORT len, const wchar_t* other, USHORT n);

	void _substr(wchar_t*& buffer, const wchar_t* other, USHORT pos, USHORT len) const;

	void _decreaseCapacity(const size_t cap);

	bool _digit_find();

	void _erase(USHORT position, USHORT len);

	USHORT _getLength(const UnicodeString& str, USHORT pos, USHORT len) const;

	int _compare(const UnicodeString& str) const;
	
	inline void _append(const wchar_t* other) { return _append(other, wcslen(other)); }

	void _append(const wchar_t* other, USHORT n);

	void _setLength(const USHORT len);

	void _clear_str(const USHORT pos);

	void _fill_str(wchar_t* other, const USHORT len, const USHORT pos, wchar_t c) const;

	void _increaseCapacity(const USHORT n);

	void _setCapacity(const USHORT cap);

	void _mem_allocator(wchar_t*& buffer, const USHORT n)  const;

	void _alloc_cwstring(wchar_t*& buffer, const USHORT n)  const;

	void _alloc_cwstring(wchar_t*& buffer, const USHORT n, wchar_t c) const;

	char* _utf8_encode(const wchar_t* wstr, int nchars);

	wchar_t* _utf8_decode(const char* str, int nbytes);
};

#endif