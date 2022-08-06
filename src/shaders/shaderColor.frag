#version 330 core

smooth in vec3 fragmentColor;
in vec2 fragmentTextCoord;

uniform sampler2D myTexture;

out vec4 color;

void main(){

	color = vec4(fragmentColor, 1.0);
}