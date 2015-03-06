#include "stdafx.h"
#include "xCurvature.h"
// ***************************************************************
//  xCurvature   version:  1.0   ·  date: 04/22/2009
//  -------------------------------------------------------------
//  Author : jh.wu@siat.ac.cn 
//  -------------------------------------------------------------
//  Copyright (C) 2009 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************


#include <limits>
#include <functional>


/*===================================================================*/
// Gaussian Curvature  (The Gauss-Bonnet Scheme)
// Refs: A Comparision of Gaussian and Mean curvature estimation methods 
// on triangular meshes (Tatiana Surazhsky et.)
/*===================================================================*/
double CxCurvature::CurvatureGauss(const CxVertex3d& v, int nMethod /*= COMMON*/)const
{
	unsigned int nFaceNeighbor = v.NumFaceNeighbor();
	double area = 0.0;
	double angle = 0.0;
	for (unsigned int i=0; i<nFaceNeighbor; ++i )
	{
		const CxFace& face_neigbhor = m_pMesh->m_ArrayFace[v.GetFaceNeighbor(i)];
		int index = 0;
		CxVertex3d neighbor[2];
		for(unsigned int j=0; j<3; ++j )
		{
			const CxVertex3d& neighbor_vertex = m_pMesh->m_ArrayVertex[face_neigbhor[j]];
			if ( v != neighbor_vertex )
				neighbor[index++] = neighbor_vertex;
		}

		assert( 2 == index );
		CxVector3d vec1(v, neighbor[0]);
		CxVector3d vec2(v, neighbor[1]);
		double rad = ::Angle(vec1, vec2);
		angle += rad;

		if ( nMethod == COMMON )
		{
			//angle += ::Angle(v1, v2);
			area  += m_pMesh->Area(face_neigbhor);
		}
		else if ( nMethod == VORONOI )
		{
			CxVector3d vec3(neighbor[0], neighbor[1]);
			double belta = ::Angle(vec3, -vec1);
			double delta = M_PI - rad - belta;

			//if ( (sin(rad)>0) && (cos(rad)>0) ) // 锐角;
			if ( (rad < M_PI_2) && (belta < M_PI_2) && (delta < M_PI_2))
			{
				area += 0.125*(vec2.LengthSquared()/tan(belta) + 
					vec1.LengthSquared()/tan(delta));
			}
			else
			{
				//double a1 = 0.125*tan(belta)*vec1.LengthSquared();
				//double a2 = 0.125*tan(delta)*vec2.LengthSquared();
				//area += Area(face_neigbhor) - a1 - a2;
				area += m_pMesh->Area(face_neigbhor)/6.0;
			}
		}

	} // end for i;

	if ( nMethod == COMMON )
	{
		// barycentric area which is one third of the area of 
		// the facets  adjacent to v
		double barycentricArea = area /3.0;
		return ((2.0*M_PI - angle )/barycentricArea);

	}
	else if ( nMethod == VORONOI )
	{
		return ((2.0*M_PI - angle )/area);
	}
	else
		return .0;


}

double CxCurvature::CurvatureMean(const CxVertex3d& v)const
{
	int nNeighborVertex = (int)v.NumVertexNeighbor();
	double sum = .0;
	double area = 0.0;
	for ( int i=0; i<nNeighborVertex; ++ i)
	{
		const CxVertex3d& neighborVertex = m_pMesh->m_ArrayVertex[v.GetVertexNeighbor(i)];
		double length = ::Distance(v, neighborVertex);

		FaceArray arrayFace;
		arrayFace.reserve(2);
#if 0
		int size = v.NumFaceNeighbor();
		for ( int j=0; j<size; ++j )
		{		
			if ( v.GetFaceNeighbor(j) == -1 )
				continue;

			const CxFace& rFace = m_pMesh->GetFace(v.GetFaceNeighbor(j));
			if (rFace.HasVertex(v.GetVertexNeighbor(i)))
				arrayFace.push_back(rFace);
		}

		if ( 2 == arrayFace.size() )
		{
			area += m_pMesh->Area(arrayFace[0]) + m_pMesh->Area(arrayFace[1]);
			double belta = m_pMesh->Angle(arrayFace[0], arrayFace[1]);
			sum += length * belta;
		}
		else
		{
			assert( 0 != arrayFace.size());
			area +=m_pMesh->Area(arrayFace[0]);
			sum += length;
		}
#endif

		if ( m_pMesh->FindNeighborFaceContainVertex(v, neighborVertex, arrayFace) )
		{
			if ( 2 == arrayFace.size() )
			{
				area += m_pMesh->Area(arrayFace[0]) + m_pMesh->Area(arrayFace[1]);
				double belta = ::Angle(arrayFace[0], arrayFace[1]);
				sum += length * belta;
			}
		}
		
	} // end for i;

	area = 0.5 * area;	
	return ((0.25f * sum) * 3.0/area);
}

void CxCurvature::DrawCurvature()
{
	::glPushAttrib(GL_LIGHTING_BIT);
	::glDisable(GL_LIGHTING);

	// CalculateCurvature();

	int nv = (int)m_pMesh->m_ArrayVertex.size();
	double clen = .5f*m_pMesh->FeatureSize();

	::glColor3f(1, 0, 0);
	::glBegin(GL_LINES);
	int i;
	for (i = 0; i < nv; i++) 
	{
		glVertex3dv(m_pMesh->m_ArrayVertex[i]);
		const CxVector3d& n = m_pMesh->m_ArrayVertex[i].GetNormal();
		double x = m_pMesh->m_ArrayVertex[i][0] + clen * n[0];
		double y = m_pMesh->m_ArrayVertex[i][1] + clen * n[1];
		double z = m_pMesh->m_ArrayVertex[i][2] + clen * n[2];
		glVertex3d(x, y, z);
		//glVertex3fv(m_ArrayVertex[i] + clen*m_ArrayNormalVertex[i]);
	}

	glColor3f(0.0, 0.0, 1);
	for (i = 0; i < nv; i++) 
	{
		CxVector3d v(m_pMesh->m_ArrayVertex[i][0], m_pMesh->m_ArrayVertex[i][1], m_pMesh->m_ArrayVertex[i][2]);
		glVertex3dv(v - clen*m_ArrayPrinDir1[i]);
		glVertex3dv(v + clen*m_ArrayPrinDir1[i]);
	}

	glColor3f(0, 1, 0);
	for (i = 0; i < nv; i++) 
	{
		CxVector3d v(m_pMesh->m_ArrayVertex[i][0], m_pMesh->m_ArrayVertex[i][1], m_pMesh->m_ArrayVertex[i][2]);
		glVertex3dv(v - clen*m_ArrayPrinDir2[i]);
		glVertex3dv(v + clen*m_ArrayPrinDir2[i]);
	}

	::glEnd();
	::glPopAttrib();

}



void CxCurvature::GradientColorVertex(const vector<float>& arrayCurvature, float sum)
{
	if ( arrayCurvature.empty() )
		return;

	int nv = (int)m_pMesh->m_ArrayVertex.size();
	float mid = sum / (float)nv;
	sum = 0.0f;
	for (int i=0; i<nv; ++i )
		sum += (arrayCurvature[i] - mid )*(arrayCurvature[i]-mid);

	float factor = (float)sqrt(sum/float(nv));

	for ( int i=0; i<nv; ++i )
	{
		CxVertex3d& rVertex = m_pMesh->m_ArrayVertex[i];
		float tmp = (arrayCurvature[i] - mid) / factor * 1.5f;
		//TRACE("%f\n", tmp);
		CxColor c;

		if ( tmp < -2.1f )
			c.Set((unsigned char)0, 0, 0);
		else if ( tmp < -1.5f )
			c.Set(0, 0, (tmp + 2.1f)/0.6f);
		else if ( tmp < -0.9f )
			c.Set(0, (tmp + 1.5f)/0.6f, 1.0);
		else if ( tmp < -0.3f )
			c.Set(0.0, 1.0, -(tmp + 0.3f)/0.6f);
		else if ( tmp < 0.3f )
			c.Set((tmp + 0.3f)/0.6f, 1.0, 0);
		else if ( tmp < 0.9f )
			c.Set(1.0, -(tmp - 0.9f)/0.6f, 0.0);
		else if ( tmp < 1.5f )
			c.Set(1.0, 0, (tmp - 0.9f)/0.6f);
		else if ( tmp < 2.1f )
			c.Set(1.0, (tmp - 1.5f)/0.6f, 1.0);
		else
			c.Set((float)1.0, .75, 1.0);

		rVertex.SetColor(c);

	} // end for i;

	m_pMesh->SetColorBinding(COLOR_PER_VERTEX);

}

void CxCurvature::ColorCurvature(int nType)
{
	int nv = (int)m_pMesh->m_ArrayVertex.size();

	vector<float> arrayCurvature(nv);

	double sum = 0.0;
	for (int i=0; i<nv; ++i)
	{
		const CxVertex3d& v = m_pMesh->m_ArrayVertex[i];
		double c = 0.0;
		if ( nType == CUR_GAUSS )
			c = CurvatureGauss(m_pMesh->m_ArrayVertex[i]);
		else if ( nType == CUR_MEAN )
			c = CurvatureMean(m_pMesh->m_ArrayVertex[i]);
		else if ( nType == CUR_TOTAL )
		{
			double c1 = CurvatureGauss(m_pMesh->m_ArrayVertex[i]);
			double c2 = CurvatureMean(m_pMesh->m_ArrayVertex[i]);
			//c = c1*c1 + c2*c2;
			c = fabs(c2 + sqrt(c2*c2-c1)) + fabs(c2-sqrt(c2*c2 - c1));
		}
		else  // root mean square;
		{
			double c1 = CurvatureGauss(m_pMesh->m_ArrayVertex[i]);
			double c2 = CurvatureMean(m_pMesh->m_ArrayVertex[i]);
			c = sqrt(4*c2*c2 - 2*c1);
		}
		arrayCurvature[i] = (float)c;
		sum += c;
	}

	//	GradientColorVertex(arrayCurvature, sum);
	GradientColorVertex(arrayCurvature);
}

#include "xHistogram.h"
#include <algorithm>

void CxCurvature::GradientColorVertex(/*const */vector<float>& arrayCurvature)
{
	std::size_t size = arrayCurvature.size();
	float maxc = std::numeric_limits<float>::min();
	float minc = std::numeric_limits<float>::max();

	// 	for ( std::size_t i=0; i<size; ++i )
	// 	{
	// 		if ( maxc < arrayCurvature[i] )
	// 			maxc = arrayCurvature[i];
	// 		if ( minc > arrayCurvature[i] )
	// 			minc = arrayCurvature[i];
	// 	} // end for i;

	vector<float> tmp(arrayCurvature);
	sort(tmp.begin(), tmp.end(), std::less<float>());
	maxc = tmp.back();
	minc = tmp.front();
	
// 	// scale to [-1, 1];
// 	float max_new = 1;
// 	float min_new = -1;
// 	float factor = (max_new - min_new ) / (maxc - minc);
// 	for (std::size_t i=0; i<size; ++i )
// 	{
// 		arrayCurvature[i] = min_new + (arrayCurvature[i]-minc) * factor;
// 		float c = arrayCurvature[i];
// 		assert ( -1<= c && c <= 1);
// 	}
// 	maxc = min_new + (maxc - minc )*factor;
// 	minc = min_new + (minc - minc )*factor;

// #define Histogram_Equalization
	
	int n = 20000;

#ifdef Histogram_Equalization

	// 将[-1, 1]分成2000等份;
	// 统计每个等份的个数; // 类似于统计灰度图像每个灰度值的个数;
	vector<int> arrayHist(n, 0);
	for ( std::size_t i=0; i<size; ++i )
	{
		float c = arrayCurvature[i];
		c += 1.0f;
		c *= 1000;
		arrayHist[int(c)]++;
	}

	// 直方图均衡化;
	// 计算灰度映射表;
	vector<int> mapTable(n, 0);
	for (int i=0; i<n; ++i)
	{
		int nCounter = 0;
		for (int j=0; j<i; ++j)
			nCounter += arrayHist[j];
		mapTable[i] = nCounter;
	}

	// 计算新的"灰度"值;
	float coefficient = (max_new - min_new ) / size;
	for ( std::size_t i=0; i<size; ++i )
	{
		float c = arrayCurvature[i];
		c += 1.0f;
		c *= 1000;
		c = mapTable[int(c)]*coefficient;
		c *= 0.001;
		c -= 1.0f;
		arrayCurvature[i] = c;
	}

#endif // Histogram_Equalization

#if 0
	Histogramd h;
	h.SetRange(1, 5, 5);
	// 1, 2,2, 3, 3, 3, 4,4,4,4,5,5,5,5,5;
	for(int i=1;i<=5;++i)
		for(int j=0; j<i; ++j)
			h.Add(i);
	h.FileWrite("hist.txt");
	int n2 = h.BinCount(2);  // 2;
	int n5 = h.BinCount(5);  // 5;
	double n = h.BinWidth(4);
	int x = h.BinCount(2, 2); // 1~3 = 6;
	int nx = h.RangeCount(1, 3);
	double xyx =h.Percentile(.5);
	double xy = h.Percentile(0.6);
	double xy1 = h.Percentile(0.7);
#endif

	Histogramf h;
	int HistSize = n;
	//HistSize = (int)size+3;

	h.Clear();
	h.SetRange(minc, maxc, HistSize);
	//h.SetRange(minc, maxc, size);
	for (std::size_t i=0; i<size; ++i )
		h.Add(arrayCurvature[i]);

	// Sanity check; If some very wrong value has happened in the Q value, 
	// the histogram is messed. If a significant percentage (20% )of the values 
	// are all in a single bin
	// we should try to solve the problem. No easy solution here.
	// We choose to compute the get the 1percentile and 99 percentile values as 
	// new mixmax ranges and just to be sure enlarge the Histogram.
	if ( h.MaxCount() >  HistSize/5 )
	{
		// 		std::vector<float> QV;
		// 		QV.reserve(size);
		// 		for (std::size_t i=0; i<size; ++i )
		// 			QV.push_back(arrayCurvature[i]);
		std::vector<float> QV(arrayCurvature);

		std::nth_element(QV.begin(), QV.begin()+size/100, QV.end());
		float newmin = *(QV.begin()+size/100);
		std::nth_element(QV.begin(), QV.begin()+size-size/100, QV.end());
		//int xx = size - size/100;  // ?? 如果size/100 = 0 则出错.
		float newmax = *(QV.begin() + size - size/100);

		h.Clear();
		h.SetRange(newmin, newmax, HistSize*50);
		for (std::size_t i=0; i<size; ++i )
			h.Add(arrayCurvature[i]);
	}

	float newmin = h.Percentile(0.1f);
	float newmax = h.Percentile(0.85f);
	//newmin = h.Percentile(0);
	//newmax = h.Percentile(1);

	// set color;
	for (std::size_t i=0; i<size; ++i )
	{
		CxVertex3d& v = m_pMesh->m_ArrayVertex[i];
		CxColor c;
		c.ColorRamp(newmin, newmax, arrayCurvature[i]);
		v.SetColor(c);
	}

	m_pMesh->SetColorBinding(COLOR_PER_VERTEX);
}

void CxCurvature::HistogramEqualization(vector<float>& arrayCurvature)
{
	//
	// the curvature is first scaled to range [-1, 1] and
	// then histogram equalizaiton;
	//

	std::size_t size = arrayCurvature.size();
	vector<float> tmp(arrayCurvature);
	sort(tmp.begin(), tmp.end(), std::less<float>());
	float maxc = tmp.back();
	float minc = tmp.front();

	// scale to [-1, 1];
	float max_new = 1;
	float min_new = -1;
	float factor = (max_new - min_new ) / (maxc - minc);
	for (std::size_t i=0; i<size; ++i )
	{
		arrayCurvature[i] = min_new + (arrayCurvature[i]-minc) * factor;
		float c = arrayCurvature[i];
		assert ( -1<= c && c <= 1);
	}
	maxc = min_new + (maxc - minc )*factor;
	minc = min_new + (minc - minc )*factor;	

	// 将[-1, 1]分成2000等份;
	// 统计每个等份的个数; // 类似于统计灰度图像每个灰度值的个数;
	
	int n = 2001;
	vector<int> arrayHist(n, 0);
	for ( std::size_t i=0; i<size; ++i )
	{
		float c = arrayCurvature[i];
		c += 1.0f;
		c *= 1000;
		arrayHist[int(c)]++;
	}

	// 直方图均衡化;
	// 计算灰度映射表;
	vector<int> mapTable(n, 0);
	for (int i=0; i<n; ++i)
	{
		int nCounter = 0;
		for (int j=0; j<i; ++j)
			nCounter += arrayHist[j];
		mapTable[i] = nCounter;
	}

	// 计算新的"灰度"值;
	float coefficient = (max_new - min_new ) / size;
	for ( std::size_t i=0; i<size; ++i )
	{
		float c = arrayCurvature[i];
		c += 1.0f;
		c *= 1000;
		c = mapTable[int(c)]*coefficient;
		c *= 0.001f;
		c -= 1.0f;
		arrayCurvature[i] = c;
	}
}