/*////////////////////////////////////////////////////////////////////////////
 *  Project:
 *    Memory_and_Exception_Trace
 *
 * ///////////////////////////////////////////////////////////////////////////
 *  File:
 *    Stackwalker.h
 *
 *  Remarks:
 *
 *
 *  Note:
 *
 *
 *  Author:
 *    Jochen Kalmbach
 *
 *////////////////////////////////////////////////////////////////////////////

#ifndef __STACKWALKER_H__
#define __STACKWALKER_H__

// Only valid in the following environment: Intel platform, MS VC++ 5/6/7/7.1
#ifndef _X86_
#error Only INTEL envirnoments are supported!
#endif

// Only MS VC++ 5 to 7
#if (_MSC_VER < 1100) || (_MSC_VER > 1600)
#error Only MS VC++ 5/6/7/7.1 supported. Check if the '_CrtMemBlockHeader' has not changed with this compiler!
#endif
// 
// 　MS VC++ 10.0 _MSC_VER = 1600 　　
// 	 MS VC++ 9.0 _MSC_VER = 1500 　
// 	 　MS VC++ 8.0 _MSC_VER = 1400 　
// 	  　MS VC++ 7.1 _MSC_VER = 1310 　　
// 	   MS VC++ 7.0 _MSC_VER = 1300 　　
// 	   MS VC++ 6.0 _MSC_VER = 1200 　　
// 	   MS VC++ 5.0 _MSC_VER = 1100
typedef enum eAllocCheckOutput
{
  ACOutput_Simple,
  ACOutput_Advanced,
  ACOutput_XML
};

// Make extern "C", so it will also work with normal C-Programs
#ifdef __cplusplus
extern "C" {
#endif
extern int InitAllocCheckWN(eAllocCheckOutput eOutput, LPCTSTR pszFilename, ULONG ulShowStackAtAlloc = 0);
extern int InitAllocCheck(eAllocCheckOutput eOutput = ACOutput_Simple, BOOL bSetUnhandledExeptionFilter = TRUE, ULONG ulShowStackAtAlloc = 0);  // will create the filename by itself

extern ULONG DeInitAllocCheck();

extern DWORD StackwalkFilter( EXCEPTION_POINTERS *ep, DWORD status, LPCTSTR pszLogFile);

extern void OnlyInstallUnhandeldExceptionFilter(eAllocCheckOutput eOutput = ACOutput_Simple);

#ifdef __cplusplus
}
#endif

#endif  // __STACKWALKER_H__
