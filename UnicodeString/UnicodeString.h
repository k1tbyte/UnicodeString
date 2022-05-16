#pragma once

#ifndef _UNICODE_STRING_
#define _UNICODE_STRING_

#pragma execution_character_set( "utf-8" )

#include <iostream>
#include <Windows.h>
#include <string>
class UnicodeString {
	inline const static const wchar_t
		* _ru_Up  = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ",
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


	/// INCREMENT && DECREMENT

	UnicodeString& operator++(int) {
		if (!_symbol_find()) {
			*this = std::to_wstring(std::stoll(this->_wstr)+1).c_str();
		}
		else {
			for (int i = 0; i < this->_str_len; i++) {
				if (_wstr[i] >= '0' && _wstr[i] <= '8') _wstr[i] += 1;
				if (_wstr[i] == '9') _wstr[i] = '0';
			}
		}
		return *this;
	}

	UnicodeString& operator--(int) {
		if (!_symbol_find()) {
			*this = std::to_wstring(std::stoll(this->_wstr) - 1).c_str();
		}
		else {
			for (int i = 0; i < this->_str_len; i++) {
				if (_wstr[i] >= '1' && _wstr[i] <= '9') _wstr[i] -= 1;
				if (_wstr[i] == '0') _wstr[i] = '9';
			}
		}
		return *this;
	}



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

	inline void push_front(const wchar_t str) { _insert_str(0, &str); }

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

	inline void pop_front() { _erase(0, 1); }



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

	UnicodeString& replace(const UnicodeString& replacement_str, const UnicodeString& new_str);

	/// INSERT
	///
	UnicodeString& insert(USHORT pos, const UnicodeString& other);

	UnicodeString& insert(USHORT pos, const UnicodeString& other, USHORT subpos, USHORT sublen);

	UnicodeString& insert(USHORT pos, const wchar_t* other);

	UnicodeString& insert(USHORT pos, const wchar_t* s, USHORT n);

	UnicodeString& insert(USHORT pos, USHORT n, wchar_t c);



	/// FIND
	///
	bool find(const UnicodeString& other) const;

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


	/// SWAP

	void swap(UnicodeString& other);

	friend void operator <=> (UnicodeString & first, UnicodeString & second);

	/// DATA

	UnicodeString& cinFilter(int length, const char* mode = "str", wchar_t min = L' ', wchar_t max = L'ґ', int minDig = 0, int maxDig = 9);

	UnicodeString substr(USHORT pos, USHORT len) const;

	UnicodeString& unspaced();

	USHORT capacity() const;

	int length();

	wchar_t* c_str();

	bool empty() const;

	bool isdigit() { return _symbol_find() == false ? true : false; }

	bool isalpha() { return _digit_find() == false ? true : false; }

	UnicodeString& title();

	void resize(USHORT n);

	void resize(USHORT n, wchar_t c);

	void reserve(USHORT n);

	void clear();

	void shrink_to_fit();

	USHORT max_size() const { return (USHORT)-1; }

	const char* data() const;

	/// LOCALE

	UnicodeString& to_lower(size_t start,size_t end,const char* locale = "ALL");

	UnicodeString& to_lower(const char* locale = "ALL");
	
	UnicodeString& to_upper(const char* locale = "ALL");

	UnicodeString& to_upper(size_t start, size_t end, const char* locale = "ALL");



	/// SOME STR TYPE CONVERT

	operator std::string();

	operator std::wstring();

private:

	void _to_lower(size_t start, size_t end, const char* locale);

	void _to_upper(size_t start,size_t end, const char* locale);

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
	
	bool _symbol_find();

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

};

/// NON MEMBER

inline void swap(UnicodeString& first, UnicodeString& second) { first.swap(second); }

char* _utf8_encode(const wchar_t* wstr);

wchar_t* _utf8_decode(const char* str);

inline void SetConsoleUnicodeLocale()
{
	SetConsoleOutputCP(65001);
	SetConsoleCP(65001);
}

namespace switch_data {
	template<class T>
	struct SwitchData {
		bool bEnterFall;
		bool bEnterDefault;
		bool bDone;
		T strPtrThrSw;
		SwitchData(T arg) : strPtrThrSw(arg) {}
		inline bool transition(bool fall, const T& cnst, bool ndeflt) {
			if (bDone)
				return false;
			if (fall && bEnterFall)
				return 1;
			if (!fall && bEnterFall) {
				bDone = 1;
				return 0;
			}
			if (ndeflt) {
				bEnterFall = strPtrThrSw == cnst;
				if (bEnterFall) bEnterDefault = false;
			}
			return ndeflt ?
				bEnterFall :
				bEnterDefault;
		}
		void cpp97() {};
	};
} 

#define SWITCH(arg) if(1){switch_data::SwitchData< decltype(arg) > \
    switch__d_a_t_a(arg); \
  switch__d_a_t_a.bEnterDefault=true;switch__d_a_t_a.bEnterFall=false; \
  switch__d_a_t_a.bDone=false;if(switch__d_a_t_a.transition(false,

#define CASE(cnst)  cnst,true)){

#define BREAK       switch__d_a_t_a.bDone=true; \
                    ;} if(switch__d_a_t_a.transition(false,

#define FALL        ;} if(switch__d_a_t_a.transition(true,

#define DEFAULT     switch__d_a_t_a.strPtrThrSw,false)){

#define END         ;}};


#endif