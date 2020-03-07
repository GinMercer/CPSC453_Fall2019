#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

out Data
{
	vec2 texCoord;
    vec3 position;
    vec3 normal;
} data;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPosition; 
uniform vec3 lightColor;
uniform float ambientStrength;	
uniform float specularStrength;
uniform vec3 viewPos;

void main()
{
	data.texCoord = texCoord;
    gl_Position = projection * view * model * vec4(position,1.f);	
	data.position = vec3(model * vec4(position, 1.0));
	data.normal = mat3(transpose(inverse(model))) * normal;
}
