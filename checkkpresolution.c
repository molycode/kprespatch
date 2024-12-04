#include "main.h"

#include <io.h>//_open _read
#include <fcntl.h>//O_ flags

void CheckKPresolutionFile (HWND hwnd, char *kpfilename)//Read resolutions
{
	TCHAR buffer[128] = _T("");//128 should be enough
	int file;
	long fileng;
	long mode3offs, mode4offs, mode5offs, mode6offs, mode7offs, mode8offs, mode9offs;
	unsigned char mode3byte[7], mode4byte[7], mode5byte[7], mode6byte[8], mode7byte[8], mode8byte[8], mode9byte[9];
	char *setmode3_640, *setmode3_480, *setmode4_800, *setmode4_600, *setmode5_960, *setmode5_720, *setmode6_1024, *setmode6_768,
		*setmode7_1152, *setmode7_864, *setmode8_1280, *setmode8_960, *setmode9_1600, *setmode9_1200;
	int mode3_640, mode3_480, mode4_800, mode4_600, mode5_960, mode5_720, mode6_1024, mode6_768, mode7_1152, mode7_864,
		mode8_1280, mode8_960, mode9_1600, mode9_1200;

	if ((file = _open(kpfilename, _O_BINARY|_O_RDWR,_S_IREAD)) == -1)
   	{ 
		wsprintf(buffer, _T("Cannot open %s."), kpfilename);
		MessageBox(NULL, buffer, "Error", MB_ICONEXCLAMATION | MB_OK);                                                
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

	//640 480
	_lseek(file,mode3offs,SEEK_SET);
	_read(file,mode3byte,7);
	if (mode3byte[0], mode3byte[1], mode3byte[2], mode3byte[3], mode3byte[4], mode3byte[5], mode3byte[6])
	{
		//Can be done better but works now
		char setmode3tmp[] = "       ";//Ammount of space
//		setmode3tmp = (char*)mode3byte;
//		strncpy (setmode3tmp, (char*)mode3byte,7);	
		strncpy (setmode3tmp, (char*)mode3byte, sizeof(mode3byte));	

		//Split char
		setmode3_640 = strtok(setmode3tmp, " "); 
		setmode3_480 = strtok (NULL, " ,.-");

		//http://en.cppreference.com/w/cpp/string/byte/isdigit
//		if (isdigit(setmode3_640[2]) && isdigit(setmode3_480[2]))// Only check last 3 numbers need better fix
		if ((setmode3_640[3] == '\0') && (setmode3_480[3] == '\0'))//Better :)
		{
//			MessageBox(NULL, "Can read mode3", "Info", MB_ICONEXCLAMATION | MB_OK);
			
			//Convert to INT
			mode3_640 = atoi(setmode3_640); 
			mode3_480 = atoi(setmode3_480);

			EnableWindow(GetDlgItem(hwnd, IDC_MODE3_SPIN640), TRUE);
			EnableWindow(GetDlgItem(hwnd, IDC_MODE3_SPIN480), TRUE);
			EnableWindow(GetDlgItem(hwnd, IDC_MODE3_EDIT640), TRUE);
			EnableWindow(GetDlgItem(hwnd, IDC_MODE3_EDIT480), TRUE);

			SendMessage(GetDlgItem(hwnd, IDC_MODE3_SPIN640), UDM_SETPOS, 0, (LPARAM)(int)mode3_640);
			SendMessage(GetDlgItem(hwnd, IDC_MODE3_SPIN480), UDM_SETPOS, 0, (LPARAM)(int)mode3_480);
		}
		else
		{
//			MessageBox(NULL, "Can not read mode3", "Error", MB_ICONEXCLAMATION | MB_OK);

			EnableWindow(GetDlgItem(hwnd, IDC_MODE3_SPIN640), FALSE);
			EnableWindow(GetDlgItem(hwnd, IDC_MODE3_SPIN480), FALSE);
			EnableWindow(GetDlgItem(hwnd, IDC_MODE3_EDIT640), FALSE);
			EnableWindow(GetDlgItem(hwnd, IDC_MODE3_EDIT480), FALSE);

			SetDlgItemText(hwnd,LBL_TEXT_KP_CHECK,"Can not read mode3.");
			_close(file);
			return;
		}
	}
	//640 480 END

	//800 600
	_lseek(file,mode4offs,SEEK_SET);
	_read(file,mode4byte,7);
	if (mode4byte[0], mode4byte[1], mode4byte[2], mode4byte[3], mode4byte[4], mode4byte[5], mode4byte[6])
	{
		//Can be done better but works now
		char setmode4tmp[] = "       ";//Ammount of space	
		strncpy (setmode4tmp, (char*)mode4byte, sizeof(mode4byte));	

		//Split char
		setmode4_800 = strtok(setmode4tmp, " "); 
		setmode4_600 = strtok (NULL, " ,.-");

		if ((setmode4_800[3] == '\0') && (setmode4_600[3] == '\0'))//Better :)
		{
//			MessageBox(NULL, "Can read mode4", "Info", MB_ICONEXCLAMATION | MB_OK);
			
			//Convert to INT
			mode4_800 = atoi(setmode4_800); 
			mode4_600 = atoi(setmode4_600);

			EnableWindow(GetDlgItem(hwnd, IDC_MODE4_SPIN800), TRUE);
			EnableWindow(GetDlgItem(hwnd, IDC_MODE4_SPIN600), TRUE);
			EnableWindow(GetDlgItem(hwnd, IDC_MODE4_EDIT800), TRUE);
			EnableWindow(GetDlgItem(hwnd, IDC_MODE4_EDIT600), TRUE);

			SendMessage(GetDlgItem(hwnd, IDC_MODE4_SPIN800), UDM_SETPOS, 0, (LPARAM)(int)mode4_800);
			SendMessage(GetDlgItem(hwnd, IDC_MODE4_SPIN600), UDM_SETPOS, 0, (LPARAM)(int)mode4_600);
		}
		else
		{
//			MessageBox(NULL, "Can not read mode4", "Error", MB_ICONEXCLAMATION | MB_OK);

			EnableWindow(GetDlgItem(hwnd, IDC_MODE4_SPIN800), FALSE);
			EnableWindow(GetDlgItem(hwnd, IDC_MODE4_SPIN600), FALSE);
			EnableWindow(GetDlgItem(hwnd, IDC_MODE4_EDIT800), FALSE);
			EnableWindow(GetDlgItem(hwnd, IDC_MODE4_EDIT600), FALSE);

			SetDlgItemText(hwnd,LBL_TEXT_KP_CHECK,"Can not read mode4.");
			_close(file);
			return;
		}
	}
	//800 600 END

	//960 720
	_lseek(file,mode5offs,SEEK_SET);
	_read(file,mode5byte,7);
	if (mode5byte[0], mode5byte[1], mode5byte[2], mode5byte[3], mode5byte[4], mode5byte[5], mode5byte[6])
	{
		char setmode5tmp[] = "       ";//Ammount of space
		strncpy (setmode5tmp, (char*)mode5byte, sizeof(mode5byte));	

		//Split char
		setmode5_960 = strtok(setmode5tmp, " "); 
		setmode5_720 = strtok (NULL, " ,.-");

		if ((setmode5_960[3] == '\0') && (setmode5_720[3] == '\0'))//Better :)
		{
//			MessageBox(NULL, "Can read mode5", "Info", MB_ICONEXCLAMATION | MB_OK);
			
			//Convert to INT
			mode5_960 = atoi(setmode5_960); 
			mode5_720 = atoi(setmode5_720);

			EnableWindow(GetDlgItem(hwnd, IDC_MODE5_SPIN960), TRUE);
			EnableWindow(GetDlgItem(hwnd, IDC_MODE5_SPIN720), TRUE);
			EnableWindow(GetDlgItem(hwnd, IDC_MODE5_EDIT960), TRUE);
			EnableWindow(GetDlgItem(hwnd, IDC_MODE5_EDIT720), TRUE);

			SendMessage(GetDlgItem(hwnd, IDC_MODE5_SPIN960), UDM_SETPOS, 0, (LPARAM)(int)mode5_960);
			SendMessage(GetDlgItem(hwnd, IDC_MODE5_SPIN720), UDM_SETPOS, 0, (LPARAM)(int)mode5_720);
		}
		else
		{
//			MessageBox(NULL, "Can not read mode5", "Error", MB_ICONEXCLAMATION | MB_OK);

			EnableWindow(GetDlgItem(hwnd, IDC_MODE5_SPIN960), FALSE);
			EnableWindow(GetDlgItem(hwnd, IDC_MODE5_SPIN720), FALSE);
			EnableWindow(GetDlgItem(hwnd, IDC_MODE5_EDIT960), FALSE);
			EnableWindow(GetDlgItem(hwnd, IDC_MODE5_EDIT720), FALSE);

			SetDlgItemText(hwnd,LBL_TEXT_KP_CHECK,"Can not read mode5.");
			_close(file);
			return;
		}
	}
	//960 720 END

	//1024 768
	_lseek(file,mode6offs,SEEK_SET);
	_read(file,mode6byte,8);
	if (mode6byte[0], mode6byte[1], mode6byte[2], mode6byte[3], mode6byte[4], mode6byte[5], mode6byte[6], mode6byte[7])
	{
		char setmode6tmp[] = "        ";//Ammount of space
		strncpy (setmode6tmp, (char*)mode6byte, sizeof(mode6byte));	

		//Split char
		setmode6_1024 = strtok(setmode6tmp, " "); 
		setmode6_768 = strtok (NULL, " ,.-");

		if ((setmode6_1024[4] == '\0') && (setmode6_768[3] == '\0'))//Better :)
		{
//			MessageBox(NULL, "Can read mode6", "Info", MB_ICONEXCLAMATION | MB_OK);
			
			//Convert to INT
			mode6_1024 = atoi(setmode6_1024); 
			mode6_768 = atoi(setmode6_768);

			EnableWindow(GetDlgItem(hwnd, IDC_MODE6_SPIN1024), TRUE);
			EnableWindow(GetDlgItem(hwnd, IDC_MODE6_SPIN768), TRUE);
			EnableWindow(GetDlgItem(hwnd, IDC_MODE6_EDIT1024), TRUE);
			EnableWindow(GetDlgItem(hwnd, IDC_MODE6_EDIT768), TRUE);

			SendMessage(GetDlgItem(hwnd, IDC_MODE6_SPIN1024), UDM_SETPOS, 0, (LPARAM)(int)mode6_1024);
			SendMessage(GetDlgItem(hwnd, IDC_MODE6_SPIN768), UDM_SETPOS, 0, (LPARAM)(int)mode6_768);
		}
		else
		{
//			MessageBox(NULL, "Can not read mode6", "Error", MB_ICONEXCLAMATION | MB_OK);

			EnableWindow(GetDlgItem(hwnd, IDC_MODE6_SPIN1024), FALSE);
			EnableWindow(GetDlgItem(hwnd, IDC_MODE6_SPIN768), FALSE);
			EnableWindow(GetDlgItem(hwnd, IDC_MODE6_EDIT1024), FALSE);
			EnableWindow(GetDlgItem(hwnd, IDC_MODE6_EDIT768), FALSE);

			SetDlgItemText(hwnd,LBL_TEXT_KP_CHECK,"Can not read mode6.");
			_close(file);
			return;
		}
	}
	//1024 768 END

	//1152 864
	_lseek(file,mode7offs,SEEK_SET);
	_read(file,mode7byte,8);
	if (mode7byte[0], mode7byte[1], mode7byte[2], mode7byte[3], mode7byte[4], mode7byte[5], mode7byte[6], mode7byte[7])
	{
		char setmode7tmp[] = "        ";//Ammount of space
		strncpy (setmode7tmp, (char*)mode7byte, sizeof(mode7byte));	

		//Split char
		setmode7_1152 = strtok(setmode7tmp, " "); 
		setmode7_864 = strtok (NULL, " ,.-");

		if ((setmode7_1152[4] == '\0') && (setmode7_864[3] == '\0'))//Better :)
		{
//			MessageBox(NULL, "Can read mode7", "Info", MB_ICONEXCLAMATION | MB_OK);
			
			//Convert to INT
			mode7_1152 = atoi(setmode7_1152); 
			mode7_864 = atoi(setmode7_864);

			EnableWindow(GetDlgItem(hwnd, IDC_MODE7_SPIN1152), TRUE);
			EnableWindow(GetDlgItem(hwnd, IDC_MODE7_SPIN864), TRUE);
			EnableWindow(GetDlgItem(hwnd, IDC_MODE7_EDIT1152), TRUE);
			EnableWindow(GetDlgItem(hwnd, IDC_MODE7_EDIT864), TRUE);

			SendMessage(GetDlgItem(hwnd, IDC_MODE7_SPIN1152), UDM_SETPOS, 0, (LPARAM)(int)mode7_1152);
			SendMessage(GetDlgItem(hwnd, IDC_MODE7_SPIN864), UDM_SETPOS, 0, (LPARAM)(int)mode7_864);
		}
		else
		{
//			MessageBox(NULL, "Can not read mode7", "Error", MB_ICONEXCLAMATION | MB_OK);

			EnableWindow(GetDlgItem(hwnd, IDC_MODE7_SPIN1152), FALSE);
			EnableWindow(GetDlgItem(hwnd, IDC_MODE7_SPIN864), FALSE);
			EnableWindow(GetDlgItem(hwnd, IDC_MODE7_EDIT1152), FALSE);
			EnableWindow(GetDlgItem(hwnd, IDC_MODE7_EDIT864), FALSE);

			SetDlgItemText(hwnd,LBL_TEXT_KP_CHECK,"Can not read mode7.");
			_close(file);
			return;
		}
	}
	//1152 864 END

	//1280 960
	_lseek(file,mode8offs,SEEK_SET);
	_read(file,mode8byte,8);
	if (mode8byte[0], mode8byte[1], mode8byte[2], mode8byte[3], mode8byte[4], mode8byte[5], mode8byte[6], mode8byte[7])
	{
		char setmode8tmp[] = "        ";//Ammount of space
		strncpy (setmode8tmp, (char*)mode8byte, sizeof(mode8byte));	

		//Split char
		setmode8_1280 = strtok(setmode8tmp, " "); 
		setmode8_960 = strtok (NULL, " ,.-");

		if ((setmode8_1280[4] == '\0') && (setmode8_960[3] == '\0'))//Better :)
		{
//			MessageBox(NULL, "Can read mode8", "Info", MB_ICONEXCLAMATION | MB_OK);
			
			//Convert to INT
			mode8_1280 = atoi(setmode8_1280); 
			mode8_960 = atoi(setmode8_960);

			EnableWindow(GetDlgItem(hwnd, IDC_MODE8_SPIN1280), TRUE);
			EnableWindow(GetDlgItem(hwnd, IDC_MODE8_SPIN960), TRUE);
			EnableWindow(GetDlgItem(hwnd, IDC_MODE8_EDIT1280), TRUE);
			EnableWindow(GetDlgItem(hwnd, IDC_MODE8_EDIT960), TRUE);

			SendMessage(GetDlgItem(hwnd, IDC_MODE8_SPIN1280), UDM_SETPOS, 0, (LPARAM)(int)mode8_1280);
			SendMessage(GetDlgItem(hwnd, IDC_MODE8_SPIN960), UDM_SETPOS, 0, (LPARAM)(int)mode8_960);
		}
		else
		{
//			MessageBox(NULL, "Can not read mode8", "Error", MB_ICONEXCLAMATION | MB_OK);

			EnableWindow(GetDlgItem(hwnd, IDC_MODE8_SPIN1280), FALSE);
			EnableWindow(GetDlgItem(hwnd, IDC_MODE8_SPIN960), FALSE);
			EnableWindow(GetDlgItem(hwnd, IDC_MODE8_EDIT1280), FALSE);
			EnableWindow(GetDlgItem(hwnd, IDC_MODE8_EDIT960), FALSE);

			SetDlgItemText(hwnd,LBL_TEXT_KP_CHECK,"Can not read mode8.");
			_close(file);
			return;
		}
	}
	//1280 960 END

	//1600 1200
	_lseek(file,mode9offs,SEEK_SET);
	_read(file,mode9byte,9);
	if (mode9byte[0], mode9byte[1], mode9byte[2], mode9byte[3], mode9byte[4], mode9byte[5], mode9byte[6], mode9byte[7], mode9byte[8])
	{
		char setmode9tmp[] = "         ";//Ammount of space
		strncpy (setmode9tmp, (char*)mode9byte, sizeof(mode9byte));	

		//Split char
		setmode9_1600 = strtok(setmode9tmp, " "); 
		setmode9_1200 = strtok (NULL, " ,.-");

		if ((setmode9_1600[4] == '\0') && (setmode9_1200[4] == '\0'))//Better :)
		{
//			MessageBox(NULL, "Can read mode9", "Info", MB_ICONEXCLAMATION | MB_OK);
			
			//Convert to INT
			mode9_1600 = atoi(setmode9_1600); 
			mode9_1200 = atoi(setmode9_1200);

			EnableWindow(GetDlgItem(hwnd, IDC_MODE9_SPIN1600), TRUE);
			EnableWindow(GetDlgItem(hwnd, IDC_MODE9_SPIN1200), TRUE);
			EnableWindow(GetDlgItem(hwnd, IDC_MODE9_EDIT1600), TRUE);
			EnableWindow(GetDlgItem(hwnd, IDC_MODE9_EDIT1200), TRUE);

			SendMessage(GetDlgItem(hwnd, IDC_MODE9_SPIN1600), UDM_SETPOS, 0, (LPARAM)(int)mode9_1600);
			SendMessage(GetDlgItem(hwnd, IDC_MODE9_SPIN1200), UDM_SETPOS, 0, (LPARAM)(int)mode9_1200);
		}
		else
		{
//			MessageBox(NULL, "Can not read mode9", "Error", MB_ICONEXCLAMATION | MB_OK);

			EnableWindow(GetDlgItem(hwnd, IDC_MODE9_SPIN1600), FALSE);
			EnableWindow(GetDlgItem(hwnd, IDC_MODE9_SPIN1200), FALSE);
			EnableWindow(GetDlgItem(hwnd, IDC_MODE9_EDIT1600), FALSE);
			EnableWindow(GetDlgItem(hwnd, IDC_MODE9_EDIT1200), FALSE);

			SetDlgItemText(hwnd,LBL_TEXT_KP_CHECK,"Can not read mode9.");
			_close(file);
			return;
		}
	}
	//1600 1200 END

//		wsprintf(buffer, _T("Value is 0x%1x"), (unsigned)mode3byte[0]);//Show hex
//		wsprintf(buffer, _T("Value is %c"), (unsigned)mode3byte[0]);//Show char		

//		wsprintf(buffer, _T("Value is %s"), setmode4tmp);
//		MessageBox(NULL, buffer, "Info", MB_ICONEXCLAMATION | MB_OK); 

//		wsprintf(buffer, _T("Value is %i"), mode3_640);
//		MessageBox(NULL, buffer, "Info", MB_ICONEXCLAMATION | MB_OK); 

//		wsprintf(buffer, _T("Value is %i"), mode3_480);
//		MessageBox(NULL, buffer, "Info", MB_ICONEXCLAMATION | MB_OK); 
	

	_close(file);

	EnableWindow(GetDlgItem(hwnd, IDC_GROUP_RESOLUTION), TRUE);
	EnableWindow(GetDlgItem(hwnd, IDC_BUTTON_PATCH), TRUE);
}
