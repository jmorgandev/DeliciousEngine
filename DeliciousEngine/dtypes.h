#ifndef DTYPES_H
#define DTYPES_H

#include <array>
#include <stdint.h>

typedef uint64_t uint64, qword;
typedef int64_t int64;
typedef uint32_t uint32, dword;
typedef int32_t int32;
typedef uint16_t uint16, word;
typedef int16_t int16;
typedef uint8_t uint8, uchar, byte;
typedef int8_t int8;

typedef unsigned int uint;
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