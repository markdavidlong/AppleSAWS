#ifndef CHARSETVIEWER_H
#define CHARSETVIEWER_H

#include "binaryfile.h"
#include "characterwidget.h"

#include <QWidget>

class CharSetViewer : public QWidget
{
    Q_OBJECT
public:
    explicit CharSetViewer(QWidget *parent = 0);

    void setFile(BinaryFile *file);

signals:

private:
    BinaryFile *m_file;

    QByteArray m_data;

    CharacterSet m_charset;

};

#endif // CHARSETVIEWER_H
