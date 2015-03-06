#pragma once

struct Trial
{
    __time64_t timeInstalled;
    __time64_t timeLastUsed;
};

class CTrial
{
public:
	CTrial(CString strFileName);
	virtual ~CTrial();

public:
	bool WriteFile(int nExpiryYear, int nExpiryMonth, int nExpiryDay) const;
    bool IsExpired() const;

private:
	CString GetDirFromFilePath(CString strFilePath) const;
	CString GetTxtFilePath(CString strTxtFileName) const;
	bool ReadFile(CTime& timeInstalled, CTime& timeLastUsed, CTime& expiryDate) const;
	bool WriteFile(CTime timeInstalled, CTime timeLastUsed, CTime expiryDate) const;

private:
	CString m_strTxtFilePath;
};
