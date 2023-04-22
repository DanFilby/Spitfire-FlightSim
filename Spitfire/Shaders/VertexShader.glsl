#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexUV;
layout(location = 3) in vec3 vertexColour;

uniform mat4 MVP;
uniform mat4 M;
uniform mat4 NormalMatrix;

out vec2 UV;
out vec3 FragPos_worldSpace;
out vec3 Normal_worldSpace;

void main(){

	gl_Position = MVP * vec4(vertexPosition_modelspace,1);
	UV = vertexUV;

	//Transpose frag position and normal to worldspace for diffusion lighting
	FragPos_worldSpace = (M * vec4(vertexPosition_modelspace,1)).xyz;
	Normal_worldSpace = (NormalMatrix * vec4(vertexNormal,1)).xyz;
}