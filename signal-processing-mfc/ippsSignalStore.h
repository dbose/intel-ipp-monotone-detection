/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2000-2005 Intel Corporation. All Rights Reserved.
//
*/

// ippsSignalStore.h: interface for the CIppsSignalStore class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IPPSSIGNALSTORE_H__8E2145B4_5CD7_4D98_A090_876FE4486580__INCLUDED_)
#define AFX_IPPSSIGNALSTORE_H__8E2145B4_5CD7_4D98_A090_876FE4486580__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <mmsystem.h>
class CIppsSignal;

class CIppsSignalStore
{
public:
   CIppsSignalStore(CIppsSignal* pSignal);
   virtual ~CIppsSignalStore() {}

   // Save attached signal to wav file
   BOOL Save(CFile* pFile);
   // Load attached signal from wav file
   BOOL Load(CFile* pFile);
protected:
   CIppsSignal* m_pSignal;
   BOOL    m_bSave;
   CString m_FilePath;
   CFile*  m_pFile;
   HMMIO   m_hFile;
   WAVEFORMATEX  m_Format;

   BOOL Error(CString message);
   BOOL Error(MMIOINFO& ioInfo);
   BOOL Save();
   BOOL Load();
   void Close();
   ppType TypeFromFormat();
   BOOL TypeToFormat(ppType type);
   int  LengthFromFormat(DWORD dataSize);
};

#endif // !defined(AFX_IPPSSIGNALSTORE_H__8E2145B4_5CD7_4D98_A090_876FE4486580__INCLUDED_)
