#include <QDebug>
#include "textfile.h"

TextFile::TextFile(Dos33DiskImage *image, FileDescriptiveEntry &fde)
    : GenericFile(image,fde)
{

}


void TextFile::dump()
{
}
