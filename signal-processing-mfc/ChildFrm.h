/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2000-2005 Intel Corporation. All Rights Reserved.
//
*/

// ChildFrm.h : interface of the CChildFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHILDFRM_H__68811AF0_A30A_452C_B7CF_9C81E31A0303__INCLUDED_)
#define AFX_CHILDFRM_H__68811AF0_A30A_452C_B7CF_9C81E31A0303__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSampleView;

class CChildFrame : public CMDIChildWnd
{
   DECLARE_DYNCREATE(CChildFrame)
public:
   CChildFrame();

// Attributes
public:
   CSampleView* GetView();

// Operations
public:

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CChildFrame)
   public:
   virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
   virtual void ActivateFrame(int nCmdShow = -1);
   //}}AFX_VIRTUAL

// Implementation
public:
   virtual ~CChildFrame();
#ifdef _DEBUG
   virtual void AssertValid() const;
   virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
   //{{AFX_MSG(CChildFrame)
      // NOTE - the ClassWizard will add and remove member functions here.
      //    DO NOT EDIT what you see in these blocks of generated code!
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDFRM_H__68811AF0_A30A_452C_B7CF_9C81E31A0303__INCLUDED_)
