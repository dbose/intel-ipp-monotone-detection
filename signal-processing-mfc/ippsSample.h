/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2000-2005 Intel Corporation. All Rights Reserved.
//
*/

// ippsSample.h : main header file for the IPPSSAMPLE application
//

#if !defined(AFX_IPPSSAMPLE_H__45588C34_17AA_4FD6_961F_8DE9488F0818__INCLUDED_)
#define AFX_IPPSSAMPLE_H__45588C34_17AA_4FD6_961F_8DE9488F0818__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
   #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "ippsSignal.h"     // CIppsSignal class
class CSampleDoc;           // document class

/////////////////////////////////////////////////////////////////////////////
// CIppsSampleApp:
// See ippsSample.cpp for the implementation of this class
//

class CIppsSampleApp : public CWinApp
{
public:
   CIppsSampleApp();

// Operations
   // Open a new doc with specified signal data type & length
   CSampleDoc* CreateNewDoc(int len, ppType type, BOOL bMakeVisible);
// Attributes
   // Get signal data type
   ppType GetNewType() const { return m_NewType;}
   // Set signal data type
   void SetNewType(ppType type) {m_NewType = type;}
   // Get signal length
   int GetNewLen() const { return m_NewLen;}
   // Set signal length
   void SetNewLen(int length) {m_NewLen = length;}

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CIppsSampleApp)
   public:
   virtual BOOL InitInstance();
   //}}AFX_VIRTUAL

// Implementation
   //{{AFX_MSG(CIppsSampleApp)
   afx_msg void OnAppAbout();
      // NOTE - the ClassWizard will add and remove member functions here.
      //    DO NOT EDIT what you see in these blocks of generated code !
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
protected:
   ppType m_NewType;
   int    m_NewLen;
};

// Message box function to show IPP Error or Warning
void IppErrorMessage(CString funcName, IppStatus status);

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IPPSSAMPLE_H__45588C34_17AA_4FD6_961F_8DE9488F0818__INCLUDED_)
