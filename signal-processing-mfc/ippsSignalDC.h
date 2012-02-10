/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2000-2005 Intel Corporation. All Rights Reserved.
//
*/

// ippsSignalDC.h: interface for the CIppsSignalDC class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IPPSSIGNALDC_H__F69BD436_8334_4993_8B3A_31A6C77C621C__INCLUDED_)
#define AFX_IPPSSIGNALDC_H__F69BD436_8334_4993_8B3A_31A6C77C621C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CIppsSignal;

class CIppsSignalDC : public CDC
{
public:
   CIppsSignalDC();
   virtual ~CIppsSignalDC();
   // Attach pSignal
   // Create DC, compatible with pDC
   // Create Bitmap, compatible with DC and select it in DC
   void Create(CDC* pDC, CIppsSignal* pSignal, int initialWidth);
   // Copy data from attached image to Bitmap
   // and draw signal
   void Draw();

   // Get normal width & height of signal graph
   int GetWidth ();
   int GetHeight();

   // Get graph colors
   static COLORREF GetColorSignal()    ;
   static COLORREF GetColorSignalBack();
   static COLORREF GetColorAxis()      ;
   static COLORREF GetColorAxisBack()  ;

protected:
   enum {SIDE_LEFT = -2, SIDE_TOP = -1, SIDE_NONE = 0,
         SIDE_BOTTOM = 1, SIDE_RIGHT = 2};
   double FactorX();
   double FactorY();
   void GetMinMax(double& min, double& max);
   void SetPoints();
   void SetPointsReal();
   void SetPointsComplex();
   void AllocatePoints();
   void FreePoints();
   void SetMinMax();
   int GetMinY();
   int GetMaxY();
   double Magnitude();
   int GetX0();
   int GetX1();
   int GetZero();
   int GetZeroIm();
   int GetY0(int zero);
   int GetY1(int zero);
   void DrawReal();
   void DrawComplex();
   void DrawBackground();
   void DrawSignalBox(int zero, POINT* pPoints, BOOL bIm = FALSE);
   void DrawSignalBackground(int zero);
   void DrawSignal(int zero, POINT* pPoints);
   void DrawAxisX(int zero, BOOL bIm);
   void DrawAxisY(int zero);
   void DrawLine(int x0, int y0, int x1, int y1, COLORREF clr);
   void DrawText(int x, int y, CString text);
   void DrawLabelX(int x, int y, int val, int textSide);
   void DrawLabelY(int x, int y, double val, int textSide);
   double GetStepAxis(double minStep, double magn);
   double GetMinLabel(double step);
   double GetMinStepX();
   double GetMinStepY();

   CIppsSignal* m_pSignal;
   CBitmap* m_pBitmap;
   CFont    m_Font;
   POINT* m_Points;
   POINT* m_PointsIm;
   int    m_NumPoints;
   int    m_InitialWidth;
   double m_Min;
   double m_Max;
};

#endif // !defined(AFX_IPPSSIGNALDC_H__F69BD436_8334_4993_8B3A_31A6C77C621C__INCLUDED_)
