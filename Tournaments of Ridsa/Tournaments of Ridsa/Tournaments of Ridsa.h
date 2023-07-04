
// Tournaments of Ridsa.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CToRApp:
// See Tournaments of Ridsa.cpp for the implementation of this class
//

class CToRApp : public CWinApp
{
public:
	CToRApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CToRApp theApp;
