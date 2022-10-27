#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
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

uniform Light light;
uniform Material material;


void main()
{
  vec3 norm = normalize(Normal);

  vec3 ambientColor = material.ambient * light.ambient;
  vec3 diffuseColor = material.diffuse * max(dot(norm, normalize(light.position - FragPos)), 0.0) * light.diffuse;
  vec3 specularColor = material.specular * pow(max(dot(reflect(normalize(FragPos - light.position), norm), normalize(-FragPos)), 0.0), material.shininess) * light.specular;

  vec3 result = ambientColor + diffuseColor + specularColor;
  FragColor = vec4(result, 1.0);
}