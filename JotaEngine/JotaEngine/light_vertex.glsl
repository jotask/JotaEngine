#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

// Local object coordinates to camera coordinates
uniform mat4 model;

// Convert camera coordinates to normalize coordinates
uniform mat4 projection;

// Converts normalize coordiantes to screen coordinates
uniform mat4 view;

out vec3 Normal;
out vec3 FragPos;

void main(){
    gl_Position = projection * view * model * vec4(position, 1.0);
	FragPos = vec3(model * vec4(position, 1.0f));
	Normal = mat3(transpose(inverse(model))) * normal;
}