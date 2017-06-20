#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

// Local object coordinates to camera coordinates
uniform mat4 model;

// Convert camera coordinates to normalize coordinates
uniform mat4 projection;

// Converts normalize coordiantes to screen coordinates
uniform mat4 view;

out vec3 outColor;

void main(){
    gl_Position = projection * view * model * vec4(position, 1.0);
    outColor = color;
}