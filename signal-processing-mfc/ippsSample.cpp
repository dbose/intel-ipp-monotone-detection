/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2000-2005 Intel Corporation. All Rights Reserved.
//
*/

// ippsSample.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ippsSample.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "SampleDoc.h"
#include "SampleView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIppsSampleApp

BEGIN_MESSAGE_MAP(CIppsSampleApp, CWinApp)
   //{{AFX_MSG_MAP(CIppsSampleApp)
   ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
      // NOTE - the ClassWizard will add and remove mapping macros here.
      //    DO NOT EDIT what you see in these blocks of generated code!
   //}}AFX_MSG_MAP
   // Standard file based document commands
   ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
   ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIppsSampleApp construction

CIppsSampleApp::CIppsSampleApp()
{
// Set signal data type and length for new document
   m_NewType = pp16s;
   m_NewLen  = 512;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CIppsSampleApp object

CIppsSampleApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CIppsSampleApp initialization

BOOL CIppsSampleApp::InitInstance()
{
   // Standard initialization
   // If you are not using these features and wish to reduce the size
   //  of your final executable, you should remove from the following
   //  the specific initialization routines you do not need.

#ifdef _AFXDLL
   Enable3dControls();        // Call this when using MFC in a shared DLL
#else
   Enable3dControlsStatic();  // Call this when linking to MFC statically
#endif

   // Register the application's document templates.  Document templates
   //  serve as the connection between documents, frame windows and views.

   CMultiDocTemplate* pDocTemplate;
   pDocTemplate = new CMultiDocTemplate(
      IDR_SIGNALTYPE,
      RUNTIME_CLASS(CSampleDoc),
      RUNTIME_CLASS(CChildFrame), // custom MDI child frame
      RUNTIME_CLASS(CSampleView));
   AddDocTemplate(pDocTemplate);

   // create main MDI Frame window
   CMainFrame* pMainFrame = new CMainFrame;
   if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
      return FALSE;
   m_pMainWnd = pMainFrame;

   // Enable drag/drop open
   m_pMainWnd->DragAcceptFiles();

   // Enable DDE Execute open
   EnableShellOpen();
   RegisterShellFileTypes(TRUE);

   // Parse command line for standard shell commands, DDE, file open
   CCommandLineInfo cmdInfo;
   ParseCommandLine(cmdInfo);

   // Dispatch commands specified on the command line
   if (!ProcessShellCommand(cmdInfo))
      return FALSE;

   // The main window has been initialized, so show and update it.
   pMainFrame->ShowWindow(m_nCmdShow);
   pMainFrame->UpdateWindow();

   return TRUE;
}

CSampleDoc* CIppsSampleApp::CreateNewDoc(int len, ppType type, BOOL bMakeVisible)
{
   int lenSave = m_NewLen;
   ppType typeSave = m_NewType;
   m_NewLen = len;
   m_NewType = type;

   POSITION pos = GetFirstDocTemplatePosition();
   CDocTemplate* pTpl = GetNextDocTemplate(pos);
   CSampleDoc* pDoc = (CSampleDoc*)pTpl->OpenDocumentFile(NULL,bMakeVisible);

   m_NewLen = lenSave;
   m_NewType = typeSave;
   return pDoc;
}

////////////////////////////////////////////////////////////////////
// Message box function to show IPP Error or Warning

void IppErrorMessage(CString funcName, IppStatus status)
{
   if (status == 0) return;
   CString message;
   message.Format("%s in %s:\nIppStatus = %d:\n%s",
                status < 0 ? "Error" : "Warning",
                funcName, status, ippGetStatusString(status));
   AfxGetMainWnd()->MessageBox(message, AfxGetAppName(),
      (status < 0) ? MB_OK | MB_ICONWARNING : MB_OK);
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
   CAboutDlg();

// Dialog Data
   //{{AFX_DATA(CAboutDlg)
   enum { IDD = IDD_ABOUTBOX };
   //}}AFX_DATA

   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CAboutDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   //{{AFX_MSG(CAboutDlg)
      // No message handlers
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
   //{{AFX_DATA_INIT(CAboutDlg)
   //}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CAboutDlg)
   //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
   //{{AFX_MSG_MAP(CAboutDlg)
      // No message handlers
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CIppsSampleApp::OnAppAbout()
{
   CAboutDlg aboutDlg;
   aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CIppsSampleApp message handlers

