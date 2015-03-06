// xFileIO.h: interface for the CxFileIO class.
// ***************************************************************
//  xFileIO   version:  1.0   
//  -------------------------------------------------------------
//  Description:  Read and write file, 文件的读入与写出;
//  Author     :  wujh@sia.cn 
//  Created    :  07/18/2006
//  Modified   : 
//  -------------------------------------------------------------
//  Copyright (C) 2006 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#if !defined _XFILEIO_H__INCLUDED_
#define _XFILEIO_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#pragma warning(disable:4786)
#endif // _MSC_VER > 1000


#include "xVertex3d.h"
#include "xFace.h"

class CxMesh;
class CxFileIO  
{
public:
	
	CxFileIO();
	virtual ~CxFileIO();

	static CxMesh* ReadFile(const char* strFileName);
	static void SaveFileAs(const char* strFileName, const CxMesh* pMesh);

	static void WriteAsObjFile(const char* strFileName, const CxMesh* pMesh);


	static void WriteAsciiPlyFile(const char* filename, const CxMesh* pMesh);


private:
	static CxMesh* ReadSTLFile(const char* strFileName);
	static CxMesh* ReadSTLBinFile1(const CString& strFileName);
	static CxMesh* ReadSTLBinFile(const CString& strFileName);
	static CxMesh* ReadSTLAcsiiFile(const char* strFileName);
	
	static CxMesh* ReadObjFile(const char* strFileName);
	static CxMesh* ReadObjFile2(const char* strFileName);

	static void WriteAsSTLBinFile(const CString& strFileName, const CxMesh* pMesh);
	static void WriteNPTSFile(const char* filename, const CxMesh* pMesh);
	static CxMesh* ReadOFFFile(const char* strFileName);

	static CxMesh* ReadVMBinFile(const char* strFileName);

};

#endif // !defined(_XFILEIO_H__INCLUDED_)
