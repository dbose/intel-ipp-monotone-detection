/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2000-2005 Intel Corporation. All Rights Reserved.
//
*/

// ippsAddC.cpp: implementation of the CIppsAddC class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsSample.h"
#include "SampleDoc.h"
#include "ippsAddCDlg.h"
#include "ippsAddC.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Static parameters for ippsAddC_16s_Sfs
Ipp16s CIppsAddC::m_value = 1000;
int   CIppsAddC::m_scaleFactor = 0;

CIppsAddC::CIppsAddC()
{

}

CIppsAddC::~CIppsAddC()
{

}

////////////////////////////////////////////////////////////////////
// Determinate if function ippsAddC_16s_Sfs can process the image

BOOL CIppsAddC::FitSignal(CIppsSignal* pSignal) const
{
   if (pSignal->Type() != pp16s) return FALSE;
   return TRUE;
}

int GetPowerSpectrum(Ipp16sc* FreqData,
					float* PowerSpectrum,
					int NumDataPoints
) 
{
    int i;

    for (i = 0; i < NumDataPoints; i++) 
	{
        PowerSpectrum[i] = FreqData[i].re * FreqData[i].re + FreqData[i].im * FreqData[i].im;
    }

    return 0;
}

void DumpArray(Ipp32f* power_spectrum, int length)
{
	char* pszFileName = "c:\\FFT.dat";
	CFile myFile;
	CFileException fileException;

	if (!myFile.Open(pszFileName, CFile::modeCreate | CFile::modeReadWrite, 
					&fileException))
	{
		TRACE( "Can't open file %s, error = %u\n",
		   pszFileName, fileException.m_cause );
	}

	char      szBuffer[3] = "\n";
	CString   format;
	for(int i = 0; i < length; i++)
	{
		format.Format("%.9f ",power_spectrum[i]);
		myFile.Write((LPCTSTR)format, format.GetLength() * 2);
		myFile.Write(szBuffer, sizeof(szBuffer));
		format.Empty();
	}

	myFile.Flush();
	myFile.Close();
}

////////////////////////////////////////////////////////////////////
// Process document pSrc by ippsAddC_16s_Sfs

void CIppsAddC::ProcessSignal(CSampleDoc *pSrc)
{
   // if function doesn't fit the src image then return
   if (!FitSignal(pSrc)) return;

   // Run dialog to get function parameters
   //if (!GetParameters()) return;

   // create new invisible document for dst image
   
	IppStatus status;
	int order, NumUniquePts, sampling_rate;
	int frequency_delta;

	//FFT length, in integration this will be time windows and so as the order.
	int length = pSrc->Length();		//N
	int windowSize = 1024;				//FFT size

    sampling_rate = pSrc->SamplesPerSecond();			//16k or 8k
    //frequency_delta = sampling_rate / length;			// S/N

	int windowCount = length / windowSize;
	int offset;

	//Calculate order the signal
	order = 10;		//2^10 = 1024

	///////////////////////////////////////////
	//FFT Allocation
	IppsFFTSpec_R_32f* spec;
	status = ippsFFTInitAlloc_R_32f(&spec, 
									order,							//order
									IPP_FFT_DIV_FWD_BY_N,			//By N, IPP_FFT_DIV_FWD_BY_N or IPP_FFT_DIV_INV_BY_N,
									ippAlgHintNone);

	// Show message box if status is wrong
	IppErrorMessage("ippsFFTInitAlloc_R_32f", status);

	if (status < 0)
	   return;

	//////////////////////////////////////////
	//Working buffer
	int bufSize;
	status = ippsFFTGetBufSize_R_32f(spec, &bufSize);
	
	// Show message box if status is wrong
	IppErrorMessage("ippsDFTGetBufSize_R_32f", status);

	if (status < 0)
	   return;

	Ipp8u* pWorkBuf = (Ipp8u*)malloc(bufSize);

	Ipp32f powerThreashold = 0.45;
	int hits = 0, maxHits = 4;

	//Time block of 1024 samples
	for(int window = 0; window < windowCount; window++)
	{
		CSampleDoc* pDst = CSampleDoc::CreateNewDoc(windowSize + 2, pp32f, FALSE);
		if (!pDst) 
			return;

		CSampleDoc* pDst_Power_Spectrum = CSampleDoc::CreateNewDoc(windowSize / 2 + 1, pp32f, FALSE);
		if (!pDst) 
			return;

		offset = window * windowSize;			//overflow??

		//Expensive int->float array conversion
		//16bit data
		Ipp16s* in = ((Ipp16s*)(pSrc->DataPtr()) + offset);
		Ipp32f* in_dbl = ippsMalloc_32f(windowSize);
		status = ippsConvert_16s32f(in,
									in_dbl,
									windowSize);

		//Aliases...
		Ipp32f* out = ((Ipp32f*)pDst->DataPtr());
		Ipp32f* power_spectrum = ((Ipp32f*)pDst_Power_Spectrum->DataPtr());

		//1. Hamming window (smooth out)...
		status = ippsWinHamming_32f_I(in_dbl, 
									  windowSize);

		// Show message box if status is wrong
		IppErrorMessage("ippsWinHamming_32f_I", status);

		if (status < 0)
		   return;

		//loop-rolled spec/working buffer allocation...

		////////////////////
		//Core FFT
		////////////////////
		status = ippsFFTFwd_RToCCS_32f(in_dbl, 
									   out, 
									   spec,
									   NULL);

		// Show message box if status is wrong
		IppErrorMessage("ippsFFTFwd_RToCCS_32f", status);

		///////////////////
		//Magnitude
		if (status < 0)
		   return;

		status = ippsMagnitude_32fc((Ipp32fc*)out,
								power_spectrum,
								windowSize / 2);

		// Show message box if status is wrong
		IppErrorMessage("ippsMagnitude_32fc", status);

		if (status < 0)
		   return;


		//Scale the fft so that it is not a function of the length of x, mx = mx/length(x)
		status = ippsDivC_32f_I((Ipp32f)(windowSize / 2),
								power_spectrum,
								windowSize / 2);

		// Show message box if status is wrong
		IppErrorMessage("ippsDivC_32f_I", status);
		if (status < 0)
		   return;

		//We need power ~ sqr(mag.)
		status = ippsSqr_32f_I(power_spectrum,
								windowSize / 2);

		// Show message box if status is wrong
		IppErrorMessage("ippsSqr_32f_I", status);
		if (status < 0)
		   return;

		///*Since we dropped half the FFT, we multiply mx by 2 to keep the same energy.
		//The DC component and Nyquist component, if it exists, are unique and should not
		//be mulitplied by 2.
		//if rem(nfft, 2) % odd nfft excludes Nyquist point
		//  mx(2:end) = mx(2:end)*2;
		//else
		//  mx(2:end -1) = mx(2:end -1)*2;
		//end */
		//status = ippsMulC_32f_I((Ipp32f)2.0,
		//							(power_spectrum + 1),
		//							windowSize / 2 - 1);

		//// Show message box if status is wrong
		//IppErrorMessage("ippsMulC_32f_I", status);
		//if (status < 0)
		//   return;

		//Total power
		Ipp32f totalPower;
		status = ippsSum_32f((const Ipp32f*)power_spectrum,
							windowSize / 2,
							&totalPower,
							ippAlgHintNone);

		//Divide to normalize - p(n) / p - how much of the total power is
		//contained in each amplitude
		//
		if (totalPower > 0.0f)
		{
			status = ippsDivC_32f_I((Ipp32f)totalPower,
								power_spectrum,
								windowSize / 2);

			// Show message box if status is wrong
			IppErrorMessage("ippsDivC_32f_I", status);
			if (status < 0)
			   return;

			//Normally a "sharp" peak happens to be the overall max and it must contain
			//most of the power.
			Ipp32f maxWindowPower = 0.0f;
			ippsMax_32f(power_spectrum,
						windowSize / 2,
						&maxWindowPower);

			//probably we have a tone frame...
			if (maxWindowPower >= powerThreashold)
			{
				hits++;
				if (hits >= maxHits)
				{
					CString title = pDst_Power_Spectrum->GetTitle();
					title += " We are in a tone!";
					pDst_Power_Spectrum->SetTitle((LPCTSTR)title);

					hits = 0;
				}
			}
			else
			{
				hits = 0;
			}
		}
		else
		{
			//hits = 0;
		}

		pDst_Power_Spectrum->MakeVisible();
		//pDst_Peak->MakeVisible();
		
	}

}


////////////////////////////////////////////////////////////////////
// Run dialog to get ippsAddC_16s_Sfs parameters

BOOL CIppsAddC::GetParameters()
{
   // Dialog to get function parameters
   CIppsAddCDlg dlg;

   // Set parameters to dialog
   dlg.m_value = m_value;
   dlg.m_scaleFactor = m_scaleFactor;

   // Run dialog
   if (dlg.DoModal() != IDOK) return FALSE;

   // Get parameters from dialog
   m_value = dlg.m_value;
   m_scaleFactor = dlg.m_scaleFactor;

   return TRUE;
}
