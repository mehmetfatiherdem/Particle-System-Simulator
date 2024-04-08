#version 440 core

#define MAX_DIRECTIONAL_LIGHTS 2
#define MAX_POINT_LIGHTS 8
#define MAX_SPOT_LIGHTS 4

struct Color4 {                 //size = 48 bytes (no padding)
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

struct Color3 {                 //padded size = 48 bytes
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Attenuation {
    float constant;
    float linear;
    float quadratic;
};

struct Material {               //size = 64 bytes (no padding)
    Color4 color;
    float ambientStrength;
    float shininess;
    sampler2D diffuseMap;
    sampler2D specularMap;
    bool useDiffuseMap;
    bool useSpecularMap;
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

uniform Material material;

layout (std140, binding = 1) uniform Lights {
    SpotLight spotLights[MAX_SPOT_LIGHTS];
    PointLight pointLights[MAX_POINT_LIGHTS];
    DirectionalLight directionalLights[MAX_DIRECTIONAL_LIGHTS];
    uint32_t noOfSpotLights;
    uint32_t noOfPointLights;
    uint32_t noOfDirectionalLights;
};

layout (std140, binding = 2) uniform View {
    vec3 viewPosition;
};

vec4 calculateDirectionalLight(DirectionalLight light, vec3 viewDir, vec4 matAmbient, vec4 matDiffuse, vec4 matSpecular);
vec4 calculatePointLight(PointLight light, vec3 viewDir, vec4 matAmbient, vec4 matDiffuse, vec4 matSpecular);
vec4 calculateSpotLight(SpotLight light, vec3 viewDir, vec4 matAmbient, vec4 matDiffuse, vec4 matSpecular);
vec4 getMaterialAmbient();
vec4 getMaterialDiffuse();
vec4 getMaterialSpecular();

void main() {
    vec4 materialAmbient = getMaterialAmbient();
    vec4 materialDiffuse = getMaterialDiffuse();
    vec4 materialSpecular = getMaterialSpecular();
    vec3 viewDir = normalize(viewPosition - fs_in.worldPosition);

    vec4 color = vec4(0.0, 0.0, 0.0, 0.0);

    for(int i = 0; i < noOfDirectionalLights; i++)
        color += calculateDirectionalLight(directionalLights[i], viewDir, materialAmbient, materialDiffuse, materialSpecular);
    
    for(int i = 0; i < noOfPointLights; i++)
        color += calculatePointLight(pointLights[i], viewDir, materialAmbient, materialDiffuse, materialSpecular);
    
    for(int i = 0; i < noOfSpotLights; i++)
        color += calculateSpotLight(spotLights[i], viewDir, materialAmbient, materialDiffuse, materialSpecular);
    
    fragmentColor = color;
}

vec4 calculateDirectionalLight(DirectionalLight light, vec3 viewDir, vec4 matAmbient, vec4 matDiffuse, vec4 matSpecular) {
    vec3 lightDir = normalize(-light.direction);

    float diffuseFactor = max(dot(fs_in.worldNormal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, fs_in.worldNormal);
    float specularFactor = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec4 ambient = vec4(light.color.ambient, 1.0) * material.ambientStrength * matAmbient;
    vec4 diffuse = vec4(light.color.diffuse, 1.0) * diffuseFactor * matDiffuse;
    vec4 specular = vec4(light.color.specular, 1.0) * specularFactor * matSpecular;

    return (ambient + diffuse + specular);
}

vec4 calculatePointLight(PointLight light, vec3 viewDir, vec4 matAmbient, vec4 matDiffuse, vec4 matSpecular) {
    vec3 lightDir = light.position - fs_in.worldPosition;
    float distance = length(lightDir);
    lightDir = normalize(lightDir);

    float attenuation = 1.0 / (light.attenuation.constant +
                                (light.attenuation.linear * distance) +
                                (light.attenuation.quadratic * distance * distance));

    float diffuseFactor = max(dot(fs_in.worldNormal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, fs_in.worldNormal);
    float specularFactor = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec4 ambient = vec4(light.color.ambient, 1.0) * material.ambientStrength * matAmbient;
    vec4 diffuse = vec4(light.color.diffuse, 1.0) * diffuseFactor * matDiffuse;
    vec4 specular = vec4(light.color.specular, 1.0) * specularFactor * matSpecular;

    return (ambient + diffuse + specular) * attenuation;
}

vec4 calculateSpotLight(SpotLight light, vec3 viewDir, vec4 matAmbient, vec4 matDiffuse, vec4 matSpecular) {
    vec3 lightDir = light.position - fs_in.worldPosition;
    float distance = length(lightDir);
    lightDir = normalize(lightDir);

    float attenuation = 1.0 / (light.attenuation.constant +
                                (light.attenuation.linear * distance) +
                                (light.attenuation.quadratic * distance * distance));

    float theta = dot(lightDir, normalize(-light.direction)); 
    float intensity = clamp((theta - light.outerCutOff) / light.epsilon, 0.0, 1.0);

    float diffuseFactor = max(dot(fs_in.worldNormal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, fs_in.worldNormal);
    float specularFactor = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec4 ambient = vec4(light.color.ambient, 1.0) * material.ambientStrength * matAmbient;
    vec4 diffuse = vec4(light.color.diffuse, 1.0) * diffuseFactor * intensity * matDiffuse;
    vec4 specular = vec4(light.color.specular, 1.0) * specularFactor * intensity * matSpecular;

    return (ambient + diffuse + specular) * attenuation;
}

vec4 getMaterialAmbient() {
    if(material.useDiffuseMap)
        return texture(material.diffuseMap, fs_in.texCoords);
    else
        return material.color.ambient;
}

vec4 getMaterialDiffuse() {
    if(material.useDiffuseMap)
        return texture(material.diffuseMap, fs_in.texCoords);
    else
        return material.color.diffuse;
}

vec4 getMaterialSpecular() {
    if(material.useSpecularMap)
        return texture(material.specularMap, fs_in.texCoords);
    else
        return material.color.specular;
}