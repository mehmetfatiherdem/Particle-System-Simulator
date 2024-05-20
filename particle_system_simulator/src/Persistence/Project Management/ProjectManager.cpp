#include "Persistence/Serializer.h"
#include "Persistence/Deserializer.h"
#include "Particle System/Deserialization/ParticleSytemDeserializer.h"
#include "RenderPipeline/Application.h"
#include "GeneralUtility/FileIOUtils.h"
#include "GeneralUtility/WindowsUtils.h"
#include "ProjectManager.h"

ProjectManager::ProjectManager() : applicationDirectory(getCurrentUserDirectory() + "\\AppData\\Local\\Particle Engine\\"),
currentProject(getLastProjectAddress())
{
	loadProject();
}

ProjectManager& ProjectManager::getInstance()
{
	static ProjectManager instance;
	return instance;
}

std::string ProjectManager::getLastProjectAddress() const
{
	return utility::io::readFile(applicationDirectory + "last.txt");
}

void ProjectManager::saveProject(const std::string& fileName, bool relativeToProjectDir) const
{
	std::string fileAddress = fileName.empty() ? currentProject :
		(relativeToProjectDir ? getProjectDirectory() + fileName : fileName) + ".proj";

	if (fileAddress.empty()) return;

	Serializer serializer;
	serializer.startArray();

	for (auto& ps : Application::getInstance().getParticleSystems())
	{
		ps.serialize(serializer);
	}

	serializer.endArray();

	utility::io::writeFile(fileAddress, serializer.getJsonString());
	utility::io::writeFile(applicationDirectory + "last.txt", fileAddress);
}

void ProjectManager::loadProject(const std::string& fileName, bool relativeToProjectDir)
{
	std::string fileAddress = fileName.empty() ? currentProject : 
		(relativeToProjectDir ? getProjectDirectory() + fileName : fileName) + ".proj";

	if (fileAddress.empty()) return;

	currentProject = fileAddress;

	Application& app = Application::getInstance();
	app.deleteAllParticleSystems();

	std::string json = utility::io::readFile(fileAddress);

	Document doc = Deserializer::createDocument(json);
	Deserializer deserializer(doc);

	for (auto& it : deserializer.getArray())
	{
		deserializeParticleSystem(it, app);
	}
}

std::vector<std::string> ProjectManager::getProjectList() const
{
	 std::vector<std::string> projects = utility::io::getFilesInDirectory(getProjectDirectory(), ".proj");

	 for (auto& project : projects)
	 {
		 project = project.substr(0, project.size() - 5);
	 }

	 return projects;
}