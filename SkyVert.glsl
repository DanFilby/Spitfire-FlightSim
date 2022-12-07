#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords = aPos;

    vec3 bPos = aPos + vec3(0,0.0,0);   //If need to adjust skybox atm dont

    vec4 pos = projection * view * vec4(bPos, 1.0);

    gl_Position = pos.xyww;
}  