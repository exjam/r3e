#ifndef STR_CLASS
#define STR_CLASS

#include <stdio.h>
#include <string.h>
#include "Array.hpp"

#ifndef TCHAR
# ifdef  _UNICODE
	typedef wchar_t TCHAR;
#   define _T(x) L##x
# else
	typedef char TCHAR;
#   define _T(x) x
# endif
#endif

class String : protected Array<TCHAR, 128> {
public:
	class Iterator {
	public:
		Iterator() : mIndex(0) {}
		Iterator(unsigned int idx) : mIndex(idx) {}
		
		operator unsigned int(){
			return mIndex;
		}

		unsigned int Position(){
			return mIndex;
		}

		bool IsValid(){
			return mIndex != String::End();
		}

		Iterator& operator++(){
			mIndex += 1;
			return *this;
		}

	protected:
		unsigned int mIndex;

		friend class String;
	};

public:
	String(){}

	String(const TCHAR* src){
		Append(src);
	}

	String(const String& src){
		Append(src);
	}

	~String(){
		Array::Delete();
	}

	TCHAR At(Iterator pos) const {
		if(!pos.IsValid()) return 0;
		return Array::At(pos);
	}

	void Null(Iterator pos){
		if(!pos.IsValid()) return;
		Array::At(pos) = 0;
	}

	void Append(const TCHAR other){
		Array::PushBack(other);
	}

	void Append(const TCHAR* other){
		Array::Append(other, tcharlen(other));
	}

	void Append(const String& other){
		Array::Append(other);
	}

	void Prepend(const TCHAR other){
		Array::PushFront(other);
	}

	void Clear(){
		Array::Clear();
	}

	void Chop(int n){
		SetSize(Size() - n);
	}

	void Erase(Iterator pos, unsigned int count){
		if(!pos.IsValid()) return;
		Array::Erase(pos.mIndex, count);
	}

	Iterator Find(const TCHAR other, Iterator from = Iterator(0)) const {
		return Array::Find(other, from.Position());
	}

	Iterator Find(const TCHAR* other, Iterator from = Iterator(0)) const {
		if(!other) return End();

		for(unsigned int i = from.mIndex; i < Size(); ++i){
			const TCHAR* c = other;
			for(unsigned int j = 0; *c; ++j, ++c){
				if(i + j >= Size()) return End();
				if(At(i + j) != *c) break;
			}

			if(!*c) return Iterator(i);
		}

		return End();
	}

	Iterator Find(const String& other, Iterator from = Iterator(0)) const {
		if(Size() < other.Size()) return End();

		for(unsigned int i = from.mIndex; i <= Size() - other.Size(); ++i){
			for(unsigned int j = 0; j < other.Size(); ++j){
				if(At(i + j) != other.At(j)) break;
				if(j + 1 == other.Size()) return Iterator(i);
			}
		}

		return End();
	}

	void Insert(Iterator pos, const TCHAR* other){
		if(!pos.IsValid()) return;
		Array::Insert(pos.mIndex, other, tcharlen(other));
	}

	void Insert(Iterator pos, const String& other){
		if(!pos.IsValid()) return;
		Array::Insert(pos.mIndex, other.Data(), other.Size());
	}

	void Replace(Iterator pos, unsigned int n, const TCHAR* other){
		if(!pos.IsValid()) return;
		Array::Erase(pos, n);
		Array::Insert(pos, other, tcharlen(other));
	}

	void ReplaceAll(const String& find, const String& replace, Iterator start = Iterator(0)){
		Iterator pos = Find(find, start);
		unsigned int len1 = find.Length();
		unsigned int len2 = replace.Length();
		while(pos != End()){
			Replace(pos, len1, replace);
			pos = Find(find, pos + len2);
		}
	}

	String SubStr(unsigned int start, unsigned int len){
		String str;
		for(unsigned int i = start; i < start + len; ++i)
			str.Append(At(i));

		return str;
	}

	static Iterator Begin(){
		return Iterator(0);
	}

	static Iterator End(){
		return Iterator(0xFFFFFFFF);
	}

	static TCHAR ToLower(const TCHAR chr){
		if(chr < 'A') return chr;
		if(chr > 'Z') return chr;
		return chr + 0x20;
	}

	static TCHAR ToUpper(const TCHAR chr){
		if(chr < 'a') return chr;
		if(chr > 'z') return chr;
		return chr - 0x20;
	}

	void ToLower(){
		for(unsigned int i = 0; i < Size(); ++i)
			Array::At(i) = ToLower(Array::At(i));
	}

	void ToUpper(){
		for(unsigned int i = 0; i < Size(); ++i)
			Array::At(i) = ToUpper(Array::At(i));
	}

	int Compare(const wchar_t* other, bool caseSensitive = false) const {
		unsigned int i = 0;
		for(other; *other; ++other, ++i){
			if(i >= Size()) return -1;

			TCHAR chr1 = At(i), chr2 = (TCHAR)(*other);
			if(!caseSensitive){
				chr1 = ToLower(chr1);
				chr2 = ToLower(chr2);
			}

			if(chr1 > chr2) return 1;
			if(chr1 < chr2) return -1;
		}

		if(i < Size()) return 1;
		return 0;
	}

	int Compare(const char* other, bool caseSensitive = false) const {
		unsigned int i = 0;
		for(other; *other; ++other, ++i){
			if(i >= Size()) return -1;

			TCHAR chr1 = At(i), chr2 = *other;
			if(!caseSensitive){
				chr1 = ToLower(chr1);
				chr2 = ToLower(chr2);
			}

			if(chr1 > chr2) return 1;
			if(chr1 < chr2) return -1;
		}

		if(i < Size()) return 1;
		return 0;
	}

	int Compare(const String& other, bool caseSensitive = false) const {
		unsigned int bSize = (Size() > other.Size()) ? Size() : other.Size();
		for(unsigned int i = 0; i < bSize; ++i){
			if(i >= Size()) return -1;
			if(i >= other.Size()) return 1;

			TCHAR chr1 = At(i), chr2 = other.At(i);
			if(!caseSensitive){
				chr1 = ToLower(chr1);
				chr2 = ToLower(chr2);
			}

			if(chr1 > chr2) return 1;
			if(chr1 < chr2) return -1;
		}

		return 0;
	}

	void Trim(){
		unsigned int i;

		for(i = 0; i < Size() && isWhitespace(At(i)); ++i);
		if(i) Erase(0, i);

		for(i = Size() - 1; i > 0 && isWhitespace(At(i)); --i);
		++i;
		if(i < Size()) Erase(i, Size() - i);
	}

	void SetLength(unsigned int len){
		if(len == End()) return;
		SetSize(len);
	}

	unsigned int Length() const {
		return Size();
	}

	TCHAR* Str() const {
		return Data();
	}

	operator const TCHAR*() const {
		return Data();
	}

	String& operator=(const String& other){
		if(other == *this) return *this;
		Clear();
		Append(other);

		return *this;
	}

	String& operator=(const TCHAR* other){
		if(other == *this) return *this;
		Clear();
		Append(other);
		return *this;
	}

	String& operator+=(const String& other){
		Append(other);
		return *this;
	}

	String& operator+=(const TCHAR* other){
		Append(other);
		return *this;
	}

	const String operator+(const String& other) const {
		String result = (*this);
		result += other;
		return result;
	}

	const String operator+(const TCHAR* other) const {
		String result = (*this);
		result += other;
		return result;
	}

	bool operator ==(const String& other) const {
		return Compare(other, true) == 0;
	}

	bool operator <(const String& other) const {
		return Compare(other, true) < 0;
	}

	bool operator >(const String& other) const {
		return Compare(other, true) > 0;
	}

	bool operator <=(const String& other) const {
		return Compare(other, true) <= 0;
	}

	bool operator >=(const String& other) const {
		return Compare(other, true) >= 0;
	}

	bool operator ==(const TCHAR* other) const {
		return Compare(other, true) == 0;
	}

	bool operator <(const TCHAR* other) const {
		return Compare(other, true) < 0;
	}

	bool operator >(const TCHAR* other) const {
		return Compare(other, true) > 0;
	}

	bool operator <=(const TCHAR* other) const {
		return Compare(other, true) <= 0;
	}

	bool operator >=(const TCHAR* other) const {
		return Compare(other, true) >= 0;
	}

	/*
	ARG TIME MUDDA FUKKA
	*/
	String& arg(unsigned int value, int base = 10){
		int arg = findFirstArg();
		if(arg == 0) return *this;

		String num;
		uintToString(value, base, num);

		replaceArgs(arg, num);
		return *this;
	}

	String& arg(int value, int base = 10, bool prefix = false){
		int arg = findFirstArg();
		if(arg == 0) return *this;

		String num;
		intToString(value, base, num);

		if(prefix && base == 16)
			num.Insert((value < 0)?1:0, _T("0x"));

		replaceArgs(arg, num);
		return *this;
	}

	String& arg(const String& str){
		int arg = findFirstArg();
		if(arg == 0) return *this;

		replaceArgs(arg, str);
		return *this;
	}

private:
	static void uintToString(unsigned int val, int base, String& str){
		if(val == 0){ str.SetLength(1); *str.Str() = '0'; return; }

		while(val > 0){
			str.Prepend(toChar(char(val % base)));
			val = val / base;
		}
	}

	static void intToString(long val, int base, String& str){
		if(val == 0){ str.SetLength(1); *str.Str() = '0'; return; }

		bool addMinus = false;
		if(val < 0){
			addMinus = true;
			val = -val;
		}

		while(val > 0){
			str.Prepend(toChar(char(val % base)));
			val = val / base;
		}

		if(addMinus)
			str.Prepend('-');
	}

	void replaceArgs(int arg, const String& str){
		TCHAR findStr[] = { '%', 0, 0, 0 };
		int len;
		if(arg >= 10){
			findStr[1] = toChar(char(arg / 10));
			findStr[2] = toChar(char(arg % 10));
			len = 3;
		}else{
			findStr[1] = toChar(char(arg));
			len = 2;
		}

		Iterator pos;
		pos = Find(findStr, pos);
		while(pos.IsValid()){
			Replace(pos, len, str);
			pos = Find(findStr, pos);
		}
	}

	int findFirstArg(){
		int first = 0;
		unsigned int end = Size() - 1;
		for(unsigned int i = 0; i < end; ++i){
			if(At(i) != '%') continue;
			if(At(i + 1) == '%'){ ++i; continue; }
			if(!isNumeric(At(i + 1))) continue;
			int num = toNum(At(i + 1));
			if(isNumeric(At(i + 2)))
				num = (num*10) + toNum(At(i + 2));

			if(first == 0 || num < first)
				first = num;
		}

		return first;
	}

	static TCHAR toChar(char num){
		if(num > 9) return 'A' + (num - 10);
		else return '0' + num;
	}

	static int toNum(const TCHAR chr){
		return chr - '0';
	}

	static bool isWhitespace(const TCHAR chr){
		return chr == ' '
			|| chr == '\t'
			|| chr == '\n'
			|| chr == '\r';
	}

	static bool isNumeric(const TCHAR chr){
		return chr >= '0' && chr <= '9';
	}

	static unsigned int tcharlen(const TCHAR* str){
		unsigned int len = 0;
		for(str; *str; ++str, ++len);
		return len;
	}
};

#endif
