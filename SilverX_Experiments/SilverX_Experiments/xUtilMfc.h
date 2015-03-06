// xUtilMfc.h: interface for the CxUtilMfc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XUTILMFC_H__07A5C7E2_DE1E_41D2_8192_E82F2D59A4D5__INCLUDED_)
#define AFX_XUTILMFC_H__07A5C7E2_DE1E_41D2_8192_E82F2D59A4D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace UtilMFC
{
	// Replace the current view in the splitter pSplitter
	// at pane (row, col) by a new view of class pViewClass	
	void EkSwitchViewInSplitter(CSplitterWnd* pSplitter, int row, int col, CRuntimeClass* pViewClass );

	void StatusMessage(char* fmt,...);
	void StatusMessageNUM(char* fmt,...);

	void showWin32Console();
	
	int GetFPS();// Returns the frames per second.

								
}  // end namespace UtilMFC

#endif // !defined(AFX_XUTILMFC_H__07A5C7E2_DE1E_41D2_8192_E82F2D59A4D5__INCLUDED_)
