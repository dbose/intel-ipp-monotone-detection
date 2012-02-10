/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2000-2005 Intel Corporation. All Rights Reserved.
//
*/

// SampleView.h : interface of the CSampleView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SAMPLEVIEW_H__20BE5621_EA49_46E2_9C3B_94807546C82B__INCLUDED_)
#define AFX_SAMPLEVIEW_H__20BE5621_EA49_46E2_9C3B_94807546C82B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CIppsSignalDC;

class CSampleView : public CScrollView
{
protected: // create from serialization only
   CSampleView();
   DECLARE_DYNCREATE(CSampleView)

// Attributes
public:
   CSampleDoc* GetDocument();
   int GetWidth ();
   int GetHeight();

// Operations
public:
   // to provide making document visible
   friend CSampleDoc;

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CSampleView)
   public:
   virtual void OnDraw(CDC* pDC);  // overridden to draw this view
   virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
   protected:
   virtual void OnInitialUpdate(); // called first time after construct
   //}}AFX_VIRTUAL

// Implementation
public:
   virtual ~CSampleView();
#ifdef _DEBUG
   virtual void AssertValid() const;
   virtual void Dump(CDumpContext& dc) const;
#endif

protected:
   CIppsSignalDC* m_pSignalDC;

   // Get width of client window
   int GetClientWidth ();
   // Draw image background
   void DrawBack(CDC* pDC);

// Generated message map functions
protected:
   //{{AFX_MSG(CSampleView)
   afx_msg BOOL OnEraseBkgnd(CDC* pDC);
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in SampleView.cpp
inline CSampleDoc* CSampleView::GetDocument()
   { return (CSampleDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SAMPLEVIEW_H__20BE5621_EA49_46E2_9C3B_94807546C82B__INCLUDED_)
