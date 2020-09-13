#include "pch.h"
#include "framework.h"
#include "CCDCam.h"
#include "BitranCCDlib.h"



int MultiIndex = -1;
int MultiCount = 0;
HINSTANCE hBitranCCDlibDLL[BITRANMULTI];
int CameraType[BITRANMULTI];
LPWORD pImageData[BITRANMULTI];

char BitranCCDlibCamera[BITRANMULTI][CAMERANAME];
LPFNDLLFUNC1 BitranCCDlibCreate[BITRANMULTI];
LPFNDLLFUNC2 BitranCCDlibDestroy[BITRANMULTI];
LPFNDLLFUNC3 BitranCCDlibCameraInfo[BITRANMULTI];
LPFNDLLFUNC4 BitranCCDlibGetVoltage[BITRANMULTI];
LPFNDLLFUNC5 BitranCCDlibSetCoolerPower[BITRANMULTI];
LPFNDLLFUNC4 BitranCCDlibGetCoolerPower[BITRANMULTI];
LPFNDLLFUNC6 BitranCCDlibSetTemperature[BITRANMULTI];
LPFNDLLFUNC7 BitranCCDlibGetTemperature[BITRANMULTI];
LPFNDLLFUNC8 BitranCCDlibEnvironment[BITRANMULTI];
LPFNDLLFUNC9 BitranCCDlibStartExposure[BITRANMULTI];
LPFNDLLFUNC4 BitranCCDlibContinueExposure[BITRANMULTI];
LPFNDLLFUNC7 BitranCCDlibCameraState[BITRANMULTI];
LPFNDLLFUNC7 BitranCCDlibAbortExposure[BITRANMULTI];
LPFNDLLFUNC7 BitranCCDlibFinishExposure[BITRANMULTI];
LPFNDLLFUNC10 BitranCCDlibMonitorImage[BITRANMULTI];
LPFNDLLFUNC11 BitranCCDlibTransferImage[BITRANMULTI];
LPFNDLLFUNC12 BitranCCDlibImageInterpolation[BITRANMULTI];
LPFNDLLFUNC13 BitranCCDlibImageConvert[BITRANMULTI];
LPFNDLLFUNC14 BitranCCDlibImageSave[BITRANMULTI];




// When one camera uses BU60USBlib.lib, it is easy
BOOL InitBitranCCDlib(char* lpCmdLine)
{
	char *ptr1 = lpCmdLine, *ptr2;
	for (MultiCount = 0; MultiCount < BITRANMULTI; MultiCount++)
	{
		ptr2 = strpbrk(ptr1, ",");
		if (ptr2)
		{
			*ptr2 = 0;
			strcpy_s(BitranCCDlibCamera[MultiCount], CAMERANAME, ptr1);
			ptr1 = ptr2 + 1;
		}
		else
		{
			strcpy_s(BitranCCDlibCamera[MultiCount], CAMERANAME, ptr1);
			break;
		}
	}
	MultiCount++;

	char name[100];
	for (int i = 0; i < MultiCount; i++)
	{
		pImageData[i] = NULL;
		strcpy_s(name, sizeof(name), "BU60USBlib");
		if (i > 0)
		{
			strcat_s(name, sizeof(name), strchr(BitranCCDlibCamera[i], '(') + 1);
			*strchr(name, ')') = 0;
		}
		strcat_s(name, sizeof(name), ".dll");

		hBitranCCDlibDLL[i] = LoadLibraryA(name);
		if (hBitranCCDlibDLL[i] == NULL)
		{
			MultiCount = i;
			strcat_s(name, sizeof(name), " is not found");
			MessageBoxA(NULL, name, NULL, MB_OK | MB_ICONSTOP);
			return FALSE;
		}

		BitranCCDlibCreate[i] = (LPFNDLLFUNC1)
			GetProcAddress(hBitranCCDlibDLL[i], "BitranCCDlibCreate");
		BitranCCDlibDestroy[i] = (LPFNDLLFUNC2)
			GetProcAddress(hBitranCCDlibDLL[i], "BitranCCDlibDestroy");
		BitranCCDlibCameraInfo[i] = (LPFNDLLFUNC3)
			GetProcAddress(hBitranCCDlibDLL[i], "BitranCCDlibCameraInfo");
		BitranCCDlibGetVoltage[i] = (LPFNDLLFUNC4)
			GetProcAddress(hBitranCCDlibDLL[i], "BitranCCDlibGetVoltage");
		BitranCCDlibSetCoolerPower[i] = (LPFNDLLFUNC5)
			GetProcAddress(hBitranCCDlibDLL[i], "BitranCCDlibSetCoolerPower");
		BitranCCDlibGetCoolerPower[i] = (LPFNDLLFUNC4)
			GetProcAddress(hBitranCCDlibDLL[i], "BitranCCDlibGetCoolerPower");
		BitranCCDlibSetTemperature[i] = (LPFNDLLFUNC6)
			GetProcAddress(hBitranCCDlibDLL[i], "BitranCCDlibSetTemperature");
		BitranCCDlibGetTemperature[i] = (LPFNDLLFUNC7)
			GetProcAddress(hBitranCCDlibDLL[i], "BitranCCDlibGetTemperature");
		BitranCCDlibEnvironment[i] = (LPFNDLLFUNC8)
			GetProcAddress(hBitranCCDlibDLL[i], "BitranCCDlibEnvironment");
		BitranCCDlibStartExposure[i] = (LPFNDLLFUNC9)
			GetProcAddress(hBitranCCDlibDLL[i], "BitranCCDlibStartExposure");
		BitranCCDlibContinueExposure[i] = (LPFNDLLFUNC4)
			GetProcAddress(hBitranCCDlibDLL[i], "BitranCCDlibContinueExposure");
		BitranCCDlibCameraState[i] = (LPFNDLLFUNC7)
			GetProcAddress(hBitranCCDlibDLL[i], "BitranCCDlibCameraState");
		BitranCCDlibAbortExposure[i] = (LPFNDLLFUNC7)
			GetProcAddress(hBitranCCDlibDLL[i], "BitranCCDlibAbortExposure");
		BitranCCDlibFinishExposure[i] = (LPFNDLLFUNC7)
			GetProcAddress(hBitranCCDlibDLL[i], "BitranCCDlibFinishExposure");
		BitranCCDlibMonitorImage[i] = (LPFNDLLFUNC10)
			GetProcAddress(hBitranCCDlibDLL[i], "BitranCCDlibMonitorImage");
		BitranCCDlibTransferImage[i] = (LPFNDLLFUNC11)
			GetProcAddress(hBitranCCDlibDLL[i], "BitranCCDlibTransferImage");
		BitranCCDlibImageInterpolation[i] = (LPFNDLLFUNC12)
			GetProcAddress(hBitranCCDlibDLL[i], "BitranCCDlibImageInterpolation");
		BitranCCDlibImageConvert[i] = (LPFNDLLFUNC13)
			GetProcAddress(hBitranCCDlibDLL[i], "BitranCCDlibImageConvert");
		BitranCCDlibImageSave[i] = (LPFNDLLFUNC14)
			GetProcAddress(hBitranCCDlibDLL[i], "BitranCCDlibImageSave");
	}

	return TRUE;
}

VOID ExitBitranCCDlib()
{
	for (int i = 0; i < MultiCount; i++)
	{
		if (pImageData[i])
			delete[] pImageData[i];

		BitranCCDlibDestroy[i]();
		FreeLibrary(hBitranCCDlibDLL[i]);
	}
}

