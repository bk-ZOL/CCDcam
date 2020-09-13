#include "pch.h"
#include "framework.h"
#include "CCDCam.h"

#include "Imaging.h"
#include "BitranCCDlib.h"
#include <stdio.h>
#include <commctrl.h>

#define MAX_LOADSTRING 100
static TCHAR szWork[MAX_LOADSTRING], szText[256];

// Basic exposure example
bool WINAPI SnapExposure(ExposeInfo *expo, LPWORD pImageData, HINSTANCE hInst, HWND hBar, int& width, int& height)
{  
    // It is different by a camera
    if (expo->fandelay != -1)
    {
        if (expo->gainup != -1)
        {   // BU-61,62,63
            BitranCCDlibEnvironment[0](
                                (expo->fandelay ? 1 : 0) +   // Fan control
                                (expo->gainup ? 64 : 0),// Gain-up mode
                                64 + 3);
        }
        else
        {   // BU-60                                    // The fan works after exposure
            BitranCCDlibEnvironment[0](expo->fandelay > 0, 3);
        }

        if (expo->fandelay > 0)
            Sleep(expo->fandelay);
    }
    
    // A start of the exposure
    unsigned int result = BitranCCDlibStartExposure[0](
                                expo->time,             // Shutter speed
                                expo->binning,          // Binning mode
                                expo->binning,
                                expo->trigger,          // The trigger is optional
                                expo->center,           // Image area
                                expo->mode,             // A/D mode
                                expo->emgain);          // EM-Gain
    if (result)
    {   
        width = result % 65536;                         // Image width
        height = result / 65536;                        // Image height

        // You must wait until exposure is finished
        int status, old = 0;
        while (((status = BitranCCDlibCameraState[0](0)) >= 0) || (status == -3))
        {
            if (status == -3)
            {   // The camera is waiting for trigger input
               /* LoadString(hInst, IDS_TRIGGER, szWork, MAX_LOADSTRING);
                SendMessage(hBar, SB_SETTEXT, 5, (LPARAM)szWork);*/
            }
            else
            if (status >= 10)
            {   // Countdown of the remaining time
                if ((status /= 10) != old)
                {
                    /*LoadString(hInst, IDS_TIME, szWork, MAX_LOADSTRING);
                    sprintf_s(szText, _countof(szText), szWork, old = status);
                    SendMessage(hBar, SB_SETTEXT, 5, (LPARAM)szText);*/
                }
            }
        }         
        
        // You acquire an image
        if ((status == -2) || (status == -4))
        {
            //LoadString(hInst, IDS_SEND, szWork, MAX_LOADSTRING);

            for (int i = 0; i < height; i += 100)
            {
                int lines;
                if ((i + 100) > height)
                    lines = height - i;
                else
                    lines = 100;
                /*sprintf_s(szText, _countof(szText), szWork, i, i + lines);
                SendMessage(hBar, SB_SETTEXT, 5, (LPARAM)szText);*/

                int lng = BitranCCDlibTransferImage[0](i, lines, &pImageData[i * width]);
                if (lng != width * lines)
                    result = 0;     // failure
            }

            if (result)
                BitranCCDlibImageInterpolation[0](pImageData);
        }
        else
            result = 0;             // failure
    }

    // It was finished
    BitranCCDlibFinishExposure[0](-1);
    
   /* LoadString(hInst, result ? IDS_COMPLETE : IDS_FAILURE, szWork, MAX_LOADSTRING);
    SendMessage(hBar, SB_SETTEXT, 5, (LPARAM)szWork);*/
   
    return result != 0;
}

// This made a basic sample multi-exposure
bool WINAPI MultiExposure(ExposeInfo *expo, HINSTANCE hInst, HWND hBar)
{
    if (expo->fandelay >= 0)
    {
        for (int ix = 0; ix < MultiCount; ix++)
            if (expo->gainup >= 0)
                BitranCCDlibEnvironment[ix]((expo->fandelay > 0) | (expo->gainup << 6), 0x43);
            else
                BitranCCDlibEnvironment[ix](expo->fandelay > 0, 0x03);
        Sleep(expo->fandelay);
    }

    int width[BITRANMULTI], height[BITRANMULTI];
    unsigned int result;
    for (int ix = 0; ix < MultiCount; ix++)
    {
        result = BitranCCDlibStartExposure[ix](expo->time, expo->binning, expo->binning, expo->trigger, expo->center, expo->mode, expo->emgain);
        if (!result)
            break;                  // failure
        width[ix] = result & 0xffff;
        height[ix] = result >> 16;
    }
    
    for (int ix = 0; result && (ix < MultiCount); ix++)
    {
        int status;
        while (((status = BitranCCDlibCameraState[ix](2)) >= 0) || (status == -3))
        {
            /*sprintf_s(szText, _countof(szText), _T("Status=%i"), status);
            SendMessage(hBar, SB_SETTEXT, 5, (LPARAM)szText);*/
        }

        // You acquire an image
        if ((status == -2) || (status == -4))
        {
            //LoadString(hInst, IDS_SEND, szWork, MAX_LOADSTRING);
            int read = 256;
            for (int i = 0; i < height[ix]; i += read)
            {
                int lines = ((i + read) > height[ix]) ? height[ix] - i : read;
               /* sprintf_s(szText, _countof(szText), szWork, i, i + lines);
                SendMessage(hBar, SB_SETTEXT, 5, (LPARAM)szText);*/

                int lng = BitranCCDlibTransferImage[ix](i, lines, &pImageData[ix][i * width[ix]]);
                if (lng != width[ix] * lines)
                    result = 0;     // failure
            }

            if (result)
                BitranCCDlibImageInterpolation[ix](pImageData[ix]);
        }
        else
            result = 0;             // failure
    }

    for (int ix = 0; ix < MultiCount; ix++)
        BitranCCDlibFinishExposure[ix](-1);

    /*LoadString(hInst, result ? IDS_COMPLETE : IDS_FAILURE, szWork, MAX_LOADSTRING);
    SendMessage(hBar, SB_SETTEXT, 5, (LPARAM)szWork);*/

    return result != 0;
}

// A preview sample
int WINAPI PreviewExposure(ExposeInfo *expo, HWND hwnd)
{
    // Start exposure
    if (expo->gainup >= 0)
        BitranCCDlibEnvironment[MultiIndex](expo->gainup << 6, 0x40);
    UINT image = BitranCCDlibStartExposure[MultiIndex](expo->time, expo->binning, expo->binning,
                                        expo->trigger, expo->center, expo->mode, expo->emgain);
    if (image == 0)
        return 0;                   // failure

    // Display an image
    while (!expo->abort && BitranCCDlibMonitorImage[MultiIndex](hwnd))
    {
        BitranCCDlibFinishExposure[MultiIndex](-3);
        BitranCCDlibContinueExposure[MultiIndex]();
    }

    // Postprocessing
    return BitranCCDlibFinishExposure[MultiIndex](-1);
}

// Acquire an image continually
int WINAPI RepeatExposure(ExposeInfo* expo, LPWORD pImageData, HWND hWnd, HBITMAP* phBmp)
{
    // Start exposure
    if (expo->gainup >= 0)
        BitranCCDlibEnvironment[MultiIndex](expo->gainup * 64, 64);
    UINT image = BitranCCDlibStartExposure[MultiIndex](expo->time, expo->binning, expo->binning,
                                         expo->trigger, expo->center, expo->mode, expo->emgain);
    int cycle = 0;
    int lng = LOWORD(image) * HIWORD(image);
    while (image && !expo->abort)
    {   // You wait until you can acquire an image
        int status = BitranCCDlibCameraState[MultiIndex](1);
        if (status >= 0)
            continue;
        if (status < -4)
            break;                  // failure

        // You acquire an image, and display it
        if (lng != BitranCCDlibTransferImage[MultiIndex](0, 0, (LPWORD)pImageData))
            break;                  // failure

        BitranCCDlibImageInterpolation[MultiIndex](pImageData);
        cycle = BitranCCDlibFinishExposure[MultiIndex](-3);

        if (*phBmp)
            DeleteObject(*phBmp);
        *phBmp = BitranCCDlibImageConvert[MultiIndex](NULL, pImageData);
        InvalidateRect(hWnd, NULL, FALSE);

        if (!BitranCCDlibContinueExposure[MultiIndex]())
            break;                  // failure
    }

    // Postprocessing
    BitranCCDlibFinishExposure[MultiIndex](-1);
    return cycle;
}

// Example of the image transformation
HBITMAP WINAPI ImageConversion(int mode, bool color, LPWORD pImageData, int lng)
{
    if (mode == 0)
        return BitranCCDlibImageConvert[0](NULL, pImageData);

    unsigned int ConvData[] = {color ? 8 : 4, mode - 1, 65536, 0, 65536, 0, 65536, 0};
    for (int i = 0; i < lng; i++)
    {
        if (ConvData[2] > pImageData[i])
            ConvData[2] = pImageData[i];
        if (ConvData[3] < pImageData[i])
            ConvData[3] = pImageData[i];
    }

    if (mode == 1)
        ConvData[3] /= 2;

    if (color)
    {   // Adjust the color balance
        ConvData[4] = ConvData[6] = ConvData[2];
        ConvData[5] = ConvData[3];
        ConvData[3] = ConvData[5] * 100 / 110;
        ConvData[7] = ConvData[5] * 100 / 140;
    }

    return BitranCCDlibImageConvert[0](ConvData, pImageData);
}

