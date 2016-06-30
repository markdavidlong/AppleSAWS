#include "hexdumpviewer.h"
#include "ui_hexdumpviewer.h"

#include <QScrollBar>

HexDumpViewer::HexDumpViewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HexDumpViewer)
{
    ui->setupUi(this);
    m_offset = 0;

    QButtonGroup *bg = new QButtonGroup(this);
    bg->addButton(ui->hexButton);
    bg->addButton(ui->textButton);
    ui->hexButton->setChecked(true);

    connect(ui->hexButton, SIGNAL(clicked(bool)), SLOT(showHexValues()));
    connect(ui->textButton, SIGNAL(clicked(bool)), SLOT(showAsciiValues()));

    QString title = QString("Hex Viewer");
    setWindowTitle(title);
}

HexDumpViewer::~HexDumpViewer()
{
    delete ui;
}

void HexDumpViewer::showHexValues()
{
    int offset = ui->textArea->verticalScrollBar()->value();

    QStringList lines;

    //TODO: Align text from x00 to xFF.  Currently it will start with whatever the offset is.

    quint16 addr = m_offset;
    for (int idx = 0; idx <= m_data.count()/16; idx++) {
        QString line = QString("%1: ").arg(m_offset+(idx*16),4,16,QChar('0'));
        for (int jdx = (idx*16); jdx < (idx*16)+16; jdx++) {
            addr++;
            if (jdx < m_data.count()) {
                line += QString(" %1").arg((quint8) m_data[jdx],2,16,QChar('0'));
                if ((addr % 16) == 0) { line += " "; }
            }
        }
        if (line.length() > 6) {
            lines.append(line);
        }
    }
    setText(qPrintable(lines.join("<br>").toUpper()));
    ui->textArea->verticalScrollBar()->setValue(offset);
}

QString HexDumpViewer::valToAppleAscii(quint8 val)
{

    typedef enum {
        Inverse,
        Flash,
        Normal,
        AltUC
    } Zone;

    Zone zone;

    QString charval;
    if      (val <= 0x1F) { val += 0x40; charval = QString("%1").arg(QChar(val)); zone = Inverse; } //INV UC
    else if (val <= 0x3F) { val = val;   charval = QString("%1").arg(QChar(val)); zone = Inverse; } // INV SP
    else if (val <= 0x5F) { val = val;   charval = QString("%1").arg(QChar(val)); zone = Flash; } // FL UC
    else if (val <= 0x7F) { val -= 0x40; charval = QString("%1").arg(QChar(val)); zone = Flash; } // FL SP
    else if (val <= 0x9F) { val -= 0x40; charval = QString("%1").arg(QChar(val)); zone = AltUC; } // NORMx UC
    else if (val <= 0xBF) { val -= 0x40; charval = QString("%1").arg(QChar(val)); zone = Normal; } // NORM SP
    else if (val <= 0xDF) { val -= 0x80; charval = QString("%1").arg(QChar(val)); zone = Normal; } // NORM UC
    else if (val <  0xFF) { val -= 0x80; charval = QString("%1").arg(QChar(val)); zone = Normal; } // NORM LC
    else if (val == 0xFF) { val = val;   charval = QString("\u25a0"); zone = Normal; }

    QString htmlstr = charval.toHtmlEscaped();

    QString retval;
    if (zone == Inverse) { retval = QString("<b>%1</b>").arg(htmlstr); }
    else if (zone == Flash) { retval = QString("<i>%1</i>").arg(htmlstr);}
    else if (zone == AltUC) { retval = QString("%1").arg(htmlstr);}
    else /* zone == Normal */ { retval = QString("%1").arg(htmlstr);}

    return retval;
}

void HexDumpViewer::showAsciiValues()
{
    int offset = ui->textArea->verticalScrollBar()->value();
    QStringList lines;

    //TODO: Align text from x00 to xFF.  Currently it will start with whatever the offset is.

    quint16 addr = m_offset;
    for (int idx = 0; idx <= m_data.count()/16; idx++) {
        QString line = QString("%1: ").arg(m_offset+(idx*16),4,16,QChar('0'));

        for (int jdx = (idx*16); jdx < (idx*16)+16; jdx++) {
            addr++;
            if (jdx < m_data.count()) {

                //line += QString(" %1").arg((quint8) m_data[jdx],2,16,QChar('0'));
                line += valToAppleAscii(m_data[jdx]);

               // if ((addr % 16) == 0) { line += " "; }
            }
        }
        if (line.length() > 6) {
            lines.append(line);
        }
    }
    setText(qPrintable(lines.join("<br>")));
    ui->textArea->verticalScrollBar()->setValue(offset);
}

void HexDumpViewer::setFile(GenericFile *file, quint16 offset)
{
    QString title = QString("Hex Viewer: %1").arg(file->filename());
    setWindowTitle(title);

    m_offset = offset;

    m_data = file->data();

    showHexValues();

}

void HexDumpViewer::setData(QByteArray data)
{
    ui->textArea->setHtml(data);
}

void HexDumpViewer::setText(QString text)
{
    ui->textArea->setHtml(text);
}
