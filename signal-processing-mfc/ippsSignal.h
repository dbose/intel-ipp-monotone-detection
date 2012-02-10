/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2000-2005 Intel Corporation. All Rights Reserved.
//
*/

// ippsSignal.h: interface for the CIppsSignal class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IPPSSIGNAL_H__9ECA0E4F_BCA4_46A0_BE31_165B8B567569__INCLUDED_)
#define AFX_IPPSSIGNAL_H__9ECA0E4F_BCA4_46A0_BE31_165B8B567569__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <ippcore.h>
#include <ipps.h>
#include <ippsr.h>

// IPP Data Types definition

#define PP_MASK  0x0FF
#define PP_SIGN  0x100
#define PP_FLOAT 0x200
#define PP_CPLX  0x400

typedef enum {
   ppNONE = 0 ,
   pp8u   = 8 ,
   pp8s   = 8  | PP_SIGN,
   pp8sc  = 8  | PP_SIGN | PP_CPLX,
   pp16u  = 16,
   pp16s  = 16 | PP_SIGN,
   pp16sc = 16 | PP_SIGN | PP_CPLX,
   pp32u  = 32,
   pp32s  = 32 | PP_SIGN,
   pp32sc = 32 | PP_SIGN | PP_CPLX,
   pp32f  = 32 | PP_FLOAT ,
   pp32fc = 32 | PP_FLOAT  | PP_CPLX,
   pp64s  = 64 | PP_SIGN,
   pp64sc = 64 | PP_SIGN | PP_CPLX,
   pp64f  = 64 | PP_FLOAT ,
   pp64fc = 64 | PP_FLOAT  | PP_CPLX,
} ppType;

// Signal class
class CIppsSignal
{
public:
   CIppsSignal();
   CIppsSignal(const CIppsSignal& img);
   virtual ~CIppsSignal();
   CIppsSignal& operator =(const CIppsSignal& signal);

// General Operations
   // Set signal header and allocate data
   BOOL CreateSignal(int length, ppType type = pp8s);
   // Load signal from wav file
   BOOL LoadSignal(CFile* pFile);
   // Save signal to wav file
   BOOL SaveSignal(CFile* pFile);

// Attributes
   // Get pointer to data
   void* DataPtr();
   // Get data size in bytes
   int  DataSize() const;
   // Get complex value of vector element
   void GetValue(int index, Ipp64fc& value) const;
   // Get real value of vector element
   void GetValue(int index, Ipp64f& value) const;
   // Get minimal value of real and image signal components
   Ipp64f MinVal() const;
   // Get maximal real value of real and image signal components
   Ipp64f MaxVal() const;

   // Get signal width
   int  Length() const { return m_Length;}
   // Get data type
   ppType Type() const { return m_Type;}
   // Get size of data element in bits
   int Depth() const { return m_Type & PP_MASK;}
   // Get signed data flag
   BOOL Sign() const { return m_Type & PP_SIGN ? TRUE : FALSE;}
   // Get float data flag
   BOOL Float() const { return m_Type & PP_FLOAT ? TRUE : FALSE;}
   // Get unsigned data flag
   bool Unsign() const { return !Sign() && !Float();}
   // Get complex data flag
   BOOL Complex() const { return m_Type & PP_CPLX ? TRUE : FALSE;}
   // Get & set samples per second value
   int SamplesPerSecond() { return m_SamplesPerSecond;}
   void SamplesPerSecond(int mode) { m_SamplesPerSecond = mode;}

   // Get string with data type description
   CString TypeString() const;

   // Set data changed / unchanged flag
   // This flag must be set to TRUE if data has been changed
   // Then CView::OnDraw function will update bitmap and
   // set this flag to FALSE
   void IsUpdated(BOOL updated) {m_Updated = updated;}
   // Get data changed / unchanged flag
   // Used by CView::OnDraw function
   BOOL IsUpdated() const {return m_Updated;}
protected:
   int       m_Length;
   ppType    m_Type;
   void*     m_pData;
   int       m_SamplesPerSecond;
   BOOL      m_Updated;

   void Reset();
   void AllocateData();
   void FreeData();
};

#endif // !defined(AFX_IPPSSIGNAL_H__9ECA0E4F_BCA4_46A0_BE31_165B8B567569__INCLUDED_)
