#version 330 core

in vec3 FragPos_worldSpace;
in vec3 norm;
in vec3 DiffuseColour;


uniform vec3 LightPosition_worldspace;
uniform vec3 lightColour;
uniform float lightPower;

out vec3 color;

void main(){

	//DIFFUSION
	// Normal of the computed fragment, in camera space
	vec3 normal = normalize( norm );
	// Direction of the light (from the fragment to the light)
	vec3 lightDir = normalize( LightPosition_worldspace - FragPos_worldSpace );
	//calculate how much light is hitting surface
	float diffuse =  max(dot(normal, lightDir), 0.0);
	//mutilply by light properties
	vec3 MaterialDiffuse = diffuse * lightColour * lightPower;

	color = diffuse * vec3(0.2,0.7,0.2);
}