#include "resources.h"

#include "engine.h"
#include "std_formats.h"
#include "dff.h"
#include "dcf.h"

bool Resources::init(Engine* engine_in) {
	return true;
}

void Resources::load(cstring filepath) {

	cstring extension = dff::path_extension(filepath);
	resource_type file_type = RESOURCE_UNKNOWN;

	for (const auto& format : standard_formats) {
		if (dcf::str_cmp_exact(extension, format.extension)) {
			file_type = format.type;
		}
	}

	switch (file_type) {
	case RESOURCE_TEXTURE:
		break;
	case RESOURCE_SHADER:
		break;
	}

}