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

#ifndef GOTILINE_DLG_H
#define GOTILINE_DLG_H

#include "DockingDlgInterface.h"
#include "resource.h"
#include <string>

typedef std::basic_string<TCHAR> generic_string;

class ConversionPanel : public DockingDlgInterface
{
public :
	ConversionPanel() : lock(false), DockingDlgInterface(IDD_CONVERSION_PANEL){};

    virtual void display(bool toShow = true) const {
        DockingDlgInterface::display(toShow);
        if (toShow)
            ::SetFocus(::GetDlgItem(_hSelf, ID_ASCII_EDIT));
    };

	void setParent(HWND parent2set){
		_hParent = parent2set;
	};

	void resetExcept(int exceptID);
	void setValueFrom(int id);
	void setValueExcept(int id, size_t value);
	bool qualified(TCHAR *str, int id);
	generic_string getAsciiInfo(unsigned char value);
	void insertToNppFrom(int id);
	int getAsciiUcharFromDec();
	void copyToClipboardFrom(int id);

protected :
	virtual BOOL CALLBACK run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam);

private :
	bool lock;
    int getLine() const {
        BOOL isSuccessful;
        int line = ::GetDlgItemInt(_hSelf, ID_ASCII_EDIT, &isSuccessful, FALSE);
        return (isSuccessful?line:-1);
    };
};

HWND getCurrentScintillaHandle();


#endif //GOTILINE_DLG_H
