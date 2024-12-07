#include "md5.h"
#include "main.h"
#include "resource.h"

void CheckMD5file (HWND hwnd)
{
	UINT16 i;
	size_t len;
	md5_byte_t buffer[512];
	TCHAR hex[33]=_T("");
	TCHAR filename[MAX_PATH]=_T("");
	md5_state_t state;
	md5_byte_t digest[16];
	FILE *pfile;

	//FREDZ value of kingpin md5
	TCHAR kingpin121hex[33] = _T ("3d1e42f299a15af7088b4687085537e7");
	TCHAR kingpin121nocdhex[33] = _T ("8d78f20edf5ac6c8de513d5569093dcd");
	TCHAR kingpin121steamhex[33] = _T ("a9e23e9931477d868029f1a9c4d7a65b");
	TCHAR kingpin121goghex[33] = _T ("7f5ebcc21438e32021849c7c3fab4d44");
	TCHAR kingpin022demohex[33] = _T ("467d445f4096eb1fd9bdc84b1c73e419");

	//Can't patch yet
	TCHAR kingpin100hex[33] = _T ("6f9b0623426d5d1a3a6f6322773618ea");
	TCHAR kingpin110hex[33] = _T ("9aef36115c09c8935d0ed2fefe8b38f1");
	TCHAR kingpin120hex[33] = _T ("6e89b9107e4c1b70afc57f7ae426374a");

	GetDlgItemText(hwnd,LBL_TEXT_KP_PATH,filename,MAX_PATH);
	
	if(lstrlen(filename)&&(pfile=_tfopen(filename,_T("rb"))))
	{
		md5_init(&state);

		do
		{
			len=fread(buffer,1,512,pfile);
			if(!ferror(pfile)) 
				md5_append(&state,(const md5_byte_t *)buffer,(unsigned int)len);
			Sleep(0);
		}
		while(!feof(pfile));

		md5_finish(&state, digest);

		hex[0]=0;
		for(i = 0; i < 16; i++)
			_stprintf_s(hex, sizeof(hex) / sizeof(TCHAR), _T("%s%02x"), hex, digest[i]);

		fclose(pfile);

		SetDlgItemText(hwnd,LBL_TEXT_KP_MD5,hex);

		//FREDZ start comparing
		if (!_tcscmp(hex, kingpin121hex))//		if (!_tcscmp(hex, _T("3d1e42f299a15af7088b4687085537e7")))
		{
			SetDlgItemText(hwnd, LBL_TEXT_KP_CHECK, _T("Orginale Kingpin 1.21 file"));
		}
		else if (!_tcscmp(hex, kingpin121nocdhex))
		{
			SetDlgItemText(hwnd, LBL_TEXT_KP_CHECK, _T("Orginale Kingpin 1.21 NOCD file"));
		}
		else if (!_tcscmp(hex, kingpin121steamhex))
		{
			SetDlgItemText(hwnd, LBL_TEXT_KP_CHECK, _T("Orginale Kingpin 1.21 Steam file"));
		}
		else if (!_tcscmp(hex, kingpin121goghex))
		{
			SetDlgItemText(hwnd, LBL_TEXT_KP_CHECK, _T("Orginale Kingpin 1.21 GOG file"));
		}
		else if (!_tcscmp(hex, kingpin022demohex))
		{
			SetDlgItemText(hwnd, LBL_TEXT_KP_CHECK, _T("Orginale Kingpin alpha demo 0.22N file"));
		}
		else if (!_tcscmp(hex, kingpin100hex))
		{
			SetDlgItemText(hwnd, LBL_TEXT_KP_CHECK, _T("Orginale Kingpin 1.00 file"));
		}
		else if (!_tcscmp(hex, kingpin110hex))
		{
			SetDlgItemText(hwnd, LBL_TEXT_KP_CHECK, _T("Orginale Kingpin 1.10 file"));
		}
		else if (!_tcscmp(hex, kingpin120hex))
		{
			SetDlgItemText(hwnd, LBL_TEXT_KP_CHECK, _T("Orginale Kingpin 1.20 file"));
		}
		else
		{
			SetDlgItemText(hwnd, LBL_TEXT_KP_CHECK, _T("No orginale Kingpin file"));
		}
		//FREDZ end comparing	
	}	
	else 
	{
		SetDlgItemText(hwnd,LBL_TEXT_KP_MD5,_T("Invalid File"));
	}
}
