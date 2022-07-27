#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexColor;
layout(location = 2) in vec2 textCoord;

flat out vec3 fragmentColor;
out vec2 fragmentTextCoord;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;

void main(){	

	// Output position of the vertex, in clip space : MVP * position
	//gl_Position = transform * vec4(vertexPosition, 1.0);
	//gl_Position = vec4(vertexPosition, 1.0);

	gl_Position = projection * view * transform * vec4(vertexPosition, 1.0);

	//fragmentColor = vertexColor;
	fragmentColor = vec3(1.0);
	fragmentTextCoord = textCoord;
}
