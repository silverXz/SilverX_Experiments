#pragma once

#include <glew.h>
#include <vector>


class SilMesh
{
public:
	SilMesh();
	~SilMesh();

	bool LoadMesh(const char* filePath);

	void Render();

private:
	GLuint m_hVAO;
	GLuint m_hVBO[3];

	std::vector<float>	m_posArray;
	std::vector<float>	m_normArray;
	std::vector<float>	m_texArray;
	std::vector<int>	m_indicesArray;


};