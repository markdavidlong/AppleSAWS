#include "disassemblerviewer.h"
#include "ui_disassemblerviewer.h"
#include "disassembler.h"
#include "memory.h"

#include <QDebug>

DisassemblerViewer::DisassemblerViewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DisassemblerViewer)
{
    ui->setupUi(this);

    QString title = QString("Disassembly Viewer");
    setWindowTitle(title);
}

DisassemblerViewer::~DisassemblerViewer()
{
    delete ui;
}

void DisassemblerViewer::setFile(BinaryFile *file) {
    m_file = file;

    QString title = QString("Disassembler Viewer: %1").arg(m_file->filename());
    setWindowTitle(title);

    quint16 address = file->address();
    Memory mem;
    mem.addFile(file->data(), address);
    Disassembler dis(mem.values());

    QList<DisassembledItem> lines = dis.disassemble(file->address(), file->address()+file->length());

    QStringList formattedLines;

    foreach (DisassembledItem di, lines) {
        QString ds = di.rawDisassembledString();
        if (di.hasArg()) {
            QString potentialLabel = getPotentialLabel(di.arg16());
            if (!potentialLabel.isEmpty()) {
                if (ds.contains("_ARG16_")) { ds.replace("_ARG16_",potentialLabel); }
                else if (ds.contains("_ARG8_")) { ds.replace("_ARG8_",potentialLabel); }
            } else {
                ds = di.disassembledString();
            }
        }
        QString newline = QString("%1:  %2 %3").arg(di.hexAddress()).arg(di.hexString()).arg(ds);
        formattedLines.append(newline);
    }

    QByteArray joinedlines = qPrintable(formattedLines.join("\n"));
    setData(joinedlines);
}

QString DisassemblerViewer::getPotentialLabel(quint16 address) {
    QString retval = QString();

    if (address == 0x03d0) { retval = "DOS_WARMSTART"; }
    else if (address == 0x03d3) { retval = "DOS_COLDSTART"; }
    else if (address == 0x03d6) { retval = "DOS_FILE_MANAGER"; }
    else if (address == 0x03d9) { retval = "DOS_RWTS"; }
    else if (address == 0x03dc) { retval = "DOS_FM_PARAM_LIST_LOCATE"; }
    else if (address == 0x03e3) { retval = "DOS_RWTS_PARAM_LIST_LOCATE"; }
    else if (address == 0x03ea) { retval = "DOS_REPLACE_DOS_INTERCEPTS"; }
    else if (address == 0x03ef) { retval = "DOS_AUTOSTART_BRK_HANDLER"; }
    else if (address == 0x03f2) { retval = "DOS_AUTOSTART_RESET_HANDLER"; }
    else if (address == 0x03f4) { retval = "DOS_POWERUP_BYTE"; }
    else if (address == 0x03f5) { retval = "DOS_AMPERSAND_HANDLER"; }
    else if (address == 0x03f8) { retval = "DOS_CTRL_Y_HANDLER"; }
    else if (address == 0x03fb) { retval = "DOS_NMI_HANDLER"; }
    else if (address == 0x03fe) { retval = "DOS_IRQ_HANDLER"; }

    return retval;
}

void DisassemblerViewer::setData(QByteArray data)
{
    ui->textArea->setText(data);
}

void DisassemblerViewer::setText(QString text)
{
    ui->textArea->setHtml(text);
}
