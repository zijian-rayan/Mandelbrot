// ChildView.h : interface de la classe CChildView
#pragma once
#include "Surface.h"


// fen�tre CChildView
class CChildView : public CWnd
{
  CSurface *m_pSurf;
public:
	CChildView();

// Attributs
public:

// Op�rations
public:

// Substitutions
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Impl�mentation
public:
	virtual ~CChildView();

	// Fonctions g�n�r�es de la table des messages
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnCalcMonoThread();
  afx_msg void OnMultithread1();
  afx_msg void OnMultithread2();
  afx_msg void OnMultithread3();
  afx_msg void OnMultithread4();
};

