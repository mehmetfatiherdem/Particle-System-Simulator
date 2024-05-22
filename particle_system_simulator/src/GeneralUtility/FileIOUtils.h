#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
#include <filesystem>

namespace utility::io
{
	inline void createDirectories(const std::string& address, bool isFile)
	{
		std::string directory = address;

		if (isFile)
		{
			for (auto it = directory.rbegin(); it != directory.rend(); ++it)
			{
				if (*it == '\\' || *it == '/')
				{
					directory = std::string(directory.begin(), it.base());
					break;
				}
			}
		}

		if (!std::filesystem::exists(directory))
		{
			std::filesystem::create_directories(directory);
		}
	}

	inline std::string readFile(const std::string& fileName)
	{
		std::string dir = "";
		std::ifstream stream;

		createDirectories(fileName, true);
		stream.open(fileName, std::ios::in);

		if (!stream.is_open())
		{
			std::cerr << "Failed to open file: " << fileName << std::endl;
			return "";
		}

		std::stringstream buffer;
		buffer << stream.rdbuf();
		stream.close();
		return buffer.str();
	}

	inline void writeFile(const std::string& fileName, const std::string& content)
	{
		std::string dir = "";
		std::ofstream stream;

		createDirectories(fileName, true);
		stream.open(fileName, std::ios::out | std::ios::trunc);

		if (!stream.is_open())
		{
			std::cerr << "Failed to open file: " << fileName << std::endl;
			return;
		}

		stream << content;
		stream.close();
	}

	inline std::vector<std::string> getFilesInDirectory(const std::string& directory, const std::string& extension = "")
	{
		std::vector<std::string> files;

		createDirectories(directory, false);

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
