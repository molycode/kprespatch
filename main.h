#include <windows.h>
#include <commdlg.h>

//http://en.wikipedia.org/wiki/C_standard_library
#include <stdio.h>//http://www.cplusplus.com/reference/clibrary/cstdio/
#include <stdlib.h>//http://www.cplusplus.com/reference/clibrary/cstdlib/
#include <math.h> //http://www.cplusplus.com/reference/clibrary/cmath/
#include <string.h>//http://www.cplusplus.com/reference/clibrary/cstring/

#include <windowsx.h>
#include <commctrl.h>
#include <shellapi.h>//DragQueryFile
#include <Shlwapi.h>//PathStripPath

#include <tchar.h>
#include <time.h>//http://www.cplusplus.com/reference/clibrary/ctime/

#define DEBUG 0

void CheckMD5file (HWND hwnd);

void PatchKPresolutionFile (HWND hwnd, char *kpfilename);
void CheckKPresolutionFile (HWND hwnd, char *kpfilename);

void GetKingpinFileSize (char *kpfilename);
int BackupKingpin (char *kpfilename, char *kpbkfilename);
void CheckKingpinFile (HWND hwnd, char *kpfilename);
void PatchKingpinFile (HWND hwnd, char *kpfilename);

void DisableExtraGroup (HWND hwnd);
void DisableResolutionGroup (HWND hwnd);

//Main.rc
#define DLG_MAIN  1001
#define DLG_ABOUT  1002
#define IDM_ABOUT  6001
#define IDR_ICO_MAIN  8001

#define LBL_TEXT_INFO				4000
#define IDC_GROUP_RESOLUTION   		4001
#define IDC_GROUP_EXTRA		   		4002
#define IDC_BUTTON_KINGPIN	   		4010
#define LBL_TEXT_KP_PATH	   		4011
#define LBL_TEXT_KP_MD5		   		4012
#define LBL_TEXT_KP_CHECK	   		4013
#define IDC_BUTTON_PATCH	   		4014
#define IDC_CHECKBOX_BACKUP		 	4015
#define IDC_CHECKBOX_NOCD	   		4016
#define IDC_CHECKBOX_COOPDMFLAGS 	4017
#define IDC_EDIT_COOPMAXCLIENTS 	4018
#define IDC_SPIN_COOPMAXCLIENTS		4019
#define LBL_TEXT_COOPINFO			4020

#define LBL_TEXT_X		  4500
//Resolution Buttons
#define LBL_TEXT_MODE3	  5000
#define IDC_MODE3_SPIN640 5001
#define IDC_MODE3_EDIT640 5002
#define IDC_MODE3_SPIN480 5003
#define IDC_MODE3_EDIT480 5004
#define LBL_TEXT_MODE4	  5005
#define IDC_MODE4_SPIN800 5006
#define IDC_MODE4_EDIT800 5007
#define IDC_MODE4_SPIN600 5008
#define IDC_MODE4_EDIT600 5009
#define LBL_TEXT_MODE5	  5010
#define IDC_MODE5_SPIN960 5011
#define IDC_MODE5_EDIT960 5012
#define IDC_MODE5_SPIN720 5013
#define IDC_MODE5_EDIT720 5014
#define LBL_TEXT_MODE6	  5015
#define IDC_MODE6_SPIN1024 5016
#define IDC_MODE6_EDIT1024 5017
#define IDC_MODE6_SPIN768 5018
#define IDC_MODE6_EDIT768 5019
#define LBL_TEXT_MODE7	  5020
#define IDC_MODE7_SPIN1152 5021
#define IDC_MODE7_EDIT1152 5022
#define IDC_MODE7_SPIN864 5023
#define IDC_MODE7_EDIT864 5024
#define LBL_TEXT_MODE8	  5025
#define IDC_MODE8_SPIN1280 5026
#define IDC_MODE8_EDIT1280 5027
#define IDC_MODE8_SPIN960 5028
#define IDC_MODE8_EDIT960 5029
#define LBL_TEXT_MODE9	  5030
#define IDC_MODE9_SPIN1600 5031
#define IDC_MODE9_EDIT1600 5032
#define IDC_MODE9_SPIN1200 5033
#define IDC_MODE9_EDIT1200 5034

