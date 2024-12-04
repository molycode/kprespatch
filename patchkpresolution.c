#include "main.h"

#include "hexutils.h"

#include <io.h>//_open _read
#include <fcntl.h>//O_ flags

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
void PatchKPresolutionFile (HWND hwnd, char *kpfilename)
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

	unsigned char mode3_640_byte1[2], mode3_480_byte1[2], mode4_800_byte1[2], mode4_600_byte1[2], mode5_960_byte1[2], mode5_720_byte1[2], 
		mode6_1024_byte1[3], mode6_768_byte1[2], mode7_1152_byte1[3], mode7_864_byte1[2], mode8_1280_byte1[3], mode8_960_byte1[2],
		mode9_1600_byte1[3], mode9_1200_byte1[3];
	unsigned char mode3_640_byte3[2], mode3_480_byte3[2], mode4_800_byte3[2], mode4_600_byte3[2], mode5_960_byte3[2], mode5_720_byte3[2], 
		mode6_1024_byte3[2], mode6_768_byte3[2], mode7_1152_byte3[3], mode7_864_byte3[2], mode8_1280_byte3[3], mode8_960_byte3[2],
		mode9_1600_byte3[3], mode9_1200_byte3[2];//Always 2 bytes



	if ((file = _open(kpfilename,_O_BINARY|_O_RDWR,_S_IWRITE)) == -1)
   	{ 
		wsprintf(buffer, _T("Cannot open %s."), kpfilename);
		MessageBox(NULL, buffer, "Error", MB_ICONEXCLAMATION | MB_OK);                                                
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

	//---------------------------------------------------------------------------------------------
	//START MODE3 640x480
	//640
	HWND Dlg_640 = GetDlgItem(hwnd, IDC_MODE3_EDIT640);
	int Dlg_lgth_640 = GetWindowTextLength (Dlg_640);
	char *Dlg_txt_640 = ( char *)GlobalAlloc( GPTR, (Dlg_lgth_640 * ( sizeof (char) ) ) + 1);
	if (!Dlg_txt_640)
	{
		MessageBox(NULL, "Error with MODE3_EDIT640", "Error", MB_ICONEXCLAMATION | MB_OK);
//		_close(file);
		return;
	} 
	GetWindowText (Dlg_640, Dlg_txt_640, Dlg_lgth_640);

	char *value_mode3_640 = malloc (sizeof(char) * 3);
    memset (value_mode3_640, 0, 3) ;
    strncpy (value_mode3_640, Dlg_txt_640, 3);
	
	GlobalFree (Dlg_txt_640);

	int int_value_mode3_640 = atoi (value_mode3_640);//Make int

	//Int to Hex
	char *hexcon_value_mode3_640 = malloc (sizeof(char) * 3);
    memset (hexcon_value_mode3_640, 0, 3) ;
	_itoa (int_value_mode3_640 ,hexcon_value_mode3_640, 16);

	//Endianness
	char *hex_value_mode3_640 = malloc (sizeof(char) * 4);
    memset (hex_value_mode3_640, 0, 4) ;
	if (int_value_mode3_640 < 256)//or FF=255 add 0
	{
		hex_value_mode3_640[0] = hexcon_value_mode3_640[0];
		hex_value_mode3_640[1] = hexcon_value_mode3_640[1]; 
		hex_value_mode3_640[2] = '0';
		hex_value_mode3_640[3] = '0';
	}
	else
	{
		hex_value_mode3_640[0] = hexcon_value_mode3_640[1];
		hex_value_mode3_640[1] = hexcon_value_mode3_640[2]; //Maybe need switch with 0 but seems okay
		hex_value_mode3_640[2] = '0';
		hex_value_mode3_640[3] = hexcon_value_mode3_640[0];
	}

//	free (mode3_640_byte3);//Ugly
	stringtohex (hex_value_mode3_640, mode3_640_byte3); 

//	for(int i = 0; i < 3; i++)
//		mode3_640_byte1[i] = malloc (sizeof(char) * 3);
//	free (mode3_640_byte1);//Ugly
    memset (mode3_640_byte1, 0, 3) ;
    strncpy ((char*)mode3_640_byte1, value_mode3_640, 3);

	_lseek(file,mode3_640_offs1,SEEK_SET);
	_write(file, mode3_640_byte1, 3); 

	_lseek(file,mode3_640_offs2,SEEK_SET);
	_write(file, mode3_640_byte1, 3);

	_lseek(file,mode3_640_offs3,SEEK_SET);
	_write(file, mode3_640_byte3, 2);
	//640 END

	//480
	HWND Dlg_480 = GetDlgItem(hwnd, IDC_MODE3_EDIT480);
	int Dlg_lgth_480 = GetWindowTextLength (Dlg_480);
	char *Dlg_txt_480 = ( char *)GlobalAlloc( GPTR, (Dlg_lgth_480 * ( sizeof (char) ) ) + 1);
	if (!Dlg_txt_480)
	{
		MessageBox(NULL, "Error with MODE3_EDIT480", "Error", MB_ICONEXCLAMATION | MB_OK);
//		_close(file);
		return;
	} 
	GetWindowText (Dlg_480, Dlg_txt_480, Dlg_lgth_480);

	char *value_mode3_480 = malloc (sizeof(char) * 3);
    memset (value_mode3_480, 0, 3) ;
    strncpy (value_mode3_480, Dlg_txt_480, 3);
	
	GlobalFree (Dlg_txt_480);

	int int_value_mode3_480 = atoi (value_mode3_480);//Make int

	//Int to Hex
	char *hexcon_value_mode3_480 = malloc (sizeof(char) * 3);
    memset (hexcon_value_mode3_480, 0, 3) ;
	_itoa (int_value_mode3_480 ,hexcon_value_mode3_480, 16);

	//Endianness
	char *hex_value_mode3_480 = malloc (sizeof(char) * 4);
    memset (hex_value_mode3_480, 0, 4) ;
	if (int_value_mode3_480 < 256)//or FF=255 add 0
	{
		hex_value_mode3_480[0] = hexcon_value_mode3_480[0];
		hex_value_mode3_480[1] = hexcon_value_mode3_480[1]; 
		hex_value_mode3_480[2] = '0';
		hex_value_mode3_480[3] = '0';
	}
	else
	{
		hex_value_mode3_480[0] = hexcon_value_mode3_480[1];
		hex_value_mode3_480[1] = hexcon_value_mode3_480[2]; //Maybe need switch with 0 but seems okay
		hex_value_mode3_480[2] = '0';
		hex_value_mode3_480[3] = hexcon_value_mode3_480[0];
	}

	stringtohex (hex_value_mode3_480, mode3_480_byte3); 

    memset (mode3_480_byte1, 0, 3) ;
    strncpy ((char*)mode3_480_byte1, value_mode3_480, 3);

	_lseek(file,mode3_480_offs1,SEEK_SET);
	_write(file, mode3_480_byte1, 3); 

	_lseek(file,mode3_480_offs2,SEEK_SET);
	_write(file, mode3_480_byte1, 3);

	_lseek(file,mode3_480_offs3,SEEK_SET);
	_write(file, mode3_480_byte3, 2);
	//480 END
	//END MODE3 640x480
	//---------------------------------------------------------------------------------------------
	//START MODE4 800x600
	//800
	HWND Dlg_800 = GetDlgItem(hwnd, IDC_MODE4_EDIT800);
	int Dlg_lgth_800 = GetWindowTextLength (Dlg_800);
	char *Dlg_txt_800 = ( char *)GlobalAlloc( GPTR, (Dlg_lgth_800 * ( sizeof (char) ) ) + 1);
	if (!Dlg_txt_800)
	{
		MessageBox(NULL, "Error with MODE4_EDIT800", "Error", MB_ICONEXCLAMATION | MB_OK);
//		_close(file);
		return;
	} 
	GetWindowText (Dlg_800, Dlg_txt_800, Dlg_lgth_800);

	char *value_mode4_800 = malloc (sizeof(char) * 3);
    memset (value_mode4_800, 0, 3) ;
    strncpy (value_mode4_800, Dlg_txt_800, 3);
	
	GlobalFree (Dlg_txt_800);

	int int_value_mode4_800 = atoi (value_mode4_800);//Make int

	//Int to Hex
	char *hexcon_value_mode4_800 = malloc (sizeof(char) * 3);
    memset (hexcon_value_mode4_800, 0, 3) ;
	_itoa (int_value_mode4_800 ,hexcon_value_mode4_800, 16);

	//Endianness
	char *hex_value_mode4_800 = malloc (sizeof(char) * 4);
    memset (hex_value_mode4_800, 0, 4) ;
	if (int_value_mode4_800 < 256)//or FF=255 add 0
	{
		hex_value_mode4_800[0] = hexcon_value_mode4_800[0];
		hex_value_mode4_800[1] = hexcon_value_mode4_800[1]; 
		hex_value_mode4_800[2] = '0';
		hex_value_mode4_800[3] = '0';
	}
	else
	{
		hex_value_mode4_800[0] = hexcon_value_mode4_800[1];
		hex_value_mode4_800[1] = hexcon_value_mode4_800[2]; //Maybe need switch with 0 but seems okay
		hex_value_mode4_800[2] = '0';
		hex_value_mode4_800[3] = hexcon_value_mode4_800[0];
	}

	stringtohex (hex_value_mode4_800, mode4_800_byte3); 

    memset (mode4_800_byte1, 0, 3) ;
    strncpy ((char*)mode4_800_byte1, value_mode4_800, 3);

	_lseek(file,mode4_800_offs1,SEEK_SET);
	_write(file, mode4_800_byte1, 3); 

	_lseek(file,mode4_800_offs2,SEEK_SET);
	_write(file, mode4_800_byte1, 3);

	_lseek(file,mode4_800_offs3,SEEK_SET);
	_write(file, mode4_800_byte3, 2);
	//800 END
	
	//600
	HWND Dlg_600 = GetDlgItem(hwnd, IDC_MODE4_EDIT600);
	int Dlg_lgth_600 = GetWindowTextLength (Dlg_600);
	char *Dlg_txt_600 = ( char *)GlobalAlloc( GPTR, (Dlg_lgth_600 * ( sizeof (char) ) ) + 1);
	if (!Dlg_txt_600)
	{
		MessageBox(NULL, "Error with MODE4_EDIT600", "Error", MB_ICONEXCLAMATION | MB_OK);
//		_close(file);
		return;
	} 
	GetWindowText (Dlg_600, Dlg_txt_600, Dlg_lgth_600);

	char *value_mode4_600 = malloc (sizeof(char) * 3);
    memset (value_mode4_600, 0, 3) ;
    strncpy (value_mode4_600, Dlg_txt_600, 3);
	
	GlobalFree (Dlg_txt_600);

	int int_value_mode4_600 = atoi (value_mode4_600);//Make int

	//Int to Hex
	char *hexcon_value_mode4_600 = malloc (sizeof(char) * 3);
    memset (hexcon_value_mode4_600, 0, 3) ;
	_itoa (int_value_mode4_600 ,hexcon_value_mode4_600, 16);

	//Endianness
	char *hex_value_mode4_600 = malloc (sizeof(char) * 4);
    memset (hex_value_mode4_600, 0, 4) ;
	if (int_value_mode4_600 < 256)//or FF=255 add 0
	{
		hex_value_mode4_600[0] = hexcon_value_mode4_600[0];
		hex_value_mode4_600[1] = hexcon_value_mode4_600[1]; 
		hex_value_mode4_600[2] = '0';
		hex_value_mode4_600[3] = '0';
	}
	else
	{
		hex_value_mode4_600[0] = hexcon_value_mode4_600[1];
		hex_value_mode4_600[1] = hexcon_value_mode4_600[2]; //Maybe need switch with 0 but seems okay
		hex_value_mode4_600[2] = '0';
		hex_value_mode4_600[3] = hexcon_value_mode4_600[0];
	}

	stringtohex (hex_value_mode4_600, mode4_600_byte3); 

    memset (mode4_600_byte1, 0, 3) ;
    strncpy ((char*)mode4_600_byte1, value_mode4_600, 3);


	_lseek(file,mode4_600_offs1,SEEK_SET);
	_write(file, mode4_600_byte1, 3); 

	_lseek(file,mode4_600_offs2,SEEK_SET);
	_write(file, mode4_600_byte1, 3);

	_lseek(file,mode4_600_offs3,SEEK_SET);
	_write(file, mode4_600_byte3, 2);
	//600 END
	//END MODE4 800x600
	//---------------------------------------------------------------------------------------------
	//START MODE4 960x720
	//960
	HWND Dlg_960 = GetDlgItem(hwnd, IDC_MODE5_EDIT960);
	int Dlg_lgth_960 = GetWindowTextLength (Dlg_960);
	char *Dlg_txt_960 = ( char *)GlobalAlloc( GPTR, (Dlg_lgth_960 * ( sizeof (char) ) ) + 1);
	if (!Dlg_txt_960)
	{
		MessageBox(NULL, "Error with MODE5_EDIT960", "Error", MB_ICONEXCLAMATION | MB_OK);
//		_close(file);
		return;
	} 
	GetWindowText (Dlg_960, Dlg_txt_960, Dlg_lgth_960);

	char *value_mode5_960 = malloc (sizeof(char) * 3);
    memset (value_mode5_960, 0, 3) ;
    strncpy (value_mode5_960, Dlg_txt_960, 3);
	
	GlobalFree (Dlg_txt_960);

	int int_value_mode5_960 = atoi (value_mode5_960);//Make int

	//Int to Hex
	char *hexcon_value_mode5_960 = malloc (sizeof(char) * 3);
    memset (hexcon_value_mode5_960, 0, 3) ;
	_itoa (int_value_mode5_960 ,hexcon_value_mode5_960, 16);

	//Endianness
	char *hex_value_mode5_960 = malloc (sizeof(char) * 4);
    memset (hex_value_mode5_960, 0, 4) ;
	if (int_value_mode5_960 < 256)//or FF=255 add 0
	{
		hex_value_mode5_960[0] = hexcon_value_mode5_960[0];
		hex_value_mode5_960[1] = hexcon_value_mode5_960[1]; 
		hex_value_mode5_960[2] = '0';
		hex_value_mode5_960[3] = '0';
	}
	else
	{
		hex_value_mode5_960[0] = hexcon_value_mode5_960[1];
		hex_value_mode5_960[1] = hexcon_value_mode5_960[2]; //Maybe need switch with 0 but seems okay
		hex_value_mode5_960[2] = '0';
		hex_value_mode5_960[3] = hexcon_value_mode5_960[0];
	}

	stringtohex (hex_value_mode5_960, mode5_960_byte3); 

    memset (mode5_960_byte1, 0, 3) ;
    strncpy ((char*)mode5_960_byte1, value_mode5_960, 3);

	_lseek(file,mode5_960_offs1,SEEK_SET);
	_write(file, mode5_960_byte1, 3); 

	_lseek(file,mode5_960_offs2,SEEK_SET);
	_write(file, mode5_960_byte1, 3);

	_lseek(file,mode5_960_offs3,SEEK_SET);
	_write(file, mode5_960_byte3, 2);
	//960 END

	//720
	HWND Dlg_720 = GetDlgItem(hwnd, IDC_MODE5_EDIT720);
	int Dlg_lgth_720 = GetWindowTextLength (Dlg_720);
	char *Dlg_txt_720 = ( char *)GlobalAlloc( GPTR, (Dlg_lgth_720 * ( sizeof (char) ) ) + 1);
	if (!Dlg_txt_720)
	{
		MessageBox(NULL, "Error with MODE5_EDIT720", "Error", MB_ICONEXCLAMATION | MB_OK);
//		_close(file);
		return;
	} 
	GetWindowText (Dlg_720, Dlg_txt_720, Dlg_lgth_720);

	char *value_mode5_720 = malloc (sizeof(char) * 3);
    memset (value_mode5_720, 0, 3) ;
    strncpy (value_mode5_720, Dlg_txt_720, 3);
	
	GlobalFree (Dlg_txt_720);

	int int_value_mode5_720 = atoi (value_mode5_720);//Make int

	//Int to Hex
	char *hexcon_value_mode5_720 = malloc (sizeof(char) * 3);
    memset (hexcon_value_mode5_720, 0, 3) ;
	_itoa (int_value_mode5_720 ,hexcon_value_mode5_720, 16);

	//Endianness
	char *hex_value_mode5_720 = malloc (sizeof(char) * 4);
    memset (hex_value_mode5_720, 0, 4) ;
	if (int_value_mode5_720 < 256)//or FF=255 add 0
	{
		hex_value_mode5_720[0] = hexcon_value_mode5_720[0];
		hex_value_mode5_720[1] = hexcon_value_mode5_720[1]; 
		hex_value_mode5_720[2] = '0';
		hex_value_mode5_720[3] = '0';
	}
	else
	{
		hex_value_mode5_720[0] = hexcon_value_mode5_720[1];
		hex_value_mode5_720[1] = hexcon_value_mode5_720[2]; //Maybe need switch with 0 but seems okay
		hex_value_mode5_720[2] = '0';
		hex_value_mode5_720[3] = hexcon_value_mode5_720[0];
	}

	stringtohex (hex_value_mode5_720, mode5_720_byte3); 

    memset (mode5_720_byte1, 0, 3) ;
    strncpy ((char*)mode5_720_byte1, value_mode5_720, 3);

	_lseek(file,mode5_720_offs1,SEEK_SET);
	_write(file, mode5_720_byte1, 3); 

	_lseek(file,mode5_720_offs2,SEEK_SET);
	_write(file, mode5_720_byte1, 3);

	_lseek(file,mode5_720_offs3,SEEK_SET);
	_write(file, mode5_720_byte3, 2);
	//720 END
	//END MODE4 960x720
	//---------------------------------------------------------------------------------------------
	//START MODE4 1024x768
	//1024
	HWND Dlg_1024 = GetDlgItem(hwnd, IDC_MODE6_EDIT1024);
	int Dlg_lgth_1024 = GetWindowTextLength (Dlg_1024);
	char *Dlg_txt_1024 = ( char *)GlobalAlloc( GPTR, (Dlg_lgth_1024 * ( sizeof (char) ) ) + 1);
	if (!Dlg_txt_1024)
	{
		MessageBox(NULL, "Error with MODE6_EDIT1024", "Error", MB_ICONEXCLAMATION | MB_OK);
//		_close(file);
		return;
	} 
	GetWindowText (Dlg_1024, Dlg_txt_1024, Dlg_lgth_1024);

	char *value_mode6_1024 = malloc (sizeof(char) * 4);
    memset (value_mode6_1024, 0, 4) ;
    strncpy (value_mode6_1024, Dlg_txt_1024, 4);
	
	GlobalFree (Dlg_txt_1024);

	int int_value_mode6_1024 = atoi (value_mode6_1024);//Make int

	//Int to Hex
	char *hexcon_value_mode6_1024 = malloc (sizeof(char) * 4);
    memset (hexcon_value_mode6_1024, 0, 4) ;
	_itoa (int_value_mode6_1024 ,hexcon_value_mode6_1024, 16);

	//Endianness
	char *hex_value_mode6_1024 = malloc (sizeof(char) * 4);
    memset (hex_value_mode6_1024, 0, 4) ;
	if (int_value_mode6_1024 > 4095)//or FFF=4095 remove 0
	{
		hex_value_mode6_1024[0] = hexcon_value_mode6_1024[2];
		hex_value_mode6_1024[1] = hexcon_value_mode6_1024[3]; 
		hex_value_mode6_1024[2] = hexcon_value_mode6_1024[0];//Should always be 0?
		hex_value_mode6_1024[3] = hexcon_value_mode6_1024[1];
	}
	else
	{
		hex_value_mode6_1024[0] = hexcon_value_mode6_1024[1];
		hex_value_mode6_1024[1] = hexcon_value_mode6_1024[2]; 
		hex_value_mode6_1024[2] = '0';
		hex_value_mode6_1024[3] = hexcon_value_mode6_1024[0];
	}

	stringtohex (hex_value_mode6_1024, mode6_1024_byte3); 

    memset (mode6_1024_byte1, 0, 4) ;
    strncpy ((char*)mode6_1024_byte1, value_mode6_1024, 4);

	_lseek(file,mode6_1024_offs1,SEEK_SET);
	_write(file, mode6_1024_byte1, 4); 

	_lseek(file,mode6_1024_offs2,SEEK_SET);
	_write(file, mode6_1024_byte1, 4);

	_lseek(file,mode6_1024_offs3,SEEK_SET);
	_write(file, mode6_1024_byte3, 2);
	//1024 END

	//768
	HWND Dlg_768 = GetDlgItem(hwnd, IDC_MODE6_EDIT768);
	int Dlg_lgth_768 = GetWindowTextLength (Dlg_768);
	char *Dlg_txt_768 = ( char *)GlobalAlloc( GPTR, (Dlg_lgth_768 * ( sizeof (char) ) ) + 1);
	if (!Dlg_txt_768)
	{
		MessageBox(NULL, "Error with MODE6_EDIT768", "Error", MB_ICONEXCLAMATION | MB_OK);
//		_close(file);
		return;
	} 
	GetWindowText (Dlg_768, Dlg_txt_768, Dlg_lgth_768);

	char *value_mode6_768 = malloc (sizeof(char) * 3);
    memset (value_mode6_768, 0, 3) ;
    strncpy (value_mode6_768, Dlg_txt_768, 3);
	
	GlobalFree (Dlg_txt_768);

	int int_value_mode6_768 = atoi (value_mode6_768);//Make int

	//Int to Hex
	char *hexcon_value_mode6_768 = malloc (sizeof(char) * 3);
    memset (hexcon_value_mode6_768, 0, 3) ;
	_itoa (int_value_mode6_768 ,hexcon_value_mode6_768, 16);

	//Endianness
	char *hex_value_mode6_768 = malloc (sizeof(char) * 4);
    memset (hex_value_mode6_768, 0, 4) ;
	if (int_value_mode6_768 < 256)//or FF=255 add 0
	{
		hex_value_mode6_768[0] = hexcon_value_mode6_768[0];
		hex_value_mode6_768[1] = hexcon_value_mode6_768[1]; 
		hex_value_mode6_768[2] = '0';
		hex_value_mode6_768[3] = '0';
	}
	else
	{
		hex_value_mode6_768[0] = hexcon_value_mode6_768[1];
		hex_value_mode6_768[1] = hexcon_value_mode6_768[2]; //Maybe need switch with 0 but seems okay
		hex_value_mode6_768[2] = '0';
		hex_value_mode6_768[3] = hexcon_value_mode6_768[0];
	}

	stringtohex (hex_value_mode6_768, mode6_768_byte3); 

    memset (mode6_768_byte1, 0, 3) ;
    strncpy ((char*)mode6_768_byte1, value_mode6_768, 3);

	_lseek(file,mode6_768_offs1,SEEK_SET);
	_write(file, mode6_768_byte1, 3); 

	_lseek(file,mode6_768_offs2,SEEK_SET);
	_write(file, mode6_768_byte1, 3);

	_lseek(file,mode6_768_offs3,SEEK_SET);
	_write(file, mode6_768_byte3, 2);
	//768 END
	//END MODE4 1024x768
	//---------------------------------------------------------------------------------------------
	//START MODE4 1152x864
	//1152
	HWND Dlg_1152 = GetDlgItem(hwnd, IDC_MODE7_EDIT1152);
	int Dlg_lgth_1152 = GetWindowTextLength (Dlg_1152);
	char *Dlg_txt_1152 = ( char *)GlobalAlloc( GPTR, (Dlg_lgth_1152 * ( sizeof (char) ) ) + 1);
	if (!Dlg_txt_1152)
	{
		MessageBox(NULL, "Error with MODE7_EDIT1152", "Error", MB_ICONEXCLAMATION | MB_OK);
//		_close(file);
		return;
	} 
	GetWindowText (Dlg_1152, Dlg_txt_1152, Dlg_lgth_1152);

	char *value_mode7_1152 = malloc (sizeof(char) * 4);
    memset (value_mode7_1152, 0, 4) ;
    strncpy (value_mode7_1152, Dlg_txt_1152, 4);
	
	GlobalFree (Dlg_txt_1152);

	int int_value_mode7_1152 = atoi (value_mode7_1152);//Make int

	//Int to Hex
	char *hexcon_value_mode7_1152 = malloc (sizeof(char) * 4);
    memset (hexcon_value_mode7_1152, 0, 4) ;
	_itoa (int_value_mode7_1152 ,hexcon_value_mode7_1152, 16);

	//Endianness
	char *hex_value_mode7_1152 = malloc (sizeof(char) * 4);
    memset (hex_value_mode7_1152, 0, 4) ;
	if (int_value_mode7_1152 > 4095)//or FFF=4095 remove 0
	{
		hex_value_mode7_1152[0] = hexcon_value_mode7_1152[2];
		hex_value_mode7_1152[1] = hexcon_value_mode7_1152[3]; 
		hex_value_mode7_1152[2] = hexcon_value_mode7_1152[0];//Should always be 0?
		hex_value_mode7_1152[3] = hexcon_value_mode7_1152[1];
	}
	else
	{
		hex_value_mode7_1152[0] = hexcon_value_mode7_1152[1];
		hex_value_mode7_1152[1] = hexcon_value_mode7_1152[2]; 
		hex_value_mode7_1152[2] = '0';
		hex_value_mode7_1152[3] = hexcon_value_mode7_1152[0];
	}

	stringtohex (hex_value_mode7_1152, mode7_1152_byte3); 

    memset (mode7_1152_byte1, 0, 4) ;
    strncpy ((char*)mode7_1152_byte1, value_mode7_1152, 4);

	_lseek(file,mode7_1152_offs1,SEEK_SET);
	_write(file, mode7_1152_byte1, 4); 

	_lseek(file,mode7_1152_offs2,SEEK_SET);
	_write(file, mode7_1152_byte1, 4);

	_lseek(file,mode7_1152_offs3,SEEK_SET);
	_write(file, mode7_1152_byte3, 2);
	//1152 END

	//864
	HWND Dlg_864 = GetDlgItem(hwnd, IDC_MODE7_EDIT864);
	int Dlg_lgth_864 = GetWindowTextLength (Dlg_864);
	char *Dlg_txt_864 = ( char *)GlobalAlloc( GPTR, (Dlg_lgth_864 * ( sizeof (char) ) ) + 1);
	if (!Dlg_txt_864)
	{
		MessageBox(NULL, "Error with MODE7_EDIT864", "Error", MB_ICONEXCLAMATION | MB_OK);
//		_close(file);
		return;
	} 
	GetWindowText (Dlg_864, Dlg_txt_864, Dlg_lgth_864);

	char *value_mode7_864 = malloc (sizeof(char) * 3);
    memset (value_mode7_864, 0, 3) ;
    strncpy (value_mode7_864, Dlg_txt_864, 3);
	
	GlobalFree (Dlg_txt_864);

	int int_value_mode7_864 = atoi (value_mode7_864);//Make int

	//Int to Hex
	char *hexcon_value_mode7_864 = malloc (sizeof(char) * 3);
    memset (hexcon_value_mode7_864, 0, 3) ;
	_itoa (int_value_mode7_864 ,hexcon_value_mode7_864, 16);

	//Endianness
	char *hex_value_mode7_864 = malloc (sizeof(char) * 4);
    memset (hex_value_mode7_864, 0, 4) ;
	if (int_value_mode7_864 < 256)//or FF=255 add 0
	{
		hex_value_mode7_864[0] = hexcon_value_mode7_864[0];
		hex_value_mode7_864[1] = hexcon_value_mode7_864[1]; 
		hex_value_mode7_864[2] = '0';
		hex_value_mode7_864[3] = '0';
	}
	else
	{
		hex_value_mode7_864[0] = hexcon_value_mode7_864[1];
		hex_value_mode7_864[1] = hexcon_value_mode7_864[2]; //Maybe need switch with 0 but seems okay
		hex_value_mode7_864[2] = '0';
		hex_value_mode7_864[3] = hexcon_value_mode7_864[0];
	}

	stringtohex (hex_value_mode7_864, mode7_864_byte3); 

    memset (mode7_864_byte1, 0, 3) ;
    strncpy ((char*)mode7_864_byte1, value_mode7_864, 3);

	_lseek(file,mode7_864_offs1,SEEK_SET);
	_write(file, mode7_864_byte1, 3); 

	_lseek(file,mode7_864_offs2,SEEK_SET);
	_write(file, mode7_864_byte1, 3);

	_lseek(file,mode7_864_offs3,SEEK_SET);
	_write(file, mode7_864_byte3, 2);
	//864 END
	//END MODE4 1152x864
	//---------------------------------------------------------------------------------------------
	//START MODE4 1280x960
	//1280
	HWND Dlg_1280 = GetDlgItem(hwnd, IDC_MODE8_EDIT1280);
	int Dlg_lgth_1280 = GetWindowTextLength (Dlg_1280);
	char *Dlg_txt_1280 = ( char *)GlobalAlloc( GPTR, (Dlg_lgth_1280 * ( sizeof (char) ) ) + 1);
	if (!Dlg_txt_1280)
	{
		MessageBox(NULL, "Error with MODE8_EDIT1280", "Error", MB_ICONEXCLAMATION | MB_OK);
//		_close(file);
		return;
	} 
	GetWindowText (Dlg_1280, Dlg_txt_1280, Dlg_lgth_1280);

	char *value_mode8_1280 = malloc (sizeof(char) * 4);
    memset (value_mode8_1280, 0, 4) ;
    strncpy (value_mode8_1280, Dlg_txt_1280, 4);
	
	GlobalFree (Dlg_txt_1280);

	int int_value_mode8_1280 = atoi (value_mode8_1280);//Make int

	//Int to Hex
	char *hexcon_value_mode8_1280 = malloc (sizeof(char) * 4);
    memset (hexcon_value_mode8_1280, 0, 4) ;
	_itoa (int_value_mode8_1280 ,hexcon_value_mode8_1280, 16);

	//Endianness
	char *hex_value_mode8_1280 = malloc (sizeof(char) * 4);
    memset (hex_value_mode8_1280, 0, 4) ;
	if (int_value_mode8_1280 > 4095)//or FFF=4095 remove 0
	{
		hex_value_mode8_1280[0] = hexcon_value_mode8_1280[2];
		hex_value_mode8_1280[1] = hexcon_value_mode8_1280[3]; 
		hex_value_mode8_1280[2] = hexcon_value_mode8_1280[0];//Should always be 0?
		hex_value_mode8_1280[3] = hexcon_value_mode8_1280[1];
	}
	else
	{
		hex_value_mode8_1280[0] = hexcon_value_mode8_1280[1];
		hex_value_mode8_1280[1] = hexcon_value_mode8_1280[2]; 
		hex_value_mode8_1280[2] = '0';
		hex_value_mode8_1280[3] = hexcon_value_mode8_1280[0];
	}

	stringtohex (hex_value_mode8_1280, mode8_1280_byte3); 

    memset (mode8_1280_byte1, 0, 4) ;
    strncpy ((char*)mode8_1280_byte1, value_mode8_1280, 4);

	_lseek(file,mode8_1280_offs1,SEEK_SET);
	_write(file, mode8_1280_byte1, 4); 

	_lseek(file,mode8_1280_offs2,SEEK_SET);
	_write(file, mode8_1280_byte1, 4);

	_lseek(file,mode8_1280_offs3,SEEK_SET);
	_write(file, mode8_1280_byte3, 2);
	//1280 END

	//960
	HWND Dlg2_960 = GetDlgItem(hwnd, IDC_MODE8_EDIT960);
	int Dlg2_lgth_960 = GetWindowTextLength (Dlg2_960);
	char *Dlg2_txt_960 = ( char *)GlobalAlloc( GPTR, (Dlg2_lgth_960 * ( sizeof (char) ) ) + 1);
	if (!Dlg2_txt_960)
	{
		MessageBox(NULL, "Error with MODE8_EDIT960", "Error", MB_ICONEXCLAMATION | MB_OK);
//		_close(file);
		return;
	} 
	GetWindowText (Dlg2_960, Dlg2_txt_960, Dlg2_lgth_960);

	char *value_mode8_960 = malloc (sizeof(char) * 3);
    memset (value_mode8_960, 0, 3) ;
    strncpy (value_mode8_960, Dlg2_txt_960, 3);
	
	GlobalFree (Dlg2_txt_960);

	int int_value_mode8_960 = atoi (value_mode8_960);//Make int

	//Int to Hex
	char *hexcon_value_mode8_960 = malloc (sizeof(char) * 3);
    memset (hexcon_value_mode8_960, 0, 3) ;
	_itoa (int_value_mode8_960 ,hexcon_value_mode8_960, 16);

	//Endianness
	char *hex_value_mode8_960 = malloc (sizeof(char) * 4);
    memset (hex_value_mode8_960, 0, 4) ;
	if (int_value_mode8_960 < 256)//or FF=255 add 0
	{
		hex_value_mode8_960[0] = hexcon_value_mode8_960[0];
		hex_value_mode8_960[1] = hexcon_value_mode8_960[1]; 
		hex_value_mode8_960[2] = '0';
		hex_value_mode8_960[3] = '0';
	}
	else
	{
		hex_value_mode8_960[0] = hexcon_value_mode8_960[1];
		hex_value_mode8_960[1] = hexcon_value_mode8_960[2]; //Maybe need switch with 0 but seems okay
		hex_value_mode8_960[2] = '0';
		hex_value_mode8_960[3] = hexcon_value_mode8_960[0];
	}

	stringtohex (hex_value_mode8_960, mode8_960_byte3); 

    memset (mode8_960_byte1, 0, 3) ;
    strncpy ((char*)mode8_960_byte1, value_mode8_960, 3);

	_lseek(file,mode8_960_offs1,SEEK_SET);
	_write(file, mode8_960_byte1, 3); 

	_lseek(file,mode8_960_offs2,SEEK_SET);
	_write(file, mode8_960_byte1, 3);

	_lseek(file,mode8_960_offs3,SEEK_SET);
	_write(file, mode8_960_byte3, 2);
	//960 END
	//END MODE4 1280x960
	//---------------------------------------------------------------------------------------------
	//START MODE4 1600x1200
	//1600
	HWND Dlg_1600 = GetDlgItem(hwnd, IDC_MODE9_EDIT1600);
	int Dlg_lgth_1600 = GetWindowTextLength (Dlg_1600);
	char *Dlg_txt_1600 = ( char *)GlobalAlloc( GPTR, (Dlg_lgth_1600 * ( sizeof (char) ) ) + 1);
	if (!Dlg_txt_1600)
	{
		MessageBox(NULL, "Error with MODE9_EDIT1600", "Error", MB_ICONEXCLAMATION | MB_OK);
//		_close(file);
		return;
	} 
	GetWindowText (Dlg_1600, Dlg_txt_1600, Dlg_lgth_1600);

	char *value_mode9_1600 = malloc (sizeof(char) * 4);
    memset (value_mode9_1600, 0, 4) ;
    strncpy (value_mode9_1600, Dlg_txt_1600, 4);
	
	GlobalFree (Dlg_txt_1600);

	int int_value_mode9_1600 = atoi (value_mode9_1600);//Make int

	//Int to Hex
	char *hexcon_value_mode9_1600 = malloc (sizeof(char) * 4);
    memset (hexcon_value_mode9_1600, 0, 4) ;
	_itoa (int_value_mode9_1600 ,hexcon_value_mode9_1600, 16);

	//Endianness
	char *hex_value_mode9_1600 = malloc (sizeof(char) * 4);
    memset (hex_value_mode9_1600, 0, 4) ;
	if (int_value_mode9_1600 > 4095)//or FFF=4095 remove 0
	{
		hex_value_mode9_1600[0] = hexcon_value_mode9_1600[2];
		hex_value_mode9_1600[1] = hexcon_value_mode9_1600[3]; 
		hex_value_mode9_1600[2] = hexcon_value_mode9_1600[0];//Should always be 0?
		hex_value_mode9_1600[3] = hexcon_value_mode9_1600[1];
	}
	else
	{
		hex_value_mode9_1600[0] = hexcon_value_mode9_1600[1];
		hex_value_mode9_1600[1] = hexcon_value_mode9_1600[2]; 
		hex_value_mode9_1600[2] = '0';
		hex_value_mode9_1600[3] = hexcon_value_mode9_1600[0];
	}

	stringtohex (hex_value_mode9_1600, mode9_1600_byte3); 

    memset (mode9_1600_byte1, 0, 4) ;
    strncpy ((char*)mode9_1600_byte1, value_mode9_1600, 4);

	_lseek(file,mode9_1600_offs1,SEEK_SET);
	_write(file, mode9_1600_byte1, 4); 

	_lseek(file,mode9_1600_offs2,SEEK_SET);
	_write(file, mode9_1600_byte1, 4);

	_lseek(file,mode9_1600_offs3,SEEK_SET);
	_write(file, mode9_1600_byte3, 2);
	//1600 END

	//1200
	HWND Dlg_1200 = GetDlgItem(hwnd, IDC_MODE9_EDIT1200);
	int Dlg_lgth_1200 = GetWindowTextLength (Dlg_1200);
	char *Dlg_txt_1200 = ( char *)GlobalAlloc( GPTR, (Dlg_lgth_1200 * ( sizeof (char) ) ) + 1);
	if (!Dlg_txt_1200)
	{
		MessageBox(NULL, "Error with MODE9_EDIT1200", "Error", MB_ICONEXCLAMATION | MB_OK);
//		_close(file);
		return;
	} 
	GetWindowText (Dlg_1200, Dlg_txt_1200, Dlg_lgth_1200);

	char *value_mode9_1200 = malloc (sizeof(char) * 4);
    memset (value_mode9_1200, 0, 4) ;
    strncpy (value_mode9_1200, Dlg_txt_1200, 4);
	
	GlobalFree (Dlg_txt_1200);

	int int_value_mode9_1200 = atoi (value_mode9_1200);//Make int

	//Int to Hex
	char *hexcon_value_mode9_1200 = malloc (sizeof(char) * 4);
    memset (hexcon_value_mode9_1200, 0, 4) ;
	_itoa (int_value_mode9_1200 ,hexcon_value_mode9_1200, 16);

	//Endianness
	char *hex_value_mode9_1200 = malloc (sizeof(char) * 4);
    memset (hex_value_mode9_1200, 0, 4) ;
	if (int_value_mode9_1200 > 4095)//or FFF=4095 remove 0
	{
		hex_value_mode9_1200[0] = hexcon_value_mode9_1200[2];
		hex_value_mode9_1200[1] = hexcon_value_mode9_1200[3]; 
		hex_value_mode9_1200[2] = hexcon_value_mode9_1200[0];//Should always be 0?
		hex_value_mode9_1200[3] = hexcon_value_mode9_1200[1];
	}
	else
	{
		hex_value_mode9_1200[0] = hexcon_value_mode9_1200[1];
		hex_value_mode9_1200[1] = hexcon_value_mode9_1200[2]; //Maybe need switch with 0 but seems okay
		hex_value_mode9_1200[2] = '0';
		hex_value_mode9_1200[3] = hexcon_value_mode9_1200[0];
	}

	stringtohex (hex_value_mode9_1200, mode9_1200_byte3); 

    memset (mode9_1200_byte1, 0, 4) ;
    strncpy ((char*)mode9_1200_byte1, value_mode9_1200, 4);

	_lseek(file,mode9_1200_offs1,SEEK_SET);
	_write(file, mode9_1200_byte1, 4); 

	_lseek(file,mode9_1200_offs2,SEEK_SET);
	_write(file, mode9_1200_byte1, 4);

	_lseek(file,mode9_1200_offs3,SEEK_SET);
	_write(file, mode9_1200_byte3, 2);
	//1200 END
	//END MODE4 1600x1200
	//---------------------------------------------------------------------------------------------



//	wsprintf(buffer, _T("Value is 0x%1x"), mode3_480_byte3[0]);//Show hex
//	wsprintf(buffer, _T("Value is %c"), mode3_480_byte3[0]);
//	wsprintf(buffer, _T("Value is %s"),  hex_value_mode4_600);
//	MessageBox(NULL, buffer, "Info", MB_ICONEXCLAMATION | MB_OK); 

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
