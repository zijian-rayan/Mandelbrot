
// ShowFract.h�: fichier d'en-t�te principal pour l'application ShowFract
//
#pragma once

#ifndef __AFXWIN_H__
	#error "incluez 'stdafx.h' avant d'inclure ce fichier pour PCH"
#endif

#include "resource.h"       // symboles principaux


// CShowFractApp:
// Consultez ShowFract.cpp pour l'impl�mentation de cette classe
//

class CShowFractApp : public CWinApp
{
public:
	CShowFractApp();


// Substitutions
public:
	virtual BOOL InitInstance();

// Impl�mentation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CShowFractApp theApp;
