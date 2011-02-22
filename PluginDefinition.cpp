//this file is part of notepad++
//Copyright (C)2003 Don HO <donho@altern.org>
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

#include "PluginDefinition.h"
#include "menuCmdID.h"
#include <stdio.h>

//
// The plugin data that Notepad++ needs
//
FuncItem funcItem[nbFunc];

//
// The data of Notepad++ that you can use in your plugin commands
//
NppData nppData;

//
// Initialize your plugin data here
// It will be called while plugin loading   
void pluginInit(HANDLE hModule)
{
}

//
// Here you can do the clean up, save the parameters (if any) for the next session
//
void pluginCleanUp()
{
}

//
// Initialization of your plugin commands
// You should fill your plugins commands here
void commandMenuInit()
{

    //--------------------------------------------//
    //-- STEP 3. CUSTOMIZE YOUR PLUGIN COMMANDS --//
    //--------------------------------------------//
    // with function :
    // setCommand(int index,                      // zero based number to indicate the order of command
    //            TCHAR *commandName,             // the command name that you want to see in plugin menu
    //            PFUNCPLUGINCMD functionPointer, // the symbol of function (function pointer) associated with this command. The body should be defined below. See Step 4.
    //            ShortcutKey *shortcut,          // optional. Define a shortcut to trigger this command
    //            bool check0nInit                // optional. Make this menu item be checked visually
    //            );
    setCommand(0, TEXT("ASCII -> HEX"), ascii2Hex, NULL, false);
    setCommand(1, TEXT("HEX -> ASCII"), hex2Ascii, NULL, false);
}

//
// Here you can do the clean up (especially for the shortcut)
//
void commandMenuCleanUp()
{
	// Don't forget to deallocate your shortcut here
}


//
// This function help you to initialize your plugin commands
//
bool setCommand(size_t index, TCHAR *cmdName, PFUNCPLUGINCMD pFunc, ShortcutKey *sk, bool check0nInit) 
{
    if (index >= nbFunc)
        return false;

    if (!pFunc)
        return false;

    lstrcpy(funcItem[index]._itemName, cmdName);
    funcItem[index]._pFunc = pFunc;
    funcItem[index]._init2Check = check0nInit;
    funcItem[index]._pShKey = sk;

    return true;
}

//----------------------------------------------//
//-- STEP 4. DEFINE YOUR ASSOCIATED FUNCTIONS --//
//----------------------------------------------//

HWND getCurrentScintillaHandle() {
    int currentEdit;
    ::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&currentEdit);
	return (currentEdit == 0)?nppData._scintillaMainHandle:nppData._scintillaSecondHandle;
};

void ascii2Hex()
{
    ascii2hex(true, false);
	//ascii2hex(false, true);
}


SelectedString::SelectedString(): _str(NULL), _selStartPos(0), _selEndPos(0)
{
	HWND hCurrScintilla = getCurrentScintillaHandle();

    size_t start = ::SendMessage(hCurrScintilla, SCI_GETSELECTIONSTART, 0, 0);
	size_t end = ::SendMessage(hCurrScintilla, SCI_GETSELECTIONEND, 0, 0);
	if (end < start)
	{
		size_t tmp = start;
		start = end;
		end = tmp;
	}
	_selStartPos = start;
	_selEndPos = end;

	size_t textLen = end - start;
    if (textLen == 0) return;

    _str = new char[textLen+1];
    ::SendMessage(hCurrScintilla, SCI_GETSELTEXT, 0, (LPARAM)_str);
}	

enum hexStat{st_init, st_c, st_cc};

int getTrueHexValue(char c)
{
	switch (c)
	{
		case '0' :
		case '1' :
		case '2' :
		case '3' :
		case '4' :
		case '5' :
		case '6' :
		case '7' :
		case '8' :
		case '9' :
			return (c - '0');
		
		case 'a' :
		case 'A' :
			return 10;
			
		case 'b' :
		case 'B' :
			return 11;
		
		case 'c' :
		case 'C' :
			return 12;
			
		case 'd' :
		case 'D' :
			return 13;
			
		case 'e' :
		case 'E' :
			return 14;
			
		case 'f' :
		case 'F' :
			return 15;
		
		default :
			return -1;
	}
}

// rule :
// 0. only 0-9 a-f A-F withe space (32) and return (10, 13) are allowed.
// 1. 2 char to form 1 byte, so there are alway 2 char w/o space.
// 2. if there is a space between the 1st char and the 2nd char, then it should be it in all string, and vice and versa 
// 3. All the wite space (32) \n(10) and \r(13) will be ignored.
// 
bool HexString::toAscii()
{
	size_t l = length();
	bool hasWs = false;
	if (!l || l < 2) return false;
	if (l < 5)	//3 :  "00X" or "X00" where X == \n or " "
				//4 :  "0000"
	{

	}
	// Check 5 first characters
	else // 5: "00 00" or "00000"
	{
		hasWs = _str[2] == ' ';
	}
	// Begin conversion
	hexStat stat = st_init;
	size_t i = 0, j = 0;
	for ( ; _str[i] ; i++)
	{
		if (_str[i] == ' ')
		{
			if (!hasWs) return false;
			if (stat != st_cc) return false;
			stat = st_init;
		}
		/*
		else if (_str[i] == '\t')
		{
			
		}
		*/
		else if ((_str[i] == '\n') || (_str[i] == '\r'))
		{
			if ((stat != st_cc) && (stat != st_init)) return false;
			stat = st_init;
		}
		else if ((_str[i] >= 'a' && _str[i] <= 'f') ||
				 (_str[i] >= 'A' && _str[i] <= 'F') ||
				 (_str[i] >= '0' && _str[i] <= '9'))
		{
			if (stat == st_cc)
			{
				if (hasWs) return false;
				stat = st_c;
			}
			else if (stat == st_c)
			{
				// Process
				//::MessageBoxA(NULL, h, "", MB_OK);

				unsigned char hi = getTrueHexValue(_str[i-1]);
				unsigned char lo = getTrueHexValue(_str[i]);
				_str[j++] = hi*16+lo;
				stat = st_cc;
			}
			else if (stat == st_init)
			{
				stat = st_c;
			}
		}
		/*
		else if (_str[i] == ' ')
		{
			
		}
		*/
		else
		{
			return false;	
		}

	}
	// finalize
	if (stat == st_c) return false;
	_str[j] = '\0';
	return true;
}

void ascii2hex(bool insertSpace, bool isMaj)
{
	//::MessageBoxA(NULL, "Hit", "", MB_OK);
	SelectedString selText;
	size_t textLen = selText.length();
	if (!textLen) return;

	size_t inc = insertSpace?3:2;
	char *pDestText = new char[textLen*inc+1];

	for (size_t i = 0, j = 0 ; i < textLen ; i++)
	{
		const char *format = "";
		if (isMaj)
		{
			format = insertSpace?"%02X ":"%02X";
		}
		else
		{
			format = insertSpace?"%02x ":"%02x";
		}
		
		sprintf(pDestText + j, format, *(selText.getStr()+i));
		j += inc;
	}
	pDestText[textLen*inc] = 0x0;

	HWND hCurrScintilla = getCurrentScintillaHandle();
	size_t start = selText.getSelStartPos();
	::SendMessage(hCurrScintilla, SCI_REPLACESEL, 0, (LPARAM)pDestText);
	::SendMessage(hCurrScintilla, SCI_SETSEL, start, start+strlen(pDestText));

	delete [] pDestText;
}

void hex2Ascii()
{
	HexString transformer;
	size_t textLen = transformer.length();
	if (!textLen) return;

	bool b = transformer.toAscii();

	const char *hexStr = transformer.getStr();
	HWND hCurrScintilla = getCurrentScintillaHandle();
	size_t start = transformer.getSelStartPos();
	::SendMessage(hCurrScintilla, SCI_REPLACESEL, 0, (LPARAM)hexStr);
	::SendMessage(hCurrScintilla, SCI_SETSEL, start, start+strlen(hexStr));

}
