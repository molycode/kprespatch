#include "main.h"

#include <io.h>//_open _read
#include <fcntl.h>//O_ flags

void CheckKingpinFile (HWND hwnd, char *kpfilename)
{
	TCHAR buffer[128] = _T("");//128 should be enough
	int file, extraworking, coopclients;
	long fileng ,checkoffs, nocdoffs, flagsoffs, clientoffs;
	unsigned char checkbyte[7], nocdbyte[2], flagsbyte[1], clientbyte[2];

	if ((file = _open(kpfilename, _O_BINARY|_O_RDWR,_S_IREAD)) == -1)
   	{ 
		wsprintf(buffer, _T("Cannot open %s."), kpfilename);
		MessageBox(NULL, buffer, "Error", MB_ICONEXCLAMATION | MB_OK);                                                
   		return;
	}
	fileng = _filelength(file);
	checkoffs = -1;
	nocdoffs = -1;
	flagsoffs = -1;
	clientoffs = -1;
	//Version						Size
	//Kingpin 1.20					376832
	//Kingpin 1.21					376832
	//Kingpin 1.21 Steam			729088
	//Kingpin 1.21 GOG				393216
	//Kingpin alpha demo 0.22N		380928
	
	//Kingpin 1.00					409600
	//Kingpin 1.10					339968
	

	extraworking = 0;

	if (fileng == 376832)//1.21 and 1.20
	{ 
		checkoffs = 0x0004F880;  //1.21 check the word kingpin
		nocdoffs = 0x0004906C;   //1.21 nocd check
		flagsoffs = 0x000596C2;  //1.21 coop dmflags
		clientoffs = 0x00059208; //1.21 coop maxclients
	}
	if (fileng == 729088)//Same offset for some reasson
	{
		checkoffs = 0x0004F880;   //1.21 check the word kingpin
		flagsoffs = 0x000596C2;  //1.21 coop dmflags
		clientoffs = 0x00059208; //1.21 coop maxclients
		extraworking = 1; //Steam version no hexble
	}
	if (fileng == 393216)//Same offset for some reasson
	{
		checkoffs = 0x0004F880;   //1.21 check the word kingpin
		flagsoffs = 0x000596C2;  //1.21 coop dmflags
		clientoffs = 0x00059208; //1.21 coop maxclients
	}
	if (fileng == 380928)//Not same offset and diffrent place word
	{
		checkoffs = 0x000551D6;   //Kingpin alpha 0.22 check the word kingpin
		extraworking = 1;
	}

	if (fileng == 409600)//Not same offset
	{
		checkoffs = 0x00055764;   //Kingpin 1.00 check the word kingpin
		extraworking = 1;
	}
	if (fileng == 339968)//Not same offset
	{
		checkoffs = 0x00048738;   //Kingpin 1.10 check the word kingpin
		extraworking = 1;
	}


	if (checkoffs == -1)
	{
//		MessageBox(NULL, "Incorrect kingpin.exe version.", "Error", MB_ICONEXCLAMATION | MB_OK);
		SetDlgItemText(hwnd,LBL_TEXT_KP_CHECK,"Not a Kingpin file.");
		//Drag en drop
		DisableExtraGroup (hwnd);
		DisableResolutionGroup (hwnd);
		EnableWindow(GetDlgItem(hwnd, IDC_BUTTON_PATCH), FALSE);
		_close(file);
		return;
	}
	_lseek(file,checkoffs,SEEK_SET);
	_read(file,checkbyte,7);
/*	if ((checkbyte[0] != 0x6b) || (checkbyte[1] != 0x69) || (checkbyte[2] != 0x6e) || (checkbyte[3] != 0x67) 
		|| (checkbyte[4] != 0x70) || (checkbyte[5] != 0x69) || (checkbyte[6] != 0x6e))//Spelled kingpin hex*/
	if ((checkbyte[0] != 'k') || (checkbyte[1] != 'i') || (checkbyte[2] != 'n') || (checkbyte[3] != 'g') 
		|| (checkbyte[4] != 'p') || (checkbyte[5] != 'i') || (checkbyte[6] !='n'))
	{
//		MessageBox(NULL, "Not a Kingpin file", "Error", MB_ICONEXCLAMATION | MB_OK);
		SetDlgItemText(hwnd,LBL_TEXT_KP_CHECK,"Not a Kingpin file.");
		//Drag en drop
		DisableExtraGroup (hwnd);
		DisableResolutionGroup (hwnd);
		EnableWindow(GetDlgItem(hwnd, IDC_BUTTON_PATCH), FALSE);
		_close(file);
		return;
	}

	if (extraworking == 1)//Disable rest anyway
	{
		_close(file);
		DisableExtraGroup (hwnd);
		DisableResolutionGroup (hwnd);
		SendMessage(GetDlgItem(hwnd, IDC_SPIN_COOPMAXCLIENTS), UDM_SETPOS, 0, (LPARAM)4);
		CheckDlgButton(hwnd, IDC_CHECKBOX_NOCD, BST_UNCHECKED);
		CheckDlgButton(hwnd, IDC_CHECKBOX_COOPDMFLAGS, BST_UNCHECKED);

		EnableWindow(GetDlgItem(hwnd, IDC_BUTTON_PATCH), FALSE);
		if (fileng == 380928){
			CheckKPresolutionFile (hwnd, kpfilename);//Check resolution
		}
		return;
	}

	//No CD
	if (fileng == 376832)
	{ 
		_lseek(file,nocdoffs,SEEK_SET);
		_read(file,nocdbyte,2);
		if ((nocdbyte[0] == 0x90) || (nocdbyte[1] == 0x90))
		{
//			MessageBox(NULL, "No CD has already been applied.", "Error", MB_ICONEXCLAMATION | MB_OK);
			EnableWindow(GetDlgItem(hwnd, IDC_CHECKBOX_NOCD), TRUE);//Enable Again to remove nocd patch
			CheckDlgButton(hwnd, IDC_CHECKBOX_NOCD, BST_CHECKED);
		}
		else
		{
			CheckDlgButton(hwnd, IDC_CHECKBOX_NOCD, BST_UNCHECKED);
			EnableWindow(GetDlgItem(hwnd, IDC_CHECKBOX_NOCD), TRUE);
		}
	}
	else
		EnableWindow(GetDlgItem(hwnd, IDC_CHECKBOX_NOCD), FALSE);


	//Coop dmflags
	_lseek(file,flagsoffs,SEEK_SET);
	_read(file,flagsbyte,1);
	if (flagsbyte[0] == 0x38)
	{
		EnableWindow(GetDlgItem(hwnd, IDC_CHECKBOX_COOPDMFLAGS), TRUE);//Enable Again to remove coop dmflags patch
		CheckDlgButton(hwnd, IDC_CHECKBOX_COOPDMFLAGS, BST_CHECKED);
	}
	else
	{
		EnableWindow(GetDlgItem(hwnd, IDC_CHECKBOX_COOPDMFLAGS), TRUE);
		CheckDlgButton(hwnd, IDC_CHECKBOX_COOPDMFLAGS, BST_UNCHECKED);
	}


	//Coop maxclients
	_lseek(file,clientoffs,SEEK_SET);
	_read(file,clientbyte,1);
	if (clientbyte[0])
	{
		char setcoopclient[] = " ";//Ammount of space
		strncpy (setcoopclient, (char*)clientbyte, sizeof(clientbyte));	

		coopclients = atoi(setcoopclient); 	

//		setcoopclients = clientbyte[0] - 48;//52 normal = 4 //Old

		if ((coopclients > 1) && (coopclients < 10))//Just to make sure
		{
			EnableWindow(GetDlgItem(hwnd, IDC_EDIT_COOPMAXCLIENTS ), TRUE);
			EnableWindow(GetDlgItem(hwnd, IDC_SPIN_COOPMAXCLIENTS ), TRUE);
			EnableWindow(GetDlgItem(hwnd, LBL_TEXT_COOPINFO), TRUE);
			SendMessage(GetDlgItem(hwnd, IDC_SPIN_COOPMAXCLIENTS), UDM_SETPOS, 0, (LPARAM)(int)coopclients);
		}
		else
		{
			EnableWindow(GetDlgItem(hwnd, IDC_EDIT_COOPMAXCLIENTS ), FALSE);
			EnableWindow(GetDlgItem(hwnd, IDC_SPIN_COOPMAXCLIENTS ), FALSE);
			EnableWindow(GetDlgItem(hwnd, LBL_TEXT_COOPINFO), FALSE);
		} 

//		wsprintf(buffer, _T("Value is 0x%1x"), (unsigned)clientbyte[0]);//Show hex
//		wsprintf(buffer, _T("Value is %c"), (unsigned)clientbyte[0]);//Show char		
//		wsprintf(buffer, _T("Value is %i"), setcoopclients);
//		MessageBox(NULL, buffer, "Info", MB_ICONEXCLAMATION | MB_OK); 
	}

	_close(file);

	CheckKPresolutionFile (hwnd, kpfilename);//Check resolution 
}
void GetKingpinFileSize (char *kpfilename)//Just to get filesize check
{
   	FILE *File;
	long length;
	TCHAR buffer[128] = _T("");//128 should be enough

   	File=fopen(kpfilename,"rb");

   	if(File==NULL)   
	{ 
		wsprintf(buffer, _T("Cannot open %s."), kpfilename);
		MessageBox(NULL, buffer, "Error", MB_ICONEXCLAMATION | MB_OK);                                                
   		return;
	}
	else 
	{
      	fseek(File,0,SEEK_END);
      	length=ftell(File);
		wsprintf(buffer, _T("The Kinpin file's length is %d"), length);
		MessageBox(NULL, buffer, "Success", MB_ICONINFORMATION | MB_OK);
      	fclose(File);
   }
}

