// This file is part of Notepad++ project
// Copyright (C)2022 Don HO <don.h@free.fr>

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// at your option any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.


#ifndef WINDOW_CONTROL_H
#define WINDOW_CONTROL_H

#include <windows.h>

class Window
{
public:
	Window(): _hInst(NULL), _hParent(NULL), _hSelf(NULL){};
	virtual ~Window() {};

	virtual void init(HINSTANCE hInst, HWND parent)
	{
		_hInst = hInst;
		_hParent = parent;
	}

	virtual void destroy() = 0;

	virtual void display(bool toShow = true) const {
		::ShowWindow(_hSelf, toShow?SW_SHOW:SW_HIDE);
	};
	
	virtual void reSizeTo(RECT & rc) // should NEVER be const !!!
	{ 
		::MoveWindow(_hSelf, rc.left, rc.top, rc.right, rc.bottom, TRUE);
		redraw();
	};

	virtual void reSizeToWH(RECT & rc) // should NEVER be const !!!
	{ 
		::MoveWindow(_hSelf, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, TRUE);
		redraw();
	};

	virtual void redraw(bool forceUpdate = false) const {
		::InvalidateRect(_hSelf, NULL, TRUE);
		if (forceUpdate)
			::UpdateWindow(_hSelf);
	};
	
    virtual void getClientRect(RECT & rc) const {
		::GetClientRect(_hSelf, &rc);
	};

	virtual void getWindowRect(RECT & rc) const {
		::GetWindowRect(_hSelf, &rc);
	};

	virtual int getWidth() const {
		RECT rc;
		::GetClientRect(_hSelf, &rc);
		return (rc.right - rc.left);
	};

	virtual int getHeight() const {
		RECT rc;
		::GetClientRect(_hSelf, &rc);
		if (::IsWindowVisible(_hSelf) == TRUE)
			return (rc.bottom - rc.top);
		return 0;
	};

	virtual bool isVisible() const {
    	return (::IsWindowVisible(_hSelf)?true:false);
	};

	HWND getHSelf() const {
		//assert(_hSelf);
		return _hSelf;
	};

	HWND getHParent() const {
		return _hParent;
	};

	void getFocus() const {
		::SetFocus(_hSelf);
	};

    HINSTANCE getHinst() const {
		if (!_hInst)
		{
			::MessageBox(NULL, TEXT("_hInst == NULL"), TEXT("class Window"), MB_OK);
			throw int(1999);
		}
		return _hInst;
	};
protected:
	HINSTANCE _hInst;
	HWND _hParent;
	HWND _hSelf;
};

#endif //WINDOW_CONTROL_H


