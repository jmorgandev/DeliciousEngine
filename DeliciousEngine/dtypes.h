#ifndef DTYPES_H
#define DTYPES_H

#include <stdint.h>

typedef unsigned long long uint64, qword;
typedef signed long long int64;
typedef unsigned int uint32, dword;
typedef signed int int32;
typedef unsigned int uint;
typedef unsigned short uint16, word;
typedef signed short int16;
typedef unsigned char uint8, uchar, byte;
typedef signed char int8;
typedef const char *cstring;

template <class type> class scoped_ptr {
public:
	scoped_ptr() : raw_pointer(nullptr) {}
	scoped_ptr(type* pointer) : raw_pointer(pointer) {}
	~scoped_ptr() { free(); }

	scoped_ptr& operator=(const scoped_ptr& ptr) { raw_pointer = ptr.raw_pointer; return *this; }
	scoped_ptr& operator=(type* ptr) { raw_pointer = ptr; return *this; }
	type& operator* () { return *raw_pointer; }
	type& operator[] (int i) { return raw_pointer[i]; }
	type* operator-> () { return raw_pointer; }
	type* raw() { return raw_pointer; }
	void free() { if (raw_pointer) { delete raw_pointer; raw_pointer = nullptr; } }
private:
	type* raw_pointer;
};
template <class type> class scoped_array {
public:
	scoped_array() : raw_pointer(nullptr) {}
	scoped_array(type* pointer) : raw_pointer(pointer) {}
	~scoped_array() { free(); }

	scoped_array& operator=(const scoped_array& ptr) { raw_pointer = ptr.raw_pointer; return *this; }
	scoped_array& operator=(type* ptr) { raw_pointer = ptr; return *this; }
	type& operator* () { return *raw_pointer; }
	type& operator[] (int i) { return raw_pointer[i]; }
	type* operator-> () { return raw_pointer; }
	type* raw() { return raw_pointer; }
	void free() { if (raw_pointer) { delete[] raw_pointer; raw_pointer = nullptr; } }
private:
	type* raw_pointer;
};

#endif