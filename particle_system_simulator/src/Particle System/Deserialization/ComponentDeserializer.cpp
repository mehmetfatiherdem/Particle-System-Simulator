//#include "Particle System/Data/ComponentMethod.h"
//#include "Particle System/Components/Component.h"
//#include "Particle System/Components/ColorBySpeed.h"
//#include "Particle System/Components/ColorOverLifetime.h"
//#include "Particle System/Components/ForceOverLifetime.h"
//#include "Particle System/Components/LimitVelocityOverLifetime.h"
//#include "Particle System/Components/RotationBySpeed.h"
//#include "Particle System/Components/SizeBySpeed.h"
//#include "Particle System/Components/SizeOverLifetime.h"
//#include "Particle System/Components/VelocityOverLifetime.h"
//#include "Persistence/Deserializer.h"
//#include "Persistence/SerializationUtils.h"
//#include "GeneralUtility/BezierCurve.h"
//#include "ComponentDeserializer.h"

#include <iostream>
#include <string>
#include <cstring>
#include <cmath>
#include <stdint.h>

#include <glm/glm.hpp>
#include <glm/ext/quaternion_float.hpp>

#include "UserInterface/ParticleSystemEditor.h"
#include "ResourceManagement/ResourceManager.h"
#include "UserInterface/Gui.h"
#include "GeneralUtility/gl2fw3.h"
#include "UserInterface/Window.h"
#include "RenderPipeline/Light/PointLight.h"
#include "RenderPipeline/Light/DirectionalLight.h"
#include "RenderPipeline/Light/SpotLight.h"
#include "RenderPipeline/Transform/Transform.h"
#include "RenderPipeline/Mesh/Mesh.h"
#include "RenderPipeline/Shader/Shader.h"
#include "RenderPipeline/Texture/Texture.h"
#include "RenderPipeline/Material/Material.h"
#include "RenderPipeline/Object/MeshRenderer.h"
#include "RenderPipeline/Shader/ShaderManagement/GlobalShaderManager.h"
#include "RenderPipeline/Scene/Scene.h"
#include "MeshConstruction/Shapes.h"
#include "RenderPipeline/Application.h"
#include "Input Management/Input.h"
#include "Time Management/Time.h"
#include "Particle System/ParticleSystem.h"
#include "GeneralUtility/Random.h"
#include "Particle System/Emitter/SphereEmitter.h"
#include "Particle System/Emitter/ConeEmitter.h"
#include "Particle System/Components/Component.h"
#include "Particle System/Components/ColorBySpeed.h"
#include "Particle System/Components/ColorOverLifetime.h"
#include "Particle System/Components/ForceOverLifetime.h"
#include "Particle System/Components/LimitVelocityOverLifetime.h"
#include "Particle System/Components/RotationBySpeed.h"
#include "Particle System/Components/SizeBySpeed.h"
#include "Particle System/Components/SizeOverLifetime.h"
#include "Particle System/Components/VelocityOverLifetime.h"
#include "ComponentDeserializer.h"

#include "Persistence/Serializer.h"
#include "Persistence/Deserializer.h"
#include "Persistence/SerializationUtils.h"

#include <fstream>
#include <sstream>

Component* deserializeComponent(const Deserializer& deserializer)
{
	auto getComponentMethod = [&deserializer]()
		{
			return getComponentMethodType(deserializer["ComponentMethod"].getString());
		};

	auto getMinBezierF = [&deserializer]()
		{
			BezierCurve<float> minBezier;
			persistence::utils::deserializeBezier(deserializer["MinBezier"], &minBezier);
			return minBezier;
		};

	auto getMaxBezierF = [&deserializer]()
		{
			BezierCurve<float> maxBezier;
			persistence::utils::deserializeBezier(deserializer["MaxBezier"], &maxBezier);
			return maxBezier;
		};

	auto getMinBezierV = [&deserializer]()
		{
			BezierCurve<glm::vec3> minBezier;
			persistence::utils::deserializeBezier(deserializer["MinBezier"], &minBezier);
			return minBezier;
		};

	auto getMaxBezierV = [&deserializer]()
		{
			BezierCurve<glm::vec3> maxBezier;
			persistence::utils::deserializeBezier(deserializer["MaxBezier"], &maxBezier);
			return maxBezier;
		};
	
	bool enabled = deserializer["Enabled"].getBoolean();

	Component* component;

	switch (getComponentType(deserializer["ComponentType"].getString()))
	{
	case ComponentType::Velocity_Over_Lifetime:
	{
		glm::vec3 minVelocity, maxVelocity;
		persistence::utils::deserializeVector(deserializer["MinVelocity"], &minVelocity);
		persistence::utils::deserializeVector(deserializer["MaxVelocity"], &maxVelocity);
		component = new VelocityOverLifetime(getComponentMethod(), minVelocity, maxVelocity, getMinBezierV(), getMaxBezierV());
		break;
	}
	case ComponentType::Size_Over_Lifetime:
	{
		float minSize = deserializer["MinSize"].getReal();
		float maxSize = deserializer["MaxSize"].getReal();
		component = new SizeOverLifetime(getComponentMethod(), minSize, maxSize, getMinBezierF(), getMaxBezierF());
		break;
	}
	case ComponentType::Size_By_Speed:
	{
		float minSize = deserializer["MinSpeed"].getReal();
		float maxSize = deserializer["MaxSpeed"].getReal();
		component = new SizeBySpeed(getComponentMethod(), minSize, maxSize, getMinBezierF(), getMaxBezierF());
		break;
	}
	case ComponentType::Rotation_By_Speed:
	{
		float minSpeed = deserializer["MinSpeed"].getReal();
		float maxSpeed = deserializer["MaxSpeed"].getReal();
		component = new RotationBySpeed(getComponentMethod(), minSpeed, maxSpeed, getMinBezierF(), getMaxBezierF());
		break;
	}
	case ComponentType::Limit_Velocity_Over_Lifetime:
	{
		float dampen = deserializer["Dampen"].getReal();
		glm::vec3 minVelocity, maxVelocity;
		persistence::utils::deserializeVector(deserializer["MinVelocity"], &minVelocity);
		persistence::utils::deserializeVector(deserializer["MaxVelocity"], &maxVelocity);
		component = new LimitVelocityOverLifetime(getComponentMethod(), minVelocity, maxVelocity, getMinBezierV(), getMaxBezierV(), dampen);
		break;
	}
	case ComponentType::Force_Over_Lifetime:
	{
		glm::vec3 minForce, maxForce;
		persistence::utils::deserializeVector(deserializer["MinForce"], &minForce);
		persistence::utils::deserializeVector(deserializer["MaxForce"], &maxForce);
		component = new ForceOverLifetime(getComponentMethod(), minForce, maxForce, getMinBezierV(), getMaxBezierV());
		break;
	}
	case ComponentType::Color_Over_Lifetime:
	{
		float keypoint1 = deserializer["Keypoints"][0].getReal();
		float keypoint2 = deserializer["Keypoints"][1].getReal();

		Color4 color1, color2;
		persistence::utils::deserializeColor(deserializer["Colors"][0], &color1);
		persistence::utils::deserializeColor(deserializer["Colors"][1], &color2);

		component = new ColorOverLifetime(keypoint1, color1, keypoint2, color2);
		break;
	}
	case ComponentType::Color_By_Speed:
	{
		float minSpeed = deserializer["MinSpeed"].getReal();
		float maxSpeed = deserializer["MaxSpeed"].getReal();
		float keypoint1 = deserializer["Keypoints"][0].getReal();
		float keypoint2 = deserializer["Keypoints"][1].getReal();

		Color4 color1, color2;
		persistence::utils::deserializeColor(deserializer["Colors"][0], &color1);
		persistence::utils::deserializeColor(deserializer["Colors"][1], &color2);

		component = new ColorBySpeed(keypoint1, color1, keypoint2, color2, minSpeed, maxSpeed);
		break;
	}
	default: throw "Invalid Component Type";
	}

	if (!enabled)
	{
		component->disable();
	}

	return component;
}