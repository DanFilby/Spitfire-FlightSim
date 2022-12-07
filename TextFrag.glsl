#version 330 core

in vec2 UV;

out vec4 color;

uniform sampler2D FontSheetTexture;

void main(){
    color = texture( FontSheetTexture, UV ).rgba;
}