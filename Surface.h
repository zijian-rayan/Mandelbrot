// Surface.h : Defines the interface of a DIB surface
#pragma once
#ifndef _SURFACE_H
#define _SURFACE_H

#include <AtlImage.h>
#include <omp.h>
#include <xmmintrin.h>
#include <mmintrin.h>
#include <emmintrin.h>
#include <smmintrin.h>

template<class T> T ABS(T x) { return(x < 0 ? -x : x); }

inline void SeedRandom(int nSeed = 0) { srand(nSeed ? nSeed : ::GetTickCount()); }
inline int Random() { return(rand()); }
inline int Random(int n) { return(rand() % n ); }

class CChrono
{
  LARGE_INTEGER f, t0;
public:
  CChrono()
  {
    ::QueryPerformanceFrequency(&f);
    ::QueryPerformanceCounter(&t0);
  }
  void Reset() {::QueryPerformanceCounter(&t0);}
  DWORD GetDelayMs()
  {
    LARGE_INTEGER t1;
    ::QueryPerformanceCounter(&t1);
    return DWORD((1000*(t1.QuadPart-t0.QuadPart))/f.QuadPart);
  }
  DWORD GetDelayUs()
  {
    LARGE_INTEGER t1;
    ::QueryPerformanceCounter(&t1);
    return DWORD((1000000*(t1.QuadPart-t0.QuadPart))/f.QuadPart);
  }
  void GetDelayMs(TCHAR* str, DWORD div=1) { wsprintf(str,_T("Delai = %lu ms"),GetDelayMs()/div); }
  void GetDelayUs(TCHAR* str, DWORD div=1) { wsprintf(str,_T("Delai = %lu us"),GetDelayUs()/div); }
  void ShowDelayMs(DWORD div=1)
  {
    TCHAR buf[128];
    GetDelayMs(buf,div);
    ::MessageBox(0,buf,_T("Chrono"),MB_OK);
  }
  void ShowDelayUs(DWORD div=1)
  {
    TCHAR buf[128];
    GetDelayUs(buf,div);
    ::MessageBox(0,buf,_T("Chrono"),MB_OK);
  }
};

inline void PixFract(BYTE* pix_addr, float x0, float y0)
{
  int idx = 0;
 
  float zix = x0, ziy = y0;//, zi2x = zix*zix, zi2y = ziy*ziy;
  float z[4];

   __m128 zixintr,ziyintr,zi2;
   zixintr = _mm_set_ps(zix,ziy,0,0);
   ziyintr = _mm_set_ps(zix,ziy,0,0);
   zi2	=	_mm_mul_ps(zixintr,ziyintr);
   _mm_store_ps (z,zi2);
   float zi2x = z[3];
   float zi2y = z[2];


  const int limit = 512;
  while((zi2x+zi2y)<= 4 && idx < limit)
  {
     ++idx;
      //ziy = 2*zix*ziy +y0;
    zixintr = _mm_set_ps(zix,zix,y0,0);
    ziyintr = _mm_set_ps(ziy,ziy,1,0);
    zi2=_mm_dp_ps (zixintr,ziyintr,232);
    _mm_store_ps (z,zi2);
    ziy = z[3];

	   zix = zi2x - zi2y +x0; 

	  zixintr = _mm_set_ps(zix,ziy,0,0);
    ziyintr = _mm_set_ps(zix,ziy,0,0);
    zi2	=	_mm_mul_ps(zixintr,ziyintr);
    _mm_store_ps (z,zi2);

     zi2x = z[3];
     zi2y = z[2];
	 //zi2x = zix*zix;
	 //zi2y = ziy*ziy;
	}
  //*pix_addr=(((3*idx)%256)|(((5*idx)%256)<<8)|((7*idx)%256<<16));
    *pix_addr++ = (7*idx)%limit; *pix_addr++ = (5*idx)%limit; *pix_addr = (3*idx)%limit;
}


class CSurface
{
  CWnd *m_pSrcWnd;
  HDC m_hDestDC;
  CImage m_image;
  int m_nByteDepth;
  int m_nBitDepth;
  int m_wndWidth;
  int m_wndHeight;
public:
  CSurface() : m_pSrcWnd(NULL), m_hDestDC(NULL), m_wndHeight(0), m_wndWidth(0), m_nBitDepth(0), m_nByteDepth(0) { }
  virtual ~CSurface() { Destroy(); }

  void Create(CWnd *pWnd, int nBitDepth); // this is stored internally
  void Create(CWnd *pWnd, int nBitDepth, int w, int h);
  void Destroy();
  void Import(const CImage &image);

  void ClearBits()
  {
    int size = m_image.GetPitch() * m_image.GetHeight();
    memset(m_image.GetBits(), 0x00, size);
  }
  void StripeBits();
  void RandomBits();
  void ShiftBits();
  void BlitBits()
  {
    ASSERT(m_wndHeight && m_wndWidth);
    BOOL bStat = m_image.BitBlt(m_hDestDC, 0, 0, m_wndWidth, m_wndHeight, 0, 0);
    ASSERT(bStat);
  }
  virtual void BlurBits(); // this is where the MMX action is

  void Line(const CPoint &p1, const CPoint &p2, COLORREF c);
  void RandomLine(COLORREF c);

  void PointColor(int x, int y, COLORREF c)
  {
    // m_image.SetPixel() call ::SetPixel() which is too slow since it has to work with all DCs.
    BYTE *p = (BYTE*)m_image.GetPixelAddress(x, y);
    if (m_nBitDepth == 16)  *(WORD *)p = (WORD)(((c&0xf80000) >> 19) | ((c&0xf800) >> 6) | ((c&0xf8) << 7));
    else { *p++ = GetBValue(c); *p++ = GetGValue(c); *p = GetRValue(c); }
  }
// Somewhat poor form - for performance reasons these come back with R and B exchanged.
  COLORREF PointColor(int x, int y) const
  { 
    // m_image.GetPixel() calls ::GetPixel() which is too slow since it has to work for all types of DCs.
    if (m_nBitDepth == 16)
    {
      COLORREF c = (COLORREF)*(WORD*)m_image.GetPixelAddress(x, y);
      return((c&0x7c00) << 9 | (c&0x3e0) << 6 | (c&0x1f) << 3);		
    }
    else return(*(COLORREF*)(m_image.GetPixelAddress(x, y))); 
  }
  BYTE *GetPixelAddress(int x, int y) const { return((BYTE*)m_image.GetPixelAddress(x, y)); }

  bool IsNull() const { return m_image.IsNull(); }
  int GetVisibleWidth() const { return m_wndWidth; }
  int GetVisibleHeight() const { return m_wndHeight; }
  int GetPitch() const { return m_image.GetPitch(); }
  int GetBitDepth() const { return m_nBitDepth; }
  int GetByteDepth() const { return m_nByteDepth; }
  CImage *GetImage() { return(&m_image); }

  void CalcFractMonoThread(float x0, float y0, float dimx, float dimy)
  {
	  for(int i =0; i < m_wndWidth; ++i)
	  {	  for(int j = 0; j < m_wndHeight; ++j)
		  {
			  PixFract((BYTE*)m_image.GetPixelAddress(i, j),x0+i*dimx,y0+j*dimy);
			  
		  }
	  }	
  }
  
  //--------------------------------------------------------------V1---------------------------------------------------------
  struct ShData //commune
  {
    DWORD *pstart,szX,ypitch;
    float x0,y0,dimx,dimy;
  };
    struct ThData //commune
  {
    DWORD szY,yofs;
    ShData* pSh;
  };

  static DWORD WINAPI Th1(void* arg)
  {
    DWORD *pixCour = 0;
    ThData* thdata=(ThData*) arg;
    
	  for(unsigned short i = 0;i<thdata->szY;i++)
	  {
		  for(unsigned short j=0;j<thdata->pSh->szX;j++)
		  {
			  DWORD* pxlAddr = thdata->pSh->pstart+((i+thdata->yofs)*(thdata->pSh->ypitch/4)+j);
			  pixCour = pxlAddr;
			  PixFract((BYTE*)pxlAddr,
				  thdata->pSh->x0 + j*thdata->pSh->dimx,
				  thdata->pSh->y0 + (thdata->yofs+i)*thdata->pSh->dimy);
		  }
	  }
	return *pixCour;
  } 

  void CalcFractMultiThV1(float x0, float y0, float dimx, float dimy)
  {
    ShData shdata={(DWORD*)GetPixelAddress(0,0),m_wndWidth,GetPitch(),x0,y0,dimx,dimy};
    SYSTEM_INFO si; 
	::GetSystemInfo(&si);
    DWORD nth = si.dwNumberOfProcessors;
    ThData *thdata=new ThData[nth];
    HANDLE *hth = new HANDLE[nth];
    for(DWORD i=0;i<nth;i++)
    {
		
		//MessageBox(NULL, LPCWSTR("Hello, Windows!"),LPCWSTR("hello"), MB_OK );
      thdata[i].pSh=&shdata;
      thdata[i].yofs=i*m_wndHeight/nth;
      thdata[i].szY=(i+1)*m_wndHeight/nth-i*m_wndHeight/nth;
      hth[i]=::CreateThread(0,0,Th1,thdata+i,0,0);
      ::WaitForMultipleObjects(nth,hth,TRUE,INFINITE);//infinite      
    }
    for(DWORD i=0;i<nth;i++)
		::CloseHandle(hth[i]);
    delete[] hth;
    delete[] thdata;
 }
  //--------------------------------------------------------------V2---------------------------------------------------------
    struct ShData2 //commune
  {
    LONG ynext;
    DWORD *pstart,szX,szY,ypitch;
    float x0,y0,dimx,dimy;
  };
static DWORD WINAPI Th2(void* arg)
  {
    ShData2* shdata=(ShData2*) arg;
    DWORD y;
    y= _InterlockedIncrement(&(shdata->ynext));
	while((y= InterlockedIncrement(&shdata->ynext)-1)<shdata->szY)
	{
		DWORD* pxlAddr = shdata->pstart+(y*(shdata->ypitch/4));
	  for(DWORD x=0;x<shdata->szX;++x)
	  {
		  
		  PixFract((BYTE*)pxlAddr+x,
			  shdata->x0 + x*shdata->dimx,
			  shdata->y0 + y*shdata->dimy);
	  }
	}
	return 0;
  } 

 void CalcFractMultiThV2(float x0, float y0, float dimx, float dimy)
  {
    ShData2 shdata = {0,(DWORD*)GetPixelAddress(0,0),m_wndWidth,m_wndHeight,GetPitch(),x0,y0,dimx,dimy};
    SYSTEM_INFO si; 
	::GetSystemInfo(&si);
    DWORD nth = si.dwNumberOfProcessors;
    HANDLE *hth = new HANDLE[nth];
   for(DWORD i=0;i<nth;++i)
    {
      hth[i]=::CreateThread(0,0,Th2,&shdata,0,0);
     
    } 
   ::WaitForMultipleObjects(nth,hth,TRUE,INFINITE);
    for(DWORD i=0;i<nth;++i)
		::CloseHandle(hth[i]);
    delete[] hth;
 }

//--------------------------------------------------------------------------------V3------------------

  void CalcFractMultiOPMv1(float x0, float y0, float dimx, float dimy)
  {
    int nth=omp_get_num_procs();
    omp_set_num_threads(nth);
#pragma omp parallel
    {
      int ith=omp_get_thread_num();
      for(int y=(m_wndHeight*ith)/nth;y<(m_wndHeight*(ith+1))/nth;y++)
      {
          DWORD* ppix=(DWORD*)GetPixelAddress(0,y);
	        for(int j = 0; j < m_wndWidth; j++)
		          {
			          PixFract((BYTE*)ppix++,x0+j*dimx,y0+y*dimy);
        			  
		          }
      }
    }
	    	
  }
//--------------------------------------------------------------------------------V4------------------
  void CalcFractMultiOPMv2(float x0, float y0, float dimx, float dimy)
  {
    int nth=omp_get_num_procs();
    omp_set_num_threads(nth);
#pragma omp parallel
    {
#pragma omp for
      for(int idy=0;idy<m_wndHeight;idy++)
	        for(int idx=0;idx<m_wndWidth;idx++)
		          { DWORD* ppix=(DWORD*)GetPixelAddress(idx,idy);
			          PixFract((BYTE*)ppix,x0+idx*dimx,y0+idy*dimy);
        			  
		          }
      
    }
	    	
  }


};

#endif