/****************************************************************************
 *                                                                          *
 * File    : main.c                                                         *
 *                                                                          *
 * Purpose : Generic dialog based Win32 application.                        *
 *                                                                          *
 * History : Date      Reason                                               *
 *           06/06/12  Created    Jaap Kooistra                             *
 *                                                                          *
 ****************************************************************************/

/* 
 * Either define WIN32_LEAN_AND_MEAN, or one or more of NOCRYPT,
 * NOSERVICE, NOMCX and NOIME, to decrease compile time (if you
 * don't need these defines -- see windows.h).
 */

#define WIN32_DEFAULT_LIBS
#define WIN32_LEAN_AND_MEAN


//#include "readfile.h"
#include "main.h"


#define NELEMS(a)  (sizeof(a) / sizeof((a)[0]))

/** Prototypes **************************************************************/

static INT_PTR CALLBACK MainDlgProc(HWND, UINT, WPARAM, LPARAM);

/** Global variables ********************************************************/

static HANDLE ghInstance;

char InputFileNamePath[MAX_PATH] = "";
char OutFileKPBackupPath[MAX_PATH] = "";

void PatchKingpin(HWND hwnd)
{
	char *FilePath = malloc (sizeof(InputFileNamePath));//New path?
	strncpy (FilePath, InputFileNamePath, sizeof(InputFileNamePath) );
	PathRemoveFileSpec(FilePath);//Works only in Windows
	PathCombine(OutFileKPBackupPath,FilePath,"kingpinbackup.exe");//Works only in Windows 

	if (IsDlgButtonChecked(hwnd, IDC_CHECKBOX_BACKUP))
		BackupKingpin (InputFileNamePath, OutFileKPBackupPath);

	PatchKingpinFile (hwnd, InputFileNamePath);
	PatchKPresolutionFile (hwnd, InputFileNamePath);			
	
	//Close everything
	MessageBox(NULL, "File was patched Successfully.", "Success", MB_ICONINFORMATION | MB_OK);
	EnableWindow(GetDlgItem(hwnd, IDC_BUTTON_PATCH), FALSE); 
	DisableResolutionGroup (hwnd);
	DisableExtraGroup (hwnd);
	SetDlgItemText(hwnd, LBL_TEXT_KP_PATH, "");
	SetDlgItemText(hwnd, LBL_TEXT_KP_MD5, "");
	SetDlgItemText(hwnd, LBL_TEXT_KP_CHECK, "File was patched Successfully.");
}
void CheckKingpinBackup (HWND hwnd, char *kpfilename)
{
	FILE *Filekpbackup;
	if ((Filekpbackup = fopen(kpfilename, "r"))!=NULL)//Does kingpinbackup excist?
	{
		CheckDlgButton(hwnd, IDC_CHECKBOX_BACKUP, BST_UNCHECKED);
		EnableWindow(GetDlgItem(hwnd, IDC_CHECKBOX_BACKUP), FALSE);
		fclose (Filekpbackup);//Close file
	}
	else//If open new file in other dir
	{
		CheckDlgButton(hwnd, IDC_CHECKBOX_BACKUP, BST_CHECKED);
		EnableWindow(GetDlgItem(hwnd, IDC_CHECKBOX_BACKUP), TRUE);
		fclose (Filekpbackup);//Close file
	}
}
void DoFileOpen(HWND hwnd)
{
	OPENFILENAME ofn;

	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFilter = "Kingpin 1.21 (kingpin.exe)\0kingpin.exe\0";
 	
	ofn.lpstrFile = InputFileNamePath;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = "Select file to open";

	if(GetOpenFileName(&ofn))
	{
//		HWND hInputtextfilepath = GetDlgItem(hwnd, LBL_TEXT_KP_PATH);//Load in into path status
//		SetWindowText(hInputtextfilepath, szInputFileName);
		SetDlgItemText(hwnd, LBL_TEXT_KP_PATH, InputFileNamePath);

		CheckKingpinBackup (hwnd, "kingpinbackup.exe");
		CheckMD5file (hwnd);
		CheckKingpinFile (hwnd, InputFileNamePath);
//		GetKingpinFileSize (InputFileNamePath);//Use to find out Filesize			
	}
}

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
    INITCOMMONCONTROLSEX icc;
    WNDCLASSEX wcx;

    ghInstance = hInstance;

    /* Initialize common controls. Also needed for MANIFEST's */
    /*
     * TODO: set the ICC_???_CLASSES that you need.
     */
    icc.dwSize = sizeof(icc);
    icc.dwICC = ICC_WIN95_CLASSES /*|ICC_COOL_CLASSES|ICC_DATE_CLASSES|ICC_PAGESCROLLER_CLASS|ICC_USEREX_CLASSES|... */;
    InitCommonControlsEx(&icc);


    /* Get system dialog information */
    wcx.cbSize = sizeof(wcx);
    if (!GetClassInfoEx(NULL, MAKEINTRESOURCE(32770), &wcx))
        return 0;

    /* Add our own stuff */
    wcx.hInstance = hInstance;
    wcx.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDR_ICO_MAIN));
    wcx.lpszClassName = _T("KPresPatch_Class");

    if (!RegisterClassEx(&wcx))
        return 0;

    /* The user interface is a modal dialog box */
   return DialogBox(hInstance, MAKEINTRESOURCE(DLG_MAIN), NULL, (DLGPROC)MainDlgProc);
}
//Process messages for the About dialog.  The dialog is shown when the user selects "About" in the "Help" menu. 
static LRESULT CALLBACK AboutDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_INITDIALOG:
            /*
             * Nothing special to initialize.
             */
            return TRUE;

        case WM_COMMAND:
            switch (wParam)
            {
                case IDOK:
                case IDCANCEL:
                    /*
                     * OK or Cancel was clicked, close the dialog.
                     */
                    EndDialog(hDlg, TRUE);
                    return TRUE;
            }
            break;
    }

    return FALSE;
}
//Process application messages.
static INT_PTR CALLBACK MainDlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {

        case WM_INITDIALOG:
		{
			//Edit sysmenu for about box
            HMENU hSysMenu = GetSystemMenu(hwndDlg, FALSE);
	        if (hSysMenu != NULL)
	        {
			    RemoveMenu(hSysMenu, SC_RESTORE, MF_BYCOMMAND);
				RemoveMenu(hSysMenu, SC_MAXIMIZE, MF_BYCOMMAND);
			 	RemoveMenu(hSysMenu, SC_SIZE, MF_BYCOMMAND);

//              AppendMenu (hSysMenu, MF_SEPARATOR | MF_DISABLED, 0, NULL);
//			    AppendMenu (hSysMenu, MF_STRING | MF_ENABLED, IDM_ABOUT, TEXT("About"));

				InsertMenu (hSysMenu, 2, MF_BYPOSITION |MF_SEPARATOR, 0, "");
				InsertMenu (hSysMenu, 3, MF_BYPOSITION, IDM_ABOUT, "About");
	        }
			//End sysmenu for about box


			//Coop Maxclients
            HWND hUpdownclients = GetDlgItem(hwndDlg, IDC_SPIN_COOPMAXCLIENTS);
            HWND hEditclients = GetDlgItem(hwndDlg, IDC_EDIT_COOPMAXCLIENTS);
			SendMessage(hUpdownclients, UDM_SETBUDDY, (WPARAM)hEditclients, 0);
			SendMessage(hUpdownclients, UDM_SETRANGE, (WPARAM)0, (LPARAM)MAKELONG(9,2));
			SendMessage(hUpdownclients, UDM_SETPOS, 0, (LPARAM)4);

			//Resolution default:
            HWND hUpdown640 = GetDlgItem(hwndDlg, IDC_MODE3_SPIN640);
            HWND hEdit640 = GetDlgItem(hwndDlg, IDC_MODE3_EDIT640);
			SendMessage(hUpdown640, UDM_SETBUDDY, (WPARAM)hEdit640, 0);
			SendMessage(hUpdown640, UDM_SETRANGE, (WPARAM)0, (LPARAM)MAKELONG(999,100));
			SendMessage(hUpdown640, UDM_SETPOS, 0, (LPARAM)640);
            HWND hUpdown480 = GetDlgItem(hwndDlg, IDC_MODE3_SPIN480);
            HWND hEdit480 = GetDlgItem(hwndDlg, IDC_MODE3_EDIT480);
			SendMessage(hUpdown480, UDM_SETBUDDY, (WPARAM)hEdit480, 0);
			SendMessage(hUpdown480, UDM_SETRANGE, (WPARAM)0, (LPARAM)MAKELONG(999,100));
			SendMessage(hUpdown480, UDM_SETPOS, 0, (LPARAM)480);

            HWND hUpdown800 = GetDlgItem(hwndDlg, IDC_MODE4_SPIN800);
            HWND hEdit800 = GetDlgItem(hwndDlg, IDC_MODE4_EDIT800);
			SendMessage(hUpdown800, UDM_SETBUDDY, (WPARAM)hEdit800, 0);
			SendMessage(hUpdown800, UDM_SETRANGE, (WPARAM)0, (LPARAM)MAKELONG(999,100));
			SendMessage(hUpdown800, UDM_SETPOS, 0, (LPARAM)800);
            HWND hUpdown600 = GetDlgItem(hwndDlg, IDC_MODE4_SPIN600);
            HWND hEdit600 = GetDlgItem(hwndDlg, IDC_MODE4_EDIT600);
			SendMessage(hUpdown600, UDM_SETBUDDY, (WPARAM)hEdit600, 0);
			SendMessage(hUpdown600, UDM_SETRANGE, (WPARAM)0, (LPARAM)MAKELONG(999,100));
			SendMessage(hUpdown600, UDM_SETPOS, 0, (LPARAM)600);

            HWND hUpdown960 = GetDlgItem(hwndDlg, IDC_MODE5_SPIN960);
            HWND hEdit960 = GetDlgItem(hwndDlg, IDC_MODE5_EDIT960);
			SendMessage(hUpdown960, UDM_SETBUDDY, (WPARAM)hEdit960, 0);
			SendMessage(hUpdown960, UDM_SETRANGE, (WPARAM)0, (LPARAM)MAKELONG(999,100));
			SendMessage(hUpdown960, UDM_SETPOS, 0, (LPARAM)960);
            HWND hUpdown720 = GetDlgItem(hwndDlg, IDC_MODE5_SPIN720);
            HWND hEdit720 = GetDlgItem(hwndDlg, IDC_MODE5_EDIT720);
			SendMessage(hUpdown720, UDM_SETBUDDY, (WPARAM)hEdit720, 0);
			SendMessage(hUpdown720, UDM_SETRANGE, (WPARAM)0, (LPARAM)MAKELONG(999,100));
			SendMessage(hUpdown720, UDM_SETPOS, 0, (LPARAM)720);

            HWND hUpdown1024 = GetDlgItem(hwndDlg, IDC_MODE6_SPIN1024);
            HWND hEdit1024 = GetDlgItem(hwndDlg, IDC_MODE6_EDIT1024);
			SendMessage(hUpdown1024, UDM_SETBUDDY, (WPARAM)hEdit1024, 0);
			SendMessage(hUpdown1024, UDM_SETRANGE, (WPARAM)0, (LPARAM)MAKELONG(9999,1000));
			SendMessage(hUpdown1024, UDM_SETPOS, 0, (LPARAM)1024);
            HWND hUpdown768 = GetDlgItem(hwndDlg, IDC_MODE6_SPIN768);
            HWND hEdit768 = GetDlgItem(hwndDlg, IDC_MODE6_EDIT768);
			SendMessage(hUpdown768, UDM_SETBUDDY, (WPARAM)hEdit768, 0);
			SendMessage(hUpdown768, UDM_SETRANGE, (WPARAM)0, (LPARAM)MAKELONG(999,100));
			SendMessage(hUpdown768, UDM_SETPOS, 0, (LPARAM)768);

            HWND hUpdown1152 = GetDlgItem(hwndDlg, IDC_MODE7_SPIN1152);
            HWND hEdit1152 = GetDlgItem(hwndDlg, IDC_MODE7_EDIT1152);
			SendMessage(hUpdown1152, UDM_SETBUDDY, (WPARAM)hEdit1152, 0);
			SendMessage(hUpdown1152, UDM_SETRANGE, (WPARAM)0, (LPARAM)MAKELONG(9999,1000));
			SendMessage(hUpdown1152, UDM_SETPOS, 0, (LPARAM)1152);
            HWND hUpdown864 = GetDlgItem(hwndDlg, IDC_MODE7_SPIN864);
            HWND hEdit864 = GetDlgItem(hwndDlg, IDC_MODE7_EDIT864);
			SendMessage(hUpdown864, UDM_SETBUDDY, (WPARAM)hEdit864, 0);
			SendMessage(hUpdown864, UDM_SETRANGE, (WPARAM)0, (LPARAM)MAKELONG(999,100));
			SendMessage(hUpdown864, UDM_SETPOS, 0, (LPARAM)864);

            HWND hUpdown1280 = GetDlgItem(hwndDlg, IDC_MODE8_SPIN1280);
            HWND hEdit1280 = GetDlgItem(hwndDlg, IDC_MODE8_EDIT1280);
			SendMessage(hUpdown1280, UDM_SETBUDDY, (WPARAM)hEdit1280, 0);
			SendMessage(hUpdown1280, UDM_SETRANGE, (WPARAM)0, (LPARAM)MAKELONG(9999,1000));
			SendMessage(hUpdown1280, UDM_SETPOS, 0, (LPARAM)1280);
            HWND hUpdown960_2 = GetDlgItem(hwndDlg, IDC_MODE8_SPIN960);
            HWND hEdit960_2 = GetDlgItem(hwndDlg, IDC_MODE8_EDIT960);
			SendMessage(hUpdown960_2, UDM_SETBUDDY, (WPARAM)hEdit960_2, 0);
			SendMessage(hUpdown960_2, UDM_SETRANGE, (WPARAM)0, (LPARAM)MAKELONG(999,100));
			SendMessage(hUpdown960_2, UDM_SETPOS, 0, (LPARAM)960);

            HWND hUpdown1600 = GetDlgItem(hwndDlg, IDC_MODE9_SPIN1600);
            HWND hEdit1600 = GetDlgItem(hwndDlg, IDC_MODE9_EDIT1600);
			SendMessage(hUpdown1600, UDM_SETBUDDY, (WPARAM)hEdit1600, 0);
			SendMessage(hUpdown1600, UDM_SETRANGE, (WPARAM)0, (LPARAM)MAKELONG(9999,1000));
			SendMessage(hUpdown1600, UDM_SETPOS, 0, (LPARAM)1600);
            HWND hUpdown1200 = GetDlgItem(hwndDlg, IDC_MODE9_SPIN1200);
            HWND hEdit1200 = GetDlgItem(hwndDlg, IDC_MODE9_EDIT1200);
			SendMessage(hUpdown1200, UDM_SETBUDDY, (WPARAM)hEdit1200, 0);
			SendMessage(hUpdown1200, UDM_SETRANGE, (WPARAM)0, (LPARAM)MAKELONG(9999,1000));
			SendMessage(hUpdown1200, UDM_SETPOS, 0, (LPARAM)1200);
			//Resolution End

//			FormMain_OnInitDialog(hwndDlg);
			return TRUE;
   		}
        case WM_SIZE:
            /*
             * TODO: Add code to process resizing, when needed.
             */

            return TRUE;

		case WM_SYSCOMMAND:
			switch (GET_WM_COMMAND_ID(wParam, lParam))
			{
     		    case IDM_ABOUT:
            		DialogBox(ghInstance, MAKEINTRESOURCE(DLG_ABOUT), hwndDlg, (DLGPROC)AboutDlgProc);
					return TRUE;
			}
	        break;


		case WM_DROPFILES:
		{
			UINT uFile = 0;
			HANDLE hDrop = (HANDLE) wParam;

			uFile = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);
			if (uFile != 1)
			{
				MessageBox (NULL, "Dropping multiple files is not supported.", "Error", MB_ICONEXCLAMATION | MB_OK);
				DragFinish(hDrop);
				break;
			}
			DragQueryFile(hDrop,0, InputFileNamePath, MAX_PATH); // get the file name
			DragFinish(hDrop);                      // close the handle

			char *FileName = malloc (sizeof(InputFileNamePath));
			strncpy (FileName, InputFileNamePath, sizeof(InputFileNamePath) );
			PathStripPath(FileName);//Works only in Windows
			char *Ext = strrchr(FileName, '.');
			if (strncmp (Ext, ".exe", 4) != 0)
			{
				MessageBox(NULL, "Not a exe file.", "Error", MB_ICONEXCLAMATION | MB_OK);
				break;
			}
			SetDlgItemText(hwndDlg,LBL_TEXT_KP_PATH, InputFileNamePath);

			char *FilePath = malloc (sizeof(InputFileNamePath));//New path?
			strncpy (FilePath, InputFileNamePath, sizeof(InputFileNamePath) );
			PathRemoveFileSpec(FilePath);//Works only in Windows
			PathCombine(OutFileKPBackupPath,FilePath,"kingpinbackup.exe");//Works only in Windows 

			CheckKingpinBackup (hwndDlg, OutFileKPBackupPath);
			CheckMD5file (hwndDlg);
			CheckKingpinFile (hwndDlg, InputFileNamePath);
		}
		break;

        case WM_COMMAND:
			switch (GET_WM_COMMAND_ID(wParam, lParam))
			{
/*     		    case IDM_ABOUT://Moved to WM_SYSCOMMAND not working here
            		DialogBox(ghInstance, MAKEINTRESOURCE(DLG_ABOUT), hwndDlg, (DLGPROC)AboutDlgProc);
					return TRUE;*/
				case IDC_BUTTON_KINGPIN:
					DoFileOpen(hwndDlg);
					return TRUE;
				case IDC_BUTTON_PATCH:
					PatchKingpin(hwndDlg);
					return TRUE;

/*				case IDC_CHECKBOX_COOPDMFLAGS://For 1.1 
					if (IsDlgButtonChecked(hwndDlg, IDC_CHECKBOX_COOPDMFLAGS ))
						SetDlgItemText(hwndDlg, IDC_CHECKBOX_COOPDMFLAGS ,"Coop dmflags 784 to 788");
					else
						SetDlgItemText(hwndDlg, IDC_CHECKBOX_COOPDMFLAGS ,"Coop dmflags 788 to 784");
					return TRUE;

				case IDC_CHECKBOX_NOCD:
					if (IsDlgButtonChecked(hwndDlg, IDC_CHECKBOX_NOCD))
						SetDlgItemText(hwndDlg, IDC_CHECKBOX_NOCD,"No CD Check");
					else
						SetDlgItemText(hwndDlg, IDC_CHECKBOX_NOCD,"Add CD Check");
					return TRUE;*/

			}
	        break;


        case WM_CLOSE:
            EndDialog(hwndDlg, 0);
            return TRUE;


        /*
         * TODO: Add more messages, when needed.
         */
    }

    return FALSE;
}
