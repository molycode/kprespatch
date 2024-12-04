#include "main.h"
#include "resource.h"
#include <io.h> // _open _read
#include <fcntl.h> // O_ flags
#include <sys/stat.h> // _S_IREAD
#include <commctrl.h> // UDM_SETPOS
#include <strsafe.h> // StringCchPrintf

void CheckResolution(HWND hwnd, int file, size_t size, long offset, char* sizeX, char* sizeY, int spinX, int spinY, int editX, int editY)
{
	_lseek(file, offset, SEEK_SET);
	char* resText = (char*)malloc(size * sizeof(char));
	
	if (resText != NULL)
	{
		int readBytes = _read(file, resText, size);

		if (readBytes == -1)
		{
			_tperror(_T("Error reading file"));
			free(resText);
			return;
		}

		resText[size * sizeof(char) - 1] = '\0';

		char result[10]; // never bigger than 10
		snprintf(result, sizeof(result), "%s %s", sizeX, sizeY);

		if ((_stricmp(resText, result) == 0))
		{
			EnableWindow(GetDlgItem(hwnd, spinX), TRUE);
			EnableWindow(GetDlgItem(hwnd, spinY), TRUE);
			EnableWindow(GetDlgItem(hwnd, editX), TRUE);
			EnableWindow(GetDlgItem(hwnd, editY), TRUE);

			SendMessage(GetDlgItem(hwnd, spinX), UDM_SETPOS, 0, (LPARAM) atoi(sizeX));
			SendMessage(GetDlgItem(hwnd, spinY), UDM_SETPOS, 0, (LPARAM) atoi(sizeY));
		}
		else
		{
			EnableWindow(GetDlgItem(hwnd, spinX), FALSE);
			EnableWindow(GetDlgItem(hwnd, spinY), FALSE);
			EnableWindow(GetDlgItem(hwnd, editX), FALSE);
			EnableWindow(GetDlgItem(hwnd, editY), FALSE);

			SetDlgItemText(hwnd, LBL_TEXT_KP_CHECK, _T("Can not read mode3."));
		}

		free(resText);
	}
}

void CheckKPresolutionFile(HWND hwnd, TCHAR* kpfilename)//Read resolutions
{
	TCHAR buffer[MAX_PATH] = _T("");
	int file;
	long fileng;
	long mode3offs, mode4offs, mode5offs, mode6offs, mode7offs, mode8offs, mode9offs;

#ifdef UNICODE
	if ((file = _wopen(kpfilename, _O_BINARY | _O_RDWR, _S_IREAD)) == -1)
#else
	if ((file = _open(kpfilename, _O_BINARY | _O_RDWR, _S_IREAD)) == -1)
#endif
	{
		StringCchPrintf(buffer, 128, _T("Cannot open %s."), kpfilename);
		MessageBox(NULL, buffer, _T("Error"), MB_ICONEXCLAMATION | MB_OK);

		return;
	}

	fileng = _filelength(file);

	if (fileng == 380928)//Demo version
	{
		mode3offs = 0x00058701; //640 480
		mode4offs = 0x000586F5; //800 600
		mode5offs = 0x000586E9; //960 720
		mode6offs = 0x000586DD; //1024 768
		mode7offs = 0x000586D1; //1152 864
		mode8offs = 0x000586C5; //1280 960
		mode9offs = 0x000586B9; //1600 1200
	}
	else//All 1.21 versions got same offset
	{
		mode3offs = 0x0005A7A9; //640 480
		mode4offs = 0x0005A79D; //800 600
		mode5offs = 0x0005A791; //960 720
		mode6offs = 0x0005A785; //1024 768
		mode7offs = 0x0005A779; //1152 864
		mode8offs = 0x0005A76D; //1280 960
		mode9offs = 0x0005A761; //1600 1200
	}

	CheckResolution(hwnd, file, 8, mode3offs, "640", "480", IDC_MODE3_SPIN640, IDC_MODE3_SPIN480, IDC_MODE3_EDIT640, IDC_MODE3_EDIT480);
	CheckResolution(hwnd, file, 8, mode4offs, "800", "600", IDC_MODE4_SPIN800, IDC_MODE4_SPIN600, IDC_MODE4_EDIT800, IDC_MODE4_EDIT600);
	CheckResolution(hwnd, file, 8, mode5offs, "960", "720", IDC_MODE5_SPIN960, IDC_MODE5_SPIN720, IDC_MODE5_EDIT960, IDC_MODE5_EDIT720);
	CheckResolution(hwnd, file, 9, mode6offs, "1024", "768", IDC_MODE6_SPIN1024, IDC_MODE6_SPIN768, IDC_MODE6_EDIT1024, IDC_MODE6_EDIT768);
	CheckResolution(hwnd, file, 9, mode7offs, "1152", "864", IDC_MODE7_SPIN1152, IDC_MODE7_SPIN864, IDC_MODE7_EDIT1152, IDC_MODE7_EDIT864);
	CheckResolution(hwnd, file, 9, mode8offs, "1280", "960", IDC_MODE8_SPIN1280, IDC_MODE8_SPIN960, IDC_MODE8_EDIT1280, IDC_MODE8_EDIT960);
	CheckResolution(hwnd, file, 10, mode9offs, "1600", "1200", IDC_MODE9_SPIN1600, IDC_MODE9_SPIN1200, IDC_MODE9_EDIT1600, IDC_MODE9_EDIT1200);

	_close(file);

	EnableWindow(GetDlgItem(hwnd, IDC_GROUP_RESOLUTION), TRUE);
	EnableWindow(GetDlgItem(hwnd, IDC_BUTTON_PATCH), TRUE);
}