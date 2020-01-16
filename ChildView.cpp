
// ChildView.cpp : impl閙entation de la classe CChildView
//

#include "stdafx.h"
#include "ShowFract.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView
BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
  ON_WM_CREATE()
  ON_COMMAND(ID_MONOTHREAD, &CChildView::OnCalcMonoThread)
  ON_COMMAND(ID_CALCULER_MU, &CChildView::OnMultithread1)
  ON_COMMAND(ID_CALCULER_MULTITHREAD2, &CChildView::OnMultithread2)
  ON_COMMAND(ID_CALCULER_MULTITHREAD3, &CChildView::OnMultithread3)
  ON_COMMAND(ID_CALCULER_MULTITHREAD4, &CChildView::OnMultithread4)
END_MESSAGE_MAP()

CChildView::CChildView(): m_pSurf(0) {}
CChildView::~CChildView() { if(m_pSurf) delete m_pSurf; }

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs)) return FALSE;
	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // contexte de p閞iph閞ique pour la peinture
  if(m_pSurf) m_pSurf->BlitBits();
}


int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CWnd::OnCreate(lpCreateStruct) == -1) return -1;
  m_pSurf = new CSurface();
  m_pSurf->Create(this,32,1920,1080);
  m_pSurf->RandomBits();
  return 0;
}

void CChildView::OnCalcMonoThread()
{
  const DWORD N=1;
  CWaitCursor cw;
  CChrono c;
  for(int i=0;i<N;i++)
  {
	    m_pSurf->CalcFractMonoThread(-1.0f,-1.0f,0.001f,0.001f);
		Invalidate();
  }
  c.ShowDelayMs();
}


void CChildView::OnMultithread1()
{
	const DWORD N=1;
  CWaitCursor cw;
  CChrono c;
  for(int i=0;i<N;i++)
  {
		m_pSurf->CalcFractMultiThV1(-1.0f,-1.0f,0.001f,0.001f);
		Invalidate();
  }
  c.ShowDelayMs();
}

void CChildView::OnMultithread2()
{
	const DWORD N=1;
  CWaitCursor cw;
  CChrono c;
  for(int i=0;i<N;i++)
  {
		m_pSurf->CalcFractMultiThV2(-1.0f,-1.0f,0.001f,0.001f);
		Invalidate();
  }
  c.ShowDelayMs();
}

void CChildView::OnMultithread3()
{
	// TODO: 在此添加命令处理程序代码
  	const DWORD N=10;
  CWaitCursor cw;
  CChrono c;
  for(int i=0;i<N;i++)
  {
		m_pSurf->CalcFractMultiOPMv1(-1.0f,-1.0f,0.001f,0.001f);
		Invalidate();
  }
  c.ShowDelayMs();
}

void CChildView::OnMultithread4()
{
	// TODO: 在此添加命令处理程序代码
  	const DWORD N=1;
  CWaitCursor cw;
  CChrono c;
  for(int i=0;i<N;i++)
  {
		m_pSurf->CalcFractMultiOPMv2(-1.0f,-1.0f,0.001f,0.001f);
		Invalidate();
  }
  c.ShowDelayMs();
}
