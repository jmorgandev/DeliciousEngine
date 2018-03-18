#ifndef DELICIOUS_SYSTEM_VAR_H
#define DELICIOUS_SYSTEM_VAR_H

union SystemVar {
	int   as_int;
	float as_float;
	bool  as_bool;
	SystemVar() { as_int = 0; }
	SystemVar(int i)   { as_int   = i; }
	SystemVar(float f) { as_float = f; }
	SystemVar(bool b)  { as_bool  = b; }
};

#endif