#include "TextHexDumpViewer.h"
#include "ui_TextHexDumpViewer.h"
#include "AppleString.h"

#include <QList>
#include <QByteArray>
#include <QSettings>
#include <QMenu>
#include <QAction>

TextHexDumpViewer::TextHexDumpViewer(QWidget *parent) :
    FileViewerInterface(parent),
    ui(std::make_unique<Ui::TextHexDumpViewer>())
{
    m_file = Q_NULLPTR;
    ui->setupUi(this);
    QFont textAreaFont;
    textAreaFont.setStyleHint(QFont::Monospace);
    ui->textArea->setFont(textAreaFont);


    m_offset = 0;

    QString title = QString("Text/Hex File Viewer");
    setWindowTitle(title);

    QSettings settings;
    toggleWordWrap(settings.value("TexHexViewer.WordWrap",true).toBool());
}

TextHexDumpViewer::~TextHexDumpViewer()
{
}

void TextHexDumpViewer::toggleWordWrap(bool enabled)
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
    settings.setValue("TextHexViewer.WordWrap",enabled);
}


QString TextHexDumpViewer::makeHexStr(QByteArray data)
{
    QString retval;
    for (int idx = 0; idx < data.length(); idx++)
    {
        retval += QString(" %1").arg((quint8) data[idx],2,16,QChar('0'));
    }
    retval.remove(0,1);

    return retval;
}

QString TextHexDumpViewer::makeTextStr(QByteArray data)
{
    AppleString as;
    as.setData(data);

    QString retval = as.printable();

    retval.replace(QChar(0x7f),QChar(0x25a9));

    for (int idx = 0; idx <= 0x1f; idx++) {
        retval.replace(QChar(idx),QChar(idx+0x2400));
    }

    return retval;
}

void TextHexDumpViewer::setFile(GenericFile *file, quint16 offset)
{
    m_file = file;
    m_offset = offset;
    QByteArray data = file->data();

    QString title = QString("Text/Hex File Viewer: %1").arg(file->filename());
    setWindowTitle(title);

    QStringList outputlines;


    QList<QByteArray> chunks;

    QByteArray tmpval = data;

    while (tmpval.length()) {
        QByteArray tmpchunk = tmpval.left(16);
        tmpval.remove(0,16);
        chunks.append(tmpchunk);
    }

    quint16 addr = 0;
    foreach (QByteArray chunk, chunks) {
        QString hexstr  = makeHexStr(chunk);
//        chunk.replace(0,'.');
        QString textstr = makeTextStr(chunk);
        QString line = QString("%1:  %2      %3").arg(addr,4,16,QChar('0')).arg(hexstr).arg(textstr);

        addr += 16;
        outputlines.append(line);
    }

    setData(qPrintable(outputlines.join("\n").toUpper()));
}

void TextHexDumpViewer::setData(QByteArray data)
{
    ui->textArea->setText(data);
}

void TextHexDumpViewer::setText(QString text)
{
    ui->textArea->setHtml(text);
}

bool TextHexDumpViewer::optionsMenuItems(QMenu *menu)
{
    QSettings settings;

    QAction *action = new QAction("&Word Wrap");
    action->setCheckable(true);
    action->setChecked(settings.value("TexHexViewer.WordWrap",true).toBool());
    connect(action, &QAction::toggled,
            this, &TextHexDumpViewer::toggleWordWrap);
    menu->addAction(action);

    return true;
}

bool TextHexDumpViewer::canPrint() const { return true; }

void TextHexDumpViewer::doPrint()
{
    QPrinter printer;

    QPrintDialog dialog(&printer, this);
    dialog.setWindowTitle(tr("Print Text File Hex Dump"));
    //TODO: Fix for At6
//    if (ui->textArea->textCursor().hasSelection())
//        dialog.addEnabledOption(QAbstractPrintDialog::PrintSelection);
    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    ui->textArea->print(&printer);
}


bool TextHexDumpViewer::canExport() const { return true; }

void TextHexDumpViewer::doExport()
{
    QString defaultPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QDir savename = QDir(defaultPath).filePath(m_file->filename()+".txt");

    QString saveName = QFileDialog::getSaveFileName(this,
       tr("Export Text Hex Dump"), savename.path(), tr("Text Files (*.txt)"));

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

