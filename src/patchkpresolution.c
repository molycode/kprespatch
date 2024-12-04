#include "main.h"
#include "resource.h"

#include "hexutils.h"

#include <io.h>//_open _read
#include <fcntl.h>//O_ flags
#include <sys/stat.h>//_S_IREAD
#include <strsafe.h>

static void WriteToFile(int file, long offset, TCHAR* source, unsigned int maxChars)
{
	char singleByteData[4] = {'\0'};

#ifdef UNICODE
	// Convert wide characters to single-byte encoding (e.g., UTF-8 or ASCII)
	wcstombs(singleByteData, source, sizeof(singleByteData));
#else
	strncpy(singleByteData, source, sizeof(singleByteData));
#endif

	_lseek(file, offset, SEEK_SET);
	_write(file, singleByteData, maxChars);
}

static void WriteNewMode(HWND hwnd, int file, TCHAR* byte1, TCHAR* byte3, long offset1, long offset2, long offset3, int digits)
{
	HWND dialog = GetDlgItem(hwnd, IDC_MODE3_EDIT640);
	int dialogLength = GetWindowTextLength(dialog);
	TCHAR* dialogText = (TCHAR*) malloc((dialogLength + 1) * sizeof(TCHAR));

	if (!dialogText)
	{
		MessageBox(NULL, _T("Error allocating memory!"), _T("Error"), MB_ICONEXCLAMATION | MB_OK);
		return;
	}

	GetWindowText(dialog, dialogText, dialogLength + 1);

	TCHAR* valueString = malloc(sizeof(TCHAR) * 4);

	if (!valueString)
	{
		free(dialogText);
		MessageBox(NULL, _T("Memory allocation failed for valueString"), _T("Error"), MB_ICONEXCLAMATION | MB_OK);
		return;
	}

	memset(valueString, 0, sizeof(TCHAR) * 4); // Moly: copy 3 or 4 characters?
	_tcsncpy(valueString, dialogText, 3);
	valueString[3] = _T('\0');

	free(dialogText);

	int value = _ttoi(valueString);

	TCHAR* hexconValue = malloc(sizeof(TCHAR) * 4); // Int to Hex
	TCHAR* hexValue = malloc(sizeof(TCHAR) * 4); // Endianness

	if (hexconValue != NULL && hexValue != NULL)
	{
		memset(hexconValue, 0, sizeof(TCHAR) * 4);
		_itot(value, hexconValue, 16);

		memset(hexValue, 0, sizeof(TCHAR) * 4);

		if (digits == 3)
		{
			if (value < 256)//or FF=255 add 0
			{
				hexValue[0] = hexconValue[0];
				hexValue[1] = hexconValue[1];
				hexValue[2] = '0';
				hexValue[3] = '0';
			}
			else
			{
				hexValue[0] = hexconValue[1];
				hexValue[1] = hexconValue[2]; //Maybe need switch with 0 but seems okay
				hexValue[2] = '0';
				hexValue[3] = hexconValue[0];
			}
		}
		else if (digits == 4)
		{
			if (value > 4095)//or FFF=4095 remove 0
			{
				hexValue[0] = hexconValue[2];
				hexValue[1] = hexconValue[3];
				hexValue[2] = hexconValue[0];//Should always be 0?
				hexValue[3] = hexconValue[1];
			}
			else
			{
				hexValue[0] = hexconValue[1];
				hexValue[1] = hexconValue[2]; //Maybe need switch with 0 but seems okay
				hexValue[2] = '0';
				hexValue[3] = hexconValue[0];
			}
		}

		stringtohex(hexValue, byte3, 4);

		memset(byte1, 0, sizeof(byte1));
		_tcsncpy(byte1, valueString, 3); // Moly: copy 3 or 4?
		free(valueString);

		WriteToFile(file, offset1, byte1, 4);
		WriteToFile(file, offset2, byte1, 4);
		WriteToFile(file, offset3, byte3, 4);
	}
}

//http://www.wsgf.org/article/common-hex-values

//Kingpin Resolutions:
/*
"[640 480 ]", MODE 3
"[800 600 ]", MODE 4
"[960 720 ]", MODE 5
"[1024 768 ]", MODE 6
"[1152 864 ]", MODE 7
"[1280 960 ]", MODE 8
"[1600 1200]", MODE 9
*/

/*
1) Calculator must be set in decimal mode (dec);
2) Type a value (for example, 1920);
3) Change to hexadecimal mode (hex);
4) The value will change to hex mode (in our example, 780);
5) Using your awesome brain, add a zero in front of this value (780 --> 0780);
6) Split the value in 2 groups of 2 digits and reverse them (07 // 80 --> 80 07);
7) You're done, enter this value into your .exe file with a hex editor.
*/
/*
640x480 x= 80 02   y= E0 01
704x480 x= C0 02   y= E0 01
720x480 x= D0 02   y= E0 01
800x600 x= 20 03   y= 58 02
852x480 x= 54 03   y= E0 01
1024x768 x= 00 04   y= 00 03
1080x720 x= 38 04   y= 00 03
1280x720 x= 00 04   y= D0 02
1280x800 x= 00 05   y= 20 03
1280x1024   x= 00 05   y= 00 04
1360x850 x= 50 05   y= 52 03
1366x768 x= 56 05   y= 00 03
1440x900 x= A0 05   y= 84 03
1600x1200   x= 40 06   y= B0 04
1680x1050   x= 90 06   y= 1A 04
1920x1080   x= 80 07   y= 38 04
1920x1200   x= 80 07   y= B0 04
*/
void PatchKPresolutionFile(HWND hwnd, TCHAR* kpfilename)
{
	TCHAR buffer[128] = _T("");//128 should be enough
	int file;
	long fileng;
	long mode3_640_offs1, mode3_480_offs1, mode4_800_offs1, mode4_600_offs1, mode5_960_offs1, mode5_720_offs1,
		 mode6_1024_offs1, mode6_768_offs1, mode7_1152_offs1, mode7_864_offs1, mode8_1280_offs1, mode8_960_offs1,
		 mode9_1600_offs1, mode9_1200_offs1;
	long mode3_640_offs2, mode3_480_offs2, mode4_800_offs2, mode4_600_offs2, mode5_960_offs2, mode5_720_offs2,
		 mode6_1024_offs2, mode6_768_offs2, mode7_1152_offs2, mode7_864_offs2, mode8_1280_offs2, mode8_960_offs2,
		 mode9_1600_offs2, mode9_1200_offs2;
	long mode3_640_offs3, mode3_480_offs3, mode4_800_offs3, mode4_600_offs3, mode5_960_offs3, mode5_720_offs3,
		 mode6_1024_offs3, mode6_768_offs3, mode7_1152_offs3, mode7_864_offs3, mode8_1280_offs3, mode8_960_offs3,
		 mode9_1600_offs3, mode9_1200_offs3;

	TCHAR mode3_640_byte1[4], mode3_480_byte1[2], mode4_800_byte1[2], mode4_600_byte1[2], mode5_960_byte1[2], mode5_720_byte1[2],
		mode6_1024_byte1[3], mode6_768_byte1[2], mode7_1152_byte1[3], mode7_864_byte1[2], mode8_1280_byte1[3], mode8_960_byte1[2],
		mode9_1600_byte1[3], mode9_1200_byte1[3];
	TCHAR mode3_640_byte3[4], mode3_480_byte3[2], mode4_800_byte3[2], mode4_600_byte3[2], mode5_960_byte3[2], mode5_720_byte3[2],
		mode6_1024_byte3[2], mode6_768_byte3[2], mode7_1152_byte3[3], mode7_864_byte3[2], mode8_1280_byte3[3], mode8_960_byte3[2],
		mode9_1600_byte3[3], mode9_1200_byte3[2];//Always 2 bytes


#ifdef UNICODE
	if ((file = _wopen(kpfilename, _O_BINARY | _O_RDWR, _S_IWRITE)) == -1)
#else
	if ((file = _open(kpfilename, _O_BINARY | _O_RDWR, _S_IWRITE)) == -1)
#endif
	{
		StringCchPrintf(buffer, 128, _T("Cannot open %s."), kpfilename);
		MessageBox(NULL, buffer, _T("Error"), MB_ICONEXCLAMATION | MB_OK);

		return;
	}

	fileng = _filelength(file);

	if (fileng == 380928)//Demo version
	{
		//Start window menu
		mode3_640_offs1  = 0x00058701; //640
		mode3_480_offs1  = 0x00058705; //480
		mode4_800_offs1  = 0x000586F5; //800
		mode4_600_offs1  = 0x000586F9; //600
		mode5_960_offs1  = 0x000586E9; //960
		mode5_720_offs1  = 0x000586ED; //720
		mode6_1024_offs1 = 0x000586DD; //1024
		mode6_768_offs1  = 0x000586E2; //768
		mode7_1152_offs1 = 0x000586D1; //1152
		mode7_864_offs1  = 0x000586D6; //864
		mode8_1280_offs1 = 0x000586C5; //1280
		mode8_960_offs1  = 0x000586CA; //960
		mode9_1600_offs1 = 0x000586B9; //1600
		mode9_1200_offs1 = 0x000586BE; //1200
		//End window menu

		//Start modeset
		mode3_640_offs2  = 0x00058468; //640
		mode3_480_offs2  = 0x0005846C; //480
		mode4_800_offs2  = 0x00058458; //800
		mode4_600_offs2  = 0x0005845C; //600
		mode5_960_offs2  = 0x00058448; //960
		mode5_720_offs2  = 0x0005844C; //720
		mode6_1024_offs2 = 0x00058434; //1024
		mode6_768_offs2  = 0x00058439; //768
		mode7_1152_offs2 = 0x00058420; //1152
		mode7_864_offs2  = 0x00058425; //864
		mode8_1280_offs2 = 0x0005840C; //1280
		mode8_960_offs2  = 0x00058411; //960
		mode9_1600_offs2 = 0x000583F8; //1600
		mode9_1200_offs2 = 0x000583FD; //1200
		//End start modeset

		//Start ingame
		mode3_640_offs3  = 0x00058384; //640
		mode3_480_offs3  = 0x00058388; //480
		mode4_800_offs3  = 0x00058394; //800
		mode4_600_offs3  = 0x00058398; //600
		mode5_960_offs3  = 0x000583A4; //960
		mode5_720_offs3  = 0x000583A8; //720
		mode6_1024_offs3 = 0x000583B4; //1024
		mode6_768_offs3  = 0x000583B8; //768
		mode7_1152_offs3 = 0x000583C4; //1152
		mode7_864_offs3  = 0x000583C8; //864
		mode8_1280_offs3 = 0x000583D4; //1280
		mode8_960_offs3  = 0x000583D8; //960
		mode9_1600_offs3 = 0x000583E4; //1600
		mode9_1200_offs3 = 0x000583E8; //1200
	}
	else
	{
		//All 1.21 versions got same offset
		//Start window menu
		mode3_640_offs1  = 0x0005A7A9; //640
		mode3_480_offs1  = 0x0005A7AD; //480
		mode4_800_offs1  = 0x0005A79D; //800
		mode4_600_offs1  = 0x0005A7A1; //600
		mode5_960_offs1  = 0x0005A791; //960
		mode5_720_offs1  = 0x0005A795; //720
		mode6_1024_offs1 = 0x0005A785; //1024
		mode6_768_offs1  = 0x0005A78A; //768
		mode7_1152_offs1 = 0x0005A779; //1152
		mode7_864_offs1  = 0x0005A77E; //864
		mode8_1280_offs1 = 0x0005A76D; //1280
		mode8_960_offs1  = 0x0005A772; //960
		mode9_1600_offs1 = 0x0005A761; //1600
		mode9_1200_offs1 = 0x0005A766; //1200
		//End window menu

		//Start modeset
		mode3_640_offs2  = 0x0005A4A0; //640
		mode3_480_offs2  = 0x0005A4A4; //480
		mode4_800_offs2  = 0x0005A490; //800
		mode4_600_offs2  = 0x0005A494; //600
		mode5_960_offs2  = 0x0005A480; //960
		mode5_720_offs2  = 0x0005A484; //720
		mode6_1024_offs2 = 0x0005A46C; //1024
		mode6_768_offs2  = 0x0005A471; //768
		mode7_1152_offs2 = 0x0005A458; //1152
		mode7_864_offs2  = 0x0005A45D; //864
		mode8_1280_offs2 = 0x0005A444; //1280
		mode8_960_offs2  = 0x0005A449; //960
		mode9_1600_offs2 = 0x0005A430; //1600
		mode9_1200_offs2 = 0x0005A435; //1200
		//End start modeset

		//Start ingame
		mode3_640_offs3  = 0x0005A3BC; //640
		mode3_480_offs3  = 0x0005A3C0; //480
		mode4_800_offs3  = 0x0005A3CC; //800
		mode4_600_offs3  = 0x0005A3D0; //600
		mode5_960_offs3  = 0x0005A3DC; //960
		mode5_720_offs3  = 0x0005A3E0; //720
		mode6_1024_offs3 = 0x0005A3EC; //1024 
		mode6_768_offs3  = 0x0005A3F0; //768
		mode7_1152_offs3 = 0x0005A3FC; //1152 
		mode7_864_offs3  = 0x0005A400; //864
		mode8_1280_offs3 = 0x0005A40C; //1280 
		mode8_960_offs3  = 0x0005A410; //960
		mode9_1600_offs3 = 0x0005A41C; //1600
		mode9_1200_offs3 = 0x0005A420; //1200
	}

	WriteNewMode(hwnd, file, mode3_640_byte1, mode3_640_byte3, mode3_640_offs1, mode3_640_offs2, mode3_640_offs3, 3);
	WriteNewMode(hwnd, file, mode3_480_byte1, mode3_480_byte3, mode3_480_offs1, mode3_480_offs2, mode3_480_offs3, 3);
	WriteNewMode(hwnd, file, mode4_800_byte1, mode4_800_byte3, mode4_800_offs1, mode4_800_offs2, mode4_800_offs3, 3);
	WriteNewMode(hwnd, file, mode4_600_byte1, mode4_600_byte3, mode4_600_offs1, mode4_600_offs2, mode4_600_offs3, 3);
	WriteNewMode(hwnd, file, mode5_960_byte1, mode5_960_byte3, mode5_960_offs1, mode5_960_offs2, mode5_960_offs3, 3);
	WriteNewMode(hwnd, file, mode5_720_byte1, mode5_720_byte3, mode5_720_offs1, mode5_720_offs2, mode5_720_offs3, 3);
	WriteNewMode(hwnd, file, mode6_1024_byte1, mode6_1024_byte3, mode6_1024_offs1, mode6_1024_offs2, mode6_1024_offs3, 4);
	WriteNewMode(hwnd, file, mode6_768_byte1, mode6_768_byte3, mode6_768_offs1, mode6_768_offs2, mode6_768_offs3, 3);
	WriteNewMode(hwnd, file, mode7_1152_byte1, mode7_1152_byte3, mode7_1152_offs1, mode7_1152_offs2, mode7_1152_offs3, 4);
	WriteNewMode(hwnd, file, mode7_864_byte1, mode7_864_byte3, mode7_864_offs1, mode7_864_offs2, mode7_864_offs3, 3);
	WriteNewMode(hwnd, file, mode8_1280_byte1, mode8_1280_byte3, mode8_1280_offs1, mode8_1280_offs2, mode8_1280_offs3, 4);
	WriteNewMode(hwnd, file, mode8_960_byte1, mode8_960_byte3, mode8_960_offs1, mode8_960_offs2, mode8_960_offs3, 3);
	WriteNewMode(hwnd, file, mode9_1600_byte1, mode9_1600_byte3, mode9_1600_offs1, mode9_1600_offs2, mode9_1600_offs3, 4);
	WriteNewMode(hwnd, file, mode9_1200_byte1, mode9_1200_byte3, mode9_1200_offs1, mode9_1200_offs2, mode9_1200_offs3, 4);

	_close(file);
}

void DisableResolutionGroup (HWND hwnd)
{
	EnableWindow(GetDlgItem(hwnd, IDC_GROUP_RESOLUTION), FALSE);

	EnableWindow(GetDlgItem(hwnd, IDC_MODE3_SPIN640), FALSE);
	EnableWindow(GetDlgItem(hwnd, IDC_MODE3_SPIN480), FALSE);
	EnableWindow(GetDlgItem(hwnd, IDC_MODE4_SPIN800), FALSE);
	EnableWindow(GetDlgItem(hwnd, IDC_MODE4_SPIN600), FALSE);
	EnableWindow(GetDlgItem(hwnd, IDC_MODE5_SPIN960), FALSE);
	EnableWindow(GetDlgItem(hwnd, IDC_MODE5_SPIN720), FALSE);
	EnableWindow(GetDlgItem(hwnd, IDC_MODE6_SPIN1024), FALSE);
	EnableWindow(GetDlgItem(hwnd, IDC_MODE6_SPIN768), FALSE);
	EnableWindow(GetDlgItem(hwnd, IDC_MODE7_SPIN1152), FALSE);
	EnableWindow(GetDlgItem(hwnd, IDC_MODE7_SPIN864), FALSE);
	EnableWindow(GetDlgItem(hwnd, IDC_MODE8_SPIN1280), TRUE);
	EnableWindow(GetDlgItem(hwnd, IDC_MODE8_SPIN960), FALSE);
	EnableWindow(GetDlgItem(hwnd, IDC_MODE9_SPIN1600), FALSE);
	EnableWindow(GetDlgItem(hwnd, IDC_MODE9_SPIN1200), FALSE);

	EnableWindow(GetDlgItem(hwnd, IDC_MODE3_EDIT640), FALSE);
	EnableWindow(GetDlgItem(hwnd, IDC_MODE3_EDIT480), FALSE);
	EnableWindow(GetDlgItem(hwnd, IDC_MODE4_EDIT800), FALSE);
	EnableWindow(GetDlgItem(hwnd, IDC_MODE4_EDIT600), FALSE);
	EnableWindow(GetDlgItem(hwnd, IDC_MODE5_EDIT960), FALSE);
	EnableWindow(GetDlgItem(hwnd, IDC_MODE5_EDIT720), FALSE);
	EnableWindow(GetDlgItem(hwnd, IDC_MODE6_EDIT1024), FALSE);
	EnableWindow(GetDlgItem(hwnd, IDC_MODE6_EDIT768), FALSE);
	EnableWindow(GetDlgItem(hwnd, IDC_MODE7_EDIT1152), FALSE);
	EnableWindow(GetDlgItem(hwnd, IDC_MODE7_EDIT864), FALSE);
	EnableWindow(GetDlgItem(hwnd, IDC_MODE8_EDIT1280), FALSE);
	EnableWindow(GetDlgItem(hwnd, IDC_MODE8_EDIT960), FALSE);
	EnableWindow(GetDlgItem(hwnd, IDC_MODE9_EDIT1600), FALSE);
	EnableWindow(GetDlgItem(hwnd, IDC_MODE9_EDIT1200), FALSE);
}
