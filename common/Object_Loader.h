#pragma once

#include <string>
#include "../glutilities/GLObject.h"

namespace Object_Loader{
	bool ImportObj(const std::string& path, GLObject& object);
	
}