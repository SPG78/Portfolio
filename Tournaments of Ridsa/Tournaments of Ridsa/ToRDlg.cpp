
// ToRDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "Model.h"
#include "Tournaments of Ridsa.h"
#include "ToRDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CToRDlg dialog



CToRDlg::CToRDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TOURNAMENTSOFRIDSA_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CToRDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CToRDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_CONTROL_RANGE(BN_CLICKED, 1001, 1004, LoadDialogue)
END_MESSAGE_MAP()


// CToRDlg message handlers

BOOL CToRDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	for (int intIter = 0; intIter < 4; intIter = intIter + 1)
	{
		m_Buttons[intIter] = new(CButton);
		m_Buttons[intIter]->Create(L"", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(1, 1, 2, 2), this, 1001 + intIter);
	}
	m_Buttons[0]->SetWindowTextW(L"RUN TRIAL");
	m_Buttons[1]->SetWindowTextW(L"MANAGE PARTY");
	m_Buttons[2]->SetWindowTextW(L"MANAGE TRIALS");
	m_Buttons[3]->SetWindowTextW(L"SETTINGS");
	SetWindowTextW(L"Tournaments of Ridsa");
	SetWindowPos(NULL, 0, 0, 1200, 800, SWP_NOZORDER);

	ifstream settings(experimental::filesystem::current_path().string() + "\\settings.ini");
	if (settings.good())
	{
		string strRead;
		getline(settings, strRead);
		intGoldPerLife = stoi(strRead);
		getline(settings, strRead);
		floatFearImpairance = stof(strRead);
		getline(settings, strRead);
		floatThreshold = stof(strRead);
		getline(settings, strRead);
		intFailureModel = stoi(strRead);
		getline(settings, strRead);
		boolLimitFailures = stoi(strRead);
		getline(settings, strRead);
		boolNormalise = stoi(strRead);
		getline(settings, strRead);
		boolDetailedResults = stoi(strRead);
		getline(settings, strRead);
		strHeroDirectory = strRead;
		getline(settings, strRead);
		strTrialDirectory = strRead;
	}


	arrHeroes[0].name = "None";
	arrHeroes[0].emblem = "None";
	arrHeroes[0].age = 0;
	for (int intIter = 0; intIter < 7; intIter = intIter + 1)
	{
		arrHeroes[0].skills[intIter] = 0;
		arrHeroes[0].fears[intIter] = 0;
	}
	for (int intIter = 7; intIter < 12; intIter = intIter + 1)
	{
		arrHeroes[0].skills[intIter] = 0;
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CToRDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	CRect rectTemp;
	GetWindowRect(rectTemp);
	ScreenToClient(rectTemp);
	int intWidth = rectTemp.right - rectTemp.left;
	int intHeight = rectTemp.bottom - rectTemp.top;
	if (m_Buttons[0] != nullptr)
	{
		for (int intIter = 0; intIter < 4; intIter = intIter + 1)
		{
			m_Buttons[intIter]->SetWindowPos(NULL, intWidth / 42, intHeight * (7 + 4 * intIter) / 28, intWidth * 2 / 7, intHeight / 14, 0);
		}
	}
	if (m_pWindow != nullptr)
	{
		CString strTemp;
		CRect rectWindow;
		rectWindow = CRect(intWidth * 5 / 14, intHeight / 40, intWidth * 41 / 42, intHeight * 9 / 10);
		//ClientToScreen(rectWindow);
		m_pWindow->MoveWindow(rectWindow);
	}


}


void CToRDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CToRDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CToRDlg::LoadDialogue(UINT nID)
{
	if (m_pWindow != nullptr)
	{
		m_pWindow->EndDialog(0); //Doesn't seem to clear RAM?
	}
	switch (nID)
	{
	case 1001: m_pWindow = new CDlgSimulate(this); break; //SIMULATE CHALLENGE
	case 1002: m_pWindow = new CDlgHeroes(this); break; //MANAGE WARRIORS
	case 1003: m_pWindow = new CDlgTrials(this); break; //MANAGE CHALLENGES
	case 1004: m_pWindow = new CDlgSettings(this); break; //SETTINGS
	}
	CRect rectTemp;
	GetClientRect(rectTemp);
	CToRDlg::OnSize(SIZE_RESTORED, rectTemp.right - rectTemp.left, rectTemp.bottom - rectTemp.top);
}


// CDlgSimulate dialog

CDlgSimulate::CDlgSimulate(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHILD, pParent)
{
	Create(IDD_CHILD, pParent);
}

void CDlgSimulate::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlgSimulate, CDialogEx)
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_CBN_SELCHANGE(1005, SetTrial)
	ON_CONTROL_RANGE(CBN_SELCHANGE, 1024, 1087, OnHeroChange)
	ON_BN_CLICKED(1088, SimulateTrial)
END_MESSAGE_MAP()


// CDlgMain message handlers

BOOL CDlgSimulate::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//importWarriors(arrWarriors, "D:\\Samuel\\Documents\\Visual Studio 2012\\Projects\\Raven Model 5\\warriors.csv");
	//importChallenges(arrChallenges, "D:\\Samuel\\Documents\\Visual Studio 2012\\Projects\\Raven Model 5\\challenges.csv");

	string strInput = arrTrials[0].name;
	//wstring strTemp(strInput.begin(), strInput.end());
	//strTemp.c_str();

	//AfxMessageBox(strToLPCTSTR());
	m_Trial = new(CComboBox);
	m_Trial->Create(WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_TABSTOP | CBS_DROPDOWNLIST | WS_VSCROLL, CRect(1, 1, 2, 2), this, 1005);
	for (trial trialIter : arrTrials)
	{
		if (trialIter.name != "")
		{
			m_Trial->AddString(strToLPCTSTR(trialIter.name));
		}
		else
		{
			break;
		}
	}
	m_Trial->SetCurSel(0);

	m_Labels[0] = new(CEdit);
	m_Labels[0]->Create(WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_TABSTOP | ES_CENTER | WS_DISABLED, CRect(1, 1, 2, 2), this, 1006);
	m_Labels[0]->SetWindowTextW(strToLPCTSTR(arrTrials[0].name));

	m_Labels[1] = new(CEdit);
	m_Labels[1]->Create(WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_TABSTOP | ES_RIGHT | WS_DISABLED, CRect(1, 1, 2, 2), this, 1007);
	m_Labels[1]->SetWindowTextW(L"Trial: ");

	for (int intIter = 1; intIter < 9; intIter = intIter + 1)
	{
		m_Labels[intIter + 1] = new(CEdit);
		m_Labels[intIter + 1]->Create(WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_TABSTOP | ES_RIGHT | WS_DISABLED, CRect(1, 1, 2, 2), this, 1007 + intIter);
		CString strTemp;
		strTemp.Format(_T("Team %d"), intIter);
		m_Labels[intIter + 1]->SetWindowTextW(strTemp);
	}

	for (int intIter = 0; intIter < 8; intIter = intIter + 1)
	{
		for (int intSubIter = 0; intSubIter < 8; intSubIter = intSubIter + 1)
		{
			m_Heroes[intIter][intSubIter] = new(CComboBox);
			m_Heroes[intIter][intSubIter]->Create(WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_TABSTOP | CBS_DROPDOWNLIST | WS_DISABLED | WS_VSCROLL, CRect(1, 1, 2, 2), this, 1024 + intIter * 8 + intSubIter);
			for (hero heroIter : arrHeroes)
			{
				if (heroIter.name != "")
				{
					m_Heroes[intIter][intSubIter]->AddString(strToLPCTSTR(heroIter.name));
				}
				else
				{
					break;
				}
			}
			m_Heroes[intIter][intSubIter]->SetCurSel(0);
		}
	}
	m_Heroes[0][0]->ModifyStyle(WS_DISABLED, 0);
	m_Simulate = new(CButton);
	m_Simulate->Create(L"RUN", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(1, 1, 2, 2), this, 1088);

	m_VScroll = new(CScrollBar);
	m_VScroll->Create(WS_CHILD | SBS_VERT | SBS_RIGHTALIGN | WS_VISIBLE, CRect(1, 1, 2, 2), this, 1384);
	m_VScroll->SetScrollRange(0, 1000);
	m_VScroll->EnableScrollBar();
	m_HScroll = new(CScrollBar);
	m_HScroll->Create(WS_CHILD | SBS_HORZ | SBS_BOTTOMALIGN | WS_VISIBLE, CRect(1, 1, 2, 2), this, 1385);
	m_HScroll->SetScrollRange(0, 1000);
	m_HScroll->EnableScrollBar();


	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDlgSimulate::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	CRect rectTemp;
	GetWindowRect(rectTemp);
	//ScreenToClient(rectTemp);
	int intWidth = rectTemp.right - rectTemp.left;
	int intHeight = rectTemp.bottom - rectTemp.top;
	if (m_Simulate != nullptr)
	{
		if (intWidth < INT_SIMWIDTH)
		{
			SCROLLINFO sInfo;
			m_HScroll->GetScrollInfo(&sInfo);
			sInfo.nMax = INT_SIMWIDTH - intWidth + sInfo.nPage - 1;
			sInfo.nPage = INT_SIMWIDTH * INT_SIMWIDTH / intWidth;
			m_HScroll->SetScrollInfo(&sInfo);
			m_HScroll->SetWindowPos(NULL, intWidth * 3 / 20, intHeight * 82 / 84, intWidth * 4 / 5, intHeight / 84, 0);

			intXScroll = intXScroll + INT_SIMWIDTH - intWidth < 0 ? intWidth - INT_SIMWIDTH : intXScroll;
			intWidth = INT_SIMWIDTH;
			m_HScroll->ModifyStyle(0, WS_VISIBLE);
		}
		else
		{
			m_HScroll->ModifyStyle(WS_VISIBLE, 0);
		}
		if (intHeight < INT_SIMHEIGHT)
		{
			SCROLLINFO sInfo;
			m_VScroll->GetScrollInfo(&sInfo);
			sInfo.nMax = INT_SIMHEIGHT - intHeight + sInfo.nPage - 1;
			sInfo.nPage = INT_SIMHEIGHT * INT_SIMHEIGHT / intHeight;
			m_VScroll->SetScrollInfo(&sInfo);
			if (intWidth == INT_SIMWIDTH)
			{
				m_VScroll->SetWindowPos(NULL, (rectTemp.right - rectTemp.left) * 39 / 40, intHeight / 40, (rectTemp.right - rectTemp.left) / 40, intHeight * 19 / 20, 0);
			}
			else
			{
				m_VScroll->SetWindowPos(NULL, intWidth * 39 / 40, intHeight / 40, intWidth / 40, intHeight * 19 / 20, 0);
			}
			intYScroll = intYScroll + INT_SIMHEIGHT - intHeight < 0 ? intHeight - INT_SIMHEIGHT : intYScroll;
			intHeight = INT_SIMHEIGHT;
			m_VScroll->ModifyStyle(0, WS_VISIBLE);
		}
		else
		{
			m_VScroll->ModifyStyle(WS_VISIBLE, 0);
		}

		//m_Simulate->SetWindowPos(NULL, intWidth/4, 1, intWidth/2, intHeight/4, SWP_NOZORDER);
		m_Labels[0]->SetWindowPos(NULL, intWidth / 6 + intXScroll, intHeight / 12 + intYScroll, intWidth * 2 / 3, intHeight / 12, 0);
		m_Labels[1]->SetWindowPos(NULL, intWidth / 20 + intXScroll, intHeight / 6 + intYScroll, intWidth / 5, intHeight / 21, 0);
		m_Trial->SetWindowPos(NULL, intWidth / 4 + intXScroll, intHeight / 6 + intYScroll, intWidth / 4, intHeight / 21, 0);

		for (int intIter = 0; intIter < 8; intIter = intIter + 1)
		{
			m_Labels[intIter + 2]->SetWindowPos(NULL, intWidth / 20 + intXScroll, intHeight * (26 + 5 * intIter) / 84 + intYScroll, intWidth / 10, intHeight * 5 / 84, 0);
			for (int intSubIter = 0; intSubIter < 8; intSubIter = intSubIter + 1)
			{
				m_Heroes[intIter][intSubIter]->SetWindowPos(NULL, intWidth * (3 + 2 * intSubIter) / 20 + intXScroll, intHeight * (26 + 5 * intIter) / 84 + intYScroll, intWidth / 10, intHeight * 5 / 84, 0);
			}
		}

		m_Simulate->SetWindowPos(NULL, intWidth / 4 + intXScroll, intHeight * 5 / 6 + intYScroll, intWidth * 1 / 2, intHeight / 12, 0);
	}

	/*if (m_Buttons[0] != nullptr)
	{
		for (int intIter = 0; intIter < 4; intIter = intIter + 1)
		{

		}
	}
	if (m_pWindow != nullptr)
	{
		CRect rectWindow;
		rectWindow = CRect(intWidth * 5 / 14, intHeight / 42, intWidth * 41 / 42, intHeight * 41 / 42);
		ClientToScreen(rectWindow);
		m_pWindow->MoveWindow(rectWindow);
	}*/
}

void CDlgSimulate::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
	//intYScroll = pScrollBar->GetScrollPos();

	updateScroll(nSBCode, nPos, pScrollBar);
	intYScroll = pScrollBar->GetScrollPos() * -1;
	CRect rectTemp;
	GetClientRect(rectTemp);
	OnSize(SIZE_RESTORED, rectTemp.right - rectTemp.left, rectTemp.bottom - rectTemp.top);

	//this->ScrollWindowEx(0, 10, NULL, NULL, NULL, NULL, SW_SCROLLCHILDREN);
}

void CDlgSimulate::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
	//intYScroll = pScrollBar->GetScrollPos();

	updateScroll(nSBCode, nPos, pScrollBar);
	intXScroll = pScrollBar->GetScrollPos() * -1;
	CRect rectTemp;
	GetClientRect(rectTemp);
	OnSize(SIZE_RESTORED, rectTemp.right - rectTemp.left, rectTemp.bottom - rectTemp.top);

	//this->ScrollWindowEx(0, 10, NULL, NULL, NULL, NULL, SW_SCROLLCHILDREN);
}

void CDlgSimulate::SetTrial()
{
	CString strTemp;
	m_Trial->GetWindowTextW(strTemp);
	m_Labels[0]->SetWindowTextW(strTemp);

	for (int intIter = 0; intIter < 8; intIter = intIter + 1)
	{
		if (m_Heroes[intIter][0]->GetCurSel() == 0)
		{
			m_Heroes[intIter][0]->ModifyStyle(WS_DISABLED, 0);
			break;
		}
		for (int intSubIter = 1; intSubIter < 8; intSubIter = intSubIter + 1)
		{
			if (m_Heroes[intIter][intSubIter]->GetCurSel() == 0)
			{
				m_Heroes[intIter][intSubIter]->ModifyStyle(WS_DISABLED, 0);
				break;
			}
		}
	}

	for (int intIter = arrTrials[m_Trial->GetCurSel()].numberOfTeams; intIter < 8; intIter = intIter + 1)
	{
		for (int intSubIter = 0; intSubIter < 8; intSubIter = intSubIter + 1)
		{
			m_Heroes[intIter][intSubIter]->SetCurSel(0);
			m_Heroes[intIter][intSubIter]->ModifyStyle(0, WS_DISABLED);
		}
	}

	for (int intIter = 0; intIter < 8; intIter = intIter + 1)
	{
		for (int intSubIter = arrTrials[m_Trial->GetCurSel()].heroesPerTeam; intSubIter < 8; intSubIter = intSubIter + 1)
		{
			m_Heroes[intIter][intSubIter]->SetCurSel(0);
			m_Heroes[intIter][intSubIter]->ModifyStyle(0, WS_DISABLED);
		}
	}
}

void CDlgSimulate::SimulateTrial()
{
	hero arrTeams[8][8];


	for (int intIter = 0; intIter < 8; intIter = intIter + 1)
	{
		for (int intSubIter = 0; intSubIter < 8; intSubIter = intSubIter + 1)
		{
			arrTeams[intIter][intSubIter] = arrHeroes[m_Heroes[intIter][intSubIter]->GetCurSel()];
		}
	}
	//runChallenge(arrChallenges[m_Challenge->GetCurSel()], arrTeams);

	trial trialSelected = arrTrials[m_Trial->GetCurSel()];
	int intTeams = 8;
	int intHeroesPerTeam = 8;
	float arrScores[8][4]; // Overall score, Virtues, Fears, Luck
	float arrRelative[8]; // Scores relative to average
	float arrFailure[8]; // Risks of failure
	string arrStates[8]; // Challenge outcomes (OK, LOSE, etc.)
	int arrRanks[8];
	int intMaxRank; // Number of warriors guarenteed not to lose a life
	int intLastPlace; // Final place in a race that does not lose a life
	int arrGold[8];
	float arrTimes[8];


	for (int intIter = 0; intIter < 8; intIter = intIter + 1)
	{
		if (arrTeams[intIter][0].name == "None")
		{
			intTeams = intIter;
			break;
		}
	}

	for (int intIter = 0; intIter < 8; intIter = intIter + 1)
	{
		if (arrTeams[0][intIter].name == "None")
		{
			intHeroesPerTeam = intIter;
			break;
		}
	}

	intTeams = (intTeams > trialSelected.numberOfTeams) ? trialSelected.numberOfTeams : intTeams;
	intHeroesPerTeam = (intHeroesPerTeam > trialSelected.heroesPerTeam) ? trialSelected.heroesPerTeam : intHeroesPerTeam;


	if (trialSelected.notFirstCosts)
	{
		intLastPlace = 1;
	}
	else if (trialSelected.losingCosts)
	{
		intLastPlace = intTeams - 1;
	}
	else
	{
		intLastPlace = intTeams;
	}

	int intFail;
	intFail = int(trialSelected.maximumFailures[0]) - 48;
	if (intFail >= 0 && intFail <= 9) // If the first character of chalSelected.maximumFailures is an integer
	{
		intMaxRank = intTeams - intFail;
	}
	else
	{
		if (trialSelected.maximumFailures == "few")
		{
			intMaxRank = ceil(intTeams * 3 / 4);
		}
		else if (trialSelected.maximumFailures == "many")
		{
			intMaxRank = 1;
		}
		else
		{
			intMaxRank = 0;
		}
	}

	// Calculating proportional virtue relevance
	float arrSkillProp[12];
	float floatSum;
	floatSum = 0;
	for (int intIter = 0; intIter < 12; intIter = intIter + 1)
	{
		floatSum = floatSum + abs(trialSelected.skills[intIter]);
	}
	if (floatSum == 0)
	{
		for (int intIter = 0; intIter < 12; intIter = intIter + 1)
		{
			arrSkillProp[intIter] = 0;
		}
	}
	else
	{
		for (int intIter = 0; intIter < 12; intIter = intIter + 1)
		{
			arrSkillProp[intIter] = trialSelected.skills[intIter] / floatSum;
		}
	}

	// Calculating proportional fears relevance
	float arrFearProp[7];
	floatSum = 0;
	for (int intIter = 0; intIter < 7; intIter = intIter + 1)
	{
		floatSum = floatSum + trialSelected.fears[intIter];
	}
	if (floatSum == 0)
	{
		for (int intIter = 0; intIter < 7; intIter = intIter + 1)
		{
			arrFearProp[intIter] = 0;
		}
	}
	else
	{
		for (int intIter = 0; intIter < 7; intIter = intIter + 1)
		{
			arrFearProp[intIter] = trialSelected.fears[intIter] / floatSum;
		}
	}

	// Calculating team scores
	// default_random_engine generator (time(0));
	normal_distribution<float> distribution(1, trialSelected.luck / 2);
	floatSum = 0;

	for (int intIter = 0; intIter < intTeams; intIter = intIter + 1)
	{
		arrScores[intIter][1] = skills(arrSkillProp, arrTeams[intIter], intHeroesPerTeam);
		//cout << "Skill score: " << arrScores[intIter][1] << '\n';
		arrScores[intIter][2] = fears(arrFearProp, arrTeams[intIter], intHeroesPerTeam);
		//cout << "Fear score: " << arrScores[intIter][2] << '\n';
		arrScores[intIter][3] = distribution(generator);
		//cout << "   Luck changes score: " << arrScores[intIter][3] << '\n';
		//cout << "Luck score: " << arrScores[intIter][3] << '\n';
		arrScores[intIter][0] = arrScores[intIter][1] * arrScores[intIter][3] * (1 - arrScores[intIter][2] * floatFearImpairance);
		//cout << "Team " << intIter << " score: " << arrScores[intIter][0] << "\n\n";
		floatSum = floatSum + arrScores[intIter][0];
	}
	floatSum = floatSum / intTeams;

	// Failure percentages
	float floatScale = 3 / (artanh(1 - 2 * trialSelected.medFailureRisk) - artanh(1 - 2 * trialSelected.badFailureRisk));
	float floatOffset = 7 - floatScale * artanh(1 - 2 * trialSelected.medFailureRisk);

	for (int intIter = 0; intIter < intTeams; intIter = intIter + 1)
	{
		arrRelative[intIter] = arrScores[intIter][0] / floatSum - 1;
		if (intFailureModel == 1)
		{
			if (arrRelative[intIter] < 0)
			{
				arrFailure[intIter] = trialSelected.medFailureRisk - arrRelative[intIter] * (1 - trialSelected.medFailureRisk);
			}
			else if (arrRelative[intIter] > 0)
			{
				arrFailure[intIter] = trialSelected.medFailureRisk - arrRelative[intIter] * trialSelected.medFailureRisk;
			}
			else
			{
				arrFailure[intIter] = trialSelected.medFailureRisk;
			}
		}
		else if (intFailureModel == 2)
		{
			// cout << arrScores[intIter][0] << '\n' << floatOffset << '\n' << floatScale << '\n' << tanh((arrScores[intIter][0]-floatOffset)/floatScale) << '\n';
			arrFailure[intIter] = (1 - tanh((arrScores[intIter][0] - floatOffset) / floatScale)) / 2;
		}
		else
		{
			arrFailure[intIter] = 0;
		}
		//cout << "Team " << intIter << " has a failure probability of " << arrFailure[intIter] << '\n';
	}

	float arrSort[8];
	for (int intIter = 0; intIter < intTeams; intIter = intIter + 1)
	{
		arrSort[intIter] = arrScores[intIter][0];
	}
	bubbleSort(arrSort, false);
	for (int intIter = 0; intIter < intTeams; intIter = intIter + 1)
	{
		for (int intRank = 0; intRank < intTeams; intRank = intRank + 1)
		{
			if (arrScores[intIter][0] == arrSort[intRank])
			{
				arrRanks[intIter] = intRank + 1;
				break;
			}
		}
	}

	bool boolEndangered;

	for (int intIter = 0; intIter < intTeams; intIter = intIter + 1)
	{
		if (boolLimitFailures)
		{
			if (arrFailure[intIter] > 0 && arrRanks[intIter] > intMaxRank)
			{
				boolEndangered = true;
			}
			else
			{
				boolEndangered = false;
			}
		}
		else
		{
			boolEndangered = true;
		}

		float floatMin = 0;
		float floatMax = 0;
		float arrLeadership[8] = { 0,0,0,0,0,0,0,0 };
		floatSum = 0;
		for (int intHero = 0; intHero < intHeroesPerTeam; intHero = intHero + 1)
		{
			floatSum = floatSum + arrTeams[intIter][intHero].skills[4];
			floatMax = (floatMax < arrTeams[intIter][intHero].skills[11]) ? arrTeams[intIter][intHero].skills[11] : floatMax;
			floatMin = (floatMin > arrTeams[intIter][intHero].skills[11]) ? arrTeams[intIter][intHero].skills[11] : floatMin;
			arrLeadership[intHero] = arrTeams[intIter][intHero].skills[11];
		}
		floatSum = floatSum / intHeroesPerTeam;
		bubbleSort(arrLeadership, false);
		int intMedian;
		intMedian = int(intHeroesPerTeam * 0.49);
		if (floatMin >= 7 && arrLeadership[intMedian] > 8)
		{
			floatMax = arrLeadership[intMedian];
		}
		if (refusal(arrScores[intIter][2], floatSum, floatMax))
		{

			arrStates[intIter] = outcomes(arrFailure[intIter], boolEndangered, trialSelected.race, arrRanks[intIter] > intLastPlace, trialSelected.failureDescription);
		}
		else
		{
			arrStates[intIter] = "REFUSED";
		}

		arrGold[intIter] = rings(trialSelected, arrStates[intIter], arrRanks[intIter], arrRelative[intIter]);
		//cout << "Team " << intIter << " win " << arrRings[intIter] << " rings\n";
	}
	// Relegating failed teams in ranks
	for (int intIter = 0; intIter < intTeams; intIter = intIter + 1)
	{
		if (arrStates[intIter].substr(0, 4) == "LOSE" || arrStates[intIter] == "REFUSED")
		{
			for (int intSubIter = 0; intSubIter < intTeams; intSubIter = intSubIter + 1)
			{
				if (arrRanks[intSubIter] > arrRanks[intIter])
				{
					arrRanks[intSubIter] = arrRanks[intSubIter] - 1;
				}
			}
			arrRanks[intIter] = intTeams;
		}
	}
	if (!trialSelected.race && trialSelected.optionalGold)
	{
		float arrGoldSort[8];
		for (int intIter = 0; intIter < 8; intIter = intIter + 1)
		{
			arrGoldSort[intIter] = float(arrGold[intIter]);
		}
		bubbleSort(arrGoldSort, false);
		for (int intIter = 0; intIter < intTeams; intIter = intIter + 1)
		{
			arrGold[intIter] = int(arrGoldSort[arrRanks[intIter] - 1]);
		}
	}

	for (int intIter = 0; intIter < intTeams; intIter = intIter + 1)
	{
		arrTimes[intIter] = raceTime(trialSelected, arrScores[intIter][0]);
	}

	for (int intIter = 0; intIter < intTeams; intIter = intIter + 1)
	{
		for (int intSubIter = 0; intSubIter < intHeroesPerTeam; intSubIter = intSubIter + 1)
		{
			arrHeroes[m_Heroes[intIter][intSubIter]->GetCurSel()].gold = arrHeroes[m_Heroes[intIter][intSubIter]->GetCurSel()].gold + arrGold[intIter];
			arrTeams[intIter][intSubIter].gold = arrTeams[intIter][intSubIter].gold + arrGold[intIter];
			if (arrStates[intIter].substr(0, 4) == "LOSE" || arrStates[intIter] == "REFUSED")
			{
				arrHeroes[m_Heroes[intIter][intSubIter]->GetCurSel()].gold = arrHeroes[m_Heroes[intIter][intSubIter]->GetCurSel()].gold - intGoldPerLife;
				arrTeams[intIter][intSubIter].gold = arrTeams[intIter][intSubIter].gold - intGoldPerLife;
			}
		}
	}


	//OUTPUT
	/*
	cout << trialSelected.name << '\n';
	for (int intIter = 0; intIter < intTeams; intIter = intIter + 1)
	{
		for (int intSubIter = 0; intSubIter < intHeroesPerTeam; intSubIter = intSubIter + 1)
		{
			cout << arrTeams[intIter][intSubIter].name << ' ';
		}
		cout << '\n';
		cout << "OUTCOME: " << arrStates[intIter] << '\n';
		cout << "GOLD: " << arrRings[intIter] << '\n';
		cout << "TIME: " << arrTimes[intIter] << "\n\n";
	}
	*/

	//END runTrial

	//m_pResults = new CDlgResults(this);
	CDlgResults hWND;

	hWND.trialSelected = trialSelected;
	hWND.pTeams = &arrTeams[0][0];
	hWND.pStates = &arrStates[0];
	hWND.pGold = &arrGold[0];
	hWND.pTimes = &arrTimes[0];
	hWND.pScores = &arrScores[0][0];
	hWND.pFailure = &arrFailure[0];

	hWND.DoModal();

}

void CDlgSimulate::OnHeroChange(UINT nID)
{
	int intTeam;
	int intPos;
	intTeam = (nID - 1024) / 8;
	intPos = nID % 8;

	if (m_Heroes[intTeam][intPos]->GetCurSel() == 0)
	{
		if (intPos == 0 && intTeam < 7)
		{
			for (int intIter = intTeam + 1; intIter < 8; intIter = intIter + 1)
			{
				for (int intSubIter = 0; intSubIter < 8; intSubIter = intSubIter + 1)
				{
					m_Heroes[intIter][intSubIter]->SetCurSel(0);
					m_Heroes[intIter][intSubIter]->ModifyStyle(0, WS_DISABLED);
				}
			}
		}
		if (intPos < 7)
		{
			for (int intIter = intPos + 1; intIter < 8; intIter = intIter + 1)
			{
				m_Heroes[intTeam][intIter]->SetCurSel(0);
				m_Heroes[intTeam][intIter]->ModifyStyle(0, WS_DISABLED);
			}
		}
	}
	else
	{
		if (intPos == 0 && intTeam < arrTrials[m_Trial->GetCurSel()].numberOfTeams - 1)
		{
			m_Heroes[intTeam + 1][intPos]->ModifyStyle(WS_DISABLED, 0);
		}
		if (intPos < arrTrials[m_Trial->GetCurSel()].heroesPerTeam - 1)
		{
			m_Heroes[intTeam][intPos + 1]->ModifyStyle(WS_DISABLED, 0);
		}
	}
}

// CDlgResults dialog

CDlgResults::CDlgResults(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TOURNAMENTSOFRIDSA_DIALOG, pParent)
{
	//Create(IDD_CHILD, pParent);
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDlgResults::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlgResults, CDialogEx)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDlgMain message handlers

BOOL CDlgResults::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_Trial = new(CEdit);
	m_Trial->Create(WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_TABSTOP | ES_CENTER | WS_DISABLED, CRect(1, 1, 2, 2), this, 1089);
	m_Trial->SetWindowTextW(strToLPCTSTR(trialSelected.name));

	string strDirectory = experimental::filesystem::current_path().string() + "\\emblems\\";
	string strPath;

	intTeams = trialSelected.numberOfTeams;
	for (int intIter = 0; intIter < trialSelected.numberOfTeams; intIter = intIter + 1)
	{
		if ((pTeams + 8 * intIter)->name == "None")
		{
			intTeams = intIter;
			break;
		}
	}

	if (trialSelected.time[1] > 0) // If race, sort
	{
		bool boolLoop = true;
		hero heroHold;
		string strState;
		int intGold;
		float floatTime;
		float floatScore;
		float floatFailure;


		while (boolLoop)
		{
			boolLoop = false;
			for (int intIter = 0; intIter < intTeams - 1; intIter = intIter + 1)
			{
				if (*(pTimes + intIter) > * (pTimes + intIter + 1))
				{
					boolLoop = true;
					for (int intSubIter = 0; intSubIter < 8; intSubIter = intSubIter + 1)
					{
						heroHold = *(pTeams + 8 * intIter + intSubIter);
						*(pTeams + 8 * intIter + intSubIter) = *(pTeams + 8 * intIter + intSubIter + 8);
						*(pTeams + 8 * intIter + intSubIter + 8) = heroHold;
					}
					for (int intSubIter = 0; intSubIter < 4; intSubIter = intSubIter + 1)
					{
						floatScore = *(pScores + 4 * intIter + intSubIter);
						*(pScores + 4 * intIter + intSubIter) = *(pScores + 4 * intIter + intSubIter + 4);
						*(pScores + 4 * intIter + intSubIter + 4) = floatScore;
					}
					strState = *(pStates + intIter);
					*(pStates + intIter) = *(pStates + intIter + 1);
					*(pStates + intIter + 1) = strState;

					intGold = *(pGold + intIter);
					*(pGold + intIter) = *(pGold + intIter + 1);
					*(pGold + intIter + 1) = intGold;

					floatTime = *(pTimes + intIter);
					*(pTimes + intIter) = *(pTimes + intIter + 1);
					*(pTimes + intIter + 1) = floatTime;

					floatFailure = *(pFailure + intIter);
					*(pFailure + intIter) = *(pFailure + intIter + 1);
					*(pFailure + intIter + 1) = floatFailure;
				}
			}
		}
	}



	for (int intIter = 0; intIter < intTeams; intIter = intIter + 1)
	{
		CString strOut;
		string strTemp = (pTeams + intIter * 8)->name;
		int intSubIter = 1;
		while ((pTeams + intIter * 8 + intSubIter)->name != "None")
		{
			strTemp = strTemp + ", " + (pTeams + intIter * 8 + intSubIter)->name;
			intSubIter = intSubIter + 1;
		}
		m_Output[intIter][0] = new(CEdit);
		m_Output[intIter][0]->Create(WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_TABSTOP | ES_CENTER | WS_DISABLED, CRect(1, 1, 2, 2), this, 1090 + 5 * intIter);
		m_Output[intIter][0]->SetWindowTextW(strToLPCTSTR(strTemp));
		m_Output[intIter][1] = new(CEdit);
		m_Output[intIter][1]->Create(WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_TABSTOP | ES_CENTER | WS_DISABLED, CRect(1, 1, 2, 2), this, 1091 + 5 * intIter);
		m_Output[intIter][1]->SetWindowTextW(strToLPCTSTR(*(pStates + intIter)));

		if (boolDetailedResults)
		{
			m_Debug[intIter + 1][0] = new(CEdit);
			m_Debug[intIter + 1][0]->Create(WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_TABSTOP | ES_CENTER | WS_DISABLED, CRect(1, 1, 2, 2), this, 1144 + 6 * intIter);
			m_Debug[intIter + 1][0]->SetWindowTextW(strToLPCTSTR(strTemp));
			for (int intSubIter = 0; intSubIter < 4; intSubIter = intSubIter + 1)
			{
				m_Debug[intIter + 1][intSubIter + 1] = new(CEdit);
				m_Debug[intIter + 1][intSubIter + 1]->Create(WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_TABSTOP | ES_CENTER | WS_DISABLED, CRect(1, 1, 2, 2), this, 1145 + 6 * intIter + intSubIter);
				strOut.Format(_T("%f"), *(pScores + 4 * intIter + intSubIter));
				m_Debug[intIter + 1][intSubIter + 1]->SetWindowTextW(strOut);
			}
			m_Debug[intIter + 1][5] = new(CEdit);
			m_Debug[intIter + 1][5]->Create(WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_TABSTOP | ES_CENTER | WS_DISABLED, CRect(1, 1, 2, 2), this, 1149 + 6 * intIter);
			strOut.Format(_T("%f"), *(pFailure + intIter));
			m_Debug[intIter + 1][5]->SetWindowTextW(strOut);

		}

		m_Output[intIter][2] = new(CEdit);
		m_Output[intIter][2]->Create(WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_TABSTOP | ES_CENTER | WS_DISABLED, CRect(1, 1, 2, 2), this, 1092 + 5 * intIter);
		strOut.Format(_T("%d"), *(pGold + intIter));
		if (*(pGold + intIter) == 0)
		{
			m_Output[intIter][2]->SetWindowTextW(strOut + " gold");
		}
		else
		{
			m_Output[intIter][2]->SetWindowTextW('+' + strOut + " gold");
		}
		m_Output[intIter][3] = new(CEdit);
		m_Output[intIter][3]->Create(WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_TABSTOP | ES_CENTER | WS_DISABLED, CRect(1, 1, 2, 2), this, 1093 + 5 * intIter);
		strTemp = "";
		for (int intHero = 0; intHero < intSubIter; intHero = intHero + 1)
		{
			strTemp = strTemp + to_string((pTeams + intIter * 8 + intHero)->gold / intGoldPerLife);
			strTemp = strTemp + ":";
			strTemp = strTemp + to_string((pTeams + intIter * 8 + intHero)->gold % intGoldPerLife);
			if (intSubIter - intHero > 1)
			{
				strTemp = strTemp + " / ";
			}
		}
		m_Output[intIter][3]->SetWindowTextW(strToLPCTSTR(strTemp));

		if (trialSelected.time[1] > 0)
		{
			m_Output[intIter][4] = new(CEdit);
			m_Output[intIter][4]->Create(WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_TABSTOP | ES_CENTER | WS_DISABLED, CRect(1, 1, 2, 2), this, 1094 + 5 * intIter);
			strOut.Format(_T("%.3f"), *(pTimes + intIter));
			m_Output[intIter][4]->SetWindowTextW(strOut + 's');
		}

		if ((pTeams + intIter * 8)->emblem != "None")
		{
			strPath = strDirectory + (pTeams + intIter * 8)->emblem + ".png";
			imgEmblems[intIter].Load(strToLPCTSTR(strPath));
			CDC* screenDC = GetDC();
			CDC* pMDC = new CDC;
			pMDC->CreateCompatibleDC(screenDC);
			CBitmap* pb = new CBitmap;
			pb->CreateCompatibleBitmap(screenDC, imgEmblems[intIter].GetWidth(), imgEmblems[intIter].GetHeight());
			CBitmap* pob = pMDC->SelectObject(pb);
			imgEmblems[intIter].StretchBlt(pMDC->m_hDC, 0, 0, imgEmblems[intIter].GetWidth(), imgEmblems[intIter].GetHeight(), 0, 0, imgEmblems[intIter].GetWidth(), imgEmblems[intIter].GetHeight(), SRCCOPY);
			pMDC->SelectObject(pob);
			CImage imgEmblemTmp;
			imgEmblemTmp.Attach((HBITMAP)(*pb));
			bmpEmblems[intIter].Attach(imgEmblemTmp.Detach());
			ReleaseDC(screenDC);

			m_Emblems[intIter] = new(CStatic);
			m_Emblems[intIter]->Create(strToLPCTSTR((pTeams + intIter * 8)->emblem), WS_CHILD | WS_VISIBLE | SS_BITMAP | SS_REALSIZECONTROL, CRect(1, 1, 2, 2), this, 1130 + intIter);
			m_Emblems[intIter]->SetBitmap(bmpEmblems[intIter]);
		}
		else
		{
			m_Emblems[intIter] = new(CStatic);
			m_Emblems[intIter]->Create(strToLPCTSTR((pTeams + intIter * 8)->emblem), WS_CHILD | WS_VISIBLE | SS_BITMAP | SS_REALSIZECONTROL, CRect(1, 1, 2, 2), this, 1130 + intIter);
		}

	}

	if (boolDetailedResults)
	{
		m_Debug[0][0] = new(CEdit);
		m_Debug[0][0]->Create(WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_TABSTOP | ES_CENTER | WS_DISABLED, CRect(1, 1, 2, 2), this, 1138);
		m_Debug[0][0]->SetWindowTextW(L"Debug table");
		m_Debug[0][1] = new(CEdit);
		m_Debug[0][1]->Create(WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_TABSTOP | ES_CENTER | WS_DISABLED, CRect(1, 1, 2, 2), this, 1139);
		m_Debug[0][1]->SetWindowTextW(L"Score");
		m_Debug[0][2] = new(CEdit);
		m_Debug[0][2]->Create(WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_TABSTOP | ES_CENTER | WS_DISABLED, CRect(1, 1, 2, 2), this, 1140);
		m_Debug[0][2]->SetWindowTextW(L"Skills");
		m_Debug[0][3] = new(CEdit);
		m_Debug[0][3]->Create(WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_TABSTOP | ES_CENTER | WS_DISABLED, CRect(1, 1, 2, 2), this, 1141);
		m_Debug[0][3]->SetWindowTextW(L"Fear");
		m_Debug[0][4] = new(CEdit);
		m_Debug[0][4]->Create(WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_TABSTOP | ES_CENTER | WS_DISABLED, CRect(1, 1, 2, 2), this, 1142);
		m_Debug[0][4]->SetWindowTextW(L"Luck");
		m_Debug[0][5] = new(CEdit);
		m_Debug[0][5]->Create(WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_TABSTOP | ES_CENTER | WS_DISABLED, CRect(1, 1, 2, 2), this, 1143);
		m_Debug[0][5]->SetWindowTextW(L"Failure Prob.");

	}
	SetWindowTextW(L"Results");
	SetWindowPos(NULL, 0, 0, 800, 600, SWP_NOZORDER);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDlgResults::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	CRect rectTemp;
	GetWindowRect(rectTemp);
	//ScreenToClient(rectTemp);
	int intWidth = rectTemp.right - rectTemp.left;
	int intHeight = rectTemp.bottom - rectTemp.top;
	if (m_Trial != nullptr)
	{
		if (boolDetailedResults)
		{
			m_Trial->SetWindowPos(NULL, intWidth / 8, intHeight / (6 * intTeams + 15), intWidth * 3 / 4, intHeight * 4 / (6 * intTeams + 15), 0);
			if (trialSelected.time[1] == 0)
			{
				for (int intIter = 0; intIter < intTeams; intIter = intIter + 1)
				{
					m_Emblems[intIter]->SetWindowPos(NULL, intWidth * 3 / 32 - intHeight * 2 / (6 * intTeams + 15), intHeight * (intIter + 2) / (2 * intTeams + 5), intHeight * 2 / (6 * intTeams + 9), intHeight * 2 / (6 * intTeams + 15), 0);
					m_Output[intIter][0]->SetWindowPos(NULL, intWidth / 8, intHeight * (intIter + 2) / (2 * intTeams + 5), intWidth * 9 / 32, intHeight * 2 / (6 * intTeams + 15), 0);
					m_Output[intIter][1]->SetWindowPos(NULL, intWidth * 7 / 16, intHeight * (intIter + 2) / (2 * intTeams + 5), intWidth * 5 / 32, intHeight * 2 / (6 * intTeams + 15), 0);
					m_Output[intIter][2]->SetWindowPos(NULL, intWidth * 5 / 8, intHeight * (intIter + 2) / (2 * intTeams + 5), intWidth / 8, intHeight * 2 / (6 * intTeams + 15), 0);
					m_Output[intIter][3]->SetWindowPos(NULL, intWidth * 25 / 32, intHeight * (intIter + 2) / (2 * intTeams + 5), intWidth * 3 / 32, intHeight * 2 / (6 * intTeams + 15), 0);
				}
			}
			else
			{
				for (int intIter = 0; intIter < intTeams; intIter = intIter + 1)
				{
					m_Emblems[intIter]->SetWindowPos(NULL, intWidth * 3 / 32 - intHeight * 2 / (6 * intTeams + 15), intHeight * (intIter + 2) / (2 * intTeams + 5), intHeight * 2 / (6 * intTeams + 9), intHeight * 2 / (6 * intTeams + 15), 0);
					m_Output[intIter][0]->SetWindowPos(NULL, intWidth / 8, intHeight * (intIter + 2) / (2 * intTeams + 5), intWidth / 4, intHeight * 2 / (6 * intTeams + 15), 0);
					m_Output[intIter][1]->SetWindowPos(NULL, intWidth * 13 / 32, intHeight * (intIter + 2) / (2 * intTeams + 5), intWidth / 8, intHeight * 2 / (6 * intTeams + 15), 0);
					m_Output[intIter][2]->SetWindowPos(NULL, intWidth * 9 / 16, intHeight * (intIter + 2) / (2 * intTeams + 5), intWidth * 3 / 32, intHeight * 2 / (6 * intTeams + 15), 0);
					m_Output[intIter][3]->SetWindowPos(NULL, intWidth * 11 / 16, intHeight * (intIter + 2) / (2 * intTeams + 5), intWidth / 16, intHeight * 2 / (6 * intTeams + 15), 0);
					m_Output[intIter][4]->SetWindowPos(NULL, intWidth * 25 / 32, intHeight * (intIter + 2) / (2 * intTeams + 5), intWidth * 3 / 32, intHeight * 2 / (6 * intTeams + 15), 0);
				}
			}
			for (int intIter = 0; intIter < intTeams + 1; intIter = intIter + 1)
			{
				m_Debug[intIter][0]->SetWindowPos(NULL, intWidth / 8, intHeight * (intIter + intTeams + 2) / (2 * intTeams + 5), intWidth * 17 / 96, intHeight * 2 / (6 * intTeams + 15), 0);
				m_Debug[intIter][1]->SetWindowPos(NULL, intWidth * 31 / 96, intHeight * (intIter + intTeams + 2) / (2 * intTeams + 5), intWidth * 3 / 32, intHeight * 2 / (6 * intTeams + 15), 0);
				m_Debug[intIter][2]->SetWindowPos(NULL, intWidth * 7 / 16, intHeight * (intIter + intTeams + 2) / (2 * intTeams + 5), intWidth * 3 / 32, intHeight * 2 / (6 * intTeams + 15), 0);
				m_Debug[intIter][3]->SetWindowPos(NULL, intWidth * 53 / 96, intHeight * (intIter + intTeams + 2) / (2 * intTeams + 5), intWidth * 3 / 32, intHeight * 2 / (6 * intTeams + 15), 0);
				m_Debug[intIter][4]->SetWindowPos(NULL, intWidth * 2 / 3, intHeight * (intIter + intTeams + 2) / (2 * intTeams + 5), intWidth * 3 / 32, intHeight * 2 / (6 * intTeams + 15), 0);
				m_Debug[intIter][5]->SetWindowPos(NULL, intWidth * 25 / 32, intHeight * (intIter + intTeams + 2) / (2 * intTeams + 5), intWidth * 3 / 32, intHeight * 2 / (6 * intTeams + 15), 0);
			}
		}
		else
		{
			m_Trial->SetWindowPos(NULL, intWidth / 8, intHeight / (6 * intTeams + 6), intWidth * 3 / 4, intHeight * 2 / (3 * intTeams + 3), 0);
			if (trialSelected.time[1] == 0)
			{
				for (int intIter = 0; intIter < intTeams; intIter = intIter + 1)
				{
					m_Emblems[intIter]->SetWindowPos(NULL, intWidth * 3 / 32 - intHeight / (3 * intTeams + 3), intHeight * (12 * intIter + 13) / (12 * intTeams + 12), intHeight / (3 * intTeams + 3), intHeight / (3 * intTeams + 3), 0);
					m_Output[intIter][0]->SetWindowPos(NULL, intWidth / 8, intHeight * (12 * intIter + 13) / (12 * intTeams + 12), intWidth * 9 / 32, intHeight / (3 * intTeams + 3), 0);
					m_Output[intIter][1]->SetWindowPos(NULL, intWidth * 7 / 16, intHeight * (12 * intIter + 13) / (12 * intTeams + 12), intWidth * 5 / 32, intHeight / (3 * intTeams + 3), 0);
					m_Output[intIter][2]->SetWindowPos(NULL, intWidth * 5 / 8, intHeight * (12 * intIter + 13) / (12 * intTeams + 12), intWidth / 8, intHeight / (3 * intTeams + 3), 0);
					m_Output[intIter][3]->SetWindowPos(NULL, intWidth * 25 / 32, intHeight * (12 * intIter + 13) / (12 * intTeams + 12), intWidth * 3 / 32, intHeight / (3 * intTeams + 3), 0);
				}
			}
			else
			{
				for (int intIter = 0; intIter < intTeams; intIter = intIter + 1)
				{
					m_Emblems[intIter]->SetWindowPos(NULL, intWidth * 3 / 32 - intHeight / (3 * intTeams + 3), intHeight * (12 * intIter + 13) / (12 * intTeams + 12), intHeight / (3 * intTeams + 3), intHeight / (3 * intTeams + 3), 0);
					m_Output[intIter][0]->SetWindowPos(NULL, intWidth / 8, intHeight * (12 * intIter + 13) / (12 * intTeams + 12), intWidth / 4, intHeight / (3 * intTeams + 3), 0);
					m_Output[intIter][1]->SetWindowPos(NULL, intWidth * 13 / 32, intHeight * (12 * intIter + 13) / (12 * intTeams + 12), intWidth / 8, intHeight / (3 * intTeams + 3), 0);
					m_Output[intIter][2]->SetWindowPos(NULL, intWidth * 9 / 16, intHeight * (12 * intIter + 13) / (12 * intTeams + 12), intWidth * 3 / 32, intHeight / (3 * intTeams + 3), 0);
					m_Output[intIter][3]->SetWindowPos(NULL, intWidth * 11 / 16, intHeight * (12 * intIter + 13) / (12 * intTeams + 12), intWidth / 16, intHeight / (3 * intTeams + 3), 0);
					m_Output[intIter][4]->SetWindowPos(NULL, intWidth * 25 / 32, intHeight * (12 * intIter + 13) / (12 * intTeams + 12), intWidth * 3 / 32, intHeight / (3 * intTeams + 3), 0);
				}
			}
		}
		RedrawWindow();
	}
}

void CDlgResults::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDlgResults::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// CDlgHeroes dialog

CDlgHeroes::CDlgHeroes(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHILD, pParent)
{
	Create(IDD_CHILD, pParent);
}

void CDlgHeroes::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlgHeroes, CDialogEx)
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_LBN_SELCHANGE(1202, ChangeHero)
	ON_BN_CLICKED(1203, LoadImportWindow)
	ON_BN_CLICKED(1204, DeleteHero)
	//ON_CBN_SELCHANGE(1252, ChangeEmblem)
	ON_BN_CLICKED(1255, SaveChanges)
	ON_BN_CLICKED(1257, Import)
	ON_BN_CLICKED(1258, ResetState)
	ON_BN_CLICKED(1259, NewHero)
	ON_BN_CLICKED(1260, Export)
	ON_BN_CLICKED(1380, SelectAll)
	ON_BN_CLICKED(1381, SelectNone)
END_MESSAGE_MAP()


// CDlgHeroes message handlers

BOOL CDlgHeroes::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_Heroes = new(CListBox);
	m_Heroes->Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | LBS_NOTIFY | WS_VSCROLL, CRect(1, 1, 2, 2), this, 1202);
	RefreshList();
	m_Import = new(CButton);
	m_Import->Create(L"Import from File", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(1, 1, 2, 2), this, 1203);
	m_Delete = new(CButton);
	m_Delete->Create(L"Delete Hero", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(1, 1, 2, 2), this, 1204);
	m_Save = new(CButton);
	m_Save->Create(L"Save", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(1, 1, 2, 2), this, 1255);
	for (int intIter = 0; intIter < 26; intIter = intIter + 1)
	{
		m_Labels[intIter] = new(CEdit);
		m_Labels[intIter]->Create(WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_TABSTOP | ES_RIGHT | WS_DISABLED, CRect(1, 1, 2, 2), this, 1205 + intIter);
	}
	m_Labels[0]->SetWindowTextW(L"Lives");
	m_Labels[1]->SetWindowTextW(L"Gold");
	m_Labels[2]->SetWindowTextW(L"Skills");
	m_Labels[3]->SetWindowTextW(L"Accuracy");
	m_Labels[4]->SetWindowTextW(L"Patience");
	m_Labels[5]->SetWindowTextW(L"Courage");
	m_Labels[6]->SetWindowTextW(L"Stamina");
	m_Labels[7]->SetWindowTextW(L"Strength");
	m_Labels[8]->SetWindowTextW(L"Agility");
	m_Labels[9]->SetWindowTextW(L"Speed");
	m_Labels[10]->SetWindowTextW(L"Teamwork");
	m_Labels[11]->SetWindowTextW(L"Wisdom");
	m_Labels[12]->SetWindowTextW(L"Leadership");
	m_Labels[13]->SetWindowTextW(L"Standard");
	m_Labels[14]->SetWindowTextW(L"Attributes");
	m_Labels[15]->SetWindowTextW(L"Age");
	m_Labels[16]->SetWindowTextW(L"Height");
	m_Labels[17]->SetWindowTextW(L"Weight");
	m_Labels[18]->SetWindowTextW(L"Fears");
	m_Labels[19]->SetWindowTextW(L"Dark");
	m_Labels[20]->SetWindowTextW(L"Death");
	m_Labels[21]->SetWindowTextW(L"Enclosed Spaces");
	m_Labels[22]->SetWindowTextW(L"Heights");
	m_Labels[23]->SetWindowTextW(L"Water");
	m_Labels[24]->SetWindowTextW(L"Snakes");
	m_Labels[25]->SetWindowTextW(L"Spiders");
	//m_Labels[2]->ModifyStyle(ES_RIGHT, ES_CENTER);
	//m_Labels[13]->ModifyStyle(ES_RIGHT, ES_CENTER);
	//m_Labels[18]->ModifyStyle(ES_RIGHT, ES_CENTER);

	for (int intIter = 0; intIter < 23; intIter = intIter + 1)
	{
		m_Properties[intIter] = new(CEdit);
		m_Properties[intIter]->Create(WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_TABSTOP | ES_LEFT, CRect(1, 1, 2, 2), this, 1231 + intIter);
	}
	m_Properties[0]->ModifyStyle(ES_LEFT, ES_CENTER);
	//AfxMessageBox(strToLPCTSTR());
	m_Emblem = new(CComboBox);
	m_Emblem->Create(WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_TABSTOP | CBS_DROPDOWNLIST, CRect(1, 1, 2, 2), this, 1252);
	m_Emblem->AddString(L"None");
	string strDirectory = experimental::filesystem::current_path().string() + "\\emblems";
	string strFile;
	for (const auto& entry : experimental::filesystem::directory_iterator(strDirectory))
	{
		strFile = entry.path().string();
		if (strFile.substr(strFile.length() - 4, 4) == ".png")
		{
			m_Emblem->AddString(strToLPCTSTR(strFile.substr(strDirectory.length() + 1, strFile.length() - strDirectory.length() - 5)));
		}
	}
	m_Emblem->SetCurSel(0);
	m_Load = new(CCheckListBox);
	m_Load->Create(WS_CHILD | LBS_HASSTRINGS | LBS_OWNERDRAWFIXED | WS_VSCROLL, CRect(1, 1, 2, 2), this, 1256);
	m_Accept = new(CButton);
	m_Accept->Create(L"Accept", WS_CHILD | BS_PUSHBUTTON, CRect(1, 1, 2, 2), this, 1257);
	m_Cancel = new(CButton);
	m_Cancel->Create(L"Cancel", WS_CHILD | BS_PUSHBUTTON, CRect(1, 1, 2, 2), this, 1258);
	m_SelectAll = new(CButton);
	m_SelectAll->Create(L"Select All", WS_CHILD | BS_PUSHBUTTON, CRect(1, 1, 2, 2), this, 1380);
	m_SelectNone = new(CButton);
	m_SelectNone->Create(L"Select None", WS_CHILD | BS_PUSHBUTTON, CRect(1, 1, 2, 2), this, 1381);
	m_Export = new(CButton);
	m_Export->Create(L"Export Heroes", WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE, CRect(1, 1, 2, 2), this, 1260);
	m_New = new(CButton);
	m_New->Create(L"New Hero", WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE, CRect(1, 1, 2, 2), this, 1259);

	m_VScroll = new(CScrollBar);
	m_VScroll->Create(WS_CHILD | SBS_VERT | SBS_RIGHTALIGN | WS_VISIBLE, CRect(1, 1, 2, 2), this, 1386);
	m_VScroll->SetScrollRange(0, 1000);
	m_VScroll->EnableScrollBar();
	m_HScroll = new(CScrollBar);
	m_HScroll->Create(WS_CHILD | SBS_HORZ | SBS_BOTTOMALIGN | WS_VISIBLE, CRect(1, 1, 2, 2), this, 1387);
	m_HScroll->SetScrollRange(0, 1000);
	m_HScroll->EnableScrollBar();

	boolInit = true;
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDlgHeroes::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	CRect rectTemp;
	GetWindowRect(rectTemp);
	//ScreenToClient(rectTemp);
	int intWidth = rectTemp.right - rectTemp.left;
	int intHeight = rectTemp.bottom - rectTemp.top;
	if (boolInit)
	{
		if (intWidth < INT_HEROWIDTH)
		{
			SCROLLINFO sInfo;
			m_HScroll->GetScrollInfo(&sInfo);
			sInfo.nMax = INT_HEROWIDTH - intWidth + sInfo.nPage - 1;
			sInfo.nPage = INT_HEROWIDTH * INT_HEROWIDTH / intWidth;
			m_HScroll->SetScrollInfo(&sInfo);
			m_HScroll->SetWindowPos(NULL, intWidth / 16, intHeight * 48 / 49, intWidth * 7 / 8, intHeight / 49, 0);

			intXScroll = intXScroll + INT_HEROWIDTH - intWidth < 0 ? intWidth - INT_HEROWIDTH : intXScroll;
			intWidth = INT_HEROWIDTH;
			m_HScroll->ModifyStyle(0, WS_VISIBLE);
		}
		else
		{
			m_HScroll->ModifyStyle(WS_VISIBLE, 0);
			intXScroll = 0;
		}
		if (intHeight < INT_HEROHEIGHT)
		{
			SCROLLINFO sInfo;
			m_VScroll->GetScrollInfo(&sInfo);
			sInfo.nMax = INT_HEROHEIGHT - intHeight + sInfo.nPage - 1;
			sInfo.nPage = INT_HEROHEIGHT * INT_HEROHEIGHT / intHeight;
			m_VScroll->SetScrollInfo(&sInfo);
			if (intWidth == INT_HEROWIDTH)
			{
				m_VScroll->SetWindowPos(NULL, (rectTemp.right - rectTemp.left) * 31 / 32, intHeight / 49, (rectTemp.right - rectTemp.left) / 64, intHeight * 48 / 49, 0);
			}
			else
			{
				m_VScroll->SetWindowPos(NULL, intWidth * 31 / 32, intHeight / 49, intWidth / 64, intHeight * 48 / 49, 0);
			}
			intYScroll = intYScroll + INT_HEROHEIGHT - intHeight < 0 ? intHeight - INT_HEROHEIGHT : intYScroll;
			intHeight = INT_HEROHEIGHT;
			m_VScroll->ModifyStyle(0, WS_VISIBLE);
		}
		else
		{
			m_VScroll->ModifyStyle(WS_VISIBLE, 0);
			intYScroll = 0;
		}

		m_Heroes->SetWindowPos(NULL, intWidth / 16 + intXScroll, intHeight * 4 / 49 + intYScroll, intWidth / 4, intHeight * 32 / 49, 0);
		m_New->SetWindowPos(NULL, intWidth / 16 + intXScroll, intHeight * 37 / 49 + intYScroll, intWidth / 4, intHeight * 2 / 49, 0);
		m_Delete->SetWindowPos(NULL, intWidth / 16 + intXScroll, intHeight * 40 / 49 + intYScroll, intWidth / 4, intHeight * 2 / 49, 0);
		m_Import->SetWindowPos(NULL, intWidth / 16 + intXScroll, intHeight * 43 / 49 + intYScroll, intWidth / 4, intHeight * 2 / 49, 0);
		m_Export->SetWindowPos(NULL, intWidth / 16 + intXScroll, intHeight * 46 / 49 + intYScroll, intWidth / 4, intHeight * 2 / 49, 0);
		for (int intIter = 0; intIter < 13; intIter = intIter + 1)
		{
			m_Labels[intIter]->SetWindowPos(NULL, intWidth * 3 / 8 + intXScroll, intHeight * (7 + 3 * intIter) / 49 + intYScroll, intWidth / 6, intHeight * 2 / 49, 0);
			m_Labels[intIter + 13]->SetWindowPos(NULL, intWidth * 11 / 16 + intXScroll, intHeight * (7 + 3 * intIter) / 49 + intYScroll, intWidth / 6, intHeight * 2 / 49, 0);
		}
		//m_Labels[2]->SetWindowPos(NULL, intWidth * 3 / 8+intXScroll, intHeight * 13 / 49 + intYScroll, intWidth / 4, intHeight * 2 / 49, 0);
		//m_Labels[13]->SetWindowPos(NULL, intWidth * 11 / 16+intXScroll, intHeight * 7 / 49 + intYScroll, intWidth / 4, intHeight * 2 / 49, 0);
		//m_Labels[18]->SetWindowPos(NULL, intWidth * 11 / 16+intXScroll, intHeight * 22 / 49 + intYScroll, intWidth / 4, intHeight * 2 / 49, 0);
		m_Properties[0]->SetWindowPos(NULL, intWidth * 3 / 8 + intXScroll, intHeight / 49 + intYScroll, intWidth * 9 / 16, intHeight * 5 / 49, 0);
		m_Properties[1]->SetWindowPos(NULL, intWidth * 23 / 40 + intXScroll, intHeight / 7 + intYScroll, intWidth / 20, intHeight * 2 / 49, 0);
		m_Properties[2]->SetWindowPos(NULL, intWidth * 23 / 40 + intXScroll, intHeight * 10 / 49 + intYScroll, intWidth / 20, intHeight * 2 / 49, 0);
		for (int intIter = 0; intIter < 10; intIter = intIter + 1)
		{
			m_Properties[3 + intIter]->SetWindowPos(NULL, intWidth * 23 / 40 + intXScroll, intHeight * (16 + 3 * intIter) / 49 + intYScroll, intWidth / 20, intHeight * 2 / 49, 0);
		}
		for (int intIter = 0; intIter < 3; intIter = intIter + 1)
		{
			m_Properties[13 + intIter]->SetWindowPos(NULL, intWidth * 71 / 80 + intXScroll, intHeight * (13 + 3 * intIter) / 49 + intYScroll, intWidth / 20, intHeight * 2 / 49, 0);
		}
		for (int intIter = 0; intIter < 7; intIter = intIter + 1)
		{
			m_Properties[16 + intIter]->SetWindowPos(NULL, intWidth * 71 / 80 + intXScroll, intHeight * (25 + 3 * intIter) / 49 + intYScroll, intWidth / 20, intHeight * 2 / 49, 0);
		}
		m_Labels[13]->SetWindowPos(NULL, intWidth * 11 / 16 + intXScroll, intHeight * 7 / 49 + intYScroll, intWidth * 9 / 80, intHeight * 2 / 49, 0);
		m_Emblem->SetWindowPos(NULL, intWidth * 66 / 80 + intXScroll, intHeight * 7 / 49 + intYScroll, intWidth * 9 / 80, intHeight * 2 / 49, 0);
		m_Save->SetWindowPos(NULL, intWidth * 31 / 64 + intXScroll, intHeight * 46 / 49 + intYScroll, intWidth * 9 / 32, intHeight * 2 / 49, 0);
		m_Load->SetWindowPos(NULL, intWidth * 3 / 8 + intXScroll, intHeight * 4 / 49 + intYScroll, intWidth * 9 / 16, intHeight * 38 / 49, 0);
		m_SelectAll->SetWindowPos(NULL, intWidth * 3 / 8 + intXScroll, intHeight * 43 / 49 + intYScroll, intWidth / 4, intHeight * 2 / 49, 0);
		m_SelectNone->SetWindowPos(NULL, intWidth * 11 / 16 + intXScroll, intHeight * 43 / 49 + intYScroll, intWidth / 4, intHeight * 2 / 49, 0);
		m_Accept->SetWindowPos(NULL, intWidth * 3 / 8 + intXScroll, intHeight * 46 / 49 + intYScroll, intWidth / 4, intHeight * 2 / 49, 0);
		m_Cancel->SetWindowPos(NULL, intWidth * 11 / 16 + intXScroll, intHeight * 46 / 49 + intYScroll, intWidth / 4, intHeight * 2 / 49, 0);
	}
}

void CDlgHeroes::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
	//intYScroll = pScrollBar->GetScrollPos();

	updateScroll(nSBCode, nPos, pScrollBar);
	intYScroll = pScrollBar->GetScrollPos() * -1;
	CRect rectTemp;
	GetClientRect(rectTemp);
	OnSize(SIZE_RESTORED, rectTemp.right - rectTemp.left, rectTemp.bottom - rectTemp.top);

	//this->ScrollWindowEx(0, 10, NULL, NULL, NULL, NULL, SW_SCROLLCHILDREN);
}

void CDlgHeroes::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
	//intYScroll = pScrollBar->GetScrollPos();

	updateScroll(nSBCode, nPos, pScrollBar);
	intXScroll = pScrollBar->GetScrollPos() * -1;
	CRect rectTemp;
	GetClientRect(rectTemp);
	OnSize(SIZE_RESTORED, rectTemp.right - rectTemp.left, rectTemp.bottom - rectTemp.top);

	//this->ScrollWindowEx(0, 10, NULL, NULL, NULL, NULL, SW_SCROLLCHILDREN);
}


void CDlgHeroes::RefreshList()
{
	m_Heroes->ResetContent();
	for (int intIter = 1; intIter < INT_MAXHEROES; intIter = intIter + 1)
	{
		if (arrHeroes[intIter].name == "None" || arrHeroes[intIter].name == "")
		{
			break;
		}
		else
		{
			m_Heroes->AddString(strToLPCTSTR(arrHeroes[intIter].name));
		}
	}
}

void CDlgHeroes::ChangeHero()
{
	int intIndex = m_Heroes->GetCurSel() + 1;
	if (intIndex == 0)
	{
		intIndex = 1;
	}
	CString strTemp;
	m_Properties[0]->SetWindowTextW(strToLPCTSTR(arrHeroes[intIndex].name));
	strTemp.Format(_T("%d"), static_cast<int>(arrHeroes[intIndex].gold / intGoldPerLife));
	m_Properties[1]->SetWindowTextW(strTemp);
	strTemp.Format(_T("%d"), (arrHeroes[intIndex].gold % intGoldPerLife));
	m_Properties[2]->SetWindowTextW(strTemp);
	for (int intIter = 2; intIter < 12; intIter = intIter + 1)
	{
		strTemp.Format(_T("%g"), arrHeroes[intIndex].skills[intIter]);
		m_Properties[intIter + 1]->SetWindowTextW(strTemp);
	}
	strTemp.Format(_T("%g"), arrHeroes[intIndex].age);
	m_Properties[13]->SetWindowTextW(strTemp);
	strTemp.Format(_T("%g"), arrHeroes[intIndex].skills[0]);
	m_Properties[14]->SetWindowTextW(strTemp);
	strTemp.Format(_T("%g"), arrHeroes[intIndex].skills[1]);
	m_Properties[15]->SetWindowTextW(strTemp);
	for (int intIter = 0; intIter < 7; intIter = intIter + 1)
	{
		strTemp.Format(_T("%g"), arrHeroes[intIndex].fears[intIter]);
		m_Properties[intIter + 16]->SetWindowTextW(strTemp);
	}
	int intEmblem = m_Emblem->FindStringExact(-1, strToLPCTSTR(arrHeroes[intIndex].emblem));
	if (intEmblem == CB_ERR)
	{
		m_Emblem->SetCurSel(0);
	}
	else
	{
		m_Emblem->SetCurSel(intEmblem);
	}
}

void CDlgHeroes::SaveChanges()
{
	int intIndex = m_Heroes->GetCurSel() + 1;
	CString strTemp;
	m_Properties[0]->GetWindowTextW(strTemp);
	arrHeroes[intIndex].name = CStringToStr(strTemp);
	m_Properties[1]->GetWindowTextW(strTemp);
	arrHeroes[intIndex].gold = intGoldPerLife * stoi(CStringToStr(strTemp));
	m_Properties[2]->GetWindowTextW(strTemp);
	arrHeroes[intIndex].gold = arrHeroes[intIndex].gold + stoi(CStringToStr(strTemp));
	for (int intIter = 2; intIter < 12; intIter = intIter + 1)
	{
		m_Properties[intIter + 1]->GetWindowTextW(strTemp);
		arrHeroes[intIndex].skills[intIter] = stof(CStringToStr(strTemp));
	}
	m_Properties[13]->GetWindowTextW(strTemp);
	arrHeroes[intIndex].age = stof(CStringToStr(strTemp));
	m_Properties[14]->GetWindowTextW(strTemp);
	arrHeroes[intIndex].skills[0] = stof(CStringToStr(strTemp));
	m_Properties[15]->GetWindowTextW(strTemp);
	arrHeroes[intIndex].skills[1] = stof(CStringToStr(strTemp));
	for (int intIter = 0; intIter < 7; intIter = intIter + 1)
	{
		m_Properties[intIter + 16]->GetWindowTextW(strTemp);
		arrHeroes[intIndex].fears[intIter] = stof(CStringToStr(strTemp));
	}
	m_Emblem->GetWindowTextW(strTemp);
	arrHeroes[intIndex].emblem = CStringToStr(strTemp);
	RefreshList();
}

void CDlgHeroes::DeleteHero()
{
	int intIndex = m_Heroes->GetCurSel() + 1;
	for (int intIter = intIndex; intIter < INT_MAXHEROES - 1; intIter = intIter + 1)
	{
		if (arrHeroes[intIndex].name == "None" || arrHeroes[intIndex].name == "")
		{
			break;
		}
		else
		{
			arrHeroes[intIter] = arrHeroes[intIter + 1];
		}
	}
	if (arrHeroes[INT_MAXHEROES - 1].name != "None" && arrHeroes[INT_MAXHEROES - 1].name != "")
	{
		arrHeroes[INT_MAXHEROES - 1] = arrHeroes[0];
	}

	m_Heroes->DeleteString(intIndex - 1);
}

void CDlgHeroes::LoadImportWindow()
{
	const TCHAR szFilter[] = _T("CSV Files (*.csv)|*.csv|All Files (*.*)|*.*||");
	CFileDialog m_FileDialog(true, _T("csv"), NULL, OFN_HIDEREADONLY, szFilter, this);
	CString strFilename;

	if (m_FileDialog.DoModal() == IDOK)
	{
		strFilename = m_FileDialog.GetPathName();


		m_Load->ModifyStyle(0, WS_VISIBLE);
		m_Accept->ModifyStyle(0, WS_VISIBLE);
		m_Cancel->ModifyStyle(0, WS_VISIBLE);
		m_SelectAll->ModifyStyle(0, WS_VISIBLE);
		m_SelectNone->ModifyStyle(0, WS_VISIBLE);
		m_Heroes->ModifyStyle(0, WS_DISABLED);
		m_Import->ModifyStyle(0, WS_DISABLED);
		m_Delete->ModifyStyle(0, WS_DISABLED);
		m_Export->ModifyStyle(0, WS_DISABLED);
		m_New->ModifyStyle(0, WS_DISABLED);
		for (int intIter = 0; intIter < 23; intIter = intIter + 1)
		{
			m_Labels[intIter]->ModifyStyle(WS_VISIBLE, 0);
			m_Properties[intIter]->ModifyStyle(WS_VISIBLE, 0);
		}
		for (int intIter = 23; intIter < 26; intIter = intIter + 1)
		{
			m_Labels[intIter]->ModifyStyle(WS_VISIBLE, 0);
		}
		m_Emblem->ModifyStyle(WS_VISIBLE, 0);
		m_Save->ModifyStyle(WS_VISIBLE, 0);

		this->RedrawWindow();

		for (int intIter = 0; intIter < INT_MAXHEROES; intIter = intIter + 1)
		{
			arrImport[intIter][0] = "";
		}

		importCSV(&arrImport[0][0], CStringToStr(strFilename));
		for (int intIter = 1; intIter < INT_MAXHEROES; intIter = intIter + 1)
		{
			if (arrImport[intIter][0] == "")
			{
				break;
			}
			else
			{
				m_Load->AddString(strToLPCTSTR(arrImport[intIter][0]));
			}
		}
	}
}

void CDlgHeroes::ResetState()
{
	m_Load->ModifyStyle(WS_VISIBLE, 0);
	m_Accept->ModifyStyle(WS_VISIBLE, 0);
	m_Cancel->ModifyStyle(WS_VISIBLE, 0);
	m_SelectAll->ModifyStyle(WS_VISIBLE, 0);
	m_SelectNone->ModifyStyle(WS_VISIBLE, 0);
	m_Heroes->ModifyStyle(WS_DISABLED, 0);
	m_Import->ModifyStyle(WS_DISABLED, 0);
	m_Delete->ModifyStyle(WS_DISABLED, 0);
	m_Export->ModifyStyle(WS_DISABLED, 0);
	m_New->ModifyStyle(WS_DISABLED, 0);
	for (int intIter = 0; intIter < 23; intIter = intIter + 1)
	{
		m_Labels[intIter]->ModifyStyle(0, WS_VISIBLE);
		m_Properties[intIter]->ModifyStyle(0, WS_VISIBLE);
	}
	for (int intIter = 23; intIter < 26; intIter = intIter + 1)
	{
		m_Labels[intIter]->ModifyStyle(0, WS_VISIBLE);
	}
	m_Emblem->ModifyStyle(0, WS_VISIBLE);
	m_Save->ModifyStyle(0, WS_VISIBLE);

	//Update screen
	this->RedrawWindow();
	m_Load->ResetContent();
}

void CDlgHeroes::Import()
{
	int arrSelect[INT_MAXHEROES];
	int intCount = 0;
	for (int intIter = 0; intIter < INT_MAXHEROES; intIter = intIter + 1)
	{
		if (m_Load->GetCheck(intIter) == BST_CHECKED)
		{
			arrSelect[intCount] = intIter + 1;
			intCount = intCount + 1;
		}
	}
	int intFirst = 1;
	while (arrHeroes[intFirst].name != "None" && arrHeroes[intFirst].name != "")
	{
		intFirst = intFirst + 1;
	}
	if (intFirst + intCount > INT_MAXHEROES)
	{
		AfxMessageBox(L"Too many heroes selected.");
	}
	else
	{
		importWarriorsGUI(arrHeroes, arrImport, arrSelect, intFirst, intCount);
		for (int intIter = 0; intIter < intCount; intIter = intIter + 1)
		{
			if (m_Emblem->FindStringExact(-1, strToLPCTSTR(arrHeroes[intFirst + intIter].emblem)) == CB_ERR)
			{
				arrHeroes[intFirst + intIter].emblem = "None";
			}
		}
		RefreshList();
		ResetState();
	}
}

void CDlgHeroes::NewHero()
{
	int intIter = 1;
	while (arrHeroes[intIter].name != "None" && arrHeroes[intIter].name != "")
	{
		intIter = intIter + 1;
	}
	if (intIter < INT_MAXHEROES)
	{
		arrHeroes[intIter] = arrHeroes[0];
		arrHeroes[intIter].name = "Hero";
		arrHeroes[intIter].gold = intGoldPerLife * intGoldPerLife;
		for (int intSkill = 0; intSkill < 12; intSkill = intSkill + 1)
		{
			arrHeroes[intIter].skills[intSkill] = 7;
		}
	}
	else
	{
		AfxMessageBox(L"Maximum number of heroes has been reached. Please delete a hero before adding more.");
	}
	RefreshList();
}

void CDlgHeroes::SelectAll()
{
	int intLength = m_Load->GetCount();
	for (int intIter = 0; intIter < intLength; intIter = intIter + 1)
	{
		m_Load->SetCheck(intIter, 1);
	}
}

void CDlgHeroes::SelectNone()
{
	int intLength = m_Load->GetCount();
	for (int intIter = 0; intIter < intLength; intIter = intIter + 1)
	{
		m_Load->SetCheck(intIter, 0);
	}
}

void CDlgHeroes::Export()
{
	int intCount = 1;
	//string arrExport[64][25];
	const TCHAR szFilter[] = _T("CSV Files (*.csv)|*.csv|All Files (*.*)|*.*||");
	CFileDialog m_FileDialog(false, _T("csv"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
	CString strFilename;

	if (m_FileDialog.DoModal() == IDOK)
	{
		strFilename = m_FileDialog.GetPathName();
		arrImport[0][0] = "name";
		arrImport[0][1] = "emblem";
		arrImport[0][2] = "";
		arrImport[0][3] = "age";
		arrImport[0][4] = "skills";
		arrImport[0][5] = "height";
		arrImport[0][6] = "weight";
		arrImport[0][7] = "accuracy";
		arrImport[0][8] = "patience";
		arrImport[0][9] = "courage";
		arrImport[0][10] = "stamina";
		arrImport[0][11] = "strength";
		arrImport[0][12] = "agility";
		arrImport[0][13] = "speed";
		arrImport[0][14] = "teamwork";
		arrImport[0][15] = "wisdom";
		arrImport[0][16] = "leadership";
		arrImport[0][17] = "fears";
		arrImport[0][18] = "dark";
		arrImport[0][19] = "death";
		arrImport[0][20] = "enclosed spaces";
		arrImport[0][21] = "heights";
		arrImport[0][22] = "water";
		arrImport[0][23] = "snakes";
		arrImport[0][24] = "spiders";

		while (arrHeroes[intCount].name != "None" && arrHeroes[intCount].name != "")
		{
			arrImport[intCount][0] = arrHeroes[intCount].name;
			arrImport[intCount][1] = arrHeroes[intCount].emblem;
			arrImport[intCount][2] = "";
			arrImport[intCount][3] = to_string(arrHeroes[intCount].age);
			for (int intSkill = 0; intSkill < 12; intSkill = intSkill + 1)
			{
				arrImport[intCount][5 + intSkill] = to_string(arrHeroes[intCount].skills[intSkill]);
			}
			for (int intFear = 0; intFear < 7; intFear = intFear + 1)
			{
				arrImport[intCount][18 + intFear] = to_string(arrHeroes[intCount].fears[intFear]);
			}
			intCount = intCount + 1;
		}
		writeToCSV(&arrImport[0][0], 25, intCount, CStringToStr(strFilename));
	}
}

// CDlgTrials dialog

CDlgTrials::CDlgTrials(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHILD, pParent)
{
	Create(IDD_CHILD, pParent);
}

void CDlgTrials::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlgTrials, CDialogEx)
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_LBN_SELCHANGE(1261, ChangeTrial)
	ON_BN_CLICKED(1262, LoadImportWindow)
	ON_BN_CLICKED(1263, DeleteTrial)
	ON_BN_CLICKED(1359, SaveChanges)
	ON_BN_CLICKED(1361, Import)
	ON_BN_CLICKED(1362, ResetState)
	ON_BN_CLICKED(1363, NewTrial)
	ON_BN_CLICKED(1364, Export)
	ON_BN_CLICKED(1382, SelectAll)
	ON_BN_CLICKED(1383, SelectNone)
END_MESSAGE_MAP()


// CDlgTrials message handlers

BOOL CDlgTrials::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_Trials = new(CListBox);
	m_Trials->Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | LBS_NOTIFY | WS_VSCROLL, CRect(1, 1, 2, 2), this, 1261);
	RefreshList();
	m_Import = new(CButton);
	m_Import->Create(L"Import from File", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(1, 1, 2, 2), this, 1262);
	m_Delete = new(CButton);
	m_Delete->Create(L"Delete Trial", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(1, 1, 2, 2), this, 1263);
	m_Save = new(CButton);
	m_Save->Create(L"Save", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(1, 1, 2, 2), this, 1359);
	for (int intIter = 0; intIter < 48; intIter = intIter + 1)
	{
		m_Labels[intIter] = new(CEdit);
		m_Labels[intIter]->Create(WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_TABSTOP | ES_RIGHT | WS_DISABLED, CRect(1, 1, 2, 2), this, 1264 + intIter);
	}
	m_Labels[0]->SetWindowTextW(L"Properties");
	m_Labels[1]->SetWindowTextW(L"Max teams");
	m_Labels[2]->SetWindowTextW(L"Heroes per team");
	m_Labels[3]->SetWindowTextW(L"Race");
	m_Labels[4]->SetWindowTextW(L"Last means failure");
	m_Labels[5]->SetWindowTextW(L"Not first means failure");
	m_Labels[6]->SetWindowTextW(L"Luck");
	m_Labels[7]->SetWindowTextW(L"Max reward:");
	m_Labels[8]->SetWindowTextW(L"1st");
	m_Labels[9]->SetWindowTextW(L"2nd");
	m_Labels[10]->SetWindowTextW(L"3rd");
	m_Labels[11]->SetWindowTextW(L"4th");
	m_Labels[12]->SetWindowTextW(L"5th");
	m_Labels[13]->SetWindowTextW(L"6th");
	m_Labels[14]->SetWindowTextW(L"Winner's Bonus");
	m_Labels[15]->SetWindowTextW(L"Optional gold?");
	m_Labels[16]->SetWindowTextW(L"Optional gold bias");
	m_Labels[17]->SetWindowTextW(L"Failure");
	m_Labels[18]->SetWindowTextW(L"Loser keeps reward");
	m_Labels[19]->SetWindowTextW(L"How many can fail?");
	m_Labels[20]->SetWindowTextW(L"Average failure probability");
	m_Labels[21]->SetWindowTextW(L"Max failure probability");
	m_Labels[22]->SetWindowTextW(L"Failure descriptions:");
	m_Labels[23]->SetWindowTextW(L"Skills relevance");
	m_Labels[24]->SetWindowTextW(L"Height");
	m_Labels[25]->SetWindowTextW(L"Weight");
	m_Labels[26]->SetWindowTextW(L"Accuracy");
	m_Labels[27]->SetWindowTextW(L"Patience");
	m_Labels[28]->SetWindowTextW(L"Courage");
	m_Labels[29]->SetWindowTextW(L"Stamina");
	m_Labels[30]->SetWindowTextW(L"Strength");
	m_Labels[31]->SetWindowTextW(L"Agility");
	m_Labels[32]->SetWindowTextW(L"Speed");
	m_Labels[33]->SetWindowTextW(L"Teamwork");
	m_Labels[34]->SetWindowTextW(L"Wisdom");
	m_Labels[35]->SetWindowTextW(L"Leadership");
	m_Labels[36]->SetWindowTextW(L"Fears relevance");
	m_Labels[37]->SetWindowTextW(L"Dark");
	m_Labels[38]->SetWindowTextW(L"Death");
	m_Labels[39]->SetWindowTextW(L"Enclosed spaces");
	m_Labels[40]->SetWindowTextW(L"Heights");
	m_Labels[41]->SetWindowTextW(L"Water");
	m_Labels[42]->SetWindowTextW(L"Snakes");
	m_Labels[43]->SetWindowTextW(L"Spiders");
	m_Labels[44]->SetWindowTextW(L"Time taken:");
	m_Labels[45]->SetWindowTextW(L"Very fast time");
	m_Labels[46]->SetWindowTextW(L"Average time");
	m_Labels[47]->SetWindowTextW(L"Very slow time");

	for (int intIter = 0; intIter < 38; intIter = intIter + 1)
	{
		m_TextBox[intIter] = new(CEdit);
		m_TextBox[intIter]->Create(WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_TABSTOP | ES_LEFT, CRect(1, 1, 2, 2), this, 1312 + intIter);
	}
	for (int intIter = 0; intIter < 6; intIter = intIter + 1)
	{
		m_ComboBox[intIter] = new(CComboBox);
		m_ComboBox[intIter]->Create(WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_TABSTOP | CBS_DROPDOWNLIST, CRect(1, 1, 2, 2), this, 1351 + intIter);
		m_ComboBox[intIter]->AddString(L"FALSE");
		m_ComboBox[intIter]->AddString(L"TRUE");
	}
	m_TextBox[0]->ModifyStyle(ES_LEFT, ES_CENTER);

	/*for (string strIter : list of emblems)
	{
			m_Emblem->AddString(strToLPCTSTR(strIter));
	}
	m_Emblem->SetCurSel(0);*/
	m_Load = new(CCheckListBox);
	m_Load->Create(WS_CHILD | LBS_HASSTRINGS | LBS_OWNERDRAWFIXED | WS_VSCROLL, CRect(1, 1, 2, 2), this, 1360);
	m_Accept = new(CButton);
	m_Accept->Create(L"Accept", WS_CHILD | BS_PUSHBUTTON, CRect(1, 1, 2, 2), this, 1361);
	m_Cancel = new(CButton);
	m_Cancel->Create(L"Cancel", WS_CHILD | BS_PUSHBUTTON, CRect(1, 1, 2, 2), this, 1362);
	m_ComboBox[5]->ResetContent();
	m_ComboBox[5]->AddString(L"0");
	m_ComboBox[5]->AddString(L"1");
	m_ComboBox[5]->AddString(L"few");
	m_ComboBox[5]->AddString(L"many");
	m_ComboBox[5]->AddString(L"all");

	m_SelectAll = new(CButton);
	m_SelectAll->Create(L"Select All", WS_CHILD | BS_PUSHBUTTON, CRect(1, 1, 2, 2), this, 1382);
	m_SelectNone = new(CButton);
	m_SelectNone->Create(L"Select None", WS_CHILD | BS_PUSHBUTTON, CRect(1, 1, 2, 2), this, 1383);
	m_Export = new(CButton);
	m_Export->Create(L"Export Trials", WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE, CRect(1, 1, 2, 2), this, 1364);
	m_New = new(CButton);
	m_New->Create(L"New Trial", WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE, CRect(1, 1, 2, 2), this, 1363);

	m_VScroll = new(CScrollBar);
	m_VScroll->Create(WS_CHILD | SBS_VERT | SBS_RIGHTALIGN | WS_VISIBLE, CRect(1, 1, 2, 2), this, 1388);
	m_VScroll->SetScrollRange(0, 1000);
	m_VScroll->EnableScrollBar();
	m_HScroll = new(CScrollBar);
	m_HScroll->Create(WS_CHILD | SBS_HORZ | SBS_BOTTOMALIGN | WS_VISIBLE, CRect(1, 1, 2, 2), this, 1389);
	m_HScroll->SetScrollRange(0, 1000);
	m_HScroll->EnableScrollBar();


	boolInit = true;
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDlgTrials::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	CRect rectTemp;
	GetWindowRect(rectTemp);
	//ScreenToClient(rectTemp);
	int intWidth = rectTemp.right - rectTemp.left;
	int intHeight = rectTemp.bottom - rectTemp.top;
	if (boolInit)
	{
		if (intWidth < INT_TRIALWIDTH)
		{
			SCROLLINFO sInfo;
			m_HScroll->GetScrollInfo(&sInfo);
			sInfo.nMax = INT_TRIALWIDTH - intWidth + sInfo.nPage - 1;
			sInfo.nPage = INT_TRIALWIDTH * INT_TRIALWIDTH / intWidth;
			m_HScroll->SetScrollInfo(&sInfo);
			m_HScroll->SetWindowPos(NULL, intWidth / 16, intHeight * 82 / 85, intWidth * 7 / 8, intHeight / 85, 0);

			intXScroll = intXScroll + INT_TRIALWIDTH - intWidth < 0 ? intWidth - INT_TRIALWIDTH : intXScroll;
			intWidth = INT_TRIALWIDTH;
			m_HScroll->ModifyStyle(0, WS_VISIBLE);
		}
		else
		{
			m_HScroll->ModifyStyle(WS_VISIBLE, 0);
			intXScroll = 0;
		}
		if (intHeight < INT_TRIALHEIGHT)
		{
			SCROLLINFO sInfo;
			m_VScroll->GetScrollInfo(&sInfo);
			sInfo.nMax = INT_TRIALHEIGHT - intHeight + sInfo.nPage - 1;
			sInfo.nPage = INT_TRIALHEIGHT * INT_TRIALHEIGHT / intHeight;
			m_VScroll->SetScrollInfo(&sInfo);
			if (intWidth == INT_TRIALWIDTH)
			{
				m_VScroll->SetWindowPos(NULL, (rectTemp.right - rectTemp.left) * 31 / 32, intHeight * 4 / 85, (rectTemp.right - rectTemp.left) / 64, intHeight * 77 / 49, 0);
			}
			else
			{
				m_VScroll->SetWindowPos(NULL, intWidth * 31 / 32, intHeight * 4 / 85, intWidth / 64, intHeight * 77 / 85, 0);
			}
			intYScroll = intYScroll + INT_TRIALHEIGHT - intHeight < 0 ? intHeight - INT_TRIALHEIGHT : intYScroll;
			intHeight = INT_TRIALHEIGHT;
			m_VScroll->ModifyStyle(0, WS_VISIBLE);
		}
		else
		{
			m_VScroll->ModifyStyle(WS_VISIBLE, 0);
			intYScroll = 0;
		}

		m_Trials->SetWindowPos(NULL, intWidth / 16 + intXScroll, intHeight * 4 / 85 + intYScroll, intWidth / 4, intHeight * 65 / 85, 0);
		m_New->SetWindowPos(NULL, intWidth / 16 + intXScroll, intHeight * 70 / 85 + intYScroll, intWidth / 4, intHeight * 2 / 85, 0);
		m_Delete->SetWindowPos(NULL, intWidth / 16 + intXScroll, intHeight * 73 / 85 + intYScroll, intWidth / 4, intHeight * 2 / 85, 0);
		m_Import->SetWindowPos(NULL, intWidth / 16 + intXScroll, intHeight * 76 / 85 + intYScroll, intWidth / 4, intHeight * 2 / 85, 0);
		m_Export->SetWindowPos(NULL, intWidth / 16 + intXScroll, intHeight * 79 / 85 + intYScroll, intWidth / 4, intHeight * 2 / 85, 0);
		for (int intIter = 0; intIter < 23; intIter = intIter + 1)
		{
			m_Labels[intIter]->SetWindowPos(NULL, intWidth * 3 / 8 + intXScroll, intHeight * (7 + 3 * intIter) / 85 + intYScroll, intWidth / 6, intHeight * 2 / 85, 0);
			m_Labels[intIter + 23]->SetWindowPos(NULL, intWidth * 11 / 16 + intXScroll, intHeight * (7 + 3 * intIter) / 85 + intYScroll, intWidth / 6, intHeight * 2 / 85, 0);
		}
		m_Labels[46]->SetWindowPos(NULL, intWidth * 11 / 16 + intXScroll, intHeight * 76 / 85 + intYScroll, intWidth / 6, intHeight * 2 / 85, 0);
		m_Labels[47]->SetWindowPos(NULL, intWidth * 11 / 16 + intXScroll, intHeight * 79 / 85 + intYScroll, intWidth / 6, intHeight * 2 / 85, 0);

		m_TextBox[0]->SetWindowPos(NULL, intWidth * 3 / 8 + intXScroll, intHeight / 85 + intYScroll, intWidth * 9 / 16, intHeight * 5 / 85, 0);
		m_TextBox[1]->SetWindowPos(NULL, intWidth * 23 / 40 + intXScroll, intHeight * 10 / 85 + intYScroll, intWidth / 20, intHeight * 2 / 85, 0);
		m_TextBox[2]->SetWindowPos(NULL, intWidth * 23 / 40 + intXScroll, intHeight * 13 / 85 + intYScroll, intWidth / 20, intHeight * 2 / 85, 0);
		for (int intIter = 0; intIter < 3; intIter = intIter + 1)
		{
			m_ComboBox[intIter]->SetWindowPos(NULL, intWidth * 23 / 40 + intXScroll, intHeight * (16 + 3 * intIter) / 85 + intYScroll, intWidth / 20, intHeight * 2 / 85, 0);
		}
		m_TextBox[3]->SetWindowPos(NULL, intWidth * 23 / 40 + intXScroll, intHeight * 25 / 85 + intYScroll, intWidth / 20, intHeight * 2 / 85, 0);
		for (int intIter = 0; intIter < 7; intIter = intIter + 1)
		{
			m_TextBox[4 + intIter]->SetWindowPos(NULL, intWidth * 23 / 40 + intXScroll, intHeight * (31 + 3 * intIter) / 85 + intYScroll, intWidth / 20, intHeight * 2 / 85, 0);
		}
		m_ComboBox[3]->SetWindowPos(NULL, intWidth * 23 / 40 + intXScroll, intHeight * 52 / 85 + intYScroll, intWidth / 20, intHeight * 2 / 85, 0);
		m_TextBox[11]->SetWindowPos(NULL, intWidth * 23 / 40 + intXScroll, intHeight * 55 / 85 + intYScroll, intWidth / 20, intHeight * 2 / 85, 0);
		m_ComboBox[4]->SetWindowPos(NULL, intWidth * 23 / 40 + intXScroll, intHeight * 61 / 85 + intYScroll, intWidth / 20, intHeight * 2 / 85, 0);
		m_ComboBox[5]->SetWindowPos(NULL, intWidth * 23 / 40 + intXScroll, intHeight * 64 / 85 + intYScroll, intWidth / 20, intHeight * 2 / 85, 0);
		m_TextBox[12]->SetWindowPos(NULL, intWidth * 23 / 40 + intXScroll, intHeight * 67 / 85 + intYScroll, intWidth / 20, intHeight * 2 / 85, 0);
		m_TextBox[13]->SetWindowPos(NULL, intWidth * 23 / 40 + intXScroll, intHeight * 70 / 85 + intYScroll, intWidth / 20, intHeight * 2 / 85, 0);
		m_TextBox[14]->SetWindowPos(NULL, intWidth * 3 / 8 + intXScroll, intHeight * 76 / 85 + intYScroll, intWidth / 4, intHeight * 2 / 85, 0);
		m_TextBox[15]->SetWindowPos(NULL, intWidth * 3 / 8 + intXScroll, intHeight * 79 / 85 + intYScroll, intWidth / 4, intHeight * 2 / 85, 0);

		for (int intIter = 0; intIter < 12; intIter = intIter + 1)
		{
			m_TextBox[16 + intIter]->SetWindowPos(NULL, intWidth * 71 / 80 + intXScroll, intHeight * (10 + 3 * intIter) / 85 + intYScroll, intWidth / 20, intHeight * 2 / 85, 0);
		}
		for (int intIter = 0; intIter < 7; intIter = intIter + 1)
		{
			m_TextBox[28 + intIter]->SetWindowPos(NULL, intWidth * 71 / 80 + intXScroll, intHeight * (49 + 3 * intIter) / 85 + intYScroll, intWidth / 20, intHeight * 2 / 85, 0);
		}
		for (int intIter = 0; intIter < 3; intIter = intIter + 1)
		{
			m_TextBox[35 + intIter]->SetWindowPos(NULL, intWidth * 71 / 80 + intXScroll, intHeight * (73 + 3 * intIter) / 85 + intYScroll, intWidth / 20, intHeight * 2 / 85, 0);
		}
		m_Save->SetWindowPos(NULL, intWidth * 31 / 64 + intXScroll, intHeight * 82 / 85 + intYScroll, intWidth * 9 / 32, intHeight * 2 / 85, 0);
		m_Load->SetWindowPos(NULL, intWidth * 3 / 8 + intXScroll, intHeight * 4 / 85 + intYScroll, intWidth * 9 / 16, intHeight * 65 / 85, 0);
		m_SelectAll->SetWindowPos(NULL, intWidth * 3 / 8 + intXScroll, intHeight * 70 / 85 + intYScroll, intWidth / 4, intHeight * 2 / 49, 0);
		m_SelectNone->SetWindowPos(NULL, intWidth * 11 / 16 + intXScroll, intHeight * 70 / 85 + intYScroll, intWidth / 4, intHeight * 2 / 49, 0);
		m_Accept->SetWindowPos(NULL, intWidth * 3 / 8 + intXScroll, intHeight * 76 / 85 + intYScroll, intWidth / 4, intHeight * 2 / 49, 0);
		m_Cancel->SetWindowPos(NULL, intWidth * 11 / 16 + intXScroll, intHeight * 76 / 85 + intYScroll, intWidth / 4, intHeight * 2 / 49, 0);
	}
}

void CDlgTrials::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
	//intYScroll = pScrollBar->GetScrollPos();

	updateScroll(nSBCode, nPos, pScrollBar);
	intYScroll = pScrollBar->GetScrollPos() * -1;
	CRect rectTemp;
	GetClientRect(rectTemp);
	OnSize(SIZE_RESTORED, rectTemp.right - rectTemp.left, rectTemp.bottom - rectTemp.top);

	//this->ScrollWindowEx(0, 10, NULL, NULL, NULL, NULL, SW_SCROLLCHILDREN);
}

void CDlgTrials::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
	//intYScroll = pScrollBar->GetScrollPos();

	updateScroll(nSBCode, nPos, pScrollBar);
	intXScroll = pScrollBar->GetScrollPos() * -1;
	CRect rectTemp;
	GetClientRect(rectTemp);
	OnSize(SIZE_RESTORED, rectTemp.right - rectTemp.left, rectTemp.bottom - rectTemp.top);

	//this->ScrollWindowEx(0, 10, NULL, NULL, NULL, NULL, SW_SCROLLCHILDREN);
}


void CDlgTrials::RefreshList()
{
	m_Trials->ResetContent();
	for (int intIter = 0; intIter < INT_MAXTRIALS; intIter = intIter + 1)
	{
		if (arrTrials[intIter].name == "None" || arrTrials[intIter].name == "")
		{
			break;
		}
		else
		{
			m_Trials->AddString(strToLPCTSTR(arrTrials[intIter].name));
		}
	}
}

void CDlgTrials::ChangeTrial()
{
	int intIndex = m_Trials->GetCurSel();
	if (intIndex == -1)
	{
		intIndex = 0;
	}
	CString strTemp;
	m_TextBox[0]->SetWindowTextW(strToLPCTSTR(arrTrials[intIndex].name));
	strTemp.Format(_T("%d"), (arrTrials[intIndex].numberOfTeams));
	m_TextBox[1]->SetWindowTextW(strTemp);
	strTemp.Format(_T("%d"), (arrTrials[intIndex].heroesPerTeam));
	m_TextBox[2]->SetWindowTextW(strTemp);
	m_ComboBox[0]->SetCurSel(arrTrials[intIndex].race);
	m_ComboBox[1]->SetCurSel(arrTrials[intIndex].losingCosts);
	m_ComboBox[2]->SetCurSel(arrTrials[intIndex].notFirstCosts);
	strTemp.Format(_T("%g"), (arrTrials[intIndex].luck));
	m_TextBox[3]->SetWindowTextW(strTemp);
	for (int intIter = 0; intIter < 6; intIter = intIter + 1)
	{
		strTemp.Format(_T("%d"), arrTrials[intIndex].rewards[intIter]);
		m_TextBox[intIter + 4]->SetWindowTextW(strTemp);
	}
	strTemp.Format(_T("%d"), (arrTrials[intIndex].winnerBonus));
	m_TextBox[10]->SetWindowTextW(strTemp);
	m_ComboBox[3]->SetCurSel(arrTrials[intIndex].optionalGold);
	strTemp.Format(_T("%d"), (arrTrials[intIndex].optionalGoldBias));
	m_TextBox[11]->SetWindowTextW(strTemp);
	m_ComboBox[4]->SetCurSel(arrTrials[intIndex].loserKeepsReward);
	/*switch (arrChallenges[intIndex].maximumFailures)
	{
	case "0": m_ComboBox[5]->SetCurSel(0); break;
	case "1": m_ComboBox[5]->SetCurSel(1); break;
	case "few": m_ComboBox[5]->SetCurSel(2); break;
	case "many": m_ComboBox[5]->SetCurSel(3); break;
	case "all": m_ComboBox[5]->SetCurSel(4); break;
	}*/
	if (arrTrials[intIndex].maximumFailures == "0")
	{
		m_ComboBox[5]->SetCurSel(0);
	}
	else if (arrTrials[intIndex].maximumFailures == "1")
	{
		m_ComboBox[5]->SetCurSel(1);
	}
	else if (arrTrials[intIndex].maximumFailures == "few")
	{
		m_ComboBox[5]->SetCurSel(2);
	}
	else if (arrTrials[intIndex].maximumFailures == "many")
	{
		m_ComboBox[5]->SetCurSel(3);
	}
	else if (arrTrials[intIndex].maximumFailures == "all")
	{
		m_ComboBox[5]->SetCurSel(4);
	}

	strTemp.Format(_T("%g"), (arrTrials[intIndex].medFailureRisk));
	m_TextBox[12]->SetWindowTextW(strTemp);
	strTemp.Format(_T("%g"), (arrTrials[intIndex].badFailureRisk));
	m_TextBox[13]->SetWindowTextW(strTemp);
	m_TextBox[14]->SetWindowTextW(strToLPCTSTR(arrTrials[intIndex].failureDescription[0]));
	m_TextBox[15]->SetWindowTextW(strToLPCTSTR(arrTrials[intIndex].failureDescription[1]));
	for (int intIter = 0; intIter < 12; intIter = intIter + 1)
	{
		strTemp.Format(_T("%g"), arrTrials[intIndex].skills[intIter]);
		m_TextBox[intIter + 16]->SetWindowTextW(strTemp);
	}
	for (int intIter = 0; intIter < 7; intIter = intIter + 1)
	{
		strTemp.Format(_T("%g"), arrTrials[intIndex].fears[intIter]);
		m_TextBox[intIter + 28]->SetWindowTextW(strTemp);
	}
	for (int intIter = 0; intIter < 3; intIter = intIter + 1)
	{
		strTemp.Format(_T("%d"), arrTrials[intIndex].time[intIter]);
		m_TextBox[intIter + 35]->SetWindowTextW(strTemp);
	}
}

void CDlgTrials::SaveChanges()
{
	int intIndex = m_Trials->GetCurSel();
	CString strTemp;
	m_TextBox[0]->GetWindowTextW(strTemp);
	arrTrials[intIndex].name = CStringToStr(strTemp);
	m_TextBox[1]->GetWindowTextW(strTemp);
	arrTrials[intIndex].numberOfTeams = stoi(CStringToStr(strTemp));
	m_TextBox[2]->GetWindowTextW(strTemp);
	arrTrials[intIndex].heroesPerTeam = stoi(CStringToStr(strTemp));
	arrTrials[intIndex].race = m_ComboBox[0]->GetCurSel();
	arrTrials[intIndex].losingCosts = m_ComboBox[1]->GetCurSel();
	arrTrials[intIndex].notFirstCosts = m_ComboBox[2]->GetCurSel();
	m_TextBox[3]->GetWindowTextW(strTemp);
	arrTrials[intIndex].luck = stof(CStringToStr(strTemp));
	for (int intIter = 0; intIter < 6; intIter = intIter + 1)
	{
		m_TextBox[intIter + 4]->GetWindowTextW(strTemp);
		arrTrials[intIndex].rewards[intIter] = stoi(CStringToStr(strTemp));
	}
	m_TextBox[10]->GetWindowTextW(strTemp);
	arrTrials[intIndex].winnerBonus = stoi(CStringToStr(strTemp));
	arrTrials[intIndex].optionalGold = m_ComboBox[3]->GetCurSel();
	m_TextBox[11]->SetWindowTextW(strTemp);
	arrTrials[intIndex].optionalGoldBias = stoi(CStringToStr(strTemp));
	arrTrials[intIndex].loserKeepsReward = m_ComboBox[4]->GetCurSel();
	switch (m_ComboBox[5]->GetCurSel())
	{
	case 0:arrTrials[intIndex].maximumFailures = "0"; break;
	case 1:arrTrials[intIndex].maximumFailures = "1"; break;
	case 2:arrTrials[intIndex].maximumFailures = "few"; break;
	case 3:arrTrials[intIndex].maximumFailures = "many"; break;
	case 4:arrTrials[intIndex].maximumFailures = "all"; break;
	}
	m_TextBox[12]->GetWindowTextW(strTemp);
	arrTrials[intIndex].medFailureRisk = stof(CStringToStr(strTemp));
	m_TextBox[13]->GetWindowTextW(strTemp);
	arrTrials[intIndex].badFailureRisk = stof(CStringToStr(strTemp));
	m_TextBox[14]->GetWindowTextW(strTemp);
	arrTrials[intIndex].failureDescription[0] = CStringToStr(strTemp);
	m_TextBox[15]->GetWindowTextW(strTemp);
	arrTrials[intIndex].failureDescription[1] = CStringToStr(strTemp);
	for (int intIter = 0; intIter < 12; intIter = intIter + 1)
	{
		m_TextBox[intIter + 16]->GetWindowTextW(strTemp);
		arrTrials[intIndex].skills[intIter] = stof(CStringToStr(strTemp));
	}
	for (int intIter = 0; intIter < 7; intIter = intIter + 1)
	{
		m_TextBox[intIter + 28]->GetWindowTextW(strTemp);
		arrTrials[intIndex].fears[intIter] = stof(CStringToStr(strTemp));
	}
	for (int intIter = 0; intIter < 3; intIter = intIter + 1)
	{
		m_TextBox[intIter + 35]->GetWindowTextW(strTemp);
		arrTrials[intIndex].time[intIter] = stof(CStringToStr(strTemp));
	}
	RefreshList();
}

void CDlgTrials::DeleteTrial()
{
	int intIndex = m_Trials->GetCurSel();
	for (int intIter = intIndex; intIter < INT_MAXTRIALS - 1; intIter = intIter + 1)
	{
		if (arrTrials[intIndex].name == "None" || arrTrials[intIndex].name == "")
		{
			break;
		}
		else
		{
			arrTrials[intIter] = arrTrials[intIter + 1];
		}
	}
	if (arrTrials[INT_MAXTRIALS - 1].name != "None" && arrTrials[INT_MAXTRIALS - 1].name != "")
	{
		//AfxMessageBox(strToLPCTSTR(arrTrials[82].name));
		arrTrials[INT_MAXTRIALS - 1].name = "None";
	}

	m_Trials->DeleteString(intIndex - 1);
}

void CDlgTrials::LoadImportWindow()
{
	const TCHAR szFilter[] = _T("CSV Files (*.csv)|*.csv|All Files (*.*)|*.*||");
	CFileDialog m_FileDialog(true, _T("csv"), NULL, OFN_HIDEREADONLY, szFilter, this);
	CString strFilename;

	if (m_FileDialog.DoModal() == IDOK)
	{
		strFilename = m_FileDialog.GetPathName();

		m_Load->ModifyStyle(0, WS_VISIBLE);
		m_Accept->ModifyStyle(0, WS_VISIBLE);
		m_Cancel->ModifyStyle(0, WS_VISIBLE);
		m_SelectAll->ModifyStyle(0, WS_VISIBLE);
		m_SelectNone->ModifyStyle(0, WS_VISIBLE);
		m_Trials->ModifyStyle(0, WS_DISABLED);
		m_Import->ModifyStyle(0, WS_DISABLED);
		m_Delete->ModifyStyle(0, WS_DISABLED);
		m_New->ModifyStyle(0, WS_DISABLED);
		m_Export->ModifyStyle(0, WS_DISABLED);
		for (int intIter = 0; intIter < 6; intIter = intIter + 1)
		{
			m_Labels[intIter]->ModifyStyle(WS_VISIBLE, 0);
			m_TextBox[intIter]->ModifyStyle(WS_VISIBLE, 0);
			m_ComboBox[intIter]->ModifyStyle(WS_VISIBLE, 0);
		}
		for (int intIter = 6; intIter < 38; intIter = intIter + 1)
		{
			m_Labels[intIter]->ModifyStyle(WS_VISIBLE, 0);
			m_TextBox[intIter]->ModifyStyle(WS_VISIBLE, 0);
		}
		for (int intIter = 38; intIter < 48; intIter = intIter + 1)
		{
			m_Labels[intIter]->ModifyStyle(WS_VISIBLE, 0);
		}
		m_Save->ModifyStyle(WS_VISIBLE, 0);

		this->RedrawWindow();

		for (int intIter = 0; intIter < INT_MAXTRIALS; intIter = intIter + 1)
		{
			arrImport[intIter][0] = "";
		}

		importCSV(&arrImport[0][0], CStringToStr(strFilename));
		for (int intIter = 1; intIter < INT_MAXTRIALS + 1; intIter = intIter + 1)
		{
			if (arrImport[intIter][0] == "")
			{
				break;
			}
			else
			{
				m_Load->AddString(strToLPCTSTR(arrImport[intIter][0]));
			}
		}
	}
}

void CDlgTrials::ResetState()
{
	m_Load->ModifyStyle(WS_VISIBLE, 0);
	m_Accept->ModifyStyle(WS_VISIBLE, 0);
	m_Cancel->ModifyStyle(WS_VISIBLE, 0);
	m_SelectAll->ModifyStyle(WS_VISIBLE, 0);
	m_SelectNone->ModifyStyle(WS_VISIBLE, 0);
	m_Trials->ModifyStyle(WS_DISABLED, 0);
	m_Import->ModifyStyle(WS_DISABLED, 0);
	m_Delete->ModifyStyle(WS_DISABLED, 0);
	m_New->ModifyStyle(WS_DISABLED, 0);
	m_Export->ModifyStyle(WS_DISABLED, 0);
	for (int intIter = 0; intIter < 6; intIter = intIter + 1)
	{
		m_Labels[intIter]->ModifyStyle(0, WS_VISIBLE);
		m_TextBox[intIter]->ModifyStyle(0, WS_VISIBLE);
		m_ComboBox[intIter]->ModifyStyle(0, WS_VISIBLE);
	}
	for (int intIter = 6; intIter < 38; intIter = intIter + 1)
	{
		m_Labels[intIter]->ModifyStyle(0, WS_VISIBLE);
		m_TextBox[intIter]->ModifyStyle(0, WS_VISIBLE);
	}
	for (int intIter = 38; intIter < 48; intIter = intIter + 1)
	{
		m_Labels[intIter]->ModifyStyle(0, WS_VISIBLE);
	}
	m_Save->ModifyStyle(0, WS_VISIBLE);
	//Update screen
	this->RedrawWindow();
	m_Load->ResetContent();
}

void CDlgTrials::Import()
{
	int arrSelect[INT_MAXTRIALS + 1];
	int intCount = 0;
	for (int intIter = 0; intIter < INT_MAXTRIALS + 1; intIter = intIter + 1)
	{
		if (m_Load->GetCheck(intIter) == BST_CHECKED)
		{
			arrSelect[intCount] = intIter + 1;
			intCount = intCount + 1;
		}
	}
	int intFirst = 0;
	while (arrTrials[intFirst].name != "None" && arrTrials[intFirst].name != "")
	{
		intFirst = intFirst + 1;
	}
	if (intFirst + intCount > INT_MAXTRIALS)
	{
		AfxMessageBox(L"Too many trials selected.");
	}
	else
	{
		importChallengesGUI(arrTrials, arrImport, arrSelect, intFirst, intCount);
		RefreshList();
		ResetState();
	}
}

void CDlgTrials::NewTrial()
{
	int intIter = 0;
	while (arrTrials[intIter].name != "None" && arrTrials[intIter].name != "")
	{
		intIter = intIter + 1;
	}
	if (intIter < INT_MAXTRIALS)
	{
		arrTrials[intIter].name = "Trial";
		arrTrials[intIter].numberOfTeams = 1;
		arrTrials[intIter].heroesPerTeam = 1;
		arrTrials[intIter].race = false;
		arrTrials[intIter].losingCosts = false;
		arrTrials[intIter].notFirstCosts = false;
		arrTrials[intIter].luck = 0;
		for (int intIter = 0; intIter < 6; intIter = intIter + 1)
		{
			arrTrials[intIter].rewards[intIter] = 0;
		}
		arrTrials[intIter].winnerBonus = 0;
		arrTrials[intIter].optionalGold = false;
		arrTrials[intIter].optionalGoldBias = 0;
		arrTrials[intIter].loserKeepsReward = false;
		arrTrials[intIter].maximumFailures = "0";
		arrTrials[intIter].medFailureRisk = 0;
		arrTrials[intIter].badFailureRisk = 0;
		arrTrials[intIter].failureDescription[0] = "";
		arrTrials[intIter].failureDescription[1] = "";
		for (int intIter = 0; intIter < 12; intIter = intIter + 1)
		{
			arrTrials[intIter].skills[intIter] = 0;
		}
		for (int intIter = 0; intIter < 7; intIter = intIter + 1)
		{
			arrTrials[intIter].fears[intIter] = 0;
		}
		for (int intIter = 0; intIter < 3; intIter = intIter + 1)
		{
			arrTrials[intIter].time[intIter] = 0;
		}
	}
	else
	{
		AfxMessageBox(L"Maximum number of trials has been reached. Please delete a challenge before adding more.");
	}
	RefreshList();

}

void CDlgTrials::SelectAll()
{
	int intLength = m_Load->GetCount();
	for (int intIter = 0; intIter < intLength; intIter = intIter + 1)
	{
		m_Load->SetCheck(intIter, 1);
	}
}

void CDlgTrials::SelectNone()
{
	int intLength = m_Load->GetCount();
	for (int intIter = 0; intIter < intLength; intIter = intIter + 1)
	{
		m_Load->SetCheck(intIter, 0);
	}
}

void CDlgTrials::Export()
{
	int intCount = 0;
	const TCHAR szFilter[] = _T("CSV Files (*.csv)|*.csv|All Files (*.*)|*.*||");
	CFileDialog m_FileDialog(false, _T("csv"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
	CString strFilename;

	if (m_FileDialog.DoModal() == IDOK)
	{
		strFilename = m_FileDialog.GetPathName();

		arrImport[0][0] = "Name";
		arrImport[0][1] = "How many teams?";
		arrImport[0][2] = "Heroes per team";
		arrImport[0][3] = "Race";
		arrImport[0][4] = "Last means failure";
		arrImport[0][5] = "Not first means failure";
		arrImport[0][6] = "Skills relevance";
		arrImport[0][7] = "height";
		arrImport[0][8] = "weight";
		arrImport[0][9] = "accuracy";
		arrImport[0][10] = "patience";
		arrImport[0][11] = "courage";
		arrImport[0][12] = "stamina";
		arrImport[0][13] = "strength";
		arrImport[0][14] = "agility";
		arrImport[0][15] = "speed";
		arrImport[0][16] = "teamwork";
		arrImport[0][17] = "wisdom";
		arrImport[0][18] = "leadership";
		arrImport[0][19] = "fears";
		arrImport[0][20] = "dark";
		arrImport[0][21] = "death";
		arrImport[0][22] = "enclosed spaces";
		arrImport[0][23] = "heights";
		arrImport[0][24] = "water";
		arrImport[0][25] = "snakes";
		arrImport[0][26] = "spiders";
		arrImport[0][27] = "Luck";
		arrImport[0][28] = "luck (maximum effect)";
		arrImport[0][29] = "Risk";
		arrImport[0][30] = "How many can fail";
		arrImport[0][31] = "Chance of losing a life (score==7)";
		arrImport[0][32] = "Chance of losing a life (score==4)";
		arrImport[0][33] = "Description";
		arrImport[0][34] = "";
		arrImport[0][35] = "Rewards - max rings per warrior";
		arrImport[0][36] = "1st";
		arrImport[0][37] = "2nd";
		arrImport[0][38] = "3rd";
		arrImport[0][39] = "4th";
		arrImport[0][40] = "5th";
		arrImport[0][41] = "6th";
		arrImport[0][42] = "winner's bonus (e.g. final ring)";
		arrImport[0][43] = "Optional rewards";
		arrImport[0][44] = "Optional gold bias";
		arrImport[0][45] = "Race-loser keeps optional reward";
		arrImport[0][46] = "Time taken in race";
		arrImport[0][47] = "very fast time";
		arrImport[0][48] = "average time";
		arrImport[0][49] = "very slow time";


		while (arrTrials[intCount].name != "None" && arrTrials[intCount].name != "")
		{
			arrImport[intCount][0] = arrTrials[intCount].name;
			arrImport[intCount][1] = to_string(arrTrials[intCount].numberOfTeams);
			arrImport[intCount][2] = to_string(arrTrials[intCount].heroesPerTeam);
			arrImport[intCount][3] = to_string(arrTrials[intCount].race);
			arrImport[intCount][4] = to_string(arrTrials[intCount].losingCosts);
			arrImport[intCount][5] = to_string(arrTrials[intCount].notFirstCosts);
			for (int intSkill = 0; intSkill < 12; intSkill = intSkill + 1)
			{
				arrImport[intCount][7 + intSkill] = to_string(arrTrials[intCount].skills[intSkill]);
			}
			for (int intFear = 0; intFear < 7; intFear = intFear + 1)
			{
				arrImport[intCount][20 + intFear] = to_string(arrTrials[intCount].fears[intFear]);
			}
			arrImport[intCount][28] = to_string(arrTrials[intCount].luck);
			arrImport[intCount][30] = arrTrials[intCount].maximumFailures;
			arrImport[intCount][31] = to_string(arrTrials[intCount].medFailureRisk);
			arrImport[intCount][32] = to_string(arrTrials[intCount].badFailureRisk);
			arrImport[intCount][33] = arrTrials[intCount].failureDescription[0];
			arrImport[intCount][34] = arrTrials[intCount].failureDescription[1];
			for (int intReward = 0; intReward < 6; intReward = intReward + 1)
			{
				arrImport[intCount][36 + intReward] = to_string(arrTrials[intCount].rewards[intReward]);
			}
			arrImport[intCount][42] = to_string(arrTrials[intCount].winnerBonus);
			arrImport[intCount][43] = to_string(arrTrials[intCount].optionalGold);
			arrImport[intCount][44] = to_string(arrTrials[intCount].optionalGoldBias);
			arrImport[intCount][45] = to_string(arrTrials[intCount].loserKeepsReward);
			for (int intTime = 1; intTime < 4; intTime = intTime + 1)
			{
				arrImport[intCount][46 + intTime] = to_string(arrTrials[intCount].time[intTime]);
			}


			intCount = intCount + 1;
		}
		writeToCSV(&arrImport[0][0], 50, intCount, CStringToStr(strFilename));
	}
}

// CDlgSettings dialog

CDlgSettings::CDlgSettings(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHILD, pParent)
{
	Create(IDD_CHILD, pParent);
}

void CDlgSettings::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlgSettings, CDialogEx)
	ON_WM_SIZE()
	ON_BN_CLICKED(1379, Apply)
END_MESSAGE_MAP()


// CDlgSettings message handlers

BOOL CDlgSettings::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CString strTemp;
	for (int intIter = 0; intIter < 7; intIter = intIter + 1)
	{
		m_Labels[intIter] = new(CEdit);
		m_Labels[intIter]->Create(WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_TABSTOP | ES_RIGHT | WS_DISABLED, CRect(1, 1, 2, 2), this, 1365 + intIter);
	}
	m_Labels[0]->SetWindowTextW(L"Gold Per Life");
	m_Labels[1]->SetWindowTextW(L"Fear Impairance");
	m_Labels[2]->SetWindowTextW(L"Fear Threshold");
	m_Labels[3]->SetWindowTextW(L"Failure model");
	m_Labels[4]->SetWindowTextW(L"Limit number of failures");
	m_Labels[5]->SetWindowTextW(L"Equalise heroes");
	m_Labels[6]->SetWindowTextW(L"Detailed results screen");
	for (int intIter = 0; intIter < 3; intIter = intIter + 1)
	{
		m_TextBox[intIter] = new(CEdit);
		m_TextBox[intIter]->Create(WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_TABSTOP | ES_LEFT, CRect(1, 1, 2, 2), this, 1372 + intIter);
	}
	strTemp.Format(_T("%d"), (intGoldPerLife));
	m_TextBox[0]->SetWindowTextW(strTemp);
	strTemp.Format(_T("%f"), (floatFearImpairance));
	m_TextBox[1]->SetWindowTextW(strTemp);
	strTemp.Format(_T("%f"), (floatThreshold));
	m_TextBox[2]->SetWindowTextW(strTemp);
	m_ComboBox[0] = new(CComboBox);
	m_ComboBox[0]->Create(WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_TABSTOP | CBS_DROPDOWNLIST, CRect(1, 1, 2, 2), this, 1375);
	m_ComboBox[0]->AddString(L"1");
	m_ComboBox[0]->AddString(L"2");
	for (int intIter = 1; intIter < 4; intIter = intIter + 1)
	{
		m_ComboBox[intIter] = new(CComboBox);
		m_ComboBox[intIter]->Create(WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_TABSTOP | CBS_DROPDOWNLIST, CRect(1, 1, 2, 2), this, 1375 + intIter);
		m_ComboBox[intIter]->AddString(L"FALSE");
		m_ComboBox[intIter]->AddString(L"TRUE");
	}
	m_ComboBox[0]->SetCurSel(intFailureModel - 1);
	m_ComboBox[1]->SetCurSel(boolLimitFailures);
	m_ComboBox[2]->SetCurSel(boolNormalise);
	m_ComboBox[3]->SetCurSel(boolDetailedResults);

	m_Button = new(CButton);
	m_Button->Create(L"Apply", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(1, 1, 2, 2), this, 1379);

	boolInit = true;
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDlgSettings::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	CRect rectTemp;
	GetWindowRect(rectTemp);
	//ScreenToClient(rectTemp);
	int intWidth = rectTemp.right - rectTemp.left;
	int intHeight = rectTemp.bottom - rectTemp.top;
	if (boolInit)
	{
		for (int intIter = 0; intIter < 7; intIter = intIter + 1)
		{
			m_Labels[intIter]->SetWindowPos(NULL, intWidth / 8, intHeight * (1 + 3 * intIter) / 25, intWidth * 5 / 16, intHeight * 2 / 25, 0);
		}
		for (int intIter = 0; intIter < 3; intIter = intIter + 1)
		{
			m_TextBox[intIter]->SetWindowPos(NULL, intWidth * 9 / 16, intHeight * (1 + 3 * intIter) / 25, intWidth * 5 / 16, intHeight * 2 / 25, 0);
		}
		for (int intIter = 0; intIter < 4; intIter = intIter + 1)
		{
			m_ComboBox[intIter]->SetWindowPos(NULL, intWidth * 9 / 16, intHeight * (10 + 3 * intIter) / 25, intWidth * 5 / 16, intHeight * 2 / 25, 0);
		}
		m_Button->SetWindowPos(NULL, intWidth / 8, intHeight * 22 / 25, intWidth * 3 / 4, intHeight * 2 / 25, 0);
	}
}

void CDlgSettings::Apply()
{
	CString strTemp;
	string strOut = "";
	m_TextBox[0]->GetWindowTextW(strTemp);
	intGoldPerLife = stoi(CStringToStr(strTemp));
	strOut = strOut + CStringToStr(strTemp) + '\n';
	m_TextBox[1]->GetWindowTextW(strTemp);
	floatFearImpairance = stof(CStringToStr(strTemp));
	strOut = strOut + CStringToStr(strTemp) + '\n';
	m_TextBox[2]->GetWindowTextW(strTemp);
	floatThreshold = stof(CStringToStr(strTemp));
	strOut = strOut + CStringToStr(strTemp) + '\n';
	intFailureModel = m_ComboBox[0]->GetCurSel() + 1;
	strOut = strOut + to_string(m_ComboBox[0]->GetCurSel() + 1) + '\n';
	boolLimitFailures = m_ComboBox[1]->GetCurSel();
	strOut = strOut + to_string(m_ComboBox[1]->GetCurSel()) + '\n';
	boolNormalise = m_ComboBox[2]->GetCurSel();
	strOut = strOut + to_string(m_ComboBox[2]->GetCurSel()) + '\n';
	boolDetailedResults = m_ComboBox[3]->GetCurSel();
	strOut = strOut + to_string(m_ComboBox[3]->GetCurSel()) + '\n';
	strOut = strOut + strHeroDirectory + '\n' + strTrialDirectory;
	writeToTxt(strOut, experimental::filesystem::current_path().string() + "\\settings.ini");
}
