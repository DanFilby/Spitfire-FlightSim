#version 330 core

in vec2 UV;
in vec3 Normal_worldSpace;
in vec3 FragPos_worldSpace;

out vec3 color;

uniform sampler2D myTextureSampler;
uniform vec3 LightPosition_worldspace;
uniform vec3 lightColour;
uniform float lightPower;
uniform vec3 viewPos;

void main(){
	//AMBIENT
	vec3 MaterialAmbient = vec3(0.1,0.1,0.1) * lightColour;

	//DIFFUSION
	// Normal of the computed fragment, in camera space
	vec3 normal = normalize( Normal_worldSpace );
	// Direction of the light (from the fragment to the light)
	vec3 lightDir = normalize( LightPosition_worldspace - FragPos_worldSpace );
	//calculate how much light is hitting surface
	float diffuse =  max(dot(normal, lightDir), 0.0) ;
	//mutilply by light properties
	vec3 MaterialDiffuse = diffuse * lightColour * lightPower;

	//SPECULAR - bling phong model
	float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos - FragPos_worldSpace);  
	//halfway between light and view direction, used so doesn't go over 90 degrees
	vec3 halfwayDir = normalize(lightDir + viewDir);
	//calculate how much light is going in view direction and raise to power of 32
	//32 represents the shininessof the object, the more is reflects rather than scattering
	float spec = pow(max(dot(normal, halfwayDir), 0.0), 32);
	vec3 MaterialSpecular = specularStrength * spec * lightColour;  

	color =	(MaterialDiffuse + MaterialAmbient + MaterialSpecular) * texture(myTextureSampler, UV).rgb;
	
}

//TODO: include distance into light calcs