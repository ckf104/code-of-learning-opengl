#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 texCoords;
out vec3 FragPos;  
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat3 model_inverse_trans;

void main()
{
   gl_Position = projection * view * model * vec4(aPos, 1.0);
   FragPos = vec3(view * model * vec4(aPos, 1.0));
   Normal = normalize(mat3(view) * model_inverse_trans * aNormal);
   TexCoords = texCoords;
}