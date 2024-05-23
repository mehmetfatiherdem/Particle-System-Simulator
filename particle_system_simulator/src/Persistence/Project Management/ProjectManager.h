#pragma once

#include <string>
#include <vector>

class ProjectManager
{
private:
	std::string applicationDirectory;
	std::string currentProject;

	std::string getLastProjectAddress() const;

	ProjectManager();

public:

	~ProjectManager();

	static ProjectManager& getInstance();

	std::string getProjectDirectory() const { return applicationDirectory + "Projects\\"; }
	std::vector<std::string> getProjectList() const;

	bool isProjectRegistered() const { return !currentProject.empty(); }
	void newProject() { currentProject = ""; }

	void saveProject(const std::string& fileName = "", bool relativeToProjectDir = true);
	void loadProject(const std::string& fileName = "", bool relativeToProjectDir = true);
};