#include "pch.h"
#include "ShuffleData.h"
#include <fstream>

namespace shuffle
{
	//Folder part of a path
	static std::string DirOf(const std::string& path)
	{
		size_t cut = path.find_last_of("\\/");
		return (cut == std::string::npos) ? std::string() : path.substr(0, cut + 1);
	}

	static bool Exists(const std::string& path)
	{
		std::ifstream file(path.c_str(), std::ios::binary);
		return file.good();
	}

	std::string FindDataFile(const char* name)
	{

		std::vector<std::string> folders;
		folders.push_back(DirOf(__FILE__));   //source folder
		folders.push_back(std::string());     //working directory

		char exePath[MAX_PATH] = {};
		GetModuleFileNameA(nullptr, exePath, MAX_PATH);
		std::string folder = DirOf(exePath);
		for (int level = 0; level < 8; level++)
		{
			if (folder.empty())
			{
				break;
			}
			folders.push_back(folder);
			folder.pop_back();
			folder = DirOf(folder);
		}

		const char* subFolders[] = { "", "data/" };
		for (const std::string& f : folders)
		{
			for (const char* sub : subFolders)
			{
				std::string path = f + sub + name;
				if (Exists(path))
				{
					return path;
				}
			}
		}
		return std::string();
	}
}
