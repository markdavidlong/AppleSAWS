#ifndef APPLESOFTLINE_H
#define APPLESOFTLINE_H
#include <Qt>
#include <QVector>

class ApplesoftToken;

struct ApplesoftLine {
    qint16 address;
    quint16 next_address;
    quint16 linenum;
    QVector<ApplesoftToken> tokens;
   // QString detokenized_line;
};


#endif // APPLESOFTLINE_H
