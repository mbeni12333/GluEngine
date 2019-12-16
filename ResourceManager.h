#pragma once
#include <unordered_map>
#include <string>
#include <sstream>
#include "Utilities.h"

template<typename Derived, typename T>
class ResourceManager{
public:
	ResourceManager(const std::string& l_pathsFile){
		LoadPaths(l_pathsFile);
	}
	virtual ~ResourceManager(){ PurgeResources();}

	T* GetResource(const std::string& l_id){
		auto res = Find(l_id);
		return (res ? res->first : nullptr);
	}

	std::string GetPath(const std::string& l_id){
		auto path = m_paths.find(l_id);

		return(paths!=m_paths.end() ? paths->second : "");
	}

	bool RequireResource(const std::string& l_id){
		auto res = Find(l_id);
		// if res is found
		if(res){
			// increment the counter
			++res->second;
			return true;
		}
		// not found, load the ressource
		auto path = m_paths.find(l_id);
		if(path==m_paths.end()){ return false; }

		T* resource = Load(path->second);

		if(!resource){ return false; }
		// put the new loaded ressource to the map
		m_resources.emplace(l_id, std::make_pair(resource, 1));
		return true;
	}

	bool ReleaseResource(const std::string& l_id){
		auto res = Find(l_id);
		if(!res){ return false; }
		// the resource was found
		--(res->second);
		// if the resource is no longer required, free the memory
		if(!res->second){
			Unload(l_id);
		}
		return true;
	}
	void PurgeResources(){
		while(m_resources.begin()!=m_resources.end()){
			delete m_resources.begin()->second.first;
			m_resources.erase(m_resources.begin());
		}
	}

	T* Load(const std::string& l_id){
		return static_cast<Derived*>(this)->Load(l_id);
	}


private:
	std::pair<T*, unsigned int>* Find(const std::string& l_id){

		auto itr = m_resources.find(l_id);
		
		return (itr!=m_resources.end() ? &(itr->second) : nullptr);
	}
	bool Unload(const std::string& l_id){
		auto itr = m_resources.find(l_id);
		if(itr == m_resources.end()){ return false; }
		// free the allocated memory
		delete itr->second.first;
		// remove from resources
		m_resources.erase(itr);
		return true;
	}
	void LoadPaths(const std::string& l_pathFile){
		
		std::ifstream paths;
		paths.open(Utils::GetWorkingDirectory()+l_pathFile);

		if(paths.is_open()){
			std::string line;
			while(std::getline(paths, line)){

				std::stringstream keystream(line);

				std::string pathName;
				std::string path;
				keystream >> pathName; 
				keystream >> path;
				m_paths.emplace(pathName, path);
			}
			paths.close();
			return;
		}
		// file did not open;
		std::cerr<<"!Failed Loading the path file: "<<l_pathFile<<std::endl;
	}
	// countainer of elements of type T, and a counter
	std::unordered_map<std::string, std::pair<T*, unsigned int>> m_resources;
	// names - files
	std::unordered_map<std::string, std::string> m_paths;
};

