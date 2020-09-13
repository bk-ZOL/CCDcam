#pragma once


#include "resource.h"

#include "types.h"

bool WINAPI SnapExposure(ExposeInfo *expo, LPWORD pImageData, HINSTANCE hInst, HWND hBar, int& width, int& height);
bool WINAPI MultiExposure(ExposeInfo *expo, HINSTANCE hInst, HWND hBar);
int  WINAPI PreviewExposure(ExposeInfo *expo, HWND hwnd);
int  WINAPI RepeatExposure(ExposeInfo *expo, LPWORD pImageData, HWND hWnd, HBITMAP *phBmp);
HBITMAP WINAPI ImageConversion(int mode, bool color, LPWORD pImageData, int lng);

