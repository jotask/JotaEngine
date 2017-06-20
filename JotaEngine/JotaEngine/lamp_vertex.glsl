#version 330 core

layout (location = 0) in vec3 position;

// Local object coordinates to camera coordinates
uniform mat4 model;

// Convert camera coordinates to normalize coordinates
uniform mat4 projection;

// Converts normalize coordiantes to screen coordinates
uniform mat4 view;

void main(){
    gl_Position = projection * view * model * vec4(position, 1.0);
}