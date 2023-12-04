// TGAtool.h : main header file for the TGATOOL application
//

#if !defined(AFX_TGATOOL_H__4AFA5EC4_58D7_43A5_B990_E7FA641FC46E__INCLUDED_)
#define AFX_TGATOOL_H__4AFA5EC4_58D7_43A5_B990_E7FA641FC46E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTGAtoolApp:
// See TGAtool.cpp for the implementation of this class
//




class CTGAtoolApp : public CWinApp
{
public:
	CTGAtoolApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTGAtoolApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTGAtoolApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TGATOOL_H__4AFA5EC4_58D7_43A5_B990_E7FA641FC46E__INCLUDED_)
