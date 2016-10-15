#ifndef DISASSEMBLERVIEWER_H
#define DISASSEMBLERVIEWER_H

#include <QWidget>
#include <QByteArray>

#include "binaryfile.h"
#include "relocatablefile.h"
#include "fileviewerinterface.h"

namespace Ui {
class DisassemblerViewer;
}

class DisassemblerViewer : public FileViewerInterface
{
    Q_OBJECT

public:
    explicit DisassemblerViewer(QWidget *parent = 0);
    ~DisassemblerViewer();

    void setFile(BinaryFile *file);
    void setFile(RelocatableFile *file);
    void setData(QByteArray data);
    void setText(QString text);

    QString getPotentialLabel(quint16 address);
    virtual bool optionsMenuItems(QMenu *);

public slots:
    void setFile(GenericFile *file);
    void toggleWordWrap(bool enabled);

private:
    Ui::DisassemblerViewer *ui;

    GenericFile *m_file;
};

#endif // DISASSEMBLERVIEWER_H
