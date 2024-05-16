#pragma once

#include <string>
#include "GeneralUtility/WindowsUtils.h"

//Particle Engine
//last.proj
//Projects
	//fire.proj

class ProjectManager
{
private:
	std::string applicationDirectory;
	std::string currentProject;

	std::string getLastProjectAddress() const;

	ProjectManager();

public:
	

	static ProjectManager& getInstance();

	std::string getProjectDirectory() const { return applicationDirectory + "Projects\\"; }

	void saveProject(const std::string& fileName = "", bool relativeToProjectDir = true) const;
	void loadProject(const std::string& fileName = "", bool relativeToProjectDir = true);
};