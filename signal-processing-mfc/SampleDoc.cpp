/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2000-2005 Intel Corporation. All Rights Reserved.
//
*/

// SampleDoc.cpp : implementation of the CSampleDoc class
//

#include "stdafx.h"
#include "ippsSample.h"

#include "SampleDoc.h"
#include "SampleView.h"
#include "ChildFrm.h"
#include "ippsAddC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSampleDoc

IMPLEMENT_DYNCREATE(CSampleDoc, CDocument)

BEGIN_MESSAGE_MAP(CSampleDoc, CDocument)
   //{{AFX_MSG_MAP(CSampleDoc)
   ON_UPDATE_COMMAND_UI(ID_FUNC_ADDC, OnUpdateFuncAddc)
   ON_COMMAND(ID_FUNC_ADDC, OnFuncAddc)
   ON_COMMAND(ID_FILE_PROPERTIES, OnFileProperties)
   //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSampleDoc construction/destruction

CSampleDoc::CSampleDoc()
{
   // TODO: add one-time construction code here

}

CSampleDoc::~CSampleDoc()
{
}

BOOL CSampleDoc::OnNewDocument()
{
   if (!CDocument::OnNewDocument())
      return FALSE;

   // Create new signal
   CreateSignal(
      ((CIppsSampleApp*)AfxGetApp())->GetNewLen(),
      ((CIppsSampleApp*)AfxGetApp())->GetNewType());

   if (Type() == pp16s) {
      // Set Jaehne signal for 16s data
      ippsVectorJaehne_16s((Ipp16s*)DataPtr(), Length(), SHRT_MAX>>3);
   }

   return TRUE;
}

BOOL CSampleDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
   if (!CDocument::OnOpenDocument(lpszPathName))
      return FALSE;

   if (Length() == 0) return FALSE;

   return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CSampleDoc serialization

void CSampleDoc::Serialize(CArchive& ar)
{
   CFile *pFile = ar.GetFile();
   if (ar.IsStoring())
   {
      // Save signal
      SaveSignal(pFile);
   }
   else
   {
      // Load signal
      LoadSignal(pFile);
   }
}

/////////////////////////////////////////////////////////////////////////////
// CSampleDoc attributes

CSampleView* CSampleDoc::GetView()
{
   POSITION pos = GetFirstViewPosition();
   return (CSampleView*)GetNextView(pos);
}

CChildFrame* CSampleDoc::GetFrame()
{
   return (CChildFrame*)(GetView() ? GetView()->GetParentFrame() : NULL);
}

/////////////////////////////////////////////////////////////////////////////
// CSampleDoc operations

void CSampleDoc::UpdateView()
{
   GetView()->InvalidateRect(NULL);
   UpdateAllViews(NULL);
}

CSampleDoc* CSampleDoc::CreateNewDoc(CIppsSignal* pSignal, BOOL bMakeVisible)
{
    return ((CIppsSampleApp*)AfxGetApp())->
        CreateNewDoc(pSignal->Length(), pSignal->Type(), bMakeVisible);
}

CSampleDoc* CSampleDoc::CreateNewDoc(int len, ppType type, BOOL bMakeVisible)
{
    return ((CIppsSampleApp*)AfxGetApp())->CreateNewDoc(len, type, bMakeVisible);
}

void CSampleDoc::MakeVisible()
{
   // send initial update to the view
   GetView()->OnInitialUpdate();
   // activate the frame
   GetFrame()->ActivateFrame();
}

/////////////////////////////////////////////////////////////////////////////
// CSampleDoc diagnostics

#ifdef _DEBUG
void CSampleDoc::AssertValid() const
{
   CDocument::AssertValid();
}

void CSampleDoc::Dump(CDumpContext& dc) const
{
   CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSampleDoc commands

void CSampleDoc::OnUpdateFuncAddc(CCmdUI* pCmdUI)
{
   CIppsAddC add;
   pCmdUI->Enable(add.FitSignal(this));
}

void CSampleDoc::OnFuncAddc()
{
   CIppsAddC add;
   add.ProcessSignal(this);
}

void CSampleDoc::OnFileProperties()
{
   CString message;
   message.Format(
      "Length       \t%d\n"
      "Type         \t%s\n",
      Length(),
      TypeString());
   if (!GetPathName().IsEmpty())
      message = "File Name    \t" + GetPathName() + "\n"
         + message;
   AfxGetMainWnd()->MessageBox(message, GetTitle() + " properties", MB_OK);
}
