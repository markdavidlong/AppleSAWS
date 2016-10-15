#include "hexdumpviewer.h"
#include "ui_hexdumpviewer.h"

#include <QDebug>
#include <QScrollBar>
#include <QSettings>
#include <QAction>
#include <QMenu>

HexDumpViewer::HexDumpViewer(QWidget *parent) :
    FileViewerInterface(parent),
    ui(new Ui::HexDumpViewer)
{
    ui->setupUi(this);
    m_offset = 0;

    QString title = QString("Hex Viewer");
    setWindowTitle(title);

    QSettings settings;
    toggleWordWrap(settings.value("HexViewer.WordWrap",true).toBool());
}

HexDumpViewer::~HexDumpViewer()
{
    delete ui;
}

void HexDumpViewer::toggleWordWrap(bool enabled)
{
    if (enabled)
    {
        ui->textArea->setWordWrapMode(QTextOption::WordWrap);
    }
    else
    {
        ui->textArea->setWordWrapMode(QTextOption::NoWrap);
    }
    QSettings settings;
    settings.setValue("HexViewer.WordWrap",enabled);
}





void HexDumpViewer::showHexAndAsciiValues()
{
    int offset = ui->textArea->verticalScrollBar()->value();

    QStringList lines;

    //TODO: Align text from x00 to xFF.  Currently it will start with whatever the offset is.

    quint16 addr = m_offset;
    for (int idx = 0; idx <= m_data.count()/16; idx++) {
        QString line = QString("(%1) %2: ")
                .arg(m_offset+(idx*16),4,10,QChar('0'))
                .arg(m_offset+(idx*16),4,16,QChar('0'));
        QString asciiline;

        for (int jdx = (idx*16); jdx < (idx*16)+16; jdx++) {
            addr++;
            if (jdx < m_data.count()) {
                asciiline += valToAppleAscii(m_data[jdx]);

                line += QString(" %1").arg((quint8) m_data[jdx],2,16,QChar('0'));
            }
        }

        if (line.length() > 14) {
            int diff = (62 - line.length());
            if (diff < 0) { diff = 0; }
            if (diff>0) {
                for (int pdx = 0; pdx < diff; pdx++)
                {
                    line.append("&#160;");
                }
            }
            lines.append(line + "&#160;&#160;" + asciiline);
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
    if (zone == Inverse) { retval = QString("<font color=\"blue\"><b>%1</b></font>").arg(htmlstr); }
    else if (zone == Flash) { retval = QString("<font color=\"green\"><b><i>%1</i></b></font>").arg(htmlstr);}
    else if (zone == AltUC) { retval = QString("<font color=\"red\"><i>%1</i></font>").arg(htmlstr);}
    else /* zone == Normal */ { retval = QString("%1").arg(htmlstr);}

    return retval;
}

void HexDumpViewer::setFile(GenericFile *file, quint16 offset)
{
    QString title = QString("Hex Viewer: %1").arg(file->filename());
    setWindowTitle(title);

    m_offset = offset;

    m_data = file->data();

    showHexAndAsciiValues();

}

bool HexDumpViewer::optionsMenuItems(QMenu *menu)
{
    QSettings settings;

    QAction *action = new QAction("&Word Wrap");
    action->setCheckable(true);
    action->setChecked(settings.value("HexViewer.WordWrap",true).toBool());
    connect(action, SIGNAL(toggled(bool)), SLOT(toggleWordWrap(bool)));
    menu->addAction(action);

    return true;
}

void HexDumpViewer::setData(QByteArray data)
{
    ui->textArea->setHtml(data);
}

void HexDumpViewer::setText(QString text)
{
    ui->textArea->setHtml(text);
}
