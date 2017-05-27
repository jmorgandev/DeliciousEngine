#ifndef DELICIOUS_RESOURCE_TYPES_H
#define DELICIOUS_RESOURCE_TYPES_H

enum resource_type {
	RESOURCE_UNKNOWN,
	RESOURCE_TEXTURE,
	RESOURCE_SHADER
};
struct file_format {
	char extension[8];
	resource_type type;
};

#endif