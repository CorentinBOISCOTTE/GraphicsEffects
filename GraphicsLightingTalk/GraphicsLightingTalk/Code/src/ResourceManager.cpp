#include "ResourceManager.h"

IResource::~IResource()
{
	std::cout << "Resource unloaded" << std::endl;
}

ResourceManager::~ResourceManager()
{
	for (std::pair<std::string, IResource*> r : resources)
	{
		r.second->Unload();
		delete r.second;
	}
	resources.clear();
}