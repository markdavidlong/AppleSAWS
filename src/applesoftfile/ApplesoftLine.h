#pragma once

#include <QtGlobal>
#include <QList>

class ApplesoftToken;

struct ApplesoftLine {
    qint16 address{0};
    quint16 next_address{0};
    quint16 linenum{0};
    QList<ApplesoftToken> tokens;
};


