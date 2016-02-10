#ifndef DISASSEMBLERVIEWER_H
#define DISASSEMBLERVIEWER_H

#include <QWidget>
#include <QByteArray>

#include "binaryfile.h"
namespace Ui {
class DisassemblerViewer;
}

class DisassemblerViewer : public QWidget
{
    Q_OBJECT

public:
    explicit DisassemblerViewer(QWidget *parent = 0);
    ~DisassemblerViewer();

    void setFile(BinaryFile *file);
    void setData(QByteArray data);
    void setText(QString text);

    QString getPotentialLabel(quint16 address);
private:
    Ui::DisassemblerViewer *ui;

    BinaryFile *m_file;
};

#endif // DISASSEMBLERVIEWER_H
