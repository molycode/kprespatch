#pragma once

#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <commdlg.h>
#include <commctrl.h>
#include <stdlib.h>

void CheckMD5file(HWND hwnd);

void PatchKPresolutionFile(HWND hwnd, TCHAR* kpfilename);
void CheckKPresolutionFile(HWND hwnd, TCHAR* kpfilename);

void GetKingpinFileSize(TCHAR* kpfilename);
int BackupKingpin(TCHAR* kpfilename, TCHAR* kpbkfilename);
void CheckKingpinFile(HWND hwnd, TCHAR* kpfilename);
void PatchKingpinFile(HWND hwnd, TCHAR* kpfilename);

void DisableExtraGroup(HWND hwnd);
void DisableResolutionGroup(HWND hwnd);
