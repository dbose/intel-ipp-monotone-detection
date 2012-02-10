/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2000-2005 Intel Corporation. All Rights Reserved.
//
*/

// ippsAddCDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ippsSample.h"
#include "ippsAddCDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIppsAddCDlg dialog


CIppsAddCDlg::CIppsAddCDlg(CWnd* pParent /*=NULL*/)
   : CDialog(CIppsAddCDlg::IDD, pParent)
{
   //{{AFX_DATA_INIT(CIppsAddCDlg)
   m_value = 0;
   m_scaleFactor = 0;
   //}}AFX_DATA_INIT
}


void CIppsAddCDlg::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CIppsAddCDlg)
   DDX_Control(pDX, IDC_SCALE_SPIN, m_ScaleSpin);
   DDX_Text(pDX, IDC_VAL, m_value);
   DDX_Text(pDX, IDC_SCALE_EDIT, m_scaleFactor);
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIppsAddCDlg, CDialog)
   //{{AFX_MSG_MAP(CIppsAddCDlg)
   //}}AFX_MSG_MAP
   ON_BN_CLICKED(IDOK, &CIppsAddCDlg::OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIppsAddCDlg message handlers

BOOL CIppsAddCDlg::OnInitDialog()
{
   CDialog::OnInitDialog();

   m_ScaleSpin.SetRange(SHRT_MIN, SHRT_MAX);

   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

void CIppsAddCDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}
