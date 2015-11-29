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

#ifndef IMAGEPOOL_NETLOADER_H
#define IMAGEPOOL_NETLOADER_H

#include "loader.h"
#include "poolstudy.h"

namespace ImagePool {

class NetLoader : public Loader {
public:

	NetLoader(const std::string& local_aet);

	bool load(const Glib::RefPtr< ImagePool::Study >& study, const std::string& server);
	
protected:

	bool run();

private:

	Glib::RefPtr< ImagePool::Study > m_study;

	std::string m_server;

	std::string m_local_aet;
};
	
} // namespace ImagePool

#endif // IMAGEPOOL_NETLOADER_H
