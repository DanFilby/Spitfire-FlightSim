#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexNormal_modelspace;

uniform mat4 MVP;
uniform mat4 M;

out vec3 norm;
out vec3 FragPos_worldSpace;


void main(){
	gl_Position = MVP * vec4(vertexPosition_modelspace,1);
	norm = vertexNormal_modelspace;

	FragPos_worldSpace = (M * vec4(vertexPosition_modelspace,1)).xyz;
}