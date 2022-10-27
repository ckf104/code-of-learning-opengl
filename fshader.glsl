#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 FragColor;
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;


uniform Light light;
uniform Material material;


void main()
{
  vec3 norm = normalize(Normal);
// rgba in png picture
  vec3 ambientColor = vec3(texture(material.diffuse, TexCoords)) * light.ambient;
  vec3 diffuseColor = vec3(texture(material.diffuse, TexCoords)) * max(dot(norm, normalize(light.position - FragPos)), 0.0) * light.diffuse;
  vec3 specularColor = vec3(texture(material.specular, TexCoords)) * pow(max(dot(reflect(normalize(FragPos - light.position), norm), normalize(-FragPos)), 0.0), material.shininess) * light.specular;

  vec3 result = ambientColor + diffuseColor + specularColor;
  FragColor = vec4(result, 1.0);
}