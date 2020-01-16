
// ShowFract.h : fichier d'en-tête principal pour l'application ShowFract
//
#pragma once

#ifndef __AFXWIN_H__
	#error "incluez 'stdafx.h' avant d'inclure ce fichier pour PCH"
#endif

#include "resource.h"       // symboles principaux


// CShowFractApp:
// Consultez ShowFract.cpp pour l'implémentation de cette classe
//

class CShowFractApp : public CWinApp
{
public:
	CShowFractApp();


// Substitutions
public:
	virtual BOOL InitInstance();

// Implémentation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CShowFractApp theApp;
