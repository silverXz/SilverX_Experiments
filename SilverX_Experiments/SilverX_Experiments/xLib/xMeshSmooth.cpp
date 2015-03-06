// xMeshSmooth.cpp: implementation of the CxMeshSmooth class.
// ***************************************************************
//  xMeshSmooth   version:  1.0   ? date: 08/16/2006
//  -------------------------------------------------------------
//  Description:  对三角网格模型光顺/噪声去除;
//  Author     :  wujh@sia.cn 
//  Created    :  08/16/2006
//  Modified   :   
//  -------------------------------------------------------------
//  Copyright (C) 2006 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************


#include "stdafx.h"

#include "xMeshSmooth.h"
#include "xMesh.h"
#include "xUtil.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


using namespace std;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CxMeshSmooth::CxMeshSmooth():m_pMesh(NULL)
{

}

CxMeshSmooth::CxMeshSmooth(CxMesh* pMesh):m_pMesh(pMesh)
{

}

CxMeshSmooth::~CxMeshSmooth()
{
	
}


/*================================================================
 * Author/date      : wujh@sia.cn   2006-8-16 21:15:45
 * Function name	: CxMeshSmooth::AddGaussianNoise
 * Description	    : 
 * Return type		: void 
 * Argument         : double sigma
 *================================================================*/
void CxMeshSmooth::AddGaussianNoise(double sigma)
{
	if ( m_pMesh == NULL )
		return;

	int nVertex = m_pMesh->NumVertices();
	if ( nVertex <= 0 )
		return;

 	sigma *= m_pMesh->GetAverageLengthEdge();
	for ( int i=0; i<nVertex; ++i )
	{
		CxVertex3d& rVertex = m_pMesh->GetVertex(i);

		double noise = ::GaussianRandom()*sigma;

		rVertex[0] = rVertex[0] + rVertex.GetNormal().x()*noise;
		rVertex[1] = rVertex[1] + rVertex.GetNormal().y()*noise;
		rVertex[2] = rVertex[2] + rVertex.GetNormal().z()*noise;
	}

	m_pMesh->CalculateNormalPerFace();
	m_pMesh->CalculateNormalPerVertex();
}


void CxMeshSmooth::Noisify(float fNoise)
{
	// 	BuildVertexNeighbor();
	// 	CalculateNormalPerVertex();
	int nv = (int)m_pMesh->NumVertices();
	vector<CxVector3d> disp(nv);

	for (int i = 0; i < nv; i++)
	{
		CxVertex3d& v = m_pMesh->GetVertex(i);

		// Tangential
		int size = v.NumVertexNeighbor();

		for (int j = 0; j < size; j++)
		{
			const CxVertex3d& n = m_pMesh->GetVertex(v.GetVertexNeighbor(j));
			float scale = fNoise / (fNoise + (float)::Distance(v, n));
			disp[i] +=(float) ::tinyrnd() * scale * CxVector3d(v, n);
		}

		if ( size )
			disp[i] /= (float)size;

		// Normal
		disp[i] +=(2.0f * (float) ::tinyrnd() - 1.0f) * fNoise * v.GetNormal(); 
	}

	for (int i = 0; i < nv; i++)
	{
		CxVertex3d& rVertex = m_pMesh->GetVertex(i);
		rVertex[0] += disp[i][0];
		rVertex[1] += disp[i][1];
		rVertex[2] += disp[i][2];
	}

	// recalcualating normal,bbox...
	m_pMesh->CalculateNormalPerFace();
	m_pMesh->CalculateNormalPerVertex();	
	m_pMesh->SetHasBoundingBox(false);
	m_pMesh->CalculateBoundingBox();
	m_pMesh->SetModifiedFlag(true);
}


void CxMeshSmooth::AddPerlinNoise()
{

}

/*================================================================
 * Author/date      : wujh@sia.cn   2006-8-16 21:34:44
 * Function name	: CxMeshSmooth::Laplacian
 * Description	    : 
 * Return type		: void 
 * Argument         : double delta
 *================================================================*/
void CxMeshSmooth::Laplacian(double delta /*= 0.6307*/)
{
	if ( m_pMesh == NULL )
		return;

	int nVertex = m_pMesh->NumVertices();
	if ( nVertex <= 0 )
		return;

	std::vector<double> tmpArrayVertex(nVertex*3, 0);   // store new vertex;
	for ( int i=0; i<nVertex; ++i )
	{
		CxVertex3d& rVertex = m_pMesh->GetVertex(i);
		int nNeighborVertex = rVertex.NumVertexNeighbor();

		// laplacian operator;
		CxVector3d vec;
		for ( int j=0; j<nNeighborVertex; ++j )
		{
			vec += CxVector3d(rVertex, m_pMesh->GetVertex(rVertex.GetVertexNeighbor(j)));
		}
		vec /= nNeighborVertex;
		vec *= delta;

		tmpArrayVertex[ 3*i ] = rVertex.x() + vec.x();
		tmpArrayVertex[3*i+1] = rVertex.y() + vec.y();
		tmpArrayVertex[3*i+2] = rVertex.z() + vec.z();
	}

	// restore vertex;
	for (int i=0; i<nVertex; ++i )
	{
		CxVertex3d& rVertex = m_pMesh->GetVertex(i);
		rVertex.Set(tmpArrayVertex[ 3*i ],
		            tmpArrayVertex[3*i+1],
		            tmpArrayVertex[3*i+2]);
	}

	// recalculate normal of face and vertex;
	m_pMesh->CalculateNormalPerFace();
	m_pMesh->CalculateNormalPerVertex();
}



/*================================================================
 * Author/date      : wujh@sia.cn   2006-8-17 10:47:30
 * Function name	: CxMeshSmooth::LaplacianTangential
 * Description	    : tangential Laplacian smoothing
 * Return type		: void 
 *================================================================*/
void CxMeshSmooth::LaplacianTangential()
{

}


/*================================================================
 * Author/date      : wujh@sia.cn   2006-8-17 10:27:03
 * Function name	: CxMeshSmooth::Bilateral
 * Description	    : Bilateral Mesh Denoising
 * Return type		: void 
 * Argument         : double sigmac
 * Argument         : double sigmas
 *================================================================*/
void CxMeshSmooth::Bilateral(double sigmac, double sigmas)
{
	if ( m_pMesh == NULL )
		return;

	int nVertex = m_pMesh->NumVertices();
	if ( nVertex <= 0 )
		return;

	std::vector<double> tmpArrayVertex(nVertex*3, 0);  // store new vertex;
	for ( int i=0; i<nVertex; ++i )
	{
		CxVertex3d& rVertex = m_pMesh->GetVertex(i);
		int nNeighborVertex = rVertex.NumVertexNeighbor();
		double sum = .0;
		double normalizer = .0;
		
		for ( int j=0; j<nNeighborVertex; ++j )
		{
			const CxVertex3d& rVertexNeighbor = m_pMesh->GetVertex(rVertex.GetVertexNeighbor(j));
			double t = ::Distance(rVertex, rVertexNeighbor);
			double h = rVertex.GetNormal().Dot(CxVector3d(rVertex, rVertexNeighbor));
			double wc = exp(-t*t/(2*sigmac*sigmac));
			double ws = exp(-h*h/(2*sigmas*sigmas));

			double wcs = wc*ws;
			sum += wcs*h;
			normalizer += wcs;

		}

		double tmp = sum / normalizer;
		tmpArrayVertex[ 3*i ] = rVertex.x() + rVertex.GetNormal().x()*tmp;
		tmpArrayVertex[3*i+1] = rVertex.y() + rVertex.GetNormal().y()*tmp;
		tmpArrayVertex[3*i+2] = rVertex.z() + rVertex.GetNormal().z()*tmp;

	}  // end for i;
	
	// restore vertex;
	for ( int i=0; i<nVertex; ++i )
	{
		CxVertex3d& rVertex = m_pMesh->GetVertex(i);
		rVertex.Set(tmpArrayVertex[ 3*i ],
		            tmpArrayVertex[3*i+1],
		            tmpArrayVertex[3*i+2]);
	}

	// recalculate normal of face and vertex;
	m_pMesh->CalculateNormalPerFace();
	m_pMesh->CalculateNormalPerVertex();
	m_pMesh->SetModifiedFlag(true);
	
}

void CxMeshSmooth::LambdaSmooth(int nIters)
{
	for (int i=0; i<nIters; ++i)
	{
		Laplacian(0.330f);
		Laplacian(-0.331f);
	}

	// recalculate normal of face and vertex;
	m_pMesh->CalculateNormalPerFace();
	m_pMesh->CalculateNormalPerVertex();
	m_pMesh->SetModifiedFlag(true);

}

// END OF FILE