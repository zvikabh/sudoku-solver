// SudokuDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Sudoku.h"
#include "SudokuDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSudokuDlg dialog

CSudokuDlg::CSudokuDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSudokuDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSudokuDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSudokuDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSudokuDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSudokuDlg, CDialog)
	//{{AFX_MSG_MAP(CSudokuDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSudokuDlg message handlers

BOOL CSudokuDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	CString s1,s2;
	for(int i=0; i<9; i++) {
		for(int j=0; j<9; j++) {
			s1.Format("%d,%d",i,j);
			GetDlgItem(IDC_EDIT1+i*9+j)->SetWindowText(AfxGetApp()->GetProfileString("Settings",s1,""));
		}
	}
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSudokuDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSudokuDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSudokuDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CSudokuDlg::OnOK() 
{
	int vals[9][9];
	CString str,s2;
	int i,j;

	for(i=0; i<9; i++) {
		for(j=0; j<9; j++) {
			GetDlgItem(IDC_EDIT1 + i*9 + j)->GetWindowText(str);
			s2.Format("%d,%d",i,j);
			AfxGetApp()->WriteProfileString("Settings", s2, str);
			if(sscanf(str, "%d", &vals[i][j]) != 1)
				vals[i][j] = 0;
		}
	}

	BeginWaitCursor();

	STATUS status = Solve(vals, 1);

	EndWaitCursor();

	for(i=0; i<9; i++) {
		for(j=0; j<9; j++) {
			if(vals[i][j]!=0)
				str.Format("%d",vals[i][j]);
			else
				str = "";

			GetDlgItem(IDC_EDIT1 + i*9 + j)->SetWindowText(str);
		}
	}

	switch(status) {
	case SOLVED:
		AfxMessageBox("Solved!");
		break;
	case UNSOLVED:
		AfxMessageBox("Unsolved.");
		break;
	case CONTRADICTION:
		AfxMessageBox("Contradiction!");
		break;
	}
}

int CSudokuDlg::FindLegalSolutions(int vals[][9], int nRow, int nCol, bool bLegal[10], int nLevel)
{
	int i,j;
	int r,c;

	ASSERT(vals[nRow][nCol]==0);

	for(i=1; i<=9; i++)
		bLegal[i]=true;

	// Row
	for(i=0; i<9; i++)
		bLegal[vals[nRow][i]] = false;

	// Column
	for(i=0; i<9; i++)
		bLegal[vals[i][nCol]] = false;

	// Box
	r = nRow - (nRow%3);
	c = nCol - (nCol%3);
	for(i=r; i<r+3; i++)
		for(j=c; j<c+3; j++)
			bLegal[vals[i][j]] = false;

	// Count legal solutions
	int nLegal=0;
	for(i=1; i<=9; i++)
		if(bLegal[i])
			nLegal++;

	return nLegal;
}

CSudokuDlg::STATUS CSudokuDlg::Solve(int vals[][9], int nLevel)
{
	bool bLegal[10], bLegalBest[10];
	int nLegal, nLegalBest;
	int nRowBest, nColBest;
	int row,col;
	int nUnknown;
	CString str;

	const int MAX_LEVEL = 100;

	if(nLevel > MAX_LEVEL)
		return UNSOLVED;

	// Find position with minimum legal possibilities
	nUnknown=0;
	nLegalBest=100;
	for(row=0; row<9; row++) {
		for(col=0; col<9; col++) {
			if(vals[row][col]==0) {
				nUnknown++;
				nLegal = FindLegalSolutions(vals, row, col, bLegal, nLevel);
				if(nLegal < nLegalBest) {
					nLegalBest = nLegal;
					memcpy(bLegalBest, bLegal, sizeof(bool)*10);
					nRowBest = row;
					nColBest = col;
				}
				if(nLegalBest==0)
					break;
			}
		}
		if(nLegalBest==0)
			break;
	}

	// Check end conditions
	if(nUnknown==0)
		return SOLVED;

	if(nLegalBest==0)
		return CONTRADICTION;

	// need to guess & recurse
	bool bContradiction = true;
	for(int i=1; i<=9; i++) {
		if(!bLegalBest[i])
			continue;
		vals[nRowBest][nColBest] = i;
		str.Format("%d",i);
		GetDlgItem(IDC_EDIT1+nRowBest*9+nColBest)->SetWindowText(str);
		GetDlgItem(IDC_EDIT1+nRowBest*9+nColBest)->UpdateWindow();
		STATUS statusChild = Solve(vals, nLevel+1);
		if(statusChild==SOLVED)
			return SOLVED;
		else if(statusChild==UNSOLVED)
			bContradiction=false;
	}

	// Erase the failed hypothesis
	vals[nRowBest][nColBest] = 0;

	if(bContradiction)
		return CONTRADICTION;
	else
		return UNSOLVED;
}
