#pragma once

#include <QtGlobal>
#include <QVector>

class ApplesoftToken;

struct ApplesoftLine {
    qint16 address;
    quint16 next_address;
    quint16 linenum;
    QVector<ApplesoftToken> tokens;
};


