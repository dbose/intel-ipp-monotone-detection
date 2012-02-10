/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2000-2005 Intel Corporation. All Rights Reserved.
//
*/

// ippsSignalStore.cpp: implementation of the CIppsSignalStore class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsSample.h"
#include "ippsSignal.h"
#include "ippsSignalStore.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIppsSignalStore::CIppsSignalStore(CIppsSignal* pSignal) :
   m_pSignal(pSignal), m_pFile(NULL), m_hFile(NULL), m_bSave(FALSE)
{
   memset(&m_Format,0,sizeof(WAVEFORMATEX));
}

BOOL CIppsSignalStore::Error(CString message)
{
   Close();
   CString oper = m_bSave ? "save" : "open";
   AfxMessageBox("Can't " + oper + " file "
      + m_FilePath + ":\n" + message);
   return FALSE;
}

BOOL CIppsSignalStore::Error(MMIOINFO& ioInfo)
{
   CString message;
   switch (ioInfo.wErrorRet) {
   case MMIOERR_ACCESSDENIED    : message = "Access is denied"; break;
   case MMIOERR_INVALIDFILE     : message = "Invalid file"; break;
   case MMIOERR_NETWORKERROR    : message = "Network error"; break;
   case MMIOERR_PATHNOTFOUND    : message = "Path is not found"; break;
   case MMIOERR_SHARINGVIOLATION: message = "Sharing violation"; break;
   case MMIOERR_TOOMANYOPENFILES: message = "Too many open files"; break;
   }
   return Error(message);
}

BOOL CIppsSignalStore::Save(CFile* pFile)
{
   m_bSave = TRUE;
   m_FilePath = pFile->GetFilePath();
   pFile->Close();
   BOOL result = Save();
   pFile->Open(m_FilePath,CFile::modeRead);
   return result;
}

BOOL CIppsSignalStore::Load(CFile* pFile)
{
   m_bSave = FALSE;
   m_FilePath = pFile->GetFilePath();
   pFile->Close();
   BOOL result = Load();
   pFile->Open(m_FilePath,CFile::modeRead);
   return result;
}

BOOL CIppsSignalStore::Save()
{
   MMIOINFO  ioInfo;
   MMCKINFO  ckInfo;

   memset(&ioInfo,0,sizeof(MMIOINFO));
   m_hFile = mmioOpen(m_FilePath.GetBuffer(1), &ioInfo,
      MMIO_CREATE | MMIO_READWRITE);
   if (!m_hFile)
     return Error(ioInfo);
   if (!TypeToFormat(m_pSignal->Type())) return FALSE;
   m_Format.nSamplesPerSec = m_pSignal->SamplesPerSecond() ?
      m_pSignal->SamplesPerSecond() : 22050;
   m_Format.nAvgBytesPerSec = (m_Format.nChannels * m_Format.nSamplesPerSec
      *m_Format.wBitsPerSample) / 8;
   m_Format.nBlockAlign = (m_Format.nChannels*m_Format.wBitsPerSample) / 8;

   int fmtSize = sizeof(PCMWAVEFORMAT);
   int dataSize = m_pSignal->DataSize();

   ckInfo.ckid = MMIO_CREATERIFF;
   ckInfo.cksize = 20 + fmtSize + dataSize;
   ckInfo.fccType = mmioFOURCC('W', 'A', 'V', 'E');
   mmioCreateChunk(m_hFile, &ckInfo, MMIO_CREATERIFF);

   mmioWrite(m_hFile, "fmt ", 4);
   mmioWrite(m_hFile, (HPSTR)&fmtSize, 4);
   mmioWrite(m_hFile, (HPSTR)&m_Format, fmtSize);
   mmioWrite(m_hFile, "data", 4);
   mmioWrite(m_hFile, (HPSTR)&dataSize, 4);
   mmioWrite(m_hFile, (HPSTR)m_pSignal->DataPtr(), dataSize);

   Close();
   return TRUE;
}

BOOL CIppsSignalStore::Load()
{
   MMIOINFO    ioInfo;
   MMCKINFO    mmckinfoParent;     // parent chunk information
   MMCKINFO    mmckinfoSubchunk;   // subchunk information structure
   DWORD       dwFmtSize;          // size of "FMT" chunk
   DWORD       dwDataSize;         // size of "DATA" chunk

   memset(&ioInfo,0,sizeof(MMIOINFO));
   m_hFile = mmioOpen(m_FilePath.GetBuffer(1), &ioInfo,
      MMIO_READ | MMIO_ALLOCBUF);
   if (!m_hFile) return Error(ioInfo);

   // Locate a "RIFF" chunk with a "WAVE" form type to make
   // sure the file is a waveform-audio file.
   mmckinfoParent.fccType = mmioFOURCC('W', 'A', 'V', 'E');
   if (mmioDescend(m_hFile, (LPMMCKINFO) &mmckinfoParent, NULL,
                   MMIO_FINDRIFF))
     return Error("This is not a waveform-audio file.");
   // Find the "FMT" chunk (form type "FMT"); it must be
   // a subchunk of the "RIFF" chunk.
   mmckinfoSubchunk.ckid = mmioFOURCC('f', 'm', 't', ' ');
   if (mmioDescend(m_hFile, &mmckinfoSubchunk, &mmckinfoParent,
     MMIO_FINDCHUNK))
     return Error("Waveform-audio file has no \"FMT\" chunk.");

   // Get the size of the "FMT" chunk. Allocate
   // and lock memory for it.
   dwFmtSize = mmckinfoSubchunk.cksize;
   // Read the "FMT" chunk.
   if (dwFmtSize > sizeof(WAVEFORMATEX) ||
       mmioRead(m_hFile, (HPSTR) &m_Format, dwFmtSize) != (int)dwFmtSize)
     return Error("Failed to read format chunk.");

   // Ascend out of the "FMT" subchunk.
   mmioAscend(m_hFile, &mmckinfoSubchunk, 0);

   // Find the data subchunk. The current file position should be at
   // the beginning of the data chunk; however, you should not make
   // this assumption. Use mmioDescend to locate the data chunk.
   mmckinfoSubchunk.ckid = mmioFOURCC('d', 'a', 't', 'a');
   if (mmioDescend(m_hFile, &mmckinfoSubchunk, &mmckinfoParent,
     MMIO_FINDCHUNK))
     return Error("Waveform-audio file has no data chunk.");

   // Get the size of the data subchunk.
   dwDataSize = mmckinfoSubchunk.cksize;
   if (dwDataSize == 0L)
     return Error("The data chunk contains no data.");

   // Allocate and lock memory for the waveform-audio data.
   m_pSignal->CreateSignal(LengthFromFormat(dwDataSize), TypeFromFormat());
   m_pSignal->SamplesPerSecond(m_Format.nSamplesPerSec);

   // Read the waveform-audio data subchunk.
   if(mmioRead(m_hFile, (HPSTR)m_pSignal->DataPtr(), m_pSignal->DataSize())
      != m_pSignal->DataSize())
     return Error("Failed to read data chunk.");

   Close();
   return TRUE;
}

void CIppsSignalStore::Close()
{
   if (m_hFile) {
      mmioClose(m_hFile, 0);
      m_hFile = NULL;
   }
}

ppType CIppsSignalStore::TypeFromFormat()
{
   int type = m_Format.wBitsPerSample;

   switch (m_Format.wFormatTag) {
   case WAVE_FORMAT_PCM:
   default:
      type |= PP_SIGN;
      break;
   }

   if (m_Format.nChannels == 2)
      type |= PP_CPLX;

   return (ppType)type;
}

BOOL CIppsSignalStore::TypeToFormat(ppType type)
{
   if (!(type & PP_SIGN)) {
      if (type & PP_FLOAT)
         return Error("Can't save float signal");
      else
         return Error("Can't save unsigned signal");
   }
   m_Format.wFormatTag = WAVE_FORMAT_PCM;
   m_Format.wBitsPerSample = type & PP_MASK;
   m_Format.nChannels = (type & PP_CPLX) ? 2 : 1;
   return TRUE;
}


int CIppsSignalStore::LengthFromFormat(DWORD dataSize)
{
   return dataSize / ((m_Format.wBitsPerSample/8) * m_Format.nChannels);
}
 
