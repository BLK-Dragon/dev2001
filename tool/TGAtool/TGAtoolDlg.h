// TGAtoolDlg.h : header file
//

#if !defined(AFX_TGATOOLDLG_H__5734B93E_C41F_4F11_8B62_43DCA3B9FF11__INCLUDED_)
#define AFX_TGATOOLDLG_H__5734B93E_C41F_4F11_8B62_43DCA3B9FF11__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTGAtoolDlg dialog




class CTGAtoolDlg : public CDialog
{
// Construction
public:
	CTGAtoolDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTGAtoolDlg)
	enum { IDD = IDD_TGATOOL_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTGAtoolDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;


    CListCtrl*  file_list;
    CEdit*      log_window;
    CEdit*      out_name;
    CEdit*      sequence_edit;
    CButton*    checkbox;

    void        log( const char* msg );

    char        path[256];

	// Generated message map functions
	//{{AFX_MSG(CTGAtoolDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnAdd();
	afx_msg void OnDel();
	afx_msg void OnPack();
	afx_msg void OnDevarea();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TGATOOLDLG_H__5734B93E_C41F_4F11_8B62_43DCA3B9FF11__INCLUDED_)
