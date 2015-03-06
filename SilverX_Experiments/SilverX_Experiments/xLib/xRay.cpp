/*************************************************************************

   Filename: xRay.cpp    Version 1.0
   -----------------------------------------------------------------------
   Purpose : 
   Author  : Dr.Wu (jh.wu@siat.ac.cn)
   Created : 2013/09/04   14:50
   Modified:
   -----------------------------------------------------------------------
   Copyright (c) 2013 - All Rights Reserved

   Shenzhen Institutes of Advanced Technology, Chinese Academy of Sciences

**************************************************************************/
#include "StdAfx.h"
#include "xRay.h"


// algorithm from "Fast, Minimum Storage Ray-Triangle Intersection"
bool CxRay::isIntersectTriangle( const CxVector3d& p1, const CxVector3d& p2, const CxVector3d& p3, double* result )const
{
	CxVector3d edge1, edge2, tvec, pvec, qvec;
	double det;
	double u, v;
	const double EPSILON = 0.000001f;

	edge1 = p2 - p1;
	edge2 = p3 - p1;

	pvec = getDirection().Cross( edge2 );
	det = edge1.Dot( pvec );

#if 0 // we don't want to backface cull
	if ( det < EPSILON )
		return false;
	tvec = getOrigin() - vert0;

	u = tvec.dot( pvec );
	if ( ( u < 0.0 ) || ( u > det ) )
		return false;

	qvec = tvec.Cross( edge1 );
	v = getDirection().Dot( qvec );
	if ( v < 0.0 || u + v > det )
		return false;

	*result = edge2.Dot( qvec ) / det;
	return true;
#else
	if( det > -EPSILON && det < EPSILON )
		return false;

	double inv_det = 1.0f / det;
	tvec = getOrigin() - p1 ;
	u = tvec.Dot( pvec ) * inv_det;
	if( u < 0.0 || u > 1.0 )
		return false;

	qvec = tvec.Cross( edge1 );

	v = getDirection().Dot( qvec ) * inv_det;
	if( v < 0.0 || u + v > 1.0 )
		return 0;

	*result = edge2.Dot( qvec ) * inv_det;
	return true;
#endif
}

bool CxRay::isIntersectPlane( const CxVector3d& planeOrigin, const CxVector3d& planeNormal, double* result )const
{
	double denom = planeNormal.Dot( getDirection() );

	if ( denom != 0.0 )
	{
		*result = planeNormal.Dot(planeOrigin - getOrigin()) / denom;
		return true;
	}

	return false;
}

bool CxRay::isIntersectBoundingBox( const CxBoundingBox& box )const
{
	// References:
	//  Jeffrey Mahovsky and Brian Wyvill, "Fast Ray-Axis Aligned Bounding Box
	//  Overlap Tests with Plücker Coordinates", Journal of Graphics Tools,
	//  9(1):35-46.
	//	CeVector3d direction = mDir*mLength;
	//CeVector3d direction = mDir;
	CxVector3d direction = mDirection;

	if (direction.x() < 0.0f)
	{
		if (direction.y() < 0.0f)
		{
			if (direction.z() < 0.0f)
			{
				// case MMM: side(R,HD) < 0 or side(R,FB) > 0 or side(R,EF) > 0 or side(R,DC) < 0 or side(R,CB) < 0 or side(R,HE) > 0 to miss
				if ( (mOrigin.x() < box.xMin()) ||(mOrigin.y() < box.yMin()) || (mOrigin.z() < box.zMin() ) )
					return false;

				double xa = box.xMin() - mOrigin.x();
				double ya = box.yMin() - mOrigin.y();
				double za = box.zMin() - mOrigin.z();
				double xb = box.xMax() - mOrigin.x();
				double yb = box.yMax() - mOrigin.y();
				double zb = box.zMax() - mOrigin.z();

				if (   (direction.x() * ya - direction.y() * xb < 0)
					|| (direction.x() * yb - direction.y() * xa > 0)
					|| (direction.x() * zb - direction.z() * xa > 0)
					|| (direction.x() * za - direction.z() * xb < 0)
					|| (direction.y() * za - direction.z() * yb < 0)
					|| (direction.y() * zb - direction.z() * ya > 0))
					return false;

				return true;
			}
			else
			{
				// case MMP: side(R,HD) < 0 or side(R,FB) > 0 or side(R,HG) > 0 or side(R,AB) < 0 or side(R,DA) < 0 or side(R,GF) > 0 to miss
				if ( (mOrigin.x() < box.xMin() ) || (mOrigin.y() < box.yMin() ) || (mOrigin.z() > box.zMax() ) )
					return false;

				double xa = box.xMin() - mOrigin.x();
				double ya = box.yMin() - mOrigin.y();
				double za = box.zMin() - mOrigin.z();
				double xb = box.xMax() - mOrigin.x();
				double yb = box.yMax() - mOrigin.y();
				double zb = box.zMax() - mOrigin.z();

				if (   (direction.x() * ya - direction.y() * xb < 0.0f)
					|| (direction.x() * yb - direction.y() * xa > 0.0f)
					|| (direction.x() * zb - direction.z() * xb > 0.0f)
					|| (direction.x() * za - direction.z() * xa < 0.0f)
					|| (direction.y() * za - direction.z() * ya < 0.0f)
					|| (direction.y() * zb - direction.z() * yb > 0.0f))
					return false;

				return true;
			}
		}
		else
		{
			if (direction.z() < 0.0f)
			{
				// case MPM: side(R,EA) < 0 or side(R,GC) > 0 or side(R,EF) > 0 or side(R,DC) < 0 or side(R,GF) < 0 or side(R,DA) > 0 to miss					
				if ( (mOrigin.x() < box.xMin() ) || (mOrigin.y() > box.yMax()) || (mOrigin.z() < box.zMin() ) )
					return false;

				double xa = box.xMin() - mOrigin.x();
				double ya = box.yMin() - mOrigin.y();
				double za = box.zMin() - mOrigin.z();
				double xb = box.xMax() - mOrigin.x();
				double yb = box.yMax() - mOrigin.y();
				double zb = box.zMax() - mOrigin.z();

				if ((direction.x() * ya - direction.y() * xa < 0.0f)
					|| (direction.x() * yb - direction.y() * xb > 0.0f)
					|| (direction.x() * zb - direction.z() * xa > 0.0f)
					|| (direction.x() * za - direction.z() * xb < 0.0f)
					|| (direction.y() * zb - direction.z() * yb < 0.0f)
					|| (direction.y() * za - direction.z() * ya > 0.0f))
					return false;

				return true;
			}
			else
			{
				// case MPP: side(R,EA) < 0 or side(R,GC) > 0 or side(R,HG) > 0 or side(R,AB) < 0 or side(R,HE) < 0 or side(R,CB) > 0 to miss
				if ( (mOrigin.x() < box.xMin()) || (mOrigin.y() > box.yMax()) || (mOrigin.z() > box.zMax() ) )
					return false;

				double xa = box.xMin() - mOrigin.x();
				double ya = box.yMin() - mOrigin.y();
				double za = box.zMin() - mOrigin.z();
				double xb = box.xMax() - mOrigin.x();
				double yb = box.yMax() - mOrigin.y();
				double zb = box.zMax() - mOrigin.z();

				if (   (direction.x() * ya - direction.y() * xa < 0.0f)
					|| (direction.x() * yb - direction.y() * xb > 0.0f)
					|| (direction.x() * zb - direction.z() * xb > 0.0f)
					|| (direction.x() * za - direction.z() * xa < 0.0f)
					|| (direction.y() * zb - direction.z() * ya < 0.0f)
					|| (direction.y() * za - direction.z() * yb > 0.0f))
					return false;

				return true;
			}
		}
	}
	else
	{
		if ( direction.y() < 0.0f )
		{
			if ( direction.z() < 0.0f )
			{
				// case PMM: side(R,GC) < 0 or side(R,EA) > 0 or side(R,AB) > 0 or side(R,HG) < 0 or side(R,CB) < 0 or side(R,HE) > 0 to miss
				if ( ( mOrigin.x() > box.xMax() ) || (mOrigin.y() < box.yMin() ) || (mOrigin.z() < box.zMin() ) )
					return false;

				double xa = box.xMin() - mOrigin.x();
				double ya = box.yMin() - mOrigin.y();
				double za = box.zMin() - mOrigin.z();
				double xb = box.xMax() - mOrigin.x();
				double yb = box.yMax() - mOrigin.y();
				double zb = box.zMax() - mOrigin.z();

				if (   (direction.x() * yb - direction.y() * xb < 0.0f)
					|| (direction.x() * ya - direction.y() * xa > 0.0f)
					|| (direction.x() * za - direction.z() * xa > 0.0f)
					|| (direction.x() * zb - direction.z() * xb < 0.0f)
					|| (direction.y() * za - direction.z() * yb < 0.0f)
					|| (direction.y() * zb - direction.z() * ya > 0.0f))
					return false;

				return true;
			}
			else
			{
				// case PMP: side(R,GC) < 0 or side(R,EA) > 0 or side(R,DC) > 0 or side(R,EF) < 0 or side(R,DA) < 0 or side(R,GF) > 0 to miss
				if ( (mOrigin.x() > box.xMax()) || (mOrigin.y() < box.yMin()) || (mOrigin.z() > box.zMax() ) )
					return false;

				double xa = box.xMin() - mOrigin.x();
				double ya = box.yMin() - mOrigin.y();
				double za = box.zMin() - mOrigin.z();
				double xb = box.xMax() - mOrigin.x();
				double yb = box.yMax() - mOrigin.y();
				double zb = box.zMax() - mOrigin.z();

				if (   (direction.x() * yb - direction.y() * xb < 0.0f)
					|| (direction.x() * ya - direction.y() * xa > 0.0f)
					|| (direction.x() * za - direction.z() * xb > 0.0f)
					|| (direction.x() * zb - direction.z() * xa < 0.0f)
					|| (direction.y() * za - direction.z() * ya < 0.0f)
					|| (direction.y() * zb - direction.z() * yb > 0.0f))
					return false;

				return true;
			}
		}
		else
		{
			if (direction.z() < 0.0f)
			{
				// case PPM: side(R,FB) < 0 or side(R,HD) > 0 or side(R,AB) > 0 or side(R,HG) < 0 or side(R,GF) < 0 or side(R,DA) > 0 to miss
				if ( (mOrigin.x() > box.xMax() ) || ( mOrigin.y() > box.yMax() ) || ( mOrigin.z() < box.zMin() ) )
					return false;

				double xa = box.xMin() - mOrigin.x();
				double ya = box.yMin() - mOrigin.y();
				double za = box.zMin() - mOrigin.z();
				double xb = box.xMax() - mOrigin.x();
				double yb = box.yMax() - mOrigin.y();
				double zb = box.zMax() - mOrigin.z();

				if (   (direction.x() * yb - direction.y() * xa < 0.0f)
					|| (direction.x() * ya - direction.y() * xb > 0.0f)
					|| (direction.x() * za - direction.z() * xa > 0.0f)
					|| (direction.x() * zb - direction.z() * xb < 0.0f)
					|| (direction.y() * zb - direction.z() * yb < 0.0f)
					|| (direction.y() * za - direction.z() * ya > 0.0f))
					return false;

				return true;
			}
			else
			{
				// case PPP: side(R,FB) < 0 or side(R,HD) > 0 or side(R,DC) > 0 or side(R,EF) < 0 or side(R,HE) < 0 or side(R,CB) > 0 to miss
				if ( (mOrigin.x() > box.xMax() ) || (mOrigin.y() > box.yMax()) || ( mOrigin.z() > box.zMax() ) )
					return false;

				double xa = box.xMin() - mOrigin.x();
				double ya = box.yMin() - mOrigin.y();
				double za = box.zMin() - mOrigin.z();
				double xb = box.xMax() - mOrigin.x();
				double yb = box.yMax() - mOrigin.y();
				double zb = box.zMax() - mOrigin.z();

				if (   (direction.x() * yb - direction.y() * xa < 0.0f)
					|| (direction.x() * ya - direction.y() * xb > 0.0f)
					|| (direction.x() * za - direction.z() * xb > 0.0f)
					|| (direction.x() * zb - direction.z() * xa < 0.0f)
					|| (direction.y() * zb - direction.z() * ya < 0.0f)
					|| (direction.y() * za - direction.z() * yb > 0.0f))
					return false;

				return true;
			}
		}
	}

	return false;
}

/*
 * Ray-box intersection using IEEE numerical properties to ensure that the
 * test is both robust and efficient, as described in:
 *
 *      Amy Williams, Steve Barrus, R. Keith Morley, and Peter Shirley
 *      "An Efficient and Robust Ray-Box Intersection Algorithm"
 *      Journal of graphics tools, 10(1):49-54, 2005
 *
 */
bool CxRay::isIntersectBoundingBox(const CxBoundingBox& rBox, float t0, float t1)const
{
	CxVector3d invDir(1.0f/(float)mDirection.x(), 1.0f/(float)mDirection.y(), 1.0f/(float)mDirection.z());
	int iSign[3];
	iSign[0] = ( invDir.x() < 0 );
	iSign[1] = ( invDir.y() < 0 );
	iSign[2] = ( invDir.z() < 0 );

	const CxVertex3d& minVertex = rBox.GetMinVertex();
	const CxVertex3d& maxVertex = rBox.GetMaxVertex();
	
	double tmin, tmax, tymin, tymax, tzmin, tzmax;

	if ( iSign[0] == 0 )
		tmin = ( minVertex.x() - mOrigin.x() ) * invDir.x();
	else
		tmin = ( maxVertex.x() - mOrigin.x() ) * invDir.x();

	if ( (1 - iSign[0]) == 0 )
		tmax = ( minVertex.x() - mOrigin.x() ) * invDir.x();
	else
		tmax = ( maxVertex.x() - mOrigin.x() ) * invDir.x();

	if ( iSign[1] == 0 )
		tymin = ( minVertex.y() - mOrigin.y() ) * invDir.y();
	else
		tymin = ( maxVertex.y() - mOrigin.y() ) * invDir.y();

	if ( ( 1- iSign[1]) == 0 )
		tymax = ( minVertex.y() - mOrigin.y() ) * invDir.y();
	else
		tymax = ( maxVertex.y() - mOrigin.y() ) * invDir.y();


	if ( (tmin > tymax) || (tymin > tmax) ) 
		return false;
	if ( tymin > tmin )
		tmin = tymin;
	if ( tymax < tmax )
		tmax = tymax;

	if ( iSign[2] == 0 )
		tzmin = ( minVertex.z() - mOrigin.z() ) * invDir.z();
	else
		tzmin = ( minVertex.z() - mOrigin.z() ) * invDir.z();

	if ( ( 1 - iSign[2] ) == 0 )
		tzmax = ( minVertex.z() - mOrigin.z() ) * invDir.z();
	else
		tzmax = ( maxVertex.z() - mOrigin.z() ) * invDir.z();

	if ( (tmin > tzmax) || (tzmin > tmax) ) 
		return false;

	if ( tzmin > tmin )
		tmin = tzmin;

	if ( tzmax < tmax )
		tmax = tzmax;

//	return true;
	return ( ( tmin < t1 ) && ( tmax > t0 ) );
}


/**
 *	A method to compute a ray-AABB intersection.
 *	Original code by Andrew Woo, from "Graphics Gems", Academic Press, 1990
 *	Optimized code by Pierre Terdiman, 2000 (~20-30% faster on my Celeron 500)
 *	Epsilon value added by Klaus Hartmann. (discarding it saves a few cycles only)
 *
 *	Hence this version is faster as well as more robust than the original one.
 *
 *	Should work provided:
 *	1) the integer representation of 0.0f is 0x00000000
 *	2) the sign bit of the float is the most significant one
 *
 *	Report bugs: p.terdiman@codercorner.com
 *
 *	\param		aabb		[in] the axis-aligned bounding box
 *	\param		origin		[in] ray origin
 *	\param		dir			[in] ray direction
 *	\param		coord		[out] impact coordinates
 *	\return		true if ray intersects AABB
 */

bool CxRay::isIntersectBoundingBox( const CxBoundingBox& rBox, CxVector3d& rPoint )const
{
	typedef unsigned int udword;

	//! Integer representation of a floating-point value.
	#define IR(x)	((udword&)x)
	#define RAYAABB_EPSILON 0.00001f

	bool bInside = true;
	const CxVertex3d& minBox = rBox.GetMinVertex();
	const CxVertex3d& maxBox = rBox.GetMaxVertex();
	CxVector3d maxT(-1.0f, -1.0f, -1.0f );
	// Find candidate planes.
	for ( int i=0; i<3; i++)
	{
		if ( mOrigin[i] < minBox[i] )
		{
			rPoint[i] = minBox[i];
			bInside = false;

			// Calculate T distances to candidate planes
			if ( IR( mDirection[i] ) )
				maxT[i] = ( minBox[i] - mOrigin[i] ) / mDirection[i];
		}
		else if ( mOrigin[i] > maxBox[i] )
		{
			rPoint[i] = maxBox[i];
			bInside = false;

			// Calculate T distances to candidate planes
			if ( IR ( mDirection[i]) )
				maxT[i] = ( maxBox[i] - mOrigin[i] ) / mDirection[i];
		}
	}

	// Ray origin inside bounding box
	if ( bInside )
	{
		rPoint = mOrigin;
		return true;
	}

	// Get largest of the maxT's for final choice of intersection
	int WhichPlane = 0;
	if( maxT[1] > maxT[WhichPlane])	WhichPlane = 1;
	if( maxT[2] > maxT[WhichPlane])	WhichPlane = 2;

	// Check final candidate actually inside box
	if ( IR( maxT[WhichPlane] )& 0x80000000 ) 
		return false;

	for ( int i=0; i<3; i++ )
	{
		if( i != WhichPlane )
		{
			rPoint[i] = mOrigin[i] + maxT[WhichPlane]*mDirection[i];
#ifdef RAYAABB_EPSILON
			if(rPoint[i] < minBox[i] - RAYAABB_EPSILON || rPoint[i] > maxBox[i] + RAYAABB_EPSILON)	
				return false;
#else
			if(rPoint[i] < minBox[i] || rPoint[i] > maxBox[i])	return false;
#endif
		}
	}
	return true;	// ray hits box
}

//
//  http://www.3dkingdoms.com/weekly/weekly.php?a=21
// //Does the Line (rPt0, rPt1) intersect the Box?
bool CxLine::isIntersectBoundingBox(const CxBoundingBox& rBox)const
{
	// Get line midpoint and extent
	const CxVector3d mid = 0.5*(m_Start + m_End);
	const CxVector3d L = ( m_Start - mid );
	const CxVector3d LExt = CxVector3d( fabs(L.x()), fabs(L.y()), fabs(L.z()));
	const CxVector3d extent = 0.5 * (rBox.GetMaxVertex() - rBox.GetMinVertex() );

	// Use Separating Axis Test
	// Separation vector from box center to line center is LMid, since the line is in box space
	if ( fabs( mid.x() ) > extent.x() + LExt.x() ) return false;
	if ( fabs( mid.y() ) > extent.y() + LExt.y() ) return false;
	if ( fabs( mid.z() ) > extent.z() + LExt.z() ) return false;
	// Crossproducts of line and each axis
	if ( fabs( mid.y() * L.z() - mid.z() * L.y() )  >  (extent.y() * LExt.z() + extent.z() * LExt.y() ) ) return false;
	if ( fabs( mid.x() * L.z() - mid.z() * L.x() )  >  (extent.x() * LExt.z() + extent.z() * LExt.x() ) ) return false;
	if ( fabs( mid.x() * L.y() - mid.y() * L.x() )  >  (extent.x() * LExt.y() + extent.y() * LExt.x() ) ) return false;
	// No separating axis, the line intersects
	return true;
}

bool CxLine::getIntersection(double dist1, double dist2, CxVector3d& rHit )const
{
	if ( (dist1*dist2) >= 0.0 ) 
		return false;

	if ( dist1 == dist2 )
		return false;
	rHit = m_Start + ( m_End - m_Start )* ( -dist1/(dist2-dist1));
	return true;
}

bool CxLine::isInBox(const CxBoundingBox& rBox, const CxVector3d& rHit, const int rAxis)const
{
	const CxVertex3d& rMin = rBox.GetMinVertex();
	const CxVertex3d& rMax = rBox.GetMaxVertex();

	if ( rAxis == 1 && rHit.z() > rMin.z() && rHit.z() < rMax.z() && rHit.y() > rMin.y() && rHit.y() < rMax.y() ) 
		return true;
	if ( rAxis == 2 && rHit.z() > rMin.z() && rHit.z() < rMax.z() && rHit.x() > rMin.x() && rHit.x() < rMax.x() ) 
		return true;
	if ( rAxis == 3 && rHit.x() > rMin.x() && rHit.x() < rMax.x() && rHit.y() > rMin.y() && rHit.y() < rMax.y() ) 
		return true;

	return false;

}

bool CxLine::isIntersectBoundingBox(const CxBoundingBox& rBox, CxVector3d& rHit)const
{
	const CxVertex3d& rMin = rBox.GetMinVertex();
	const CxVertex3d& rMax = rBox.GetMaxVertex();

	if ( m_End.x() < rMin.x() && m_Start.x() < rMin.x() ) return false;
	if ( m_End.x() > rMax.x() && m_Start.x() > rMax.x() ) return false;
	if ( m_End.y() < rMin.y() && m_Start.y() < rMin.y() ) return false;
	if ( m_End.y() > rMax.y() && m_Start.y() > rMax.y() ) return false;
	if ( m_End.z() < rMin.z() && m_Start.z() < rMin.z() ) return false;
	if ( m_End.z() > rMax.z() && m_Start.z() > rMax.z() ) return false;

	if (m_Start.x() > rMin.x() && m_Start.x() < rMax.x() &&
		m_Start.y() > rMin.y() && m_Start.y() < rMax.y() &&
		m_Start.z() > rMin.z() && m_Start.z() < rMax.z() ) 
	{
		rHit = m_Start; 
		return true;
	}

	if ( (  getIntersection( m_Start.x() - rMin.x(), m_End.x() - rMin.x(),  rHit) && isInBox( rBox, rHit,  1 ))
		|| (getIntersection( m_Start.y() - rMin.y(), m_End.y() - rMin.y(),  rHit) && isInBox( rBox, rHit,  2 )) 
		|| (getIntersection( m_Start.z() - rMin.z(), m_End.z() - rMin.z(),  rHit) && isInBox( rBox, rHit,  3 )) 
		|| (getIntersection( m_Start.x() - rMax.x(), m_End.x() - rMax.x(),  rHit) && isInBox( rBox, rHit,  1 )) 
		|| (getIntersection( m_Start.y() - rMax.y(), m_End.y() - rMax.y(),  rHit) && isInBox( rBox, rHit,  2 )) 
		|| (getIntersection( m_Start.z() - rMax.z(), m_End.z() - rMax.z(),  rHit) && isInBox( rBox, rHit,  3 )))
		return true;

	return false;
}

// http://www.gamedev.net/topic/338987-aabb---line-segment-intersection-test/
bool CxLine::isIntersectBoundingBox2(const CxBoundingBox& rBox)const
{
	const CxVertex3d& rMin = rBox.GetMinVertex();
	const CxVertex3d& rMax = rBox.GetMaxVertex();
	const CxVertex3d extent = rMax - rMin;

	CxVector3d d = 0.5 * (m_End - m_Start);

	CxVector3d e = 0.5 * (rBox.GetMaxVertex() - rBox.GetMinVertex());

	CxVector3d c = m_Start + d - CxVector3d((rBox.GetMinVertex() + rBox.GetMaxVertex()) * 0.5);
	CxVector3d ad(fabs(d[0]), fabs(d[1]), fabs(d[2]) );//Returns same vector with all components positive

	if ( fabs(c[0]) > e[0] + ad[0] ) return false;
	if ( fabs(c[1]) > e[1] + ad[1] ) return false;
	if ( fabs(c[2]) > e[2] + ad[2] ) return false;

	if ( fabs(d[1] * c[2] - d[2] * c[1]) > e[1] * ad[2] + e[2] * ad[1] + EPSILON)
		return false;

	if ( fabs(d[2] * c[0] - d[0] * c[2]) > e[2] * ad[0] + e[0] * ad[2] + EPSILON)
		return false;

	if ( fabs(d[0] * c[1] - d[1] * c[0]) > e[0] * ad[1] + e[1] * ad[0] + EPSILON)
		return false;

	return true;

}