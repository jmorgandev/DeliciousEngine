#ifndef DELICIOUS_MACROS_H
#define DELICIOUS_MACROS_H

#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))

#endif