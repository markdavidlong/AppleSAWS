#ifndef GENERICFILE_H
#define GENERICFILE_H

/*****************************************************************************
*    AppleSAWS - The Apple Software Analysis WorkShop                        *
*    Copyright (C) 2015-2021  Mark D. Long                                   *
*                                                                            *
*    This program is free software: you can redistribute it and/or modify    *
*    it under the terms of the GNU General Public License as published by    *
*    the Free Software Foundation, either version 3 of the License, or       *
*    (at your option) any later version.                                     *
*                                                                            *
*    This program is distributed in the hope that it will be useful,         *
*    but WITHOUT ANY WARRANTY; without even the implied warranty of          *
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
*    GNU General Public License for more details.                            *
*                                                                            *
*    You should have received a copy of the GNU General Public License       *
*    along with this program.  If not, see <https://www.gnu.org/licenses/>.  *
*****************************************************************************/

#include "dos33diskimage.h"
#include "filedescriptiveentry.h"

#include <QByteArray>
#include <QString>

class GenericFile {
public:
    GenericFile() {
        m_length = 0;
        m_address = 0;
        m_file_type = "?";
        m_sector_count = 0;
        m_locked = false;
        m_diskfile = nullptr;
        m_data_loaded = false;
    }
    GenericFile (Dos33DiskImage *image, FileDescriptiveEntry &fde)
        : m_diskfile(image), m_fde(fde)
    {
        m_data_loaded = false;
        m_address = 0;
        updateFromFDE(fde);
    }
    virtual ~GenericFile() { };

    virtual void setAddress(quint16 location) { m_address = location; }
    virtual quint16 address() const { return m_address; };

    virtual void setLength(quint16 length) { m_length = length; }
    virtual int length() const { return m_length; }

    virtual quint8 rawDataAt(int offset) ;
    virtual quint16 rawDataWordAt(int offset);
    virtual QByteArray rawData();


    [[deprecated("Only used for legacy purposes")]]
    virtual quint8 dataAt(int offset) ;
    [[deprecated("Only used for legacy purposes")]]
    virtual quint16 dataWordAt(int offset);
    [[deprecated("Only used for legacy purposes")]]
    /// Returns copy of rawData() unless user data is set.
    virtual QByteArray data();
    [[deprecated("Only used for legacy purposes")]]
    /// Overrides default data (copy of rawData) with user-supplied data
    virtual void setData(QByteArray data);
    [[deprecated("Only used for legacy purposes")]]
    /// Removes user-defined data
    virtual void resetToDefaultData();

    virtual void setFileType(QString type) { m_file_type = type; }
    virtual QString fileType() const {return m_file_type;}

    virtual Dos33DiskImage *diskFile() const { return m_diskfile; }
    virtual void setDiskFile(Dos33DiskImage *diskfile) { m_diskfile = diskfile; }

    virtual void setFilename(QString filename) { m_filename = filename; }
    virtual QString filename() const { return m_filename; }

    virtual void setLocked(bool locked) { m_locked = locked; }
    virtual bool locked() const { return m_locked; }

    virtual void updateFromFDE(FileDescriptiveEntry &fde);

    virtual void setSectorCount(int count) { m_sector_count = count; };
    virtual int sectorCount() const { return m_sector_count; };

    /// Called by rawData() and data() to lazily load data.  Call manually to preload.
    void initDataFromImage();

protected:
    QByteArray peekFirstSector() const;

private:
    Dos33DiskImage * m_diskfile;
    FileDescriptiveEntry m_fde;

    int m_length;
    quint16 m_address;
    QString m_file_type;
    int m_sector_count;
    bool m_locked;

    QString m_filename;
    QByteArray m_data;

    bool m_data_loaded;

    QByteArray m_data_cache;
    QByteArray m_local_data;

};




#endif // GENERICFILE_H
