//this file is part of notepad++
//Copyright (C)2003 Don HO ( donho@altern.org )
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either
//version 2 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#include "conversionPanel.h"
#include "PluginDefinition.h"
#include <stdio.h>


extern NppData nppData;


#ifdef UNICODE
#define generic_strtoul wcstoul
#define generic_sprintf swprintf
#else
#define generic_strtoul strtoul
#define generic_sprintf sprintf
#endif

#define BCKGRD_COLOR (RGB(255,102,102))
#define TXT_COLOR    (RGB(255,255,255))
#define CF_NPPTEXTLEN	TEXT("Notepad++ Binary Text Length")

BOOL CALLBACK ConversionPanel::run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) 
	{
		case WM_INITDIALOG :
		{
			::SendDlgItemMessage(_hSelf, ID_ASCII_EDIT, EM_LIMITTEXT, 1, 0);
			return TRUE;
		}
		break;

		case WM_COMMAND : 
		{
			
			switch (wParam)
			{
				case ID_ASCII_INSERT_BUTTON :
				{
					int v = getAsciiUcharFromDec();
					if (v != -1)
					{
						char charStr[2];
						charStr[0] = v;
						charStr[1] = '\0';
						HWND hCurrScintilla = getCurrentScintillaHandle();
						::SendMessage(hCurrScintilla, SCI_REPLACESEL, 0, (LPARAM)"");
						::SendMessage(hCurrScintilla, SCI_ADDTEXT, 1, (LPARAM)charStr);
					}
					return TRUE;
				}
				case ID_DEC_INSERT_BUTTON :
				{
					insertToNppFrom(ID_DEC_EDIT);
					return TRUE;
				}
				case ID_HEX_INSERT_BUTTON :
				{
					insertToNppFrom(ID_HEX_EDIT);
					return TRUE;
				}
				case ID_BIN_INSERT_BUTTON :
				{
					insertToNppFrom(ID_BIN_EDIT);
					return TRUE;
				}
				case ID_OCT_INSERT_BUTTON :
				{
					insertToNppFrom(ID_OCT_EDIT);
					return TRUE;
				}
				case ID_ASCII_BUTTON :
				{
					copyToClipboardFrom(ID_ASCII_EDIT);
					return TRUE;
				}
				case ID_DEC_BUTTON :
				{
					copyToClipboardFrom(ID_DEC_EDIT);
					return TRUE;
				}
				case ID_HEX_BUTTON :
				{
					copyToClipboardFrom(ID_HEX_EDIT);
					return TRUE;
				}
				case ID_BIN_BUTTON :
				{
					copyToClipboardFrom(ID_BIN_EDIT);
					return TRUE;
				}
				case ID_OCT_BUTTON :
				{
					copyToClipboardFrom(ID_OCT_EDIT);
					return TRUE;
				}
			}

			if (HIWORD(wParam) == EN_CHANGE)
			{
				switch (LOWORD(wParam))
				{
					case ID_ASCII_EDIT :
					case ID_DEC_EDIT :
					case ID_HEX_EDIT :
					case ID_BIN_EDIT :
					case ID_OCT_EDIT :
					{
						if (!lock)
							setValueFrom((int)LOWORD(wParam));
						return TRUE;
					}
				}
			}

			return FALSE;
		}

		default :
			return DockingDlgInterface::run_dlgProc(message, wParam, lParam);
	}
}

void ConversionPanel::resetExcept(int exceptID)
{
	if (exceptID != ID_ASCII_EDIT)
		::SendDlgItemMessage(_hSelf, ID_ASCII_EDIT, WM_SETTEXT, 0, (LPARAM)TEXT(""));
	if (exceptID != ID_DEC_EDIT)
		::SendDlgItemMessage(_hSelf, ID_DEC_EDIT, WM_SETTEXT, 0, (LPARAM)TEXT(""));
	if (exceptID != ID_HEX_EDIT)
		::SendDlgItemMessage(_hSelf, ID_HEX_EDIT, WM_SETTEXT, 0, (LPARAM)TEXT(""));
	if (exceptID != ID_OCT_EDIT)
		::SendDlgItemMessage(_hSelf, ID_OCT_EDIT, WM_SETTEXT, 0, (LPARAM)TEXT(""));
	if (exceptID != ID_BIN_EDIT)
		::SendDlgItemMessage(_hSelf, ID_BIN_EDIT, WM_SETTEXT, 0, (LPARAM)TEXT(""));
}

bool ConversionPanel::qualified(TCHAR *str, int id)
{
	for (int i = 0 ; i < lstrlen(str) ; i++)
	{
		if (id == ID_ASCII_EDIT)
		{
			//Only one character, Accept all
		}
		else if (id == ID_HEX_EDIT)
		{
			if (!((str[i] >= '0' && str[i] <= '9') ||
				  (str[i] >= 'A' && str[i] <= 'F') ||
				  (str[i] >= 'a' && str[i] <= 'f')))
			{

				return false;
			}
		}
		else if (id == ID_DEC_EDIT)
		{
			if (!(str[i] >= '0' && str[i] <= '9'))
			{
				
				return false;
			}
		}
		else if (id == ID_OCT_EDIT)
		{
			if (!(str[i] >= '0' && str[i] <= '7'))
			{	
				
				return false;
			}
		}
		else if (id == ID_BIN_EDIT)
		{
			if (!(str[i] == '0' || str[i] == '1'))
			{	
				
				return false;
			}
		}
	}
	return true;
}

generic_string ConversionPanel::getAsciiInfo(unsigned char value)
{
	switch (value)
	{
		case 0:
			return TEXT("NULL");
		case 1:
			return TEXT("SOH");
		case 2:
			return TEXT("STX");
		case 3:
			return TEXT("ETX");
		case 4:
			return TEXT("EOT");
		case 5:
			return TEXT("ENQ");
		case 6:
			return TEXT("ACK");
		case 7:
			return TEXT("BEL");
		case 8:
			return TEXT("BS");
		case 9:
			return TEXT("TAB");
		case 10:
			return TEXT("LF");
		case 11:
			return TEXT("VT");
		case 12:
			return TEXT("FF");
		case 13:
			return TEXT("CR");
		case 14:
			return TEXT("SO");
		case 15:
			return TEXT("SI");
		case 16:
			return TEXT("DLE");
		case 17:
			return TEXT("DC1");
		case 18:
			return TEXT("DC2");
		case 19:
			return TEXT("DC3");
		case 20:
			return TEXT("DC4");
		case 21:
			return TEXT("NAK");
		case 22:
			return TEXT("SYN");
		case 23:
			return TEXT("ETB");
		case 24:
			return TEXT("CAN");
		case 25:
			return TEXT("EM");
		case 26:
			return TEXT("SUB");
		case 27:
			return TEXT("ESC");
		case 28:
			return TEXT("FS");
		case 29:
			return TEXT("GS");
		case 30:
			return TEXT("RS");
		case 31:
			return TEXT("US");
		case 32:
			return TEXT("Space");
		case 127:
			return TEXT("DEL");
		/*
		case 0:
			return TEXT("NULL");
		case 0:
			return TEXT("NULL");
		case 0:
			return TEXT("NULL");
		case 0:
			return TEXT("NULL");
		case 0:
			return TEXT("NULL");
		*/
	}
	return TEXT("");	
}

void ConversionPanel::setValueExcept(int exceptID, size_t value)
{
	const int strLen = 1024;
	TCHAR str2Display[strLen];
	if (exceptID != ID_ASCII_EDIT)
	{
		if (value <= 255)
		{
			TCHAR ascii2Display[2];
			ascii2Display[0] = (TCHAR)value;
			ascii2Display[1] = '\0';
			::SendDlgItemMessage(_hSelf, ID_ASCII_EDIT, WM_SETTEXT, 0, (LPARAM)ascii2Display);
			::SendDlgItemMessage(_hSelf, ID_ASCII_INFO_STATIC, WM_SETTEXT, 0, (LPARAM)getAsciiInfo((unsigned char)value).c_str());
		}
		else
			::SendDlgItemMessage(_hSelf, ID_ASCII_INFO_STATIC, WM_SETTEXT, 0, (LPARAM)TEXT(""));
	}
	else
	{
		::SendDlgItemMessage(_hSelf, ID_ASCII_INFO_STATIC, WM_SETTEXT, 0, (LPARAM)getAsciiInfo((unsigned char)value).c_str());
	}

	if (exceptID != ID_DEC_EDIT)
	{
		generic_sprintf(str2Display, strLen, TEXT("%d"), (size_t)value);
		::SendDlgItemMessage(_hSelf, ID_DEC_EDIT, WM_SETTEXT, 0, (LPARAM)str2Display);
	}
	if (exceptID != ID_HEX_EDIT)
	{
		generic_sprintf(str2Display, strLen, TEXT("%X"), (size_t)value);
		::SendDlgItemMessage(_hSelf, ID_HEX_EDIT, WM_SETTEXT, 0, (LPARAM)str2Display);
	}
	if (exceptID != ID_OCT_EDIT)
	{
		generic_sprintf(str2Display, strLen, TEXT("%o"), (size_t)value);
		::SendDlgItemMessage(_hSelf, ID_OCT_EDIT, WM_SETTEXT, 0, (LPARAM)str2Display);
	}
	if (exceptID != ID_BIN_EDIT)
	{
		char str2DisplayA[1234];
		itoa(int(value), str2DisplayA, 2);
		::SendDlgItemMessageA(_hSelf, ID_BIN_EDIT, WM_SETTEXT, 0, (LPARAM)str2DisplayA);
	}
}


void ConversionPanel::setValueFrom(int id)
{
	lock = true;
	const int inStrSize = 256;
	TCHAR intStr[inStrSize];
	::SendDlgItemMessage(_hSelf, id, WM_GETTEXT, inStrSize, (LPARAM)intStr);

	resetExcept(id);

	if (!intStr[0])
	{
		lock = false;
		return;
	}

	if (!qualified(intStr, id))
	{
		lock = false;
		int len = lstrlen(intStr);
		intStr[len-1] = '\0';
		::SendDlgItemMessage(_hSelf, id, WM_SETTEXT, 0, (LPARAM)intStr);
		::SendDlgItemMessage(_hSelf, id, EM_SETSEL, len-1, len-1);
		return;
	}

	int base = 10;
	switch (id)
	{
		case ID_ASCII_EDIT :
		{
			base = 0;
			
		}
		break;
		case ID_DEC_EDIT :
		{
			base = 10;	
			
		}
		break;
		case ID_HEX_EDIT :
		{
			base = 16;
			
		}
		break;
		case ID_OCT_EDIT :
		{
			base = 8;
			
		}
		break;
		case ID_BIN_EDIT :
		{
			base = 2;
			
		}
		break;
	}

	unsigned long v = 0;
	if (!base)
		v = intStr[0];
	else
		v = generic_strtoul(intStr, NULL, base);

	setValueExcept(id, v);
	lock = false;
}

void ConversionPanel::insertToNppFrom(int id)
{
	const int inStrSize = 256;
	char intStr[inStrSize];
	::SendDlgItemMessageA(_hSelf, id, WM_GETTEXT, inStrSize, (LPARAM)intStr);
	HWND hCurrScintilla = getCurrentScintillaHandle();
	::SendMessage(hCurrScintilla, SCI_REPLACESEL, 0, (LPARAM)intStr);	
}

void ConversionPanel::copyToClipboardFrom(int id)
{
	const int intStrMaxSize = 256;
	char intStr[intStrMaxSize];
	size_t intStrLen = 0;
	if (id == ID_ASCII_EDIT)
	{
		int v = getAsciiUcharFromDec();
		if (v == -1) return;
		intStr[0] = v;
		intStr[1] = '\0';
		intStrLen = 1;
	}
	else
	{
		::SendDlgItemMessageA(_hSelf, id, WM_GETTEXT, intStrMaxSize, (LPARAM)intStr);
		intStrLen = strlen(intStr);
	}
	// Open the clipboard, and empty it. 
	if (!OpenClipboard(NULL)) 
		return;
	EmptyClipboard();
 
	// Allocate a global memory object for the text. 
	HGLOBAL hglbCopy = GlobalAlloc(GMEM_MOVEABLE, (intStrLen + 1) * sizeof(unsigned char)); 
	if (hglbCopy == NULL) 
	{ 
		CloseClipboard(); 
		return; 
	} 

	// Lock the handle and copy the text to the buffer. 
	unsigned char *lpucharCopy = (unsigned char *)GlobalLock(hglbCopy); 
	memcpy(lpucharCopy, intStr, intStrLen * sizeof(unsigned char)); 
	lpucharCopy[intStrLen] = 0;    // null character
	
	GlobalUnlock(hglbCopy); 

	// Place the handle on the clipboard.
	SetClipboardData(CF_TEXT, hglbCopy);
	

	// Allocate a global memory object for the text length.
	HGLOBAL hglbLenCopy = GlobalAlloc(GMEM_MOVEABLE, sizeof(unsigned long)); 
	if (hglbLenCopy == NULL) 
	{ 
		CloseClipboard(); 
		return; 
	} 

	// Lock the handle and copy the text to the buffer. 
	unsigned long *lpLenCopy = (unsigned long *)GlobalLock(hglbLenCopy); 
	*lpLenCopy = (unsigned long)intStrLen;
	
	GlobalUnlock(hglbLenCopy); 

	// Place the handle on the clipboard.
	UINT f = RegisterClipboardFormat(CF_NPPTEXTLEN);
	SetClipboardData(f, hglbLenCopy);

	CloseClipboard();

}

int ConversionPanel::getAsciiUcharFromDec()
{
	const int inStrSize = 256;
	TCHAR intStr[inStrSize];
	::SendDlgItemMessage(_hSelf, ID_DEC_EDIT, WM_GETTEXT, inStrSize, (LPARAM)intStr);
	int v = generic_strtoul(intStr, NULL, 10);
	if (v > 255)
		return -1;
	return v;
}