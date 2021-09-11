
// MFCApp1.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CMFCApp1App:
// See MFCApp1.cpp for the implementation of this class
//

class CMFCApp1App : public CWinApp
{
public:
	CMFCApp1App();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CMFCApp1App theApp;
