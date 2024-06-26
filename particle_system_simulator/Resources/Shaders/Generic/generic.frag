#version 440 core

#define MAX_DIRECTIONAL_LIGHTS 2
#define MAX_POINT_LIGHTS 8
#define MAX_SPOT_LIGHTS 4

struct Color4 {                 //padded size = 64 bytes
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float alpha;
};

struct Color3 {                 //padded size = 48 bytes
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Attenuation {            //padded size = 16 bytes
    float constant;
    float linear;
    float quadratic;
};

struct Material {               //padded size = 80 bytes
    Color4 color;
    float shininess;
    bool useDiffuseMap;
    bool useSpecularMap;
    //4 byte padding
};

struct DirectionalLight {       //padded size = 64 bytes
    Color3 color;
    vec3 direction;
};

struct PointLight {             //padded size = 80 bytes
    Color3 color;
    vec3 position;
    Attenuation attenuation;
};

struct SpotLight {              //padded size = 112 bytes
    Color3 color;
    vec3 position;
    vec3 direction;
    Attenuation attenuation;
    float innerCutOff;
    float outerCutOff;
    float epsilon;
    //4 byte padding
};

in VS_OUT {
    vec3 worldPosition;
    vec3 worldNormal;
    vec2 texCoords;
} fs_in;

out vec4 fragmentColor; //Final fragment color

uniform sampler2D diffuseMap;
uniform sampler2D specularMap;

layout (std140, binding = 1) uniform Lights {
    SpotLight spotLights[MAX_SPOT_LIGHTS];
    PointLight pointLights[MAX_POINT_LIGHTS];
    DirectionalLight directionalLights[MAX_DIRECTIONAL_LIGHTS];
    uint noOfSpotLights;
    uint noOfPointLights;
    uint noOfDirectionalLights;
};

layout (std140, binding = 2) uniform View {
    vec3 viewPosition;
};

layout (std140, binding = 3) uniform Mat {
    Material material;
};

Color4 getMaterialColor();
vec4 calculateDirectionalLight(DirectionalLight light, vec3 viewDir, vec3 normal, Color4 matColor);
vec4 calculatePointLight(PointLight light, vec3 viewDir, vec3 normal, Color4 matColor);
vec4 calculateSpotLight(SpotLight light, vec3 viewDir, vec3 normal, Color4 matColor);

void main() {
    Color4 matColor = getMaterialColor();
    vec3 viewDir = normalize(viewPosition - fs_in.worldPosition);
    vec3 normal = normalize(fs_in.worldNormal);

    vec4 color = vec4(0.0);

    for(int i = 0; i < noOfDirectionalLights; i++)
        color += calculateDirectionalLight(directionalLights[i], viewDir, normal, matColor);
    
    for(int i = 0; i < noOfPointLights; i++)
        color += calculatePointLight(pointLights[i], viewDir, normal, matColor);
    
    for(int i = 0; i < noOfSpotLights; i++)
        color += calculateSpotLight(spotLights[i], viewDir, normal, matColor);

    fragmentColor = color;
}

vec4 calculateDirectionalLight(DirectionalLight light, vec3 viewDir, vec3 normal, Color4 matColor) {
    vec3 lightDir = normalize(-light.direction);

    float diffuseFactor = max(dot(normal, lightDir), 0.0);

    vec3 halfwayDir = normalize(lightDir + viewDir);
    float specularFactor = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);

    vec3 ambient = light.color.ambient * matColor.ambient;
    vec3 diffuse = light.color.diffuse * diffuseFactor * matColor.diffuse;
    vec3 specular = light.color.specular * specularFactor * matColor.specular;

    return vec4((ambient + diffuse + specular), matColor.alpha);
}

vec4 calculatePointLight(PointLight light, vec3 viewDir, vec3 normal, Color4 matColor) {
    vec3 lightDir = light.position - fs_in.worldPosition;
    float distance = length(lightDir);
    lightDir = normalize(lightDir);

    float attenuation = 1.0 / ((light.attenuation.constant) +
                               (light.attenuation.linear * distance) +
                               (light.attenuation.quadratic * distance * distance));

    float diffuseFactor = max(dot(normal, lightDir), 0.0);

    vec3 halfwayDir = normalize(lightDir + viewDir);
    float specularFactor = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);

    vec3 ambient = light.color.ambient * matColor.ambient;
    vec3 diffuse = light.color.diffuse * diffuseFactor * matColor.diffuse;
    vec3 specular = light.color.specular * specularFactor * matColor.specular;

    return vec4((ambient + diffuse + specular), matColor.alpha) * attenuation;
}

vec4 calculateSpotLight(SpotLight light, vec3 viewDir, vec3 normal, Color4 matColor) {
    vec3 lightDir = light.position - fs_in.worldPosition;
    float distance = length(lightDir);
    lightDir = normalize(lightDir);

    float attenuation = 1.0 / (light.attenuation.constant +
                                (light.attenuation.linear * distance) +
                                (light.attenuation.quadratic * distance * distance));

    float theta = dot(lightDir, normalize(-light.direction)); 
    float intensity = clamp((theta - light.outerCutOff) / light.epsilon, 0.0, 1.0);

    float diffuseFactor = max(dot(normal, lightDir), 0.0);
    
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float specularFactor = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);

    vec3 ambient = light.color.ambient * matColor.ambient;
    vec3 diffuse = light.color.diffuse * diffuseFactor * intensity * matColor.diffuse;
    vec3 specular = light.color.specular * specularFactor * intensity * matColor.specular;

    return vec4((ambient + diffuse + specular), matColor.alpha) * attenuation;
}

Color4 getMaterialColor() {
    Color4 color = material.color;
    vec4 texColor = texture(diffuseMap, fs_in.texCoords);

    if(material.useDiffuseMap) {
        color.diffuse *= texColor.rgb;
        color.ambient *= texColor.rgb;
        color.alpha *= texColor.a;
    }

    texColor = texture(specularMap, fs_in.texCoords);

    if(material.useSpecularMap) {
		color.specular *= texColor.rgb;
        color.alpha *= texColor.a;
	}

    return color;
}