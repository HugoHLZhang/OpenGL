#pragma once

#include <string>
#include "../common/GLObject.h"

namespace TinyObjLoader{

	bool ImportObject(const std::string& path, GLObject& object);

}