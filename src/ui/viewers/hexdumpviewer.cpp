#include "hexdumpviewer.h"
#include "ui_hexdumpviewer.h"
#include "util.h"
#include <QDebug>
#include <QScrollBar>
#include <QSettings>
#include <QAction>
#include <QMenu>
#include <QFontDialog>
#include "applestring.h"
#include <QChar>

HexDumpViewer::HexDumpViewer(QWidget *parent) :
    FileViewerInterface(parent),
    ui(new Ui::HexDumpViewer)
{
    QFont textAreaFont;
    textAreaFont.setStyleHint(QFont::Monospace);


    m_file = Q_NULLPTR;
    ui->setupUi(this);

    setTextFont(fontFromSettings("HexDumpViewer.textFont", textAreaFont));
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

//    typedef enum {
//        Inverse,
//        Flash,
//        Normal,
//        AltUC
//    } Zone;

    TextAttribute attribute = AppleChar::getAttribute(val);
    QString charval = AppleChar::printable(val);
    if (val == 0xff) { charval = "?"; } // QChar(0x25a0); }

//    if      (val <= 0x1F)   { val += 0x40;   charval = QString("%1").arg(QChar(val)); zone = Inverse; } //INV UC
//    else if (val <= 0x3F)   {                charval = QString("%1").arg(QChar(val)); zone = Inverse; } // INV SP
//    else if (val <= 0x5F)   {                charval = QString("%1").arg(QChar(val)); zone = Flash; } // FL UC
//    else if (val <= 0x7F)   { val -= 0x40;   charval = QString("%1").arg(QChar(val)); zone = Flash; } // FL SP
//    else if (val <= 0x9F)   { val -= 0x40;   charval = QString("%1").arg(QChar(val)); zone = AltUC; } // NORMx UC
//    else if (val <= 0xBF)   { val -= 0x80;   charval = QString("%1").arg(QChar(val)); zone = Normal; } // NORM SP
//    else if (val <= 0xDF)   { val -= 0x80;   charval = QString("%1").arg(QChar(val)); zone = Normal; } // NORM UC
//    else if (val <  0xFF)   { val -= 0x80;   charval = QString("%1").arg(QChar(val)); zone = Normal; } // NORM LC
//    else /* (val == 0xFF)*/ {                charval = QString("\u25a0"); zone = Normal; }

    QString htmlstr = charval.toHtmlEscaped();

    QString retval;
    if (attribute == Inverse) { retval = QString("<font color=\"blue\"><b>%1</b></font>").arg(htmlstr); }
    else if (attribute == Flash) { retval = QString("<font color=\"green\"><b><i>%1</i></b></font>").arg(htmlstr);}
    else if (attribute == NormalLow) { retval = QString("<font color=\"red\"><i>%1</i></font>").arg(htmlstr);}
    else   { retval = QString("%1").arg(htmlstr);}

    return retval;
}

void HexDumpViewer::setFile(GenericFile *file, quint16 offset)
{
    m_file = file;
    QString title = QString("Hex Viewer: %1").arg(file->filename());
    setWindowTitle(title);

    m_offset = offset;

    m_data = file->data();

    showHexAndAsciiValues();
}

void HexDumpViewer::setRawData(QByteArray data, quint16 offset)
{
    m_offset = offset;
    m_data =  data;
    showHexAndAsciiValues();
}

bool HexDumpViewer::optionsMenuItems(QMenu *menu)
{
    QSettings settings;

    QAction *action = new QAction("&Word Wrap");
    action->setCheckable(true);
    action->setChecked(settings.value("HexViewer.WordWrap",true).toBool());
    connect(action, &QAction::toggled,
            this, &HexDumpViewer::toggleWordWrap);
    menu->addAction(action);

    menu->addSeparator();

    if (m_setFontAction) {
        m_setFontAction = new QAction("Set &Font...");
    }
    menu->addAction(m_setFontAction);

    connect(m_setFontAction, &QAction::triggered,
            this, [this] {
        bool ok;
        QFont font = QFontDialog::getFont(&ok,
                                          ui->textArea->font(),
                                          this, "Set Font",
                                          QFontDialog::MonospacedFonts);
        if (ok) {
            setTextFont(font);
            fontToSettings("HexDumpViewer.textFont", font);
        }

    });

    return true;
}

void HexDumpViewer::setTextFont(const QFont &font)
{
    ui->textArea->setFont(font);
}

void HexDumpViewer::setData(QByteArray data)
{
    ui->textArea->setHtml(data);
}

void HexDumpViewer::setText(QString text)
{
    ui->textArea->setHtml(text);
}

bool HexDumpViewer::canPrint() const { return true; }

void HexDumpViewer::doPrint()
{
    QPrinter printer;

    QPrintDialog dialog(&printer, this);
    dialog.setWindowTitle(tr("Print Hex Dump"));
    if (ui->textArea->textCursor().hasSelection())
        dialog.addEnabledOption(QAbstractPrintDialog::PrintSelection);
    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    ui->textArea->print(&printer);
}


bool HexDumpViewer::canExport() const { return true; }

void HexDumpViewer::doExport()
{
    QString defaultPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QDir savename = QDir(defaultPath).filePath(m_file->filename()+".txt");

    QString saveName = QFileDialog::getSaveFileName(this,
       tr("Export Hex Dump"), savename.path(), tr("Text Files (*.txt)"));

    if (saveName == "") return;  // User cancelled

    qDebug() << "Set filename: " << saveName;

    QFile saveFile(saveName);
    if (!saveFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this,"Save Error","Could not save "+saveName);
        return;
    }

    QTextStream out(&saveFile);
    out << ui->textArea->document()->toPlainText();
    saveFile.close();
}
