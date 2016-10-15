#include "texthexdumpviewer.h"
#include "ui_texthexdumpviewer.h"
#include "applestring.h"

#include <QVector>
#include <QByteArray>
#include <QSettings>
#include <QMenu>
#include <QAction>

TextHexDumpViewer::TextHexDumpViewer(QWidget *parent) :
    FileViewerInterface(parent),
    ui(new Ui::TextHexDumpViewer)
{
    ui->setupUi(this);
    m_offset = 0;

    QString title = QString("Text/Hex File Viewer");
    setWindowTitle(title);

    QSettings settings;
    toggleWordWrap(settings.value("TexHexViewer.WordWrap",true).toBool());
}

TextHexDumpViewer::~TextHexDumpViewer()
{
    delete ui;
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
    m_offset = offset;
    QByteArray data = file->data();

    QString title = QString("Text/Hex File Viewer: %1").arg(file->filename());
    setWindowTitle(title);

    QStringList outputlines;


    QVector<QByteArray> chunks;

    QByteArray tmpval = data;

    while (tmpval.count()) {
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
    connect(action, SIGNAL(toggled(bool)), SLOT(toggleWordWrap(bool)));
    menu->addAction(action);

    return true;
}
