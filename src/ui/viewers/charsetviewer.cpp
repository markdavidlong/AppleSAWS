#include "charsetviewer.h"

#include <QDebug>

CharSetViewer::CharSetViewer(QWidget *parent) : QWidget(parent)
{
    m_file = Q_NULLPTR;

    QString title = QString("Character Set Viewer");
    setWindowTitle(title);
}

void CharSetViewer::setFile(BinaryFile *file)
{
    m_file = file;
    QString title = QString("Character Set Viewer: %1").arg(file->filename());
    setWindowTitle(title);
    m_data = file->data();
    m_data.resize(file->length());
    m_charset.buildSetFromSetBlob(m_data);

    int xpos = 0;
    int ypos = 0;
    foreach (CharSetCharacter csc, m_charset.allCharacters())
    {
        qDebug() << "CSC: " << csc.asciiVal();
        CharacterWidget *cw = new CharacterWidget(this,csc,4);
        cw->showGrid(true);
        cw->enableBitShift(true);
        cw->setBgColor(Qt::white);
        cw->setFgColor(Qt::black);
        cw->move(xpos,ypos);
        cw->show();
        xpos+= cw->width();
        if (xpos/cw->width() > 11) {
            xpos = 0;
            ypos += cw->height();
        }
    }
}
