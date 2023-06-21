#pragma once

#include <string>
#include "../common/GLObject.h"

namespace Object_Loader{

	bool ImportObject(const std::string& path, GLObject& object);

}