#ifndef TEXTFILE_H
#define TEXTFILE_H

#include "genericfile.h"

class TextFile : public GenericFile
{
public:
    TextFile(QByteArray data = QByteArray());
    void setData(QByteArray data);

    void dump();

protected:
};

#endif // TEXTFILE_H
