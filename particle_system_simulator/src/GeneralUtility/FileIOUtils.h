#pragma once

#include <string>
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
}
