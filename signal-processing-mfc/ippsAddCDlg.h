/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2000-2005 Intel Corporation. All Rights Reserved.
//
*/

#if !defined(AFX_IPPSADDCDLG_H__F7B469DB_2EFC_4B2E_ABB3_162588B735FB__INCLUDED_)
#define AFX_IPPSADDCDLG_H__F7B469DB_2EFC_4B2E_ABB3_162588B735FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ippsAddCDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CIppsAddCDlg dialog

class CIppsAddCDlg : public CDialog
{
// Construction
public:
   CIppsAddCDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   //{{AFX_DATA(CIppsAddCDlg)
   enum { IDD = IDD_PARM_ADDC };
   CSpinButtonCtrl   m_ScaleSpin;
   Ipp16s   m_value;
   int      m_scaleFactor;
   //}}AFX_DATA


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CIppsAddCDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(CIppsAddCDlg)
   virtual BOOL OnInitDialog();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IPPSADDCDLG_H__F7B469DB_2EFC_4B2E_ABB3_162588B735FB__INCLUDED_)
