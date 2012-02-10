/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2000-2005 Intel Corporation. All Rights Reserved.
//
*/

// ippsSignalDC.cpp: implementation of the CIppsSignalDC class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsSample.h"
#include "ippsSignalDC.h"
#include "ippsSignal.h"
#include <math.h>

#define ABS(r) ((r) >= 0 ? (r) : -(r))

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIppsSignalDC::CIppsSignalDC()
{
   m_pBitmap = NULL;
   m_pSignal = NULL;
   m_Points = NULL;
   m_PointsIm = NULL;
   m_Min = 0;
   m_Max = 0;
   m_Font.CreateFont(12, 0, 0, 0, FW_THIN, FALSE, FALSE, 0,
      ANSI_CHARSET, OUT_CHARACTER_PRECIS,  CLIP_CHARACTER_PRECIS,
      DEFAULT_QUALITY, DEFAULT_PITCH, NULL);
}

CIppsSignalDC::~CIppsSignalDC()
{
   FreePoints();
}

void CIppsSignalDC::Create(CDC* pDC, CIppsSignal* pSignal, int initialWidth)
{
   m_pSignal = pSignal;
   m_InitialWidth = initialWidth;
   if (!CreateCompatibleDC(pDC)) return;
   m_pBitmap = new CBitmap;
   if (!m_pBitmap->CreateCompatibleBitmap(pDC,
      GetWidth(), GetHeight())) return;
   SelectObject(m_pBitmap);
   AllocatePoints();
}

COLORREF CIppsSignalDC::GetColorSignal()     { return RGB(  0,  0,  0);}
COLORREF CIppsSignalDC::GetColorSignalBack() { return RGB(255,255,255);}
COLORREF CIppsSignalDC::GetColorAxis()       { return RGB(128,  0,  0);}
COLORREF CIppsSignalDC::GetColorAxisBack()   { return RGB(192,192,192);}
static int GetSpaceWidth() { return 50;}
static int GetSpaceHeight() { return 24;}
static int GetSignalHeight() { return 128;}
static int GetMinStepWidth() { return 60;}
static int GetMinStepHeight() { return 30;}

int CIppsSignalDC::GetWidth()
{
   int w = m_pSignal->Length() + GetSpaceWidth()*2;
   return (w < m_InitialWidth) ?
           w : m_InitialWidth;
}

int CIppsSignalDC::GetHeight()
{
   return (m_pSignal->Complex() ?
          (GetSignalHeight()*2 + GetSpaceHeight()*3) :
          (GetSignalHeight())  + GetSpaceHeight()*2);
}

double CIppsSignalDC::FactorX()
{
   return (double)(GetWidth() -  GetSpaceWidth()*2)
        / (double)m_pSignal->Length();
}

double CIppsSignalDC::FactorY()
{
   return (double)GetSignalHeight() / (m_Max - m_Min);
}

static void getConstFrame(double val, double& minVal, double& maxVal)
{
   if (val == 0) {
      minVal = -1;
      maxVal = 1;
      return;
   }
   double aval = val > 0 ? val : -val;
   double step = pow(10.0,(double)(int)log10(aval));
   double min = ((aval - step)/aval < 0.1) ? 0 : step;
   double max = 2*step;
   minVal = val > 0 ? min : -max;
   maxVal = val > 0 ? max : -min;
}

void CIppsSignalDC::SetMinMax()
{
   m_Min = m_pSignal->MinVal();
   m_Max = m_pSignal->MaxVal();
   if (m_pSignal->Unsign()) {
      if (m_Min <= m_Max*.5) m_Min = 0;
   } else {
      if (m_Min < 0 && m_Max > 0) {
         if (m_Max < -m_Min && m_Max >= -m_Min*.5)
            m_Max = -m_Min;
         if (-m_Min < m_Max && -m_Min >= m_Max*.5)
            m_Min = -m_Max;
      } else if (m_Min > 0) {
         if (m_Min <= m_Max*.5) m_Min = 0;
      } else if (m_Max < 0) {
         if (m_Max >= m_Min*.5) m_Max = 0;
      }
   }
   if (m_Min == m_Max)
      getConstFrame(m_Min, m_Min, m_Max);

}

double  CIppsSignalDC::Magnitude()
{
   double min = ABS(m_Min);
   double max = ABS(m_Max);
   return max > min ? max : min;
}

int CIppsSignalDC::GetMinY()
{
   return (int)(m_Min*FactorY() + .5);
}

int CIppsSignalDC::GetMaxY()
{
   return (int)(m_Max*FactorY() + .5);
}

////////////////////////////////////////////////////////////////////////////
// Operations

void CIppsSignalDC::AllocatePoints()
{
   FreePoints();
   m_NumPoints = (int)(m_pSignal->Length() * FactorX());
   m_Points = new POINT[m_NumPoints];
   if (m_pSignal->Complex()) {
      m_PointsIm = new POINT[m_NumPoints];
   }
}

void CIppsSignalDC::FreePoints()
{
   if (m_Points) delete[] m_Points;
   if (m_PointsIm) delete[] m_PointsIm;
}

void CIppsSignalDC::SetPoints()
{
   if (m_pSignal->Complex())
      SetPointsComplex();
   else
      SetPointsReal();
}

void CIppsSignalDC::SetPointsReal()
{
   double step = 1. / FactorX();
   double x = 0.5;
   for (int i=0; i < m_NumPoints; i++, x+=step) {
      Ipp64f value;
      m_pSignal->GetValue((int)x, value);
      value *= FactorY();
      m_Points[i].x = i + GetSpaceWidth();
      m_Points[i].y = -(int)(value + .5);
   }
}

void CIppsSignalDC::SetPointsComplex()
{
   double step = 1. / FactorX();
   double x = 0.5;
   for (int i=0; i < m_NumPoints; i++, x+=step) {
      Ipp64fc value;
      m_pSignal->GetValue((int)x, value);
      value.re *= FactorY();
      value.im *= FactorY();
      m_Points[i].x = i + GetSpaceWidth();
      m_Points[i].y = -(int)(value.re + .5);
      m_PointsIm[i].x = i + GetSpaceWidth();
      m_PointsIm[i].y = -(int)(value.im + .5);
   }
}


///////////////////////////////


void CIppsSignalDC::Draw()
{
   SetMinMax();
   SetPoints();
   if (m_pSignal->Complex())
      DrawComplex();
   else
      DrawReal();
}

int CIppsSignalDC::GetX0() { return GetSpaceWidth(); }
int CIppsSignalDC::GetX1() { return GetX0() + (int)(m_pSignal->Length() * FactorX() + .5);}
int CIppsSignalDC::GetZero() { return GetSpaceHeight() + GetMaxY();}
int CIppsSignalDC::GetZeroIm() {
   return GetSpaceHeight()*2 + GetMaxY()*2 - GetMinY(); }
int CIppsSignalDC::GetY0(int zero) { return zero - GetMinY();}
int CIppsSignalDC::GetY1(int zero) { return zero - GetMaxY();}

void CIppsSignalDC::DrawReal()
{
   DrawBackground();
   DrawSignalBox(GetZero(), m_Points);
}

void CIppsSignalDC::DrawComplex()
{
   DrawBackground();
   DrawSignalBox(GetZero(), m_Points);
   DrawSignalBox(GetZeroIm(), m_PointsIm, TRUE);
}

void CIppsSignalDC::DrawBackground()
{
   FillSolidRect(0, 0, GetWidth(), GetHeight(), GetColorAxisBack());
}

void CIppsSignalDC::DrawSignalBox(int zero, POINT* pPoints, BOOL bIm)
{
   DrawSignalBackground(zero);
   DrawAxisX(zero, bIm);
   DrawAxisY(zero);
   DrawSignal(zero, pPoints);
}

void CIppsSignalDC::DrawSignalBackground(int zero)
{
   CBrush brush(GetColorSignalBack());
   CPen   pen(PS_SOLID, 1, GetColorAxis());
   SelectObject(&pen);
   SelectObject(&brush);
   Rectangle(GetX0(),GetY0(zero),GetX1(),GetY1(zero));
}

void CIppsSignalDC::DrawSignal(int zero, POINT* pPoints)
{
   int i;
   for (i=0; i < m_NumPoints; i++)
      pPoints[i].y += zero;

   CPen pen(PS_SOLID, 1, GetColorSignal());
   SelectObject(&pen);
   Polyline(pPoints,m_NumPoints);

   for (i=0; i < m_NumPoints; i++)
      pPoints[i].y -= zero;
}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////


void CIppsSignalDC::DrawAxisX(int zero, BOOL bIm)
{
   int x0 = GetX0();
   int x1 = GetX1();
   int y0 = GetY0(zero);
   int y1 = GetY1(zero);
   double step = GetStepAxis(GetMinStepX(), m_pSignal->Length());
   double gap = step*FactorX();

   for (int i=1 ;; i++) {
      int x = (int)(gap*i);
      if (x0 + x > x1) break;
      DrawLabelX(x0 + x, y0, (int)(step*i + .5), SIDE_BOTTOM);
      DrawLabelX(x0 + x, y1, (int)(step*i + .5), bIm ? 0 : SIDE_TOP);
   }
}

void CIppsSignalDC::DrawAxisY(int zero)
{
   int x0 = GetX0();
   int x1 = GetX1();
   double step = GetStepAxis(GetMinStepY(), Magnitude());
   double gap = step*FactorY();

   double val = GetMinLabel(step);
   double yStart = val*FactorY();
   double yEnd = m_Max*FactorY() + 1;
   for (double y = yStart; y < yEnd; y += gap, val += step) {
      if (ABS(val)/step < 1.e-3) val = 0;
      int iy = (int)(y + .5);
      DrawLabelY(x0, zero - iy,  val, SIDE_LEFT);
      DrawLabelY(x1, zero - iy,  val, SIDE_RIGHT);
   }
}

double CIppsSignalDC::GetMinStepX()
{
   return (double)GetMinStepWidth()/FactorX();
}

double CIppsSignalDC::GetMinStepY()
{
   double minStep = (double)GetMinStepHeight()/FactorY();
   if (!m_pSignal->Float() && minStep < 1) minStep = 1;
   return minStep;
}

void CIppsSignalDC::DrawLine(int x0, int y0, int x1, int y1, COLORREF clr)
{
   CPen pen(PS_SOLID, 1, clr);
   CPen* pOldPen = SelectObject(&pen);

   CPoint p1(x0, y0);
   CPoint p2(x1, y1);
   MoveTo(p1);
   LineTo(p2);

   SelectObject(pOldPen);
}

void CIppsSignalDC::DrawText(int x, int y, CString text)
{
   SelectObject(&m_Font);
   SetTextColor(GetColorAxis());
   TextOut(x, y, text);
}

void CIppsSignalDC::DrawLabelX(int x, int y, int val, int textSide)
{
   int halfLabel = 3;
   DrawLine(x, y - halfLabel, x, y + halfLabel, GetColorAxis());
   if (!textSide) return;

   CString text;
   text.Format("%d",val);

   CSize offset = GetTextExtent(text);
   offset.cx = -offset.cx/2;
   if (textSide == SIDE_TOP)
      offset.cy = -offset.cy - halfLabel*2;
   else
      offset.cy = halfLabel*2;
   DrawText(x + offset.cx, y + offset.cy, text);
}

void CIppsSignalDC::DrawLabelY(int x, int y, double val, int textSide)
{
   int halfLabel = 3;
   DrawLine(x - halfLabel, y, x + halfLabel, y, GetColorAxis());
   if (!textSide) return;

   BOOL bFloat = FALSE;
   if (m_pSignal->Float()) bFloat = TRUE;
   else if (m_pSignal->Depth() >= 32) {
      if (ABS(val) > 999999) bFloat = TRUE;
   }
   CString text;
   if (bFloat)
      text.Format("%.6g",(double)val);
   else
      text.Format("%d",(int)val);

   CSize offset = GetTextExtent(text);
   if (textSide == SIDE_LEFT)
      offset.cx = - offset.cx - halfLabel*2;
   else
      offset.cx = halfLabel*2;
   offset.cy = -offset.cy/2;
   DrawText(x + offset.cx, y + offset.cy, text);
}

enum {NUM_GAP = 8};
static double Gaps[NUM_GAP] = {.1, .2, .5, 1.0, 2.0, 5.0, 10.0, 20.0};

double CIppsSignalDC::GetStepAxis(double minStep, double magn)
{
   double step;
   do {
      double step10 = pow((double)10,(int)log10(minStep));
      for (int i=0; i<NUM_GAP; i++) {
         step = step10*Gaps[i];
         if (step >= minStep) break;
      }
      minStep *= .5;
   } while (magn > 0 && step > magn);
   return step;
}

double CIppsSignalDC::GetMinLabel(double step)
{
   double val = m_Min >= 0 ? m_Min : -m_Min;
   double lab = ((int)(val / step))*step;
   if (m_Min < 0) return -lab;
   if (ABS(val - lab)/step < 1.e-2) return lab;
   return lab + step;
}













