/*****************************************************************************
*    AppleSAWS - The Apple Software Analysis WorkShop                        *
*    Copyright (C) 2015-2021  Mark D. Long                                   *
*                                                                            *
*    This program is free software: you can redistribute it and/or modify    *
*    it under the terms of the GNU General Public License as published by    *
*    the Free Software Foundation, either version 3 of the License, or       *
*    (at your option) any later version.                                     *
*                                                                            *
*    This program is distributed in the hope that it will be useful,         *
*    but WITHOUT ANY WARRANTY; without even the implied warranty of          *
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
*    GNU General Public License for more details.                            *
*                                                                            *
*    You should have received a copy of the GNU General Public License       *
*    along with this program.  If not, see <https://www.gnu.org/licenses/>.  *
*****************************************************************************/

#include "hexdumpviewer.h"
#include "ui_hexdumpviewer.h"
#include "applestring.h"
#include "util.h"

#include <QDebug>
#include <QScrollBar>
#include <QSettings>
#include <QAction>
#include <QMenu>
#include <QFontDialog>
#include <QChar>
#include <QTextCursor>

HexDumpViewer::HexDumpViewer(QWidget *parent, int defaultFontSize) :
    FileViewerInterface(parent),
    ui(new Ui::HexDumpViewer)
{
    m_defaultFontSize = defaultFontSize;
    QFont textAreaFont;
 //   textAreaFont.setStyleHint(QFont::Monospace);

  //  if (defaultFontSize > 0) { textAreaFont.setPointSize(10); }

    m_file = Q_NULLPTR;
    ui->setupUi(this);

//    setTextFont(fontFromSettings("HexDumpViewer.textFont", textAreaFont),
//                defaultFontSize);
    m_offset = 0;



    QString title = QString("Hex Viewer");
    setWindowTitle(title);

    QSettings settings;
//    toggleWordWrap(settings.value("HexViewer.WordWrap",true).toBool());
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

    ui->textArea->clear();

    auto doc = ui->textArea->document();
    auto cursor = new QTextCursor(doc);

    QTextCharFormat text40;
    text40.setFontFamily("Print Char 21");
    QTextCharFormat text80;
    text80.setFontFamily("Pr Number 3");


    //TODO: Align text from x00 to xFF.  Currently it will start with whatever the offset is.

    for (int idx = 0; idx <= m_data.count()/16; idx++) {
        if ((idx*16) >= m_data.count()) { break; }

        cursor->setCharFormat(text80);
        QString line = QString("(%1) %2: ")
                .arg(m_offset+(idx*16),4,10,QChar('0'))
                .arg(m_offset+(idx*16),4,16,QChar('0'));
        cursor->insertText(line);

        int posHandled = 0;
        for (int jdx = (idx*16); jdx < (idx*16)+16; jdx++) {
            if (jdx < m_data.count()) {
                posHandled++;
                cursor->setCharFormat(text80);
                cursor->insertText(" ");

                cursor->insertText(
                            QString("%1")
                            .arg((quint8) m_data[jdx],2,16,QChar('0')).toUpper());
                if (posHandled == 8)
                {
                    cursor->setCharFormat(text80);
                    cursor->insertText(" ");
                }
            }
        }

        for (int pos = posHandled; pos < 16; pos++)
            cursor->insertText("   ");

        cursor->setCharFormat(text40);
        cursor->insertText("   ");

        posHandled = 0;
        for (int jdx = (idx*16); jdx < (idx*16)+16; jdx++) {
            if (jdx < m_data.count()) {
                posHandled++;
                auto colorformat = text40;
                if (AppleChar::getAttribute(m_data[jdx]) == TextAttribute::Flash)
                {
                    colorformat.setBackground(QColor(Qt::lightGray));
                    colorformat.setForeground(QColor(Qt::darkBlue));
                }
                else if ((unsigned char) m_data[jdx] >= 0x80 &&
                         (unsigned char) m_data[jdx] < 0x80+0x20)
                {
                    colorformat.setForeground(QColor(Qt::black));
                    colorformat.setBackground(QColor(Qt::green).lighter());
                }
                cursor->setCharFormat(colorformat);
                cursor->insertText(AppleChar::getAppleFontChar(m_data[jdx]));
//                if (posHandled == 8) {
//                    cursor->setCharFormat(text80);
//                    cursor->insertText(" ");
//                    cursor->setCharFormat(text40);
//                }
            }
        }
        cursor->insertText("\n");
    }
    ui->textArea->verticalScrollBar()->setValue(offset);
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

bool HexDumpViewer::optionsMenuItems(QMenu * /*menu*/)
{
    return false;
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



