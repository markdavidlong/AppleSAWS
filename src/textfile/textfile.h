#ifndef TEXTFILE_H
#define TEXTFILE_H

#include "genericfile.h"

class TextFile : public GenericFile
{
    friend class Dos33DiskImage;

public:

    void dump();

protected:
    TextFile(QByteArray data = QByteArray());
};

#endif // TEXTFILE_H
