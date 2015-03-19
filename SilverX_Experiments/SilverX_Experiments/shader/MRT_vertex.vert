#version 400

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

out float eDepth;
out vec3 eNormal;


uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 MVP;

void main()
{
	mat4 MV = ViewMatrix * ModelMatrix;
	vec3 ePosition = vec3(MV * vec4(VertexPosition,1.0));
	eDepth = ePosition.z;
	eNormal = normalize( vec3(MV * vec4(VertexNormal,0.0)));
	gl_Position = MVP * vec4(VertexPosition,1.0);
}