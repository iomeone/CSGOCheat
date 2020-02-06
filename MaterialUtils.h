#pragma once

#include "Material.h"

namespace MaterialUtils
{
	extern IMaterial* CreateMaterial(bool flat, bool ignorez, bool wireframed, std::string material_name);
	extern IMaterial* CreateMaterial(bool flat, bool ignorez, bool wireframed);
}