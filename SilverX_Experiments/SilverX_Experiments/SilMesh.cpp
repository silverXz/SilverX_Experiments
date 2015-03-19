#include "stdafx.h"
#include "SilMesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

SilMesh::SilMesh()
{
	m_hVAO = 0;
	for( int i = 0 ; i < 3 ; ++i )
		m_hVBO[i] = 0;
}

SilMesh::~SilMesh()
{
	if( m_hVAO > 0 )
		glDeleteVertexArrays(1,&m_hVAO);
	for( int i = 0 ; i < 3 ; ++i )
	{
		if( m_hVBO[i] > 0 )
			glDeleteBuffers(1,&m_hVBO[i]);
	}
}

bool SilMesh::LoadMesh( const char* filePath )
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filePath,
		aiProcess_GenSmoothNormals		|
		aiProcess_Triangulate			|
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);
	if( !scene )
	{
		printf("Load Mesh Failed!\n");
		return false;
	}

	// positon, normal , TexCoord
	const int iVertexAttributeTotalSize = sizeof(aiVector3D) * 2 + sizeof(aiVector2D);

	int iTotalVertices = 0;


	printf("There is total %d meshes!\n",scene->mNumMeshes);

	// FOR EACH MESH
	for( int i = 0 ; i < scene->mNumMeshes ; ++i )
	{
		aiMesh* mesh = scene->mMeshes[i];
		int iMeshFaces = mesh->mNumFaces;

		// FOR EACH FACE
		for( int j = 0 ; j < iMeshFaces ; ++j )
		{
			const aiFace& face = mesh->mFaces[j];

			// FOR EACH VERTEX
			for( int k = 0 ; k < 3 ; ++k )
			{
				aiVector3D pos = mesh->mVertices[face.mIndices[k]];
				m_posArray.push_back(pos.x);
				m_posArray.push_back(pos.y);
				m_posArray.push_back(pos.z);

				if( mesh->HasTextureCoords(j))
				{
					aiVector3D uv = mesh->mTextureCoords[0][face.mIndices[k]];
					m_texArray.push_back(uv.x);
					m_texArray.push_back(uv.y);
				}

				aiVector3D normal = mesh->HasNormals() ? mesh->mNormals[face.mIndices[k]] : aiVector3D(1.0f,1.0f,1.0f);
				m_normArray.push_back(normal.x);
				m_normArray.push_back(normal.y);
				m_normArray.push_back(normal.z);
			}
		}
	}

	if( m_hVAO <= 0 )
		glGenVertexArrays(1,&m_hVAO);
	if( m_hVBO[0] <= 0)
		glGenBuffers(3,m_hVBO);

	glBindVertexArray(m_hVAO);
	glBindBuffer(GL_ARRAY_BUFFER,m_hVBO[0]);
	glBufferData(GL_ARRAY_BUFFER,m_posArray.size() * sizeof(float),&m_posArray[0],GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);

	glBindBuffer(GL_ARRAY_BUFFER,m_hVBO[1]);
	glBufferData(GL_ARRAY_BUFFER,m_normArray.size() * sizeof(float),&m_normArray[0],GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,0);

	/*glBindBuffer(GL_ARRAY_BUFFER,m_hVBO[2]);
	
	glBufferData(GL_ARRAY_BUFFER,m_texArray.size() * sizeof(float),&m_texArray[0],GL_STATIC_DRAW);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,0,0);*/


	glBindVertexArray(0);
}

void SilMesh::Render()
{
	glBindVertexArray(m_hVAO);
	glDrawArrays(GL_TRIANGLES,0,m_posArray.size()/3);
}
