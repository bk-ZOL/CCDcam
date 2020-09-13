// BitranCCDlib.h : The part which communicates with a camera
//

#pragma once


#define ILLEGAL_VALUE 10000

typedef int (WINAPI* LPFNDLLFUNC1)(const char*,int*,int*);
typedef void (WINAPI* LPFNDLLFUNC2)();
typedef int (WINAPI* LPFNDLLFUNC3)(int,char*,int);
typedef int (WINAPI* LPFNDLLFUNC4)();
typedef void (WINAPI* LPFNDLLFUNC5)(int);
typedef HWND(WINAPI* LPFNDLLFUNC6)(int);
typedef int (WINAPI* LPFNDLLFUNC7)(int);
typedef int (WINAPI* LPFNDLLFUNC8)(int,int);
typedef unsigned int (WINAPI* LPFNDLLFUNC9)(int,int,int,int,int,int,int);
typedef HBITMAP (WINAPI* LPFNDLLFUNC10)(HWND);
typedef int (WINAPI* LPFNDLLFUNC11)(int,int,unsigned short*);
typedef void (WINAPI* LPFNDLLFUNC12)(unsigned short*);
typedef HBITMAP (WINAPI* LPFNDLLFUNC13)(const unsigned int*,unsigned short*);
typedef const char* (WINAPI* LPFNDLLFUNC14)(int, const char*,unsigned short*);

#define BITRANMULTI 5
#define CAMERANAME  20


#if defined BitranCCDlib


// WC wait

BOOL InitBitranCCDlib(char* lpCmdLine);
VOID ExitBitranCCDlib();


extern int MultiIndex;
extern int MultiCount;
extern HINSTANCE hBitranCCDlibDLL[BITRANMULTI];
extern int CameraType[BITRANMULTI];
extern LPWORD pImageData[BITRANMULTI];

extern char BitranCCDlibCamera[BITRANMULTI][CAMERANAME];
extern LPFNDLLFUNC1 BitranCCDlibCreate[BITRANMULTI];
extern LPFNDLLFUNC2 BitranCCDlibDestroy[BITRANMULTI];
extern LPFNDLLFUNC3 BitranCCDlibCameraInfo[BITRANMULTI];
extern LPFNDLLFUNC4 BitranCCDlibGetVoltage[BITRANMULTI];
extern LPFNDLLFUNC5 BitranCCDlibSetCoolerPower[BITRANMULTI];
extern LPFNDLLFUNC4 BitranCCDlibGetCoolerPower[BITRANMULTI];
extern LPFNDLLFUNC6 BitranCCDlibSetTemperature[BITRANMULTI];
extern LPFNDLLFUNC7 BitranCCDlibGetTemperature[BITRANMULTI];
extern LPFNDLLFUNC8 BitranCCDlibEnvironment[BITRANMULTI];
extern LPFNDLLFUNC9 BitranCCDlibStartExposure[BITRANMULTI];
extern LPFNDLLFUNC4 BitranCCDlibContinueExposure[BITRANMULTI];
extern LPFNDLLFUNC7 BitranCCDlibCameraState[BITRANMULTI];
extern LPFNDLLFUNC7 BitranCCDlibAbortExposure[BITRANMULTI];
extern LPFNDLLFUNC7 BitranCCDlibFinishExposure[BITRANMULTI];
extern LPFNDLLFUNC10 BitranCCDlibMonitorImage[BITRANMULTI];
extern LPFNDLLFUNC11 BitranCCDlibTransferImage[BITRANMULTI];
extern LPFNDLLFUNC12 BitranCCDlibImageInterpolation[BITRANMULTI];
extern LPFNDLLFUNC13 BitranCCDlibImageConvert[BITRANMULTI];
extern LPFNDLLFUNC14 BitranCCDlibImageSave[BITRANMULTI];
#else
extern int MultiIndex, MultiCount;
extern int CameraType[BITRANMULTI];
extern LPWORD pImageData[BITRANMULTI];
extern char BitranCCDlibCamera[BITRANMULTI][CAMERANAME];
extern LPFNDLLFUNC1 BitranCCDlibCreate[BITRANMULTI];
extern LPFNDLLFUNC2 BitranCCDlibDestroy[BITRANMULTI];
extern LPFNDLLFUNC3 BitranCCDlibCameraInfo[BITRANMULTI];
extern LPFNDLLFUNC4 BitranCCDlibGetVoltage[BITRANMULTI];
extern LPFNDLLFUNC5 BitranCCDlibSetCoolerPower[BITRANMULTI];
extern LPFNDLLFUNC4 BitranCCDlibGetCoolerPower[BITRANMULTI];
extern LPFNDLLFUNC6 BitranCCDlibSetTemperatue[BITRANMULTI];
extern LPFNDLLFUNC7 BitranCCDlibGetTemperatue[BITRANMULTI];
extern LPFNDLLFUNC8 BitranCCDlibEnvironment[BITRANMULTI];
extern LPFNDLLFUNC9 BitranCCDlibStartExposure[BITRANMULTI];
extern LPFNDLLFUNC4 BitranCCDlibContinueExposure[BITRANMULTI];
extern LPFNDLLFUNC7 BitranCCDlibCameraState[BITRANMULTI];
extern LPFNDLLFUNC7 BitranCCDlibAbortExposure[BITRANMULTI];
extern LPFNDLLFUNC7 BitranCCDlibFinishExposure[BITRANMULTI];
extern LPFNDLLFUNC10 BitranCCDlibMonitorImage[BITRANMULTI];
extern LPFNDLLFUNC11 BitranCCDlibTransferImage[BITRANMULTI];
extern LPFNDLLFUNC12 BitranCCDlibImageInterpolation[BITRANMULTI];
extern LPFNDLLFUNC13 BitranCCDlibImageConvert[BITRANMULTI];
extern LPFNDLLFUNC14 BitranCCDlibImageSave[BITRANMULTI];
#endif