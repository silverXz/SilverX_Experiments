/**************************************************************************
	
	File name:	xMeshAngiography.h      Version 1.0
---------------------------------------------------------------------------
	Purpose :	construct mesh for simulating angiography
    Author  :   Wang HaoYu (wang.haoyu@siat.ac.cn) 
	            Dr.Wu (jh.wu@siat.ac.cn)
	Created :	2014/08/01   16:05
    Modified:
---------------------------------------------------------------------------
   Copyright (c) 2014 - All Rights Reserved

   Shenzhen Institutes of Advanced Technology, Chinese Academy of Sciences
**************************************************************************/
#include "StdAfx.h"
#include "xMeshAngiography.h"

#include <set>
#include <algorithm>  // for sort;
#include <iostream>


CxMeshAngiography::CxMeshAngiography(CxMesh* pMesh)
{
    m_pMesh = pMesh;
	m_pMeshKdTree = new CxMeshKdTree(m_pMesh);
	m_pMeshKdTree->build();
}

CxMeshAngiography::CxMeshAngiography(const CxVector3d& rBasePoint, const CxVector3d& rBaseOrientation, const double rRadius)
{
	m_BasePoint = rBasePoint;
	m_BaseOrientation = rBaseOrientation;
	m_Radius = rRadius;

	m_pMeshKdTree = new CxMeshKdTree(m_pMesh);
	m_pMeshKdTree->build(15);
}

CxMeshAngiography::~CxMeshAngiography(void)
{
	if ( m_pMeshKdTree )
		delete m_pMeshKdTree, m_pMeshKdTree = 0;
}


bool CxMeshAngiography::IsFaceOnRightSide(const CxFace& rTargetFace)const
{
#if 0
	const CxVector3d edge0(m_BasePoint, CxVector3d(m_pMesh->GetVertex( rTargetFace.v1() ) ));
	const CxVector3d edge1(m_BasePoint, CxVector3d(m_pMesh->GetVertex( rTargetFace.v2() ) ));
	const CxVector3d edge2(m_BasePoint, CxVector3d(m_pMesh->GetVertex( rTargetFace.v3() ) ));

	bool bp0 = false;
	bool bp1 = false;
	bool bp2 = false;

	if ( edge0.Dot(m_BaseOrientation) > 0.0 )
		bp0 = true;
	if ( edge1.Dot(m_BaseOrientation) > 0.0 )
		bp1 = true;
	if ( edge2.Dot(m_BaseOrientation) > 0.0 )
		bp2 = true;

	if ( bp0 && bp1 && bp2 )
		return true;
	else
		return false;
#endif

	bool flag[3] = {false, false, false};
	for (int i=0; i<3; ++i)
	{
		const CxVertex3d& v = m_pMesh->GetVertex( rTargetFace.v(i) );
		const CxVector3d edge = CxVector3d(v[0], v[1], v[2]) - m_BasePoint;
		if ( edge.Dot(m_BaseOrientation) > 0.0 )
			flag[i] = true;
	}

	if ( flag[0] && flag[1] && flag[2] )
		return true;
	else
		return false;

}

bool CxMeshAngiography::IsFaceCrossSurface(const CxFace& rTargetFace)const
{
	const CxVector3d edge0(m_BasePoint, CxVector3d(m_pMesh->GetVertex( rTargetFace.v1() ) ));
	const CxVector3d edge1(m_BasePoint, CxVector3d(m_pMesh->GetVertex( rTargetFace.v2() ) ));
	const CxVector3d edge2(m_BasePoint, CxVector3d(m_pMesh->GetVertex( rTargetFace.v3() ) ));

	bool bp0 = false;
	bool bp1 = false;
	bool bp2 = false;

	if ( edge0.Dot(m_BaseOrientation) > 0.0 )    //在目标面
		bp0 = true;
	if ( edge1.Dot(m_BaseOrientation) > 0.0 )    //在目标面
		bp1 = true;
	if ( edge2.Dot(m_BaseOrientation) > 0.0 )	//在目标面
		bp2 = true;

	if((bp0||bp1||bp2) && !(bp0&&bp1&&bp2))
		return true;
	else
		return false;

#if 0
	bool flag[3] = {false, false, false};
	for (int i=0; i<3; ++i)
	{
		const CxVertex3d& v = m_pMesh->GetVertex( rTargetFace.v(i) );
		const CxVector3d edge = CxVector3d(v[0], v[1], v[2]) - m_BasePoint;
		if ( edge.Dot(m_BaseOrientation) > 0.0 )
			flag[i] = true;
	}

	bool b1 = flag[0] | flag[1] | flag[2];
	bool b2 = flag[0] && flag[1] && flag[2];

	if ( b1 && !b2 )
		return true;
	else
		return false;
#endif
}

bool CxMeshAngiography::IsFaceInsideSphere(const CxFace& rTargetFace)const
{
	for ( int i=0; i<3; ++ i )
	{
		const CxVertex3d& v = m_pMesh->GetVertex( rTargetFace[i] );
		const CxVertex3d  c(m_BasePoint[0], m_BasePoint[1], m_BasePoint[2]);
		double dist = ::DistanceSquared(v, c);
		if ( dist <= m_Radius * m_Radius )
			return true;
	}
	return false;
}

bool CxMeshAngiography::IsFaceInsideSphere(const CxFace& rFace,const CxVertex3d& rCenter,const double rRadius)const
{
	for ( int i=0; i<3; ++ i )
	{
		const CxVertex3d& v = m_pMesh->GetVertex( rFace[i] );
		double dist = ::DistanceSquared(v, rCenter);
		if ( dist <= rRadius * rRadius )
			return true;
	}
	return false;
}

#if 0
bool CxMeshAngiography::IsFaceCrossSurface(const CxVertex3d& rBasePoint, 
										   const CxVector3d& rBaseOrientation, 
										   const CxFace& rTargetFace)const
{
	const CxVector3d edge0(rBasePoint, m_pMesh->GetVertex( rTargetFace.v1() ) );
	const CxVector3d edge1(rBasePoint, m_pMesh->GetVertex( rTargetFace.v2() ) );
	const CxVector3d edge2(rBasePoint, m_pMesh->GetVertex( rTargetFace.v3() ) );


	bool bp0 = false;
	bool bp1 = false;
	bool bp2 = false;

	if(edge0.Dot(rBaseOrientation) > 0.0)    //在目标面
		bp0 = true;
	if(edge1.Dot(rBaseOrientation) > 0.0)    //在目标面
		bp1 = true;
	if(edge2.Dot(rBaseOrientation) > 0.0)	//在目标面
		bp2 = true;

	if((bp0||bp1||bp2) && !(bp0&&bp1&&bp2))
		return true;
	else
		return false;
}

bool CxMeshAngiography::IsFaceOnRightSide(const CxVertex3d& rBasePoint, 
										  const CxVector3d& rBaseOrientation, 
										  const CxFace& rTargetFace)const
{
	const CxVector3d edge0(rBasePoint, m_pMesh->GetVertex( rTargetFace.v1() ) );
	const CxVector3d edge1(rBasePoint, m_pMesh->GetVertex( rTargetFace.v2() ) );
	const CxVector3d edge2(rBasePoint, m_pMesh->GetVertex( rTargetFace.v3() ) );

	bool bp0 = false;
	bool bp1 = false;
	bool bp2 = false;

	if(edge0.Dot(rBaseOrientation) > 0.0)
		bp0 = true;
	if(edge1.Dot(rBaseOrientation) > 0.0)
		bp1 = true;
	if(edge2.Dot(rBaseOrientation) > 0.0)
		bp2 = true;

	if(bp0 && bp1 && bp2)
		return true;
	else
		return false;
}

CxMesh* CxMeshAngiography::GenerateSubMesh(const CxVector3d& rBasePoint, const CxVector3d& rBaseOrientation, const double rRadius)const
{
	std::vector<CxFace> faceArray;
	std::vector<CxFace> tmpFaceArray; 
	std::vector<CxFace> tmpFaceArray2;

	const CxVertex3d xcenter(rBasePoint.x(),rBasePoint.y(),rBasePoint.z());

	int last = 0;
	int cur = 0;

	const CxVertex3d baseP(rBasePoint.x(),rBasePoint.y(),rBasePoint.z());

	//Step 1: Get all the inter-cross section triangles
	int faceNum = (int)m_pMesh->NumFaces();

	for(int i = 0 ; i < faceNum ; i++)
	{
		const CxFace& xface =  m_pMesh->GetFace(i); // m_ArrayFace[i];

		if ( IsFaceCrossSurface(baseP, rBaseOrientation, xface) )
		{
			//m_FaceArray.push_back(xface);
			tmpFaceArray.push_back(xface);
		}
	}



	//Step 2: Exclude those triangles that are outside the bounding sphere.
	std::vector<bool> checkFace(faceNum, false);
	//for(int i = 0 ; i < faceNum ; i++) checkFace[i] = false;

	for(std::size_t i = 0 ; i < tmpFaceArray.size() ; i++)
	{
		const CxFace& xface = tmpFaceArray[i];
		if ( IsFaceInsideSphere(xface, xcenter, rRadius) )
		{
			faceArray.push_back(xface);
			checkFace[xface.GetIndex()] = true;
		}
	}

	// ******************更新 last 为当前数组的元素量
	last = (int)faceArray.size();

	//Step 3: Process the neighbor facets of the triangles from step 1. Exclude those neighbor facets that in the
	//wrong side of the define surface and keep the right ones.

	tmpFaceArray.clear();

	for(std::size_t i = 0 ; i < faceArray.size(); i++)
	{
#if 0
		const CxFace& face0 = m_ArrayFace[faceArray[i].f1()];
		const CxFace& face1 = m_ArrayFace[faceArray[i].f2()];
		const CxFace& face2 = m_ArrayFace[faceArray[i].f3()];
		if( IsFaceOnRightSide(baseP,baseOrientation,face0) && !checkFace[face0.GetIndex()])
		{
			tmpFaceArray.push_back(face0);
			checkFace[face0.GetIndex()] = true;
		}

		if( IsFaceOnRightSide(baseP,baseOrientation,face1) && !checkFace[face1.GetIndex()])
		{
			tmpFaceArray.push_back(face1);
			checkFace[face1.GetIndex()] = true;
		}

		if( IsFaceOnRightSide(baseP,baseOrientation,face2) && !checkFace[face2.GetIndex()])
		{
			tmpFaceArray.push_back(face2);
			checkFace[face2.GetIndex()] = true;
		}
#endif

#if 1
		// 三个邻面;
		for ( int j=0; j<3; ++j )
		{
			const CxFace& face = m_pMesh->GetFace( faceArray[i].GetFace(j)  );
			if ( checkFace[ face.GetIndex() ] == true )
				continue;
			if ( IsFaceOnRightSide( baseP, rBaseOrientation, face ) )
				//if( !checkFace[ face.GetIndex()] && IsFaceOnRightSide( baseP, baseOrientation, face ) )
			{
				tmpFaceArray.push_back(face);
				checkFace[face.GetIndex()] = true;
			}
		}
#endif
	}

#if 0
	for( std::size_t i = 0 ; i < tmpFaceArray.size() ; i++)
	{
		faceArray.push_back(tmpFaceArray[i]);
	}
#endif

	// 将查询到面片存入待构建的数组里;
	faceArray.insert(faceArray.end(), tmpFaceArray.begin(), tmpFaceArray.end() );

	// ******************更新 cur 为当前数组的元素量
	cur = (int)faceArray.size();

	int calState = 1;

	while( last != cur )
	{
		last = cur;
		switch(calState)
		{
		case 1:      //tmpFaceArray is SOURCE, tmpFaceArray2 is DESTINATION!
			{
				for( std::size_t i = 0 ; i < tmpFaceArray.size(); i++)
				{
#if 0
					if(tmpFaceArray[i].f1() >= 0)
					{
						CxFace face0 = m_ArrayFace[tmpFaceArray[i].f1()];

						if( !checkFace[face0.GetIndex()])
						{
							tmpFaceArray2.push_back(face0);
							checkFace[face0.GetIndex()] = true;
						}
					}

					if ( tmpFaceArray[i].f2() >= 0 )
					{
						CxFace face1 = m_ArrayFace[tmpFaceArray[i].f2()];

						if( !checkFace[face1.GetIndex()])
						{
							tmpFaceArray2.push_back(face1);
							checkFace[face1.GetIndex()] = true;
						}
					}

					if(tmpFaceArray[i].f3() >= 0)
					{
						CxFace face2 = m_ArrayFace[tmpFaceArray[i].f3()];
						if( !checkFace[face2.GetIndex()])
						{
							tmpFaceArray2.push_back(face2);
							checkFace[face2.GetIndex()] = true;
						}
					}
#endif
#if 1
					// 遍历3个邻面;
					for ( int j=0; j<3; ++j )
					{
						int index = tmpFaceArray[i].GetFace(j);
						if ( index < 0 ) 
							continue;
						const CxFace& face = m_pMesh->GetFace( index );
						if ( !checkFace[ face.GetIndex() ] )
						{
							tmpFaceArray2.push_back(face);
							checkFace[ face.GetIndex() ] = true;
						}
					}
#endif

				}  // end for

				//tmpFaceArray is the next Destination, so It should be cleared.
				tmpFaceArray.clear();

#if 0
				for(int i = 0; i < tmpFaceArray2.size() ; i++)
				{
					faceArray.push_back(tmpFaceArray2[i]);
				}
#endif
				faceArray.insert(faceArray.end(), tmpFaceArray2.begin(), tmpFaceArray2.end() );

				cur = (int)faceArray.size();

				break;
			}
		case -1:	//tmpFaceArray2 is SOURCE, tmpFaceArray is DESTINATION!
			{
				for ( std::size_t i = 0 ; i < tmpFaceArray2.size(); i++)
				{
#if 0
					if(tmpFaceArray2[i].f1() >= 0)
					{
						CxFace face0 = m_ArrayFace[tmpFaceArray2[i].f1()];
						if( !checkFace[face0.GetIndex()])
						{
							tmpFaceArray.push_back(face0);
							checkFace[face0.GetIndex()] = true;
						}
					}

					if(tmpFaceArray2[i].f2() >=0)
					{
						CxFace face1 = m_ArrayFace[tmpFaceArray2[i].f2()];
						if( !checkFace[face1.GetIndex()])
						{
							tmpFaceArray.push_back(face1);
							checkFace[face1.GetIndex()] = true;
						}
					}

					if(tmpFaceArray2[i].f3() >= 0)
					{
						CxFace face2 = m_ArrayFace[tmpFaceArray2[i].f3()];
						if(!checkFace[face2.GetIndex()])
						{
							tmpFaceArray.push_back(face2);
							checkFace[face2.GetIndex()] = true;
						}
					}
#endif

#if 1
					for ( int j=0; j<3; ++j )
					{
						int index = tmpFaceArray2[i].GetFace(j);
						if ( index < 0 )
							continue;

						const CxFace& face = m_pMesh->GetFace( index ); //m_ArrayFace[index];
						if ( !checkFace[ face.GetIndex() ] )
						{
							tmpFaceArray.push_back(face);
							checkFace[ face.GetIndex() ] = true;
						}
					}
#endif

				}

				tmpFaceArray2.clear();

#if 0
				for(int i = 0 ; i < tmpFaceArray.size(); i++)
				{
					faceArray.push_back(tmpFaceArray[i]);
				}
#endif
				faceArray.insert(faceArray.end(), tmpFaceArray.begin(), tmpFaceArray.end() );

				cur = (int)faceArray.size();

				break;	
			}
		default:
			break;
		}

		calState = -calState;

	} // end while


	CxMesh* pSubMesh = ConstructMesh(faceArray);

	if ( pSubMesh )
	{
		CxMeshRepair mr(pSubMesh);
		mr.fillingHoles();
	}

	//CxFileIO::SaveFileAs("AngiPart.stl",pMesh);
	return pSubMesh;
}

#endif

CxMesh* CxMeshAngiography::GenerateSubMesh(const CxVector3d& rBasePoint, const CxVector3d& rBaseOrientation, const double rRadius)
{
#if 0
	static bool first = true;

	if ( first )
	{   // 如果是第一次载入模型，则需要构建模型的Kd tree;
		m_pMeshKdTree = new CxMeshKdTree(m_pMesh);
		m_pMeshKdTree->build();
		first = false;
	}
#endif

	m_BasePoint = rBasePoint;
	m_BaseOrientation = rBaseOrientation;
	m_Radius = rRadius;

	// 如果没有提供半径,则估计半径
	if ( rRadius <= 0.0 )
	{
		double radius = EstimateRadius(); 
		m_Radius = 1.5*radius;
		std::cout<<" radius = "<<m_Radius<<"\n";
	}

// 	double radius = rRadius;
	
	std::vector<int> faceIndexArray;
	//m_pMeshKdTree->hit(CxSphere(rBasePoint, radius), faceIndexArray );

	std::vector<CxFace> faceArray;
	std::vector<CxFace> tmpFaceArray; 
	std::vector<CxFace> tmpFaceArray2;

	//Step 1: Get all the inter-cross section triangles
	int faceNum = (int)m_pMesh->NumFaces();

	for(int i = 0 ; i < faceNum ; i++)
	{
		const CxFace& xface =  m_pMesh->GetFace(i); // m_ArrayFace[i];

		//if ( IsFaceCrossSurface(baseP, rBaseOrientation, xface) )
		if ( IsFaceCrossSurface( xface ))
		{
			//m_FaceArray.push_back(xface);
			tmpFaceArray.push_back(xface);
		}
	}



	//Step 2: Exclude those triangles that are outside the bounding sphere.
	std::vector<bool> checkFace(faceNum, false);
	for(std::size_t i = 0 ; i < tmpFaceArray.size() ; i++)
	{
		const CxFace& xface = tmpFaceArray[i];
		//if ( IsFaceInsideSphere(xface, xcenter, rRadius) )
		if ( IsFaceInsideSphere( xface ) )
		{
			faceArray.push_back(xface);
			checkFace[xface.GetIndex()] = true;
		}
	}

	// ******************更新 last 为当前数组的元素量

	int last = (int)faceArray.size();

	//Step 3: Process the neighbor facets of the triangles from step 1. Exclude those neighbor facets that in the
	//wrong side of the define surface and keep the right ones.

	tmpFaceArray.clear();

	for(std::size_t i = 0 ; i < faceArray.size(); i++)
	{
		// 三个邻面;
		for ( int j=0; j<3; ++j )
		{
			const CxFace& face = m_pMesh->GetFace( faceArray[i].GetFace(j)  );
			if ( checkFace[ face.GetIndex() ] == true )
				continue;
			//if ( IsFaceOnRightSide( baseP, rBaseOrientation, face ) )
			if ( IsFaceOnRightSide( face ) )
				//if( !checkFace[ face.GetIndex()] && IsFaceOnRightSide( baseP, baseOrientation, face ) )
			{
				tmpFaceArray.push_back(face);
				checkFace[face.GetIndex()] = true;
			}
		}
	}

	// 将查询到面片存入待构建的数组里;
	faceArray.insert(faceArray.end(), tmpFaceArray.begin(), tmpFaceArray.end() );

	// ******************更新 cur 为当前数组的元素量
	int cur = (int)faceArray.size();
	int calState = 1;

	while( last != cur )
	{
		last = cur;
		switch(calState)
		{
		case 1:      //tmpFaceArray is SOURCE, tmpFaceArray2 is DESTINATION!
			{
				for( std::size_t i = 0 ; i < tmpFaceArray.size(); i++)
				{
					// 遍历3个邻面;
					for ( int j=0; j<3; ++j )
					{
						int index = tmpFaceArray[i].GetFace(j);
						if ( index < 0 ) 
							continue;
						const CxFace& face = m_pMesh->GetFace( index );
						if ( !checkFace[ face.GetIndex() ] )
						{
							tmpFaceArray2.push_back(face);
							checkFace[ face.GetIndex() ] = true;
						}
					}

				}  // end for

				//tmpFaceArray is the next Destination, so It should be cleared.
				tmpFaceArray.clear();

				faceArray.insert(faceArray.end(), tmpFaceArray2.begin(), tmpFaceArray2.end() );
				cur = (int)faceArray.size();

				break;
			}
		case -1:	//tmpFaceArray2 is SOURCE, tmpFaceArray is DESTINATION!
			{
				for ( std::size_t i = 0 ; i < tmpFaceArray2.size(); i++)
				{
					for ( int j=0; j<3; ++j )
					{
						int index = tmpFaceArray2[i].GetFace(j);
						if ( index < 0 )
							continue;

						const CxFace& face = m_pMesh->GetFace( index ); //m_ArrayFace[index];
						if ( !checkFace[ face.GetIndex() ] )
						{
							tmpFaceArray.push_back(face);
							checkFace[ face.GetIndex() ] = true;
						}
					}
				}

				tmpFaceArray2.clear();

				faceArray.insert(faceArray.end(), tmpFaceArray.begin(), tmpFaceArray.end() );
				cur = (int)faceArray.size();

				break;	
			}
		default:
			break;
		}

		calState = -calState;

	} // end while


	CxMesh* pSubMesh = ConstructMesh(faceArray);

	if ( pSubMesh )
	{
		CxMeshRepair mr(pSubMesh);
		mr.fillingHoles();
	}

	//CxFileIO::SaveFileAs("AngiPart.stl",pMesh);
	return pSubMesh;
}

CxMesh* CxMeshAngiography::ConstructMesh(const std::vector<CxFace>& rFaceArray )const
{
	if ( rFaceArray.empty() )
		return NULL;

	//Create a new CxMesh Instance.
	CxMesh* pMesh = new CxMesh;

	int meshNumFace = (int)rFaceArray.size();
	pMesh->Reserve(3 * meshNumFace, meshNumFace);


	//Dr. Wu's xFileIO code, read BIN_STL_FILE
	typedef std::set<CxVertex3d, std::less<CxVertex3d> >  VertexSet;
	VertexSet tmpVertexSet;      // 储存模型的实际顶点数;
	std::pair<VertexSet::iterator, bool> pair_set;

	int nIndex = 0;
	int nCnt = 0;

	for( int i = 0 ; i < meshNumFace ; i++)
	{

		//Calculate the normal for the face first!

		// CxVertex3d vert[3] = {m_ArrayVertex[faceArray[i].v1()], m_ArrayVertex[faceArray[i].v2()],m_ArrayVertex[faceArray[i].v3()]}; 
		CxVertex3d vert[3];
		for ( int j=0; j<3; ++j ) vert[j] = m_pMesh->GetVertex( rFaceArray[i].v(j) );
#if 0
		CxVector3d edge0(vert[0],vert[1]);
		CxVector3d edge1(vert[1],vert[2]);
		CxVector3d Normalx = edge0.Cross(edge1);
#endif
		const CxVector3d& Normalx = rFaceArray[i].GetNormal();
		CxFace xface;
		xface.SetNormal(Normalx);
		xface.SetIndex(i);

		for(int j = 0 ; j < 3 ; j++)
		{
			CxVertex3d v(vert[j]);
			pair_set = tmpVertexSet.insert(v);
			if( pair_set.second == true )
			{
				pMesh->AddVertex(CxVertex3d(vert[j].x(),vert[j].y(),vert[j].z(),nIndex));
				CxVertex3d& cv = const_cast<CxVertex3d&>(*pair_set.first);
				cv.SetIndex(nIndex++);
			}
			VertexSet::iterator it = tmpVertexSet.find(v);
			if(it != tmpVertexSet.end())
				xface[j] = (*it).GetIndex();
		}

		pMesh->AddFace(xface);
	}


	pMesh->SetHasNormalFace(true);
	pMesh->SetHasNormalVertex(false);
	pMesh->CreateAdjacency();
	pMesh->CalculateNormalPerFace();
	pMesh->CalculateNormalPerVertex();
	pMesh->CalculateBoundingBox();
	pMesh->SetModifiedFlag(true);

	return pMesh;

}

// 绕基准方向一周发射线，求与血管壁的距离，距离的平均值为血管半径;
double CxMeshAngiography::EstimateRadius()const
{
	if ( m_pMeshKdTree == NULL )
		return 0.0;

	// 得到与 base方向垂直的向量;
	const CxVector3d orth = CxVector3d( -m_BaseOrientation[2], 0, m_BaseOrientation[0]);
	assert( orth.Dot( m_BaseOrientation ) == 0 );

	const int nParts = 15; // 将圆分成30份;
	std::vector<double> distArray; // 存储basePoint到血管壁的距离;
	for ( int g = 0; g < nParts; ++g )
	{
		double angle = (g+1)*M_PI/(double)nParts;

		CxVector3d orthogonal = orth.Rotate(angle, m_BaseOrientation);
		orthogonal.Normalize();

		const CxRay ray(m_BasePoint, orth);

		std::vector<double> times;
		std::vector<double> distTmp;
		if ( m_pMeshKdTree->hit(ray, times) ) // 注意，射线反方向也会与血管壁相交, so, 15 * 2 = 30;
		{
			for ( std::size_t j = 0; j<times.size(); ++j )
			{
				const CxVector3d v = ray.getPosition( times[j] );
				const CxVector3d r = v - m_BasePoint;
				distTmp.push_back( r.Length() );
			}
		}

		// 一般情况下，射线与血管壁只有两个交点(包括反方向)
		if ( distTmp.size() == 2 )
		{
			distArray.push_back( distTmp[0] );
			distArray.push_back( distTmp[1] );
		}
        else if ( distTmp.size() > 2 )
		{	// 如果超过2个交点, 说明射线与其他分枝也相交了
		    // 所以根据basePoint到血管壁的距离排序, 取距离最短的两个
			std::sort(distTmp.begin(), distTmp.end() ); // 递增排序;
			distArray.push_back( distTmp[0] );
			distArray.push_back( distTmp[1] );
		}
		else 
		{
			// 不会没有交点;如果程序执行到这，说明求错;
			assert ( 0 );
		}
	} // end for g;

	// 
	std::sort(distArray.begin(), distArray.end() );
	return distArray[ distArray.size()-1];
	
#if 0
	std::size_t size = distArray.size();
	double sum = 0;
	for ( std::size_t i=0; i<size; ++i )
	{
		sum += distArray[i];
	}

	const double radius = sum / (double)size;

	
	return radius;
#endif
}

