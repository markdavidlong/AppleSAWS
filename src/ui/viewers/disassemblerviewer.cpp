#include "disassemblerviewer.h"
#include "ui_disassemblerviewer.h"
#include "disassembler.h"
#include "memory.h"

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

    QList<QStringList> lines = dis.disassemble(file->address(), file->address()+file->length());

    QStringList formattedLines;

    foreach (QStringList line, lines) {
        QString newline = QString("%1:  %2 %3").arg(line[0]).arg(line[1]).arg(line[2]);
        formattedLines.append(newline);
    }

    QByteArray joinedlines = qPrintable(formattedLines.join("\n"));
    setData(joinedlines);
}

void DisassemblerViewer::setData(QByteArray data)
{
    ui->textArea->setText(data);
}

void DisassemblerViewer::setText(QString text)
{
    ui->textArea->setHtml(text);
}