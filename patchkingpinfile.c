#include "main.h"

#include <io.h>//_open _read
#include <fcntl.h>//O_ flags
//http://www.gamedev.net/topic/313486-quick-way-to-printf-a-2-byte-hex-characters/
//http://www.rohitab.com/discuss/topic/37920-simple-offset-patcher/

void PatchKingpinFile (HWND hwnd, char *kpfilename)
{
	TCHAR buffer[128] = _T("");//128 should be enough
	int file, extraworking;
	long fileng , nocdoffs, flagsoffs, clientoffs;
	unsigned char nocdbyte[2], flagsbyte[1], clientbyte[2];

	if ((file = _open(kpfilename,_O_BINARY|_O_RDWR,_S_IWRITE)) == -1)
   	{ 
		wsprintf(buffer, _T("Cannot open %s."), kpfilename);
		MessageBox(NULL, buffer, "Error", MB_ICONEXCLAMATION | MB_OK);                                                
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
		_lseek(file,clientoffs,SEEK_SET);
		HWND maxclients = GetDlgItem(hwnd, IDC_EDIT_COOPMAXCLIENTS);
		int lengthclients = GetWindowTextLength (maxclients);
		char *textclientsbyte = ( char *)GlobalAlloc( GPTR, (lengthclients * ( sizeof (char) ) ) + 1);
		if (!textclientsbyte)
		{
			MessageBox(NULL, "Error with Coop maxclients", "Error", MB_ICONEXCLAMATION | MB_OK);
//			_close(file);
			return;
		} 
		GetWindowText (maxclients, textclientsbyte , lengthclients);
//		wsprintf(buffer, _T("Value is 0x%1x"), textclientsbyte[0]);//Show hex
//		wsprintf(buffer, _T("Value is %c"),textclientsbyte[0]);
//		MessageBox(NULL, buffer, "Info", MB_ICONEXCLAMATION | MB_OK); 
 		clientbyte[0] = textclientsbyte[0];
		_write(file,clientbyte,1);  
		GlobalFree (textclientsbyte);
//	}
	//Coop maxclients done
	
	_close(file);	 
}

int BackupKingpin (char *kpfilename, char *kpbkfilename)
{
	FILE *from, *to;
  	char ch;

  	//open source file
  	if((from = fopen(kpfilename, "rb"))==NULL) 
	{
		MessageBox(NULL, "Cannot open kingpin.exe file", "Error", MB_ICONEXCLAMATION | MB_OK);
    	exit(1);
  	}

  	//open destination file
  	if((to = fopen(kpbkfilename, "wb"))==NULL) 
	{
		MessageBox(NULL, "Cannot open kingpinbackup.exe file", "Error", MB_ICONEXCLAMATION | MB_OK);
    	exit(1);
  	}

  	// copy the file
	while(!feof(from))
 	{
		ch = fgetc(from);
    	if(ferror(from)) 
		{
			MessageBox(NULL, "Error reading kingpin.exe file", "Error", MB_ICONEXCLAMATION | MB_OK);;
      		exit(1);
    	}
    	if(!feof(from)) 
			fputc(ch, to);
    	if(ferror(to)) 
		{
			MessageBox(NULL, "Error writing kingpinbackup.exe file", "Error", MB_ICONEXCLAMATION | MB_OK);
      		exit(1);
    	}
  	}

  	if(fclose(from)==EOF) 
	{
		MessageBox(NULL, "Error closing kingpin.exe file", "Error", MB_ICONEXCLAMATION | MB_OK);;
    	exit(1);
  	}

  	if(fclose(to)==EOF) 
	{
		MessageBox(NULL, "Error closing kingpinbackup.exe file", "Error", MB_ICONEXCLAMATION | MB_OK);
    	exit(1);
	}
	return 0;
}
void DisableExtraGroup (HWND hwnd)
{
	EnableWindow(GetDlgItem(hwnd, IDC_GROUP_EXTRA), FALSE);

	EnableWindow(GetDlgItem(hwnd, IDC_CHECKBOX_NOCD), FALSE);
	EnableWindow(GetDlgItem(hwnd, IDC_CHECKBOX_COOPDMFLAGS), FALSE);

	EnableWindow(GetDlgItem(hwnd, IDC_EDIT_COOPMAXCLIENTS ), FALSE);
	EnableWindow(GetDlgItem(hwnd, IDC_SPIN_COOPMAXCLIENTS ), FALSE);
	EnableWindow(GetDlgItem(hwnd, LBL_TEXT_COOPINFO), FALSE);
}
