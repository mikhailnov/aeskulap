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

#ifndef IMAGEPOOL_DICOMDIRLOADER_H
#define IMAGEPOOL_DICOMDIRLOADER_H

#include "fileloader.h"
#include <list>
#include <string>

namespace ImagePool {

class DicomdirLoader: public FileLoader {
public:

    bool load(const std::string &studyinstanceuid, const Glib::ustring &dicomdir);

protected:

    class DcmDirectoryRecord *find_study(const std::string &studyinstanceuid, class DcmDicomDir &dir);

    bool scan_study(const std::string &studyinstanceuid, class DcmDirectoryRecord *study, const Glib::ustring &dicomdir);

private:

    static std::string ImageModalities;
};

} // namespace ImagePool

#endif // IMAGEPOOL_FILELOADER_H
