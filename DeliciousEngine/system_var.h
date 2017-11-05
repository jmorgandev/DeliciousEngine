#ifndef DELICIOUS_SYSTEM_VAR_H
#define DELICIOUS_SYSTEM_VAR_H

union system_var {
	int   as_int;
	float as_float;
	bool  as_bool;
	system_var() { as_int = 0; }
	system_var(int value)   { as_int   = value; }
	system_var(float value) { as_float = value; }
	system_var(bool value)  { as_bool  = value; }
};

#endif