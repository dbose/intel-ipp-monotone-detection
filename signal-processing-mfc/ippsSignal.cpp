/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2000-2005 Intel Corporation. All Rights Reserved.
//
*/

// ippsSignal.cpp: implementation of the CIppsSignal class.
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

CIppsSignal::CIppsSignal()
{
    m_pData = NULL;
    Reset();
}

void CIppsSignal::Reset()
{
    m_Updated = TRUE;
    m_Type = ppNONE;
    m_Length = 0;
    m_SamplesPerSecond = 0;
    FreeData();
}

CIppsSignal::~CIppsSignal()
{
    FreeData();
}

CIppsSignal::CIppsSignal(const CIppsSignal& image)
{
    m_pData = NULL;
   *this = image;
}

CIppsSignal& CIppsSignal::operator =(const CIppsSignal& signal)
{
   Reset();
   m_Type     = signal.m_Type    ;
   m_Length    = signal.m_Length   ;
   m_SamplesPerSecond = signal.m_SamplesPerSecond;
   m_Updated  = signal.m_Updated ;
   if (signal.m_pData) {
      AllocateData();
      memcpy(m_pData,signal.m_pData,DataSize());
   }
   return *this;
}

BOOL CIppsSignal::CreateSignal(int length, ppType type)
{
   Reset();
   m_Type = type;
   m_Length  = length;
   m_SamplesPerSecond = 0;
   AllocateData();
   return m_pData ? TRUE : FALSE;
}

BOOL CIppsSignal::SaveSignal(CFile* pFile)
{
   CIppsSignalStore store(this);
   return store.Save(pFile);
}

BOOL CIppsSignal::LoadSignal(CFile* pFile)
{
   CIppsSignalStore store(this);
   return store.Load(pFile);
}

int CIppsSignal::DataSize()  const
{
   return m_Length*(Depth()>>3)*(Complex() ? 2 : 1);
}

void CIppsSignal::AllocateData()
{
   Ipp8u value = 0;
   m_pData = malloc(DataSize());
   if (!m_pData) return;
   memset(m_pData,value,DataSize());
}

void CIppsSignal::FreeData()
{
    if (m_pData) free(m_pData);
    m_pData = NULL;
}

void* CIppsSignal::DataPtr()
{
    return m_pData;
}

CString CIppsSignal::TypeString() const
{
   switch (m_Type) {
   case pp8u  : return "8u";
   case pp8s  : return "8s";
   case pp8sc : return "8sc";
   case pp16u : return "16u";
   case pp16s : return "16s";
   case pp16sc: return "16sc";
   case pp32u : return "32u";
   case pp32s : return "32s";
   case pp32sc: return "32sc";
   case pp32f : return "32f";
   case pp32fc: return "32fc";
   case pp64s : return "64s";
   case pp64sc: return "64sc";
   case pp64f : return "64f";
   case pp64fc: return "64fc";
   default: return "";
   }
}

void CIppsSignal::GetValue(int index, Ipp64fc& value) const
{
   if (m_Length == 0) return;
   if (index < 0 || index >= m_Length) return;

   value.im = 0;
   switch (m_Type) {
   case pp8u  : value.re = (Ipp64f)((Ipp8u *)m_pData)[index]; break;
   case pp8s  : value.re = (Ipp64f)((Ipp8s *)m_pData)[index]; break;
   case pp16u : value.re = (Ipp64f)((Ipp16u*)m_pData)[index]; break;
   case pp16s : value.re = (Ipp64f)((Ipp16s*)m_pData)[index]; break;
   case pp32u : value.re = (Ipp64f)((Ipp32u*)m_pData)[index]; break;
   case pp32s : value.re = (Ipp64f)((Ipp32s*)m_pData)[index]; break;
   case pp32f : value.re = (Ipp64f)((Ipp32f*)m_pData)[index]; break;
   case pp64s : value.re = (Ipp64f)((Ipp64s*)m_pData)[index]; break;
   case pp64f : value.re = (Ipp64f)((Ipp64f*)m_pData)[index]; break;
   case pp8sc : value.re = (Ipp64f)((Ipp8sc *)m_pData)[index].re;
                value.im = (Ipp64f)((Ipp8sc *)m_pData)[index].im; break;
   case pp16sc: value.re = (Ipp64f)((Ipp16sc*)m_pData)[index].re;
                value.im = (Ipp64f)((Ipp16sc*)m_pData)[index].im; break;
   case pp32sc: value.re = (Ipp64f)((Ipp32sc*)m_pData)[index].re;
                value.im = (Ipp64f)((Ipp32sc*)m_pData)[index].im; break;
   case pp32fc: value.re = (Ipp64f)((Ipp32fc*)m_pData)[index].re;
                value.im = (Ipp64f)((Ipp32fc*)m_pData)[index].im; break;
   case pp64sc: value.re = (Ipp64f)((Ipp64sc*)m_pData)[index].re;
                value.im = (Ipp64f)((Ipp64sc*)m_pData)[index].im; break;
   case pp64fc: value.re = (Ipp64f)((Ipp64fc*)m_pData)[index].re;
                value.im = (Ipp64f)((Ipp64fc*)m_pData)[index].im; break;
   }
}

void CIppsSignal::GetValue(int index, Ipp64f& value) const
{
   Ipp64fc valueC;
   GetValue(index,valueC);
   value = valueC.re;
}

Ipp64f CIppsSignal::MinVal() const
{
   if (m_Length == 0) return 0;
   Ipp64fc value;
   GetValue(0, value);
   Ipp64f min = value.re;
   if (Complex() && (min > value.im))
      min = value.im;
   for (int i=1; i < m_Length; i++) {
      GetValue(i, value);
      if (min > value.re)
         min = value.re;
      if (Complex() && (min > value.im))
         min = value.im;
   }
   return min;
}

Ipp64f CIppsSignal::MaxVal() const
{
   if (m_Length == 0) return 0;
   Ipp64fc value;
   GetValue(0, value);
   Ipp64f max = value.re;
   if (Complex() && (max < value.im))
      max = value.im;
   for (int i=1; i < m_Length; i++) {
      GetValue(i, value);
      if (max < value.re)
         max = value.re;
      if (Complex() && (max < value.im))
         max = value.im;
   }
   return max;
}
