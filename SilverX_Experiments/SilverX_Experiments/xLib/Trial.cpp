#include "StdAfx.h"
#include "Trial.h"

CTrial::CTrial(CString strTxtFileName)
{
	m_strTxtFilePath = GetTxtFilePath(strTxtFileName);
}

CTrial::~CTrial()
{
}

// The result always ends with a '\'
CString CTrial::GetDirFromFilePath(CString strFilePath) const
{
	CString strDir(strFilePath);
	const int n = strDir.ReverseFind('\\');
	if (n != -1)
		strDir = strDir.Left(n + 1);
	else
		strDir = "";

	return strDir;
}

CString CTrial::GetTxtFilePath(CString strTxtFileName) const
{
	CString strTxtFilePath;

	GetModuleFileName(NULL, strTxtFilePath.GetBuffer(MAX_PATH), MAX_PATH);
	strTxtFilePath.ReleaseBuffer();

	strTxtFilePath = GetDirFromFilePath(strTxtFilePath) + strTxtFileName;
	return strTxtFilePath;
}

bool CTrial::ReadFile(CTime& timeInstalled, CTime& timeLastUsed, CTime& expiryDate) const
{
	__time64_t nTimeInstalled, nTimeLastUsed, nExpiryDate;

	FILE *file = fopen(m_strTxtFilePath, "rt");
	if (file)
	{
		const int n = fscanf(file, "%I64X %I64X %I64X", &nTimeInstalled, &nTimeLastUsed, &nExpiryDate);
		fclose(file);
		if (n == 3)
		{
			timeInstalled = nTimeInstalled;
			timeLastUsed = nTimeLastUsed;
			expiryDate = nExpiryDate;
			return true;
		}
	}
	return false;
}

bool CTrial::WriteFile(CTime timeInstalled, CTime timeLastUsed, CTime expiryDate) const
{
	int n = 0;

	FILE *file = fopen(m_strTxtFilePath, "wt");
	if (file)
	{
		n = fprintf(file, "%I64X %I64X %I64X", timeInstalled.GetTime(), timeLastUsed.GetTime(), expiryDate.GetTime());
		fprintf(file, "\n\n2007-2013 (c) - Shenzhen Institutes of Advanced Technology, Chinese Academy of Sciences.  All rights reserved.\n");
		fprintf(file, "\nFor more information, please contact Dr.Wu (jh.wu@siat.ac.cn).\n");

		fclose(file);
	}
	return n > 0;
}

bool CTrial::WriteFile(int nExpiryYear, int nExpiryMonth, int nExpiryDay) const
{
	int n = 0;

	FILE *file = fopen(m_strTxtFilePath, "wt");
	if (file)
	{
        const CTime currentTime = CTime::GetCurrentTime();
		const CTime expiryDate(nExpiryYear, nExpiryMonth, nExpiryDay, 0, 0, 0);
		n = fprintf(file, "%I64X %I64X %I64X", currentTime.GetTime(), currentTime.GetTime(), expiryDate.GetTime());
		fprintf(file, "\n\n2007-2013 (c) - Shenzhen Institutes of Advanced Technology, Chinese Academy of Sciences.  All rights reserved.\n");
		fprintf(file, "\nFor more information, please contact Dr.Wu (jh.wu@siat.ac.cn).\n");		
		fclose(file);
	}
	return n > 0;
}

bool CTrial::IsExpired() const
{
    bool bResult;
    bool bExpired;
	CTime timeInstalled, timeLastUsed, expiryDate;

    bResult = ReadFile(timeInstalled, timeLastUsed, expiryDate);
	if (!bResult || timeLastUsed < timeInstalled || timeLastUsed > expiryDate)
		return true; // expired

	const CTime currentTime = CTime::GetCurrentTime();
	bResult = WriteFile(timeInstalled, currentTime, expiryDate);

	bExpired = (currentTime < timeInstalled || currentTime > expiryDate);
	return bExpired;
}
