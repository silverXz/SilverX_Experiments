// xFace.h: interface for the CxFace class.
// ***************************************************************
//  xFace   version:  1.0   
//  -------------------------------------------------------------
//  Description:  三角形面片类;
//  Author     :  wujh@sia.cn 
//  Created    :  07/27/2006
//  Modified   :  
//  -------------------------------------------------------------
//	Copyright (c) 2006 - All Rights Reserved
//
//  Shenyang Institute of Automation, Chinese Academy of Sciences
//  *****************************************************************
//
//  *****************************************************************

#if !defined _XFACE_H__INCLUDED_
#define _XFACE_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "xVector3d.h"
#include "xColor.h"

class CxFace  
{
private:             
	
	int           m_nVertex[3];    // 构成面三个顶点的索引号;
	int           m_nFace[3];      // 邻面;
    int           m_nIndex;        // 面索引号;
	char          m_nFlag;         // 标识;
	CxVector3d    m_Normal;        // 面的法向量;
	CxColor       m_Color;         // Color of face;


public:

	// ctors;
	CxFace();
	CxFace(int v1, int v2, int v3);
	CxFace(const CxFace& rhs);
	CxFace(const CxFace* pFace);
	CxFace(const int* vertex);

	~CxFace();

	// data setting;
	void Set(int v1, int v2, int v3);   // set vertex;
	void Set(const CxFace& rhs);
	void Set(const CxFace* pFace);
	void Set(const int* vertex);
	void SetIndex(int index)                     { m_nIndex = index;      }
	void SetNormal(const CxVector3d& v)          { m_Normal.Set(v);       }
	void SetNormal(double x, double y, double z) { m_Normal.Set(x, y, z); }
	void SetNormal(const double* normal);
	void SetFace(int f1, int f2, int f3);
	void SetFace(int index, int face) { m_nFace[index%3] = face;}
	void SetFlag(char flag) { m_nFlag = flag; }
	void SetColor(const CxColor& c) { m_Color.Set(c);}
	void SetColor(unsigned char r, unsigned char g, unsigned char b){m_Color.Set(r,g,b);}


	// data accessing;  
	int v1()const       { return m_nVertex[0];  }
	int v2()const       { return m_nVertex[1];  }
	int v3()const       { return m_nVertex[2];  }
	int v(int v)const   { return m_nVertex[v%3];}
	int GetIndex()const { return m_nIndex;      }

//	const CxVector3d* GetNormal()const{ return &m_Normal; }
	const CxVector3d& GetNormal()const{ return  m_Normal; }
	void GetNormal(double&x, double& y, double& z)const;
	int f1()const       { return m_nFace[0];   }
	int f2()const       { return m_nFace[1];   }
	int f3()const       { return m_nFace[2];   }
	int GetFace(int i)const  { return m_nFace[i%3]; }
	char GetFlag()const  { return m_nFlag; }

	const CxColor& GetColor()const { return m_Color;}


	      int &operator[](int i)       { return m_nVertex[i]; }
	const int &operator[](int i) const { return m_nVertex[i]; }
	
	bool Equal(const CxFace& f)const;
	int  IndexFrom(int v) const;
	bool HasVertex(int index)const;
	int GetFaceNeighborInclusive(int v1, int v2);

	int GetVertexExclusive(int v1, int v2)const;
	int GetVertexExclusive(int other[2], int v)const;

	int* GetVertexArray(void);

    // Cosine Angle between two faces
	friend double CosAngle(const CxFace& f1, const CxFace& f2); 
	friend double SinAngle(const CxFace& f1, const CxFace& f2);
	friend double Angle(const CxFace& f1, const CxFace& f2);

};




#endif // !defined(_XFACE_H__INCLUDED_)
