#version 400

layout (location = 0 ) in vec3 VertexPosition;
layout ( location = 1 ) in vec3 VertexNormal;
layout ( location = 2 ) in vec2 TexCoord;

out vec2 Coord;

void main()
{
	Coord = TexCoord;
	gl_Position = vec4(VertexPosition,1.0f);
}