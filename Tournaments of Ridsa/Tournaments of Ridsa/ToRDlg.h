#include "struct.h"

// ToRDlg.h : header file
//

#pragma once


// CToRDlg dialog
class CToRDlg : public CDialogEx
{
// Construction
public:
	CToRDlg(CWnd* pParent = nullptr);	// standard constructor
	CButton* m_Buttons[4];
	CDialogEx* m_pWindow;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TOURNAMENTSOFRIDSA_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void LoadDialogue(UINT nID);
};

class CDlgSimulate : public CDialogEx
{
	// Construction
public:
	CDlgSimulate(CWnd* pParent = nullptr);	// standard constructor
	CComboBox* m_Heroes[8][8];
	CComboBox* m_Trial;
	CButton* m_Simulate = nullptr;
	CEdit* m_Labels[18];
	//CDialogEx* m_pResults;
	int intXScroll = 0;
	int intYScroll = 0;
	CScrollBar* m_VScroll;
	CScrollBar* m_HScroll;


	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHILD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	// Generated message map functions
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual void SetTrial();
	virtual void OnHeroChange(UINT nID);
	virtual void SimulateTrial();
};

// CDlgResults dialog
class CDlgResults : public CDialogEx
{
	// Construction
public:
	CDlgResults(CWnd* pParent = nullptr);	// standard constructor
	//CButton* m_Buttons[4];

	trial trialSelected;
	hero* pTeams;
	string* pStates;
	int* pGold;
	float* pTimes;
	float* pScores;
	float* pFailure;
	CImage imgEmblems[8];
	CBitmap bmpEmblems[8];

	int intTeams;

	CEdit* m_Trial;
	CStatic* m_Emblems[8];
	CEdit* m_Output[8][5];
	CEdit* m_Debug[9][6];



	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

class CDlgHeroes : public CDialogEx
{
	// Construction
public:
	CDlgHeroes(CWnd* pParent = nullptr);	// standard constructor
	CListBox* m_Heroes;
	CButton* m_Import;
	CButton* m_Delete;
	CEdit* m_Labels[26];
	CEdit* m_Properties[23];
	CComboBox* m_Emblem;
	//CDialogEx* m_Load;
	CButton* m_Save;
	bool boolInit = false;
	CCheckListBox* m_Load;
	CButton* m_Accept;
	CButton* m_Cancel;
	CButton* m_SelectAll;
	CButton* m_SelectNone;
	CButton* m_Export;
	CButton* m_New;
	string arrImport[INT_MAXHEROES][25];

	int intXScroll = 0;
	int intYScroll = 0;
	CScrollBar* m_VScroll;
	CScrollBar* m_HScroll;

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHILD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	// Generated message map functions
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual void RefreshList();
	virtual void ChangeHero();
	virtual void LoadImportWindow();
	virtual void DeleteHero();
	//virtual void ChangeEmblem();
	virtual void SaveChanges();
	virtual void ResetState();
	virtual void Import();
	virtual void Export();
	virtual void NewHero();
	virtual void SelectAll();
	virtual void SelectNone();
};

class CDlgTrials : public CDialogEx
{
	// Construction
public:
	CDlgTrials(CWnd* pParent = nullptr);	// standard constructor
	CListBox* m_Trials;
	CButton* m_Import;
	CButton* m_Delete;
	CEdit* m_Labels[48];
	CEdit* m_TextBox[38];
	CComboBox* m_ComboBox[6];
	//CDialogEx* m_Load;
	CButton* m_Save;
	bool boolInit = false;
	CCheckListBox* m_Load;
	CButton* m_Accept;
	CButton* m_Cancel;
	string arrImport[INT_MAXTRIALS + 1][50];
	CButton* m_SelectAll;
	CButton* m_SelectNone;
	CButton* m_Export;
	CButton* m_New;

	int intXScroll = 0;
	int intYScroll = 0;
	CScrollBar* m_VScroll;
	CScrollBar* m_HScroll;


	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHILD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	// Generated message map functions
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual void RefreshList();
	virtual void ChangeTrial();
	virtual void LoadImportWindow();
	virtual void DeleteTrial();
	//virtual void ChangeEmblem();
	virtual void SaveChanges();
	virtual void ResetState();
	virtual void Import();
	virtual void Export();
	virtual void NewTrial();
	virtual void SelectAll();
	virtual void SelectNone();

};

class CDlgSettings : public CDialogEx
{
	// Construction
public:
	CDlgSettings(CWnd* pParent = nullptr);	// standard constructor
	bool boolInit = false;

	CEdit* m_Labels[7];
	CEdit* m_TextBox[3];
	CComboBox* m_ComboBox[4];
	CButton* m_Button;
	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHILD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	// Generated message map functions
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void Apply();
};
