#version 400

layout ( location = 0 ) in vec3 VertexPosition;
layout ( location = 1 ) in vec3 VertexNormal;

out vec4 eNormalDepth;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 MVP;


void main()
{
	mat4 MV = ViewMatrix * ModelMatrix;
	vec3 ePosition = vec3(MV * vec4(VertexPosition,1.0f));
	vec3 eNormal = vec3(MV*vec4(VertexNormal,0.0f));
	eNormalDepth = vec4(eNormal,ePosition.z);
	gl_Position = MVP * vec4(VertexPosition,1.0f);
}