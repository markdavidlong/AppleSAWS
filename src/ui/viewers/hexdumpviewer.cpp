#include "hexdumpviewer.h"
#include "ui_hexdumpviewer.h"

HexDumpViewer::HexDumpViewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HexDumpViewer)
{
    ui->setupUi(this);
    m_offset = 0;

    QString title = QString("Hex Viewer");
    setWindowTitle(title);
}

HexDumpViewer::~HexDumpViewer()
{
    delete ui;
}

void HexDumpViewer::setFile(GenericFile *file, quint16 offset)
{
    QString title = QString("Hex Viewer: %1").arg(file->filename());
    setWindowTitle(title);

    m_offset = offset;

    QStringList lines;

    QByteArray data = file->data();

    //TODO: Align text from x00 to xFF.  Currently it will start with whatever the offset is.

    quint16 addr = m_offset;
    for (int idx = 0; idx <= data.count()/16; idx++) {
        QString line = QString("%1: ").arg(m_offset+(idx*16),4,16,QChar('0'));
        for (int jdx = (idx*16); jdx < (idx*16)+16; jdx++) {
            addr++;
            if (jdx < data.count()) {
                line += QString(" %1").arg((quint8) data[jdx],2,16,QChar('0'));
                if ((addr % 16) == 0) { line += " "; }
            }
        }
        if (line.length() > 6) {
            lines.append(line);
        }
    }
    setData(qPrintable(lines.join("\n").toUpper()));
}

void HexDumpViewer::setData(QByteArray data)
{
    ui->textArea->setText(data);
}

void HexDumpViewer::setText(QString text)
{
    ui->textArea->setHtml(text);
}
