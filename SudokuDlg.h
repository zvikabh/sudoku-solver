// SudokuDlg.h : header file
//

#if !defined(AFX_SUDOKUDLG_H__AC97CC5F_DC91_4569_8FE3_0B5383301550__INCLUDED_)
#define AFX_SUDOKUDLG_H__AC97CC5F_DC91_4569_8FE3_0B5383301550__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSudokuDlg dialog

class CSudokuDlg : public CDialog
{
// Construction
public:
	CSudokuDlg(CWnd* pParent = NULL);	// standard constructor

	int FindLegalSolutions(int vals[][9], int nRow, int nCol, bool bLegal[10], int nLevel);
	enum STATUS { SOLVED, UNSOLVED, CONTRADICTION };
	STATUS Solve(int vals[][9], int nLevel);



// Dialog Data
	//{{AFX_DATA(CSudokuDlg)
	enum { IDD = IDD_SUDOKU_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSudokuDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSudokuDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SUDOKUDLG_H__AC97CC5F_DC91_4569_8FE3_0B5383301550__INCLUDED_)
