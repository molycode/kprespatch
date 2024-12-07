#include "main.h"
#include "resource.h"

#include <io.h>//_open _read
#include <fcntl.h>//O_ flags
#include <sys/stat.h>//_S_IREAD
#include <strsafe.h>

//http://www.gamedev.net/topic/313486-quick-way-to-printf-a-2-byte-hex-characters/
//http://www.rohitab.com/discuss/topic/37920-simple-offset-patcher/

void PatchKingpinFile(HWND hwnd, TCHAR* kpfilename)
{
	TCHAR buffer[128] = _T("");//128 should be enough
	int file, extraworking;
	long fileng , nocdoffs, flagsoffs, clientoffs;
	unsigned char nocdbyte[2], flagsbyte[1];

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

	if ((fileng == 729088) |(fileng == 393216) | (fileng == 376832))
		extraworking = 1;
	else
		extraworking = 0;

	if (extraworking == 0)//Disable rest anyway
	{
		_close(file);
		return;
	}

	nocdoffs = 0x0004906C;   //1.21 nocd check
	flagsoffs = 0x000596C2;  //1.21 coop dmflags
	clientoffs = 0x00059208; //1.21 coop maxclients

	//No CD
	if (fileng == 376832)//Only 1.21 and 1.20
	{
		if (IsDlgButtonChecked(hwnd, IDC_CHECKBOX_NOCD))
		{ 
			_lseek(file,nocdoffs,SEEK_SET);
			nocdbyte[0] = 0x90;
			nocdbyte[1] = 0x90;
			_write(file,nocdbyte,2);
		}
		else//Put cd check back
		{
			_lseek(file,nocdoffs,SEEK_SET);
			nocdbyte[0] = 0x74;
			nocdbyte[1] = 0x02;
			_write(file,nocdbyte,2);
		}
	}

	//Coop dmflags
//	if (GetDlgItem(hwnd, IDC_CHECKBOX_COOPDMFLAGS))
//	{
		if (IsDlgButtonChecked(hwnd, IDC_CHECKBOX_COOPDMFLAGS))//Coop dmflags 784 to 788
		{ 
			_lseek(file,flagsoffs,SEEK_SET);
			flagsbyte[0] = 0x38;
			_write(file,flagsbyte,1);
		}
		else//Coop dmflags 788 to 784
		{
			_lseek(file,flagsoffs,SEEK_SET);
			flagsbyte[0] = 0x34;
			_write(file,flagsbyte,1);
		}
//	}


	//Coop maxclients
//	if (GetDlgItem(hwnd, IDC_EDIT_COOPMAXCLIENTS))
//	{
		_lseek(file, clientoffs, SEEK_SET);

		HWND maxclients = GetDlgItem(hwnd, IDC_EDIT_COOPMAXCLIENTS);
		int lengthclients = GetWindowTextLength(maxclients);
		TCHAR* textclientsbyte = (TCHAR*) malloc((lengthclients + 1) * sizeof(TCHAR));

		if (!textclientsbyte)
		{
			MessageBox(NULL, _T("Error with Coop maxclients"), _T("Error"), MB_ICONEXCLAMATION | MB_OK);
			return;
		}

		GetWindowText(maxclients, textclientsbyte, lengthclients + 1);
		TCHAR clientbyte[2];
		clientbyte[0] = textclientsbyte[0];

		if (_write(file, clientbyte, 1) == -1)
		{
			MessageBox(NULL, _T("Error writing to file"), _T("Error"), MB_ICONEXCLAMATION | MB_OK);
		}

		free(textclientsbyte);
//	}
	//Coop maxclients done
	
	_close(file);
}

int BackupKingpin(TCHAR* kpfilename, TCHAR* kpbkfilename)
{
	FILE *from, *to;
	int ch;

	//open source file
	if ((from = _tfopen(kpfilename, _T("rb"))) == NULL)
	{
		MessageBox(NULL, _T("Cannot open kingpin.exe file"), _T("Error"), MB_ICONEXCLAMATION | MB_OK);
		exit(1);
	}

	//open destination file
	if ((to = _tfopen(kpbkfilename, _T("wb"))) == NULL)
	{
		MessageBox(NULL, _T("Cannot open kingpinbackup.exe file"), _T("Error"), MB_ICONEXCLAMATION | MB_OK);
		exit(1);
	}

	// copy the file
	while (!feof(from))
	{
		ch = fgetc(from);

		if (ferror(from))
		{
			MessageBox(NULL, _T("Error reading kingpin.exe file"), _T("Error"), MB_ICONEXCLAMATION | MB_OK);
			exit(1);
		}
		
		if (!feof(from))
			fputc(ch, to);
		
		if (ferror(to))
		{
			MessageBox(NULL, _T("Error writing kingpinbackup.exe file"), _T("Error"), MB_ICONEXCLAMATION | MB_OK);
			exit(1);
		}
	}

	if (fclose(from)==EOF)
	{
		MessageBox(NULL, _T("Error closing kingpin.exe file"), _T("Error"), MB_ICONEXCLAMATION | MB_OK);
		exit(1);
	}

	if (fclose(to)==EOF)
	{
		MessageBox(NULL, _T("Error closing kingpinbackup.exe file"), _T("Error"), MB_ICONEXCLAMATION | MB_OK);
			exit(1);
	}

	return 0;
}

void DisableExtraGroup(HWND hwnd)
{
	EnableWindow(GetDlgItem(hwnd, IDC_GROUP_EXTRA), FALSE);

	EnableWindow(GetDlgItem(hwnd, IDC_CHECKBOX_NOCD), FALSE);
	EnableWindow(GetDlgItem(hwnd, IDC_CHECKBOX_COOPDMFLAGS), FALSE);

	EnableWindow(GetDlgItem(hwnd, IDC_EDIT_COOPMAXCLIENTS ), FALSE);
	EnableWindow(GetDlgItem(hwnd, IDC_SPIN_COOPMAXCLIENTS ), FALSE);
	EnableWindow(GetDlgItem(hwnd, LBL_TEXT_COOPINFO), FALSE);
}
