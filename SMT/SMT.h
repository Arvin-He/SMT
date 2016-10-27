
// SMT.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CSMTApp:
// See SMT.cpp for the implementation of this class
//

class CSMTApp : public CWinApp
{
public:
	CSMTApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CSMTApp theApp;