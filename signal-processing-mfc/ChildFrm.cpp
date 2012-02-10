/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2000-2005 Intel Corporation. All Rights Reserved.
//
*/

// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "stdafx.h"
#include "ippsSample.h"

#include "ChildFrm.h"
#include "SampleView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
   //{{AFX_MSG_MAP(CChildFrame)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
   // TODO: add member initialization code here

}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
   // TODO: Modify the Window class or styles here by modifying
   //  the CREATESTRUCT cs

   if( !CMDIChildWnd::PreCreateWindow(cs) )
      return FALSE;
   return TRUE;
}

//////////////////////////////////////
//        Attributes

CSampleView* CChildFrame::GetView() {
   return (CSampleView*)GetActiveView();
}


//////////////////////////////////////
//        Operations

/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
   CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
   CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers

void CChildFrame::ActivateFrame(int nCmdShow)
{
   // Update window size
   CSampleDoc *pDoc = (CSampleDoc*)(GetActiveDocument());
   WINDOWPLACEMENT wPlace;
   LPRECT pwRect = &(wPlace.rcNormalPosition);
   RECT cRect;

   GetWindowPlacement(&wPlace);

   GetClientRect(&cRect);
   int dx = (pwRect->right - pwRect->left) -
            (cRect.right - cRect.left) +
            2 * ::GetSystemMetrics(SM_CXEDGE);
   int dy = (pwRect->bottom - pwRect->top) -
            (cRect.bottom - cRect.top) +
            2 * ::GetSystemMetrics(SM_CYEDGE);
   pwRect->right  = pwRect->left + GetView()->GetWidth() + dx;
   pwRect->bottom = pwRect->top + GetView()->GetHeight() + dy;

   SetWindowPlacement(&wPlace);

   CMDIChildWnd::ActivateFrame(nCmdShow);
}

