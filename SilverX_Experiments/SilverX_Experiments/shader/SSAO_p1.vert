#version 400

layout ( location = 0 ) in vec3 VertexPosition;
layout ( location = 1 ) in vec3 VertexNormal;

out vec3 ePosition;
out vec3 eNormal;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 MVP;


void main()
{
	mat4 MV = ViewMatrix * ModelMatrix;
	ePosition = vec3(MV * vec4(VertexPosition,1.0f));
	eNormal = vec3(MV*vec4(VertexNormal,0.0f));
	gl_Position = MVP * vec4(VertexPosition,1.0f);
}