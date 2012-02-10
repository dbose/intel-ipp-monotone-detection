/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2000-2005 Intel Corporation. All Rights Reserved.
//
*/

// SampleView.cpp : implementation of the CSampleView class
//

#include "stdafx.h"
#include "ippsSample.h"

#include "ippsSignalDC.h"
#include "SampleDoc.h"
#include "SampleView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSampleView

IMPLEMENT_DYNCREATE(CSampleView, CScrollView)

BEGIN_MESSAGE_MAP(CSampleView, CScrollView)
   //{{AFX_MSG_MAP(CSampleView)
   ON_WM_ERASEBKGND()
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSampleView construction/destruction

CSampleView::CSampleView()
{
   m_pSignalDC = NULL;
}

CSampleView::~CSampleView()
{
   if (m_pSignalDC)
      delete m_pSignalDC;
}

BOOL CSampleView::PreCreateWindow(CREATESTRUCT& cs)
{
   // TODO: Modify the Window class or styles here by modifying
   //  the CREATESTRUCT cs

   return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CSampleView drawing

int CSampleView::GetWidth()
{
   return m_pSignalDC->GetWidth();
}

int CSampleView::GetHeight()
{
   return m_pSignalDC->GetHeight();
}

int CSampleView::GetClientWidth()
{
   CRect rect;
   GetClientRect(&rect);
   return rect.Width();
}

void CSampleView::OnInitialUpdate()
{
   CScrollView::OnInitialUpdate();

   // Create source device context with bitmap
   CClientDC dc(this);
   m_pSignalDC = new CIppsSignalDC;
   m_pSignalDC->Create(&dc, GetDocument(), GetClientWidth());

   // Set the total size of this view
   CSize sizeTotal;
   sizeTotal.cx = GetWidth();
   sizeTotal.cy = GetHeight();
   SetScrollSizes(MM_TEXT, sizeTotal);
}

void CSampleView::OnDraw(CDC* pDC)
{
   CSampleDoc* pDoc = GetDocument();
   ASSERT_VALID(pDoc);
   if (pDoc->Length() == 0) return;

   // if image data have been changed
   if (pDoc->IsUpdated()) {
      // Set data to source device context bitmap
      m_pSignalDC->Draw();
      // Reset updated flag
      pDoc->IsUpdated(FALSE);
   }
   // Copy a bitmap from the source device context to view device context
   pDC->BitBlt(0,0,GetWidth(),GetHeight(),
      m_pSignalDC,0,0,SRCCOPY);
   // Draw background
   DrawBack(pDC);
}

/////////////////////////////////////////////////////////////////
// Draw background polygon
// This function is necessary becuase we refuse to erase background by
// default procedure in order to prevent blincks

void CSampleView::DrawBack(CDC* pDC)
{
   POINT     points[6];
   int width  = GetWidth();
   int height = GetHeight();
   CRect     rect;

   GetClientRect(&rect);
   if (rect.right <= width && rect.bottom <= height) return;
   if (rect.right <= width) {
       width = rect.right;
       rect.top = IPP_MIN(height,rect.bottom);
   }
   if (rect.bottom <= height) {
       height = rect.bottom;
       rect.left = width;
   }
   points[0].x = width;       points[0].y = height;
   points[1].x = width;       points[1].y = rect.top;
   points[2].x = rect.right;  points[2].y = rect.top;
   points[3].x = rect.right;  points[3].y = rect.bottom;
   points[4].x = rect.left;   points[4].y = rect.bottom;
   points[5].x = rect.left;   points[5].y = height;

   CPen pen(PS_NULL, 0, RGB(0,0,0));
   CBrush brush(m_pSignalDC->GetColorAxisBack());
   pDC->SelectObject(&pen);
   pDC->SelectObject(&brush);
   pDC->Polygon(points,6);
}

/////////////////////////////////////////////////////////////////////////////
// CSampleView diagnostics

#ifdef _DEBUG
void CSampleView::AssertValid() const
{
   CScrollView::AssertValid();
}

void CSampleView::Dump(CDumpContext& dc) const
{
   CScrollView::Dump(dc);
}

CSampleDoc* CSampleView::GetDocument() // non-debug version is inline
{
   ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSampleDoc)));
   return (CSampleDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSampleView message handlers

BOOL CSampleView::OnEraseBkgnd(CDC* pDC)
{
   // Prevent blinking while drawing
   return TRUE;
}
