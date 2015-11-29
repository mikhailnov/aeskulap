/*
    Aeskulap - DICOM image viewer and network client
    Copyright (C) 2005  Alexander Pipelka

    This file is part of Aeskulap.

    Aeskulap is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    Aeskulap is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Aeskulap; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

    Alexander Pipelka
*/

#include "abusycursor.h"
#include "aiconfactory.h"

#include <iostream>

namespace Aeskulap {

static Gtk::Window* mainwindow = NULL;

void set_busy_cursor(bool busy, Gtk::Window* w) {
	if(w == NULL) {
		w = mainwindow;
	}

	if(w == NULL) {
		return;
	}

	if(busy) {
		std::cout << "turning cursor busy" << std::endl;
		w->get_window()->set_cursor(Aeskulap::IconFactory::get_cursor_watch());
	}
	else {
		std::cout << "cursor busy off" << std::endl;
		w->get_window()->set_cursor();
	}
}

void set_mainwindow(Gtk::Window* w) {
	mainwindow = w;
}

} //namespace Aeskulap
