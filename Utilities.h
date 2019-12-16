#pragma once
#define RUNNING_WINDOWS

#include <iostream>
#include <string>
#include <algorithm>

namespace Utils{
	#ifdef RUNNING_WINDOWS
		#define WIN32_LEAN_AND_MEAN
		#include <Windows.h>
		#include <Shlwapi.h>

			inline std::string GetWorkingDirectory(){
				HMODULE hModule = GetModuleHandle(nullptr);
				if(hModule){
					char path[256];
					GetModuleFileName(hModule, path, sizeof(path));
					PathRemoveFileSpec(path);
					strcat_s(path, "\\");
					return std::string(path);
				}
				return "";
			}
	#elif defined RUNNING_LINUX
		#include <unistd.h>
			inline std::string GetWorkingDirectory(){
				char cwd[1024];
				if(getcwd(cwd, sizeof(cwd))!=nullptr){
					return std::string(cwd)+std::string("/");
				}
				return "";
			}
	#endif
}