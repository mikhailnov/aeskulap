/*
    Aeskulap ImagePool - DICOM abstraction library
    Copyright (C) 2005  Alexander Pipelka

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

    Alexander Pipelka
*/

#ifndef IMAGEPOOL_FILELOADER_H
#define IMAGEPOOL_FILELOADER_H

#include "loader.h"
#include <list>
#include <string>

namespace ImagePool {

class FileLoader: public Loader {
public:

	bool load(const std::list< Glib::ustring >& filelist);

	sigc::signal< void, double > signal_prescan_progress;

protected:

	bool run();

	void prescan_files(std::list< Glib::ustring >* filelist);

	std::list< Glib::ustring >* m_filelist;

};

} // namespace ImagePool

#endif // IMAGEPOOL_FILELOADER_H
