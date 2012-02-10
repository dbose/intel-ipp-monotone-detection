/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2000-2005 Intel Corporation. All Rights Reserved.
//
*/

// ippsAddC.h: interface for the CIppsAddC class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IPPSADDC_H__AED27709_ABD1_4B51_B053_C5C1AE28FF79__INCLUDED_)
#define AFX_IPPSADDC_H__AED27709_ABD1_4B51_B053_C5C1AE28FF79__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSampleDoc;

class CIppsAddC
{
public:
   CIppsAddC();
   virtual ~CIppsAddC();
// Attributes
   // Determinate if function ippsAddC_16s_Sfs can process the signal
   BOOL FitSignal(CIppsSignal* pSignal) const;
// Operations
   // Process document pSrc by ippiAddC_8u_C3R
   void ProcessSignal(CSampleDoc *pSrc);

protected:
   // Run dialog to get ippsAddC_16s_Sfs parameters
   BOOL GetParameters();

   // ippsAddC_16s_Sfs parameters
   static Ipp16s m_value;
   static int   m_scaleFactor;
};

#endif // !defined(AFX_IPPSADDC_H__AED27709_ABD1_4B51_B053_C5C1AE28FF79__INCLUDED_)
