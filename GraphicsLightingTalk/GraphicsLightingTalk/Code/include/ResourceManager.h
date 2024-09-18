#pragma once
#include <map>
#include <string>
#include <memory>
#include <iostream>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include "Debug.h"
#include "MyTime.h"
using namespace std;

class IResource
{
public:
    IResource() = default;
    virtual ~IResource();
    virtual void Load() = 0;
    virtual void Unload() = 0;
    virtual bool IsLoaded() = 0;
    std::filesystem::path filename;
};

class ResourceManager {
private:
    std::map<std::string, IResource*> resources;

public:
    ResourceManager() = default;
    ~ResourceManager();

    template <typename T>
    T* Create(const std::string& id, std::filesystem::path filename = "") {
        auto it = resources.find(id);
        if (it != resources.end()) {
            std::cout << "Replacing resource with ID : " << id << std::endl;
            delete it->second;
        }
        else {
            std::cout << "Creating resource with ID : " << id << std::endl;
        }
        T* newResource = new T();
        resources[id] = newResource;
        resources[id]->filename = filename;
        return newResource;
    }

    template <typename T>
    T* Get(const std::string& id) {
        auto it = resources.find(id);
        if (it == resources.end()) {
            std::cout << "This resource doesn't exist" << std::endl;
            return nullptr;
        }
        else {
            std::cout << "Found the resource : " << id << std::endl;
            return static_cast<T*>(it->second);
        }
    }

    template <typename T>
    T* Delete(const std::string id) {
        auto it = resources.find(id);
        if (it == resources.end()) {
            std::cout << "This resource doesn't exist" << std::endl;
        }
        else {
            std::cout << "Deleting the resource : " << id << std::endl;
            delete it->second;
            resources.erase(id);
        }
    }
};