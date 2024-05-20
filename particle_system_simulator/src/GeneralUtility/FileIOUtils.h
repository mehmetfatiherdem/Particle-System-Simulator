#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
#include <filesystem>

namespace utility::io
{
	inline std::string readFile(const std::string& fileName)
	{
		std::ifstream file(fileName);

		if (!file.is_open())
		{
			std::cerr << "Error opening file: " << fileName << std::endl;
			return "";
		}

		std::stringstream buffer;
		buffer << file.rdbuf();
		file.close();
		return buffer.str();
	}

	inline void writeFile(const std::string& fileName, const std::string& content)
	{
		std::string dir = "";
		std::ofstream file;

		while (true)
		{
			file.open(fileName, std::ios::out | std::ios::trunc);
			if (file.is_open()) break;

			if (dir.empty())
			{
				for (auto it = fileName.rbegin(); it != fileName.rend(); ++it)
				{
					if (*it == '\\' || *it == '/')
					{
						dir = std::string(fileName.begin(), it.base());
						break;
					}
				}

				if (!std::filesystem::exists(dir))
				{
					std::filesystem::create_directories(dir);
					continue;
				}
			}

			std::cerr << "Error opening file: " << fileName << std::endl;
			return;
		}

		file << content;
		file.close();
	}

	inline std::vector<std::string> getFilesInDirectory(const std::string& directory, const std::string& extension = "")
	{
		std::vector<std::string> files;

		for (const auto& entry : std::filesystem::directory_iterator(directory))
		{
			if (entry.is_regular_file())
			{
				if (!extension.empty() && entry.path().extension().string() != extension) continue;

				files.push_back(entry.path().filename().string());
			}
		}

		return files;
	}
}
