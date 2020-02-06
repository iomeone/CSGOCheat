#include "MaterialUtils.h"
#include "MemoryUtils.h"

#include "InterfaceManager.h"
#include "RandomStuff.h"

using InitKeyValuesFn = void(__thiscall*)(void* thisptr, const char* name);
using LoadFromBufferFn = void(__thiscall*)(void* thisptr, const char* resourceName, const char* pBuffer, void* pFileSystem, const char* pPathID, void* pfnEvaluateSymbolProc);
InitKeyValuesFn InitKeyValuesEx;
LoadFromBufferFn LoadFromBufferEx;

void InitKeyValues(KeyValues* pKeyValues, const char* name)
{
	InitKeyValuesEx = (InitKeyValuesFn)(MemoryUtils::FindBytePattern(game_interfaces::get_client_dll(), "55 8B EC 51 33 C0 C7 45"));
	InitKeyValuesEx(pKeyValues, name);
}

void LoadFromBuffer(KeyValues* pKeyValues, const char* resourceName, const char* pBuffer, void* pFileSystem = nullptr, const char* pPathID = NULL, void* pfnEvaluateSymbolProc = nullptr)
{
	LoadFromBufferEx = (LoadFromBufferFn)(MemoryUtils::FindBytePattern(game_interfaces::get_client_dll(), "55 8B EC 83 E4 F8 83 EC 34 53 8B 5D 0C 89 4C 24 04"));
	LoadFromBufferEx(pKeyValues, resourceName, pBuffer, pFileSystem, pPathID, pfnEvaluateSymbolProc);
}

int LastMaterialCreated;
IMaterial* MaterialUtils::CreateMaterial(bool flat, bool ignorez, bool wireframed)
{
	LastMaterialCreated += 1;

	std::string type = (flat) ? "UnlitGeneric" : "VertexLitGeneric";

	std::string matdata = "\"" + type + "\"\n{\n\t\"$basetexture\" \"vgui/white_additive\"\n\t\"$envmap\"  \"\"\n\t\"$model\" \"1\"\n\t\"$flat\" \"1\"\n\t\"$nocull\"  \"0\"\n\t\"$selfillum\" \"1\"\n\t\"$halflambert\" \"1\"\n\t\"$nofog\"  \"0\"\n\t\"$znearer\" \"0\"\n\t\"$wireframe\" \"" + std::to_string(wireframed) + "\"\n\t\"$ignorez\" \"" + std::to_string(ignorez) + "\"\n}\n";

	std::string matname = "Custom_Material_" + std::to_string(LastMaterialCreated);

	KeyValues* pKeyValues = new KeyValues();
	InitKeyValues(pKeyValues, type.c_str());
	LoadFromBuffer(pKeyValues, matname.c_str(), matdata.c_str());

	typedef IMaterial*(__thiscall* OriginalFn)(void*, const char* pMaterialName, KeyValues* pVMTKeyValues);
	IMaterial* createdMaterial = vfunction< OriginalFn >(game_interfaces::MaterialSystem, 83)(game_interfaces::MaterialSystem, matname.c_str(), pKeyValues);

	createdMaterial->IncrementReferenceCount();

	return createdMaterial;
}

IMaterial* MaterialUtils::CreateMaterial(bool flat, bool ignorez, bool wireframed, std::string material_name)
{
	LastMaterialCreated += 1;

	std::string type = (flat) ? "UnlitGeneric" : "VertexLitGeneric";

	std::string matdata = "\"" + type + "\"\n{\n\t\"$basetexture\" \"" + material_name + "\"\n\t\"$envmap\"  \"\"\n\t\"$model\" \"1\"\n\t\"$flat\" \"1\"\n\t\"$nocull\"  \"0\"\n\t\"$selfillum\" \"1\"\n\t\"$halflambert\" \"1\"\n\t\"$nofog\"  \"0\"\n\t\"$znearer\" \"0\"\n\t\"$wireframe\" \"" + std::to_string(wireframed) + "\"\n\t\"$ignorez\" \"" + std::to_string(ignorez) + "\"\n}\n";

	std::string matname = "Custom_Material_" + std::to_string(LastMaterialCreated);

	KeyValues* pKeyValues = new KeyValues();
	InitKeyValues(pKeyValues, type.c_str());
	LoadFromBuffer(pKeyValues, matname.c_str(), matdata.c_str());

	typedef IMaterial*(__thiscall* OriginalFn)(void*, const char* pMaterialName, KeyValues* pVMTKeyValues);
	IMaterial* createdMaterial = vfunction< OriginalFn >(game_interfaces::MaterialSystem, 83)(game_interfaces::MaterialSystem, matname.c_str(), pKeyValues);

	createdMaterial->IncrementReferenceCount();

	return createdMaterial;
}