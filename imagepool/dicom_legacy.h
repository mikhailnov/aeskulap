#ifndef DICOM_LEGACY_H_INCLUDED
#define DICOM_LEGACY_H_INCLUDED

#include <dcmtk/dcmdata/dcuid.h>

#if OFFIS_DCMTK_VERSION_NUMBER >= 362

#include <dcmtk/dcmdata/dcitem.h>

inline DcmElement *newDicomElement(const DcmTagKey& key)
{
	return DcmItem::newDicomElement(key);
}

inline OFCondition newDicomElement(DcmElement *&newElement,
                                       const DcmTagKey &tag)
{
	return DcmItem::newDicomElement(newElement, tag);
}

#endif

#endif // DICOM_LEGACY_H_INCLUDED

