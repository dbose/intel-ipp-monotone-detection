/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2000-2005 Intel Corporation. All Rights Reserved.
//
*/

// SampleDoc.h : interface of the CSampleDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SAMPLEDOC_H__735CA30D_AC7D_4E8F_97E0_6CD26F4C3C23__INCLUDED_)
#define AFX_SAMPLEDOC_H__735CA30D_AC7D_4E8F_97E0_6CD26F4C3C23__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippsSignal.h"

class CSampleView;
class CChildFrame;

class CSampleDoc : public CDocument, public CIppsSignal
{
protected: // create from serialization only
   CSampleDoc();
   DECLARE_DYNCREATE(CSampleDoc)

// Attributes
public:
   // Get document view
   CSampleView* GetView() ;
   // Get document frame
   CChildFrame* GetFrame() ;

// Operations
public:
   // Update view
   void UpdateView();
   // Open a new doc with signal data type and length
   static CSampleDoc* CreateNewDoc(CIppsSignal* pSignal, BOOL bMakeVisible = TRUE);
   static CSampleDoc* CreateNewDoc(int len, ppType type, BOOL bMakeVisible = TRUE);
   // Make document visible if bMakeVisible = FALSE in CreateNewDoc
   void MakeVisible();

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CSampleDoc)
   public:
   virtual BOOL OnNewDocument();
   virtual void Serialize(CArchive& ar);
   virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
   //}}AFX_VIRTUAL

// Implementation
public:
   virtual ~CSampleDoc();
#ifdef _DEBUG
   virtual void AssertValid() const;
   virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
   //{{AFX_MSG(CSampleDoc)
   afx_msg void OnUpdateFuncAddc(CCmdUI* pCmdUI);
   afx_msg void OnFuncAddc();
   afx_msg void OnFileProperties();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SAMPLEDOC_H__735CA30D_AC7D_4E8F_97E0_6CD26F4C3C23__INCLUDED_)
