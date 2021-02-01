#include "disassemblerviewer.h"
#include "ui_disassemblerviewer.h"
#include "disassembler.h"
#include "memory.h"
#include "util.h"
#include "relocatablefile.h"
#include "opcodes.h"


#include <QSettings>
#include <QMenu>
#include <QAction>
#include <QDebug>
#include <QFontDialog>


DisassemblerViewer::DisassemblerViewer(QWidget *parent) :
    FileViewerInterface(parent),
    ui(new Ui::DisassemblerViewer)
{
    ui->setupUi(this);
    QFont textAreaFont;
    textAreaFont.setStyleHint(QFont::Monospace);


    m_isRelo = false;
    m_dmd = Q_NULLPTR;
    m_wordWrapAction = Q_NULLPTR;
    m_showMetadataAction = Q_NULLPTR;
    m_setFontAction = Q_NULLPTR;


    QString title = QString("Disassembly Viewer");
    setWindowTitle(title);

    QSettings settings;
    toggleWordWrap(settings.value("DisassemblerViewer.WordWrap",true).toBool());

    setTextFont(fontFromSettings("DisassemblerViewer.textFont", textAreaFont));
}

DisassemblerViewer::~DisassemblerViewer()
{
    delete ui;
}

void DisassemblerViewer::setTextFont(const QFont &font)
{
    ui->textArea->setFont(font);
}
void DisassemblerViewer::setFile(GenericFile *file)
{

    if (dynamic_cast<RelocatableFile*>(file))
    {
        setFile(dynamic_cast<RelocatableFile*>(file));
    }
    if (dynamic_cast<BinaryFile*>(file))
    {
        setFile(dynamic_cast<BinaryFile*>(file));
    }
}

void DisassemblerViewer::toggleWordWrap(bool enabled)
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
    settings.setValue("DisassemblerViewer.WordWrap",enabled);
}

void DisassemblerViewer::setFile(BinaryFile *file) {
    m_file = file;
    m_isRelo = false;

    setCursor(Qt::WaitCursor);
    m_bfm = new BinaryFileMetadata(m_file, file->address(), this);

    connect(m_bfm, &BinaryFileMetadata::doDisassemble,
            this, &DisassemblerViewer::handleDisassembleRequest);

    QString title = QString("Disassembler Viewer: %1").arg(m_file->filename());
    setWindowTitle(title);

    quint16 address = file->address();

    m_mem.addFile(m_file->data(), address);

    QList<quint16> addresses = m_bfm->entryPoints()->getEntryPointAddresses();
    if (!addresses.count()) { addresses.append(address); }
    handleDisassembleRequest(addresses);
    unsetCursor();
}

void DisassemblerViewer::setFile(RelocatableFile *file) {
    m_file = file;
    m_isRelo = true;

    setCursor(Qt::WaitCursor);

    m_bfm = new BinaryFileMetadata(m_file, file->address() + 6, this);

    connect(m_bfm, &BinaryFileMetadata::doDisassemble,
            this, &DisassemblerViewer::handleDisassembleRequest);

    QString title = QString("Disassembler Viewer: %1 (Relocatable)").arg(m_file->filename());
    setWindowTitle(title);

    quint16 address = file->address() + 6 ; // Handle offset for relocatable metadata
    m_mem.addFile(m_file->data(), address);

    QList<quint16> addresses = m_bfm->entryPoints()->getEntryPointAddresses();
    if (!addresses.count()) { addresses.append(address); }
    handleDisassembleRequest(addresses);
    unsetCursor();
}

void DisassemblerViewer::handleDisassembleRequest(QList<quint16> addresses)
{
    QStringList strings;

    disassemble(addresses);
    strings += getDisassemblyStrings();
    // qSort(strings);
    strings.sort();
    strings.removeDuplicates();

    if (m_isRelo)
    {
        QByteArray joinedlines = qPrintable(strings.join("\n"));
        QStringList rd = (dynamic_cast<RelocatableFile*>(m_file))->decodeRelocatableDict();
        QByteArray rdlines = qPrintable(rd.join("\n"));
        ui->textArea->clear();
        setData(joinedlines + "\n\n== Relocation Dictionary ==\n\n" + rdlines);
    }
    else
    {
        QByteArray joinedlines = qPrintable(strings.join("\n"));
        ui->textArea->clear();
        setData(joinedlines);
    }
}


QStringList DisassemblerViewer::getDisassemblyStrings() {
    return m_disassemblyStrings;
}

void DisassemblerViewer::disassemble(QList<quint16> entryPoints) {

    //Disassembler dis(m_mem.values());
    Disassembler dis(m_mem.getAllValues());
    int length = m_file->length();
    qDebug() << "DV: from: << " << m_file->address() << " to " << length;
    int end = m_file->address()+length;
    if (end > 0xffff) { end = 0xffff; }

    QList<DisassembledItem> lines = dis.disassemble(m_file->address(),
                                                    end,
                                                    entryPoints);
    dis.setUnknownToData(m_file->address(),m_file->address()+length);
    m_jumpLines = dis.getJumpLines();

    QStringList formattedLines;

    foreach (DisassembledItem di, lines) {
        QString ds = di.rawDisassembledString();
        if (di.hasArg()) {
            QString potentialLabel = getPotentialLabel(di.arg16());
            if (!potentialLabel.isEmpty()) {
                if (ds.contains("_ARG16_")) { ds.replace("_ARG16_",potentialLabel); }
                if (ds.contains("_ARG8_")) { ds.replace("_ARG8_",potentialLabel); }
            } else {
                ds = di.disassembledString();
            }
        }
        QString raw = di.rawDisassembledString();
        if (raw.contains("RTS")) { ds += "\n"; }  //TODO: Temp for spacing
        if (raw.contains("JMP")) { ds += "\n"; }  //TODO: Temp for spacing
        if (raw.contains("???")) { ds += "\n"; }  //TODO: Temp for spacing
        if (raw.contains("BRA")) { ds += "\n"; }  //TODO: Temp for spacing
        if (raw.contains("BRK")) { ds += "\n"; }  //TODO: Temp for spacing

        QString newline;

        if (m_bfm->assemblerSymbols()->hasAssemSymbolAtAddress(di.address()))
        {
            int loc = m_bfm->assemblerSymbols()->locationOfSymbolAtAddress(di.address());
            newline += (QString("%1: [%2]\n       %3 %4")
                        .arg(di.hexAddress())
                        .arg(m_bfm->assemblerSymbols()->at(loc).name)
                        .arg(di.hexString()).arg(ds)
                        );
        }
        else
        {
            newline += QString("%1:  %2 %3").arg(di.hexAddress()).arg(di.hexString()).arg(ds);
        }
        formattedLines.append(newline);
    }

    for (int idx = m_file->address(); idx < m_file->address()+length; idx++)
    {
        if (dis.memoryUsageMap()->at(idx).testFlag(Data) ||
                dis.memoryUsageMap()->at(idx).testFlag(Unknown))
        {
            QString newline;
            bool usedefault = false;

            if (m_bfm->assemblerSymbols()->hasAssemSymbolAtAddress(idx))
            {
                int loc = m_bfm->assemblerSymbols()->locationOfSymbolAtAddress(idx);
                if (m_bfm->assemblerSymbols()->at(loc).symbolsize == SizeByte)
                {
                    newline = QString("%1: .Byte $%2                 ; %3").arg(uint16ToHex(idx))
                            .arg(uint8ToHex(m_mem.at(idx)))
                            .arg(m_bfm->assemblerSymbols()->at(loc).name);;
                }
                else if (m_bfm->assemblerSymbols()->at(loc).symbolsize == SizeWord)
                {
                    newline = QString("%1: .Word $%2               ; %3").arg(uint16ToHex(idx))
                            .arg(uint16ToHex(m_mem.at(idx) + (m_mem.at(idx+1)*256)))
                            .arg(m_bfm->assemblerSymbols()->at(loc).name);
                    idx++;
                }
                else
                {
                    usedefault = true;
                }
            }
            else
            {
                usedefault = true;
            }

            if (usedefault)
            {
                newline += QString("%1:  %2                       %3\t(%4)\t'%5'").arg(uint16ToHex(idx))
                        .arg(uint8ToHex(m_mem.at(idx)))
                        .arg(makeDescriptorStringForVal(m_mem.at(idx)))
                        .arg(OpCodes::getMnemonic(m_mem.at(idx)))
                        .arg(AppleChar::printable(m_mem.at(idx)));
            }
            formattedLines.append(newline);
        }
    }
    //qSort(formattedLines);
    formattedLines.sort();

    m_disassemblyStrings = formattedLines;
}



QString DisassemblerViewer::getPotentialLabel(quint16 address)
{
    if (m_bfm)
    {
        if (m_bfm->assemblerSymbols()->hasAssemSymbolAtAddress(address))
        {
            return m_bfm->assemblerSymbols()->getSymbolAtAddress(address);
        }
    }
    if (address == 0x20) { return "MON.WINDOW_LEFT"; }
    if (address == 0x21) { return "MON.WINDOW_WIDTH"; }
    if (address == 0x22) { return "MON.WINDOW_TOP"; }
    if (address == 0x23) { return "MON.WINDOW_BOTTOM"; }

    if (address == 0x24) { return "MON.CURSORHORIZ"; }
    if (address == 0x25) { return "MON.CURSORVERT"; }

    if (address == 0x28) { return "MON.BASL"; }
    if (address == 0x29) { return "MON.BASH"; }

    if (address == 0x2A) { return "DOS.SCRATCH"; }
    if (address == 0x2B) { return "DOS.BOOT_SLOT"; }

    if (address == 0x2C) { return "DOS.RWTS_CKSUM"; }
    if (address == 0x2D) { return "DOS.RWTS_SECTOR"; }
    if (address == 0x2E) { return "DOS.RWTS_TRACK"; }
    if (address == 0x2F) { return "DOS.RWTS_VOLUME"; }

    if (address == 0x30) { return "MON.HIRES_TMPBM"; }

    if (address == 0x32) { return "MON.VIDEO_MODE"; }
    if (address == 0x33) { return "MON.PROMPTCHAR"; }
    if (address == 0x36) { return "DOS.CSWL"; }
    if (address == 0x37) { return "DOS.CSWH"; }
    if (address == 0x38) { return "DOS.KSWL"; }
    if (address == 0x39) { return "DOS.KSWH"; }
    if (address == 0x40) { return "DOS.FILE_BUFFER_L"; }
    if (address == 0x41) { return "DOS.FILE_BUFFER_H"; }
    if (address == 0x42) { return "DOS.BUFFER_ADDR_L"; }
    if (address == 0x43) { return "DOS.BUFFER_ADDR_H"; }
    if (address == 0x44) { return "DOS.NUMERIC_OPERAND_L"; }
    if (address == 0x45) { return "DOS.NUMERIC_OPERAND_H"; }

    if (address == 0x48) { return "DOS.RWTS_IOB_PTR_L"; }
    if (address == 0x49) { return "DOS.RWRS_IOB_PTR_H"; }


    if (address == 0x67) { return "AS.PROG_STARTL"; }
    if (address == 0x68) { return "AS.PROG_STARTH"; }

    if (address == 0x69) { return "AS.VAR_STARTL"; }
    if (address == 0x6A) { return "AS.VAR_STARTH"; }

    if (address == 0x6B) { return "AS.ARRAY_STARTL"; }
    if (address == 0x6C) { return "AS.ARRAY_STARTH"; }

    if (address == 0x6D) { return "AS.NUMSTORE_ENDL"; }
    if (address == 0x6E) { return "AS.NUMSTORE_ENDH"; }

    if (address == 0x6F) { return "AS.STRING_STARTL"; }
    if (address == 0x70) { return "AS.STRING_STARTH"; }

    if (address == 0x71) { return "AS.PTR_9L"; }
    if (address == 0x72) { return "AS.PTR_9H"; }

    if (address == 0x73) { return "AS.HIMEM_L"; }
    if (address == 0x74) { return "AS.HIMEM_H"; }

    if (address == 0x75) { return "AS.CURR_LINENUM_L"; }
    if (address == 0x76) { return "AS.CURR_LINENUM_H"; }

    if (address == 0x77) { return "AS.INTR_LINENUM_L"; }
    if (address == 0x78) { return "AS.INTR_LINENUM_H"; }

    if (address == 0x79) { return "AS.NEXT_STATEMENT_L"; }
    if (address == 0x7A) { return "AS.NEXT_STATEMENT_H"; }

    if (address == 0x7B) { return "AS.DATA_LINENUM_L"; }
    if (address == 0x7C) { return "AS.DATA_LINENUM_H"; }

    if (address == 0x7D) { return "AS.DATA_ADDR_L"; }
    if (address == 0x7E) { return "AS.DATA_ADDR_H"; }

    if (address == 0x7F) { return "AS.INPUT_SRC_L"; }
    if (address == 0x80) { return "AS.INPUT_SRC_H"; }

    if (address == 0x81) { return "AS.LAST_VARNAME_L"; }
    if (address == 0x82) { return "AS.LAST_VARNAME_H"; }

    if (address == 0x83) { return "AS.LAST_VARVAL_L"; }
    if (address == 0x84) { return "AS.LAST_VARVAL_H"; }

    if (address == 0xAF) { return "AS.PROGEND_L"; }
    if (address == 0xB0) { return "AS.PROGEND_H"; }

    if (address == 0xD6) { return "DOS.AS.LOCK"; }
    if (address == 0xD8) { return "DOS.AS.ONERR"; }

    if (address == 0xDA) { return "DOS.AS.ONERR_LINE_L"; }
    if (address == 0xDB) { return "DOS.AS.ONERR_LINE_H"; }

    if (address == 0xE0) { return "AS.HGR_X_L"; }
    if (address == 0xE1) { return "AS.HGR_X_H"; }
    if (address == 0xE2) { return "AS.HGR_Y"; }

    if (address == 0xE4) { return "AS.HGR_COLOR"; }
    if (address == 0xE5) { return "AS.HGR_MAP_BYTE"; }
    if (address == 0xE6) { return "AS.HGR_PLOT_PAGE"; }
    if (address == 0xE7) { return "AS.HGR_SCALE"; }

    if (address == 0xE8) { return "AS.SHAPETBL_L"; }
    if (address == 0xE9) { return "AS.SHAPETBL_H"; }

    if (address == 0xEA) { return "AS.HGR_COLLISION_CTR"; }

    if (address == 0xF0) { return "AS.LORES_SCRATCH"; }
    if (address == 0xF1) { return "AS.255_MINUS_SPEED"; }
    if (address == 0xF2) { return "AS.TRACE_FLAG"; }
    if (address == 0xF3) { return "MON.FLASH_MASK"; }
    if (address == 0xF4) { return "AS.ONERR_PTR_AREA_0"; }
    if (address == 0xF5) { return "AS.ONERR_PTR_AREA_1"; }
    if (address == 0xF6) { return "AS.ONERR_PTR_AREA_2"; }
    if (address == 0xF7) { return "AS.ONERR_PTR_AREA_3"; }
    if (address == 0xF8) { return "AS.ONERR_PTR_AREA_4"; }

    if (address == 0xF9) { return "AS.HGR_ROT"; }

    if (address == 0x03d0) { return "DOS.WARMSTART"; }
    if (address == 0x03d3) { return "DOS.COLDSTART"; }
    if (address == 0x03d6) { return "DOS.FILE_MANAGER"; }
    if (address == 0x03d9) { return "DOS.RWTS"; }
    if (address == 0x03dc) { return "DOS.FM_PARAM_LIST_LOCATE"; }
    if (address == 0x03e3) { return "DOS.RWTS_PARAM_LIST_LOCATE"; }
    if (address == 0x03ea) { return "DOS.REPLACE_DOS_INTERCEPTS"; }
    if (address == 0x03ef) { return "DOS.AUTOSTART_BRK_HANDLER"; }
    if (address == 0x03f2) { return "DOS.AUTOSTART_RESET_HANDLER"; }
    if (address == 0x03f4) { return "DOS.POWERUP_BYTE"; }
    if (address == 0x03f5) { return "AMPR_VEC"; }
    if (address == 0x03f8) { return "MON.CTRL_Y_VEC"; }
    if (address == 0x03fb) { return "DOS.NMI_HANDLER"; }
    if (address == 0x03fe) { return "DOS.IRQ_HANDLER"; }

    if (address == 0x0400) { return "MON.LINE1"; }

    if (address == 0x07f8) { return "MON.MSLOT"; }
    if (address == 0xc000) { return "KEYBOARD"; }

    if (address == 0xc001) { return "M80_SET80COL"; }
    if (address == 0xc002) { return "M80_RDMAINRAM"; }
    if (address == 0xc003) { return "M80_RDCARDRAM"; }
    if (address == 0xc004) { return "M80_WRMAINRAM"; }
    if (address == 0xc005) { return "M80_WRCARDRAM"; }

    if (address == 0xc006) { return "MON.SETSLOTCXROM"; }
    if (address == 0xc007) { return "MON.SETINTCXROM"; }

    if (address == 0xc008) { return "M80_SETSTDSZ"; }
    if (address == 0xc009) { return "M80_SETALTZP"; }
    if (address == 0xc00A) { return "M80_SETINTC3ROM"; }
    if (address == 0xc00B) { return "M80_SETSLOTC3ROM"; }
    if (address == 0xc00C) { return "M80_CLR80VID"; }
    if (address == 0xc00D) { return "M80_SET80VID"; }
    if (address == 0xc00E) { return "M80_CLRALTCHAR"; }
    if (address == 0xc00F) { return "M80_SETALTCHAR"; }

    if (address == 0xc010) { return "MON.KBDSTRB"; }

    if (address == 0xc011) { return "M80_RDLCBNK2"; }
    if (address == 0xc012) { return "M80_RDLCRAM"; }
    if (address == 0xc013) { return "M80_RDRAMRD"; }
    if (address == 0xc014) { return "M80_RDRAMWRT"; }

    if (address == 0xc015) { return "RDCXROM"; }
    if (address == 0xc016) { return "RDALTZP"; }
    if (address == 0xc017) { return "RDC3ROM"; }

    if (address == 0xc018) { return "MON.RD80STORE"; }

    if (address == 0xc019) { return "RDVBLBAR"; }
    if (address == 0xc01A) { return "RDTEXT"; }
    if (address == 0xc01B) { return "RDMIXED"; }

    if (address == 0xc01c) { return "RDPAGE2"; }
    if (address == 0xc01D) { return "RDHIRES"; }
    if (address == 0xc01E) { return "RDALTCHAR"; }

    if (address == 0xc01f) { return "RD80VID"; }

    if (address == 0xc020) { return "TAPEOUT"; }

    if (address == 0xc030) { return "SPKR"; }

    if (address == 0xc040) { return "STROBE"; }

    if (address == 0xc050) { return "SW.TXTCLR"; }
    if (address == 0xc051) { return "SW.TXTSET"; }
    if (address == 0xc052) { return "SW.MIXCLR"; }
    if (address == 0xc053) { return "SW.MIXSET"; }
    if (address == 0xc054) { return "SW.LOWSCR"; }
    if (address == 0xc055) { return "SW.HISCR"; }
    if (address == 0xc056) { return "SW.LORES"; }
    if (address == 0xc057) { return "SW.HIRES"; }
    if (address == 0xc058) { return "SW.SETAN0"; }
    if (address == 0xc059) { return "SW.CLRAN0"; }
    if (address == 0xc05a) { return "SW.SETAN1"; }
    if (address == 0xc05b) { return "SW.CLRAN1"; }
    if (address == 0xc05c) { return "SW.SETAN2"; }
    if (address == 0xc05d) { return "SW.CLRAN2"; }
    if (address == 0xc05e) { return "SW.SETAN3"; }
    if (address == 0xc05f) { return "SW.CLRAN3"; }
    if (address == 0xc060) { return "MON.TAPEIN"; }
    if (address == 0xc064) { return "MON.PADDL0"; }
    if (address == 0xc070) { return "MON.PTRIG"; }

    if (address == 0xc080) { return "RDRAMBANK2_NOWRITE"; }
    if (address == 0xc081) { return "RDROM_WRBANK2"; }
    if (address == 0xc082) { return "RDROM_NOWRITE"; }
    if (address == 0xc083) { return "RDWRBANK2"; }
    if (address == 0xc084) { return "RDRAM_NOWRITE"; }
    if (address == 0xc085) { return "READROM_WRBANK2"; }
    if (address == 0xc086) { return "RDROM_NOWRITE"; }
    if (address == 0xc087) { return "RDWRBANK2"; }
    if (address == 0xc088) { return "RDBANK1_NOWRITE"; }
    if (address == 0xc089) { return "RDROM_WRBANK1"; }
    if (address == 0xc08A) { return "RDROM_NOWRITE"; }
    if (address == 0xc08B) { return "RDWRBANK1"; }
    if (address == 0xc08C) { return "RDBANK1_NOWRITE"; }
    if (address == 0xc08D) { return "RDROM_WRBANK1"; }
    if (address == 0xc08E) { return "RDROM_NOWRITE"; }
    if (address == 0xc08F) { return "RDWR_BANK1"; }

    if (address == 0xc090) { return "SLOT1_ROM0"; }
    if (address == 0xc091) { return "SLOT1_ROM1"; }
    if (address == 0xc092) { return "SLOT1_ROM2"; }
    if (address == 0xc093) { return "SLOT1_ROM3"; }
    if (address == 0xc094) { return "SLOT1_ROM4"; }
    if (address == 0xc095) { return "SLOT1_ROM5"; }
    if (address == 0xc096) { return "SLOT1_ROM6"; }
    if (address == 0xc097) { return "SLOT1_ROM7"; }
    if (address == 0xc098) { return "SLOT1_ROM8"; }
    if (address == 0xc099) { return "SLOT1_ROM9"; }
    if (address == 0xc09A) { return "SLOT1_ROMA"; }
    if (address == 0xc09B) { return "SLOT1_ROMB"; }
    if (address == 0xc09C) { return "SLOT1_ROMC"; }
    if (address == 0xc09D) { return "SLOT1_ROMD"; }
    if (address == 0xc09E) { return "SLOT1_ROME"; }
    if (address == 0xc09F) { return "SLOT1_ROMF"; }

    if (address == 0xc0A0) { return "SLOT2_ROM0"; }
    if (address == 0xc0A1) { return "SLOT2_ROM1"; }
    if (address == 0xc0A2) { return "SLOT2_ROM2"; }
    if (address == 0xc0A3) { return "SLOT2_ROM3"; }
    if (address == 0xc0A4) { return "SLOT2_ROM4"; }
    if (address == 0xc0A5) { return "SLOT2_ROM5"; }
    if (address == 0xc0A6) { return "SLOT2_ROM6"; }
    if (address == 0xc0A7) { return "SLOT2_ROM7"; }
    if (address == 0xc0A8) { return "SLOT2_ROM8"; }
    if (address == 0xc0A9) { return "SLOT2_ROM9"; }
    if (address == 0xc0AA) { return "SLOT2_ROMA"; }
    if (address == 0xc0AB) { return "SLOT2_ROMB"; }
    if (address == 0xc0AC) { return "SLOT2_ROMC"; }
    if (address == 0xc0AD) { return "SLOT2_ROMD"; }
    if (address == 0xc0AE) { return "SLOT2_ROME"; }
    if (address == 0xc0AF) { return "SLOT2_ROMF"; }

    if (address == 0xc0B0) { return "SLOT3_ROM0"; }
    if (address == 0xc0B1) { return "SLOT3_ROM1"; }
    if (address == 0xc0B2) { return "SLOT3_ROM2"; }
    if (address == 0xc0B3) { return "SLOT3_ROM3"; }
    if (address == 0xc0B4) { return "SLOT3_ROM4"; }
    if (address == 0xc0B5) { return "SLOT3_ROM5"; }
    if (address == 0xc0B6) { return "SLOT3_ROM6"; }
    if (address == 0xc0B7) { return "SLOT3_ROM7"; }
    if (address == 0xc0B8) { return "SLOT3_ROM8"; }
    if (address == 0xc0B9) { return "SLOT3_ROM9"; }
    if (address == 0xc0BA) { return "SLOT3_ROMA"; }
    if (address == 0xc0BB) { return "SLOT3_ROMB"; }
    if (address == 0xc0BC) { return "SLOT3_ROMC"; }
    if (address == 0xc0BD) { return "SLOT3_ROMD"; }
    if (address == 0xc0BE) { return "SLOT3_ROME"; }
    if (address == 0xc0BF) { return "SLOT3_ROMF"; }

    if (address == 0xc0C0) { return "SLOT4_ROM0"; }
    if (address == 0xc0C1) { return "SLOT4_ROM1"; }
    if (address == 0xc0C2) { return "SLOT4_ROM2"; }
    if (address == 0xc0C3) { return "SLOT4_ROM3"; }
    if (address == 0xc0C4) { return "SLOT4_ROM4"; }
    if (address == 0xc0C5) { return "SLOT4_ROM5"; }
    if (address == 0xc0C6) { return "SLOT4_ROM6"; }
    if (address == 0xc0C7) { return "SLOT4_ROM7"; }
    if (address == 0xc0C8) { return "SLOT4_ROM8"; }
    if (address == 0xc0C9) { return "SLOT4_ROM9"; }
    if (address == 0xc0CA) { return "SLOT4_ROMA"; }
    if (address == 0xc0CB) { return "SLOT4_ROMB"; }
    if (address == 0xc0CC) { return "SLOT4_ROMC"; }
    if (address == 0xc0CD) { return "SLOT4_ROMD"; }
    if (address == 0xc0CE) { return "SLOT4_ROME"; }
    if (address == 0xc0CF) { return "SLOT4_ROMF"; }

    if (address == 0xc0D0) { return "SLOT5_ROM0"; }
    if (address == 0xc0D1) { return "SLOT5_ROM1"; }
    if (address == 0xc0D2) { return "SLOT5_ROM2"; }
    if (address == 0xc0D3) { return "SLOT5_ROM3"; }
    if (address == 0xc0D4) { return "SLOT5_ROM4"; }
    if (address == 0xc0D5) { return "SLOT5_ROM5"; }
    if (address == 0xc0D6) { return "SLOT5_ROM6"; }
    if (address == 0xc0D7) { return "SLOT5_ROM7"; }
    if (address == 0xc0D8) { return "SLOT5_ROM8"; }
    if (address == 0xc0D9) { return "SLOT5_ROM9"; }
    if (address == 0xc0DA) { return "SLOT5_ROMA"; }
    if (address == 0xc0DB) { return "SLOT5_ROMB"; }
    if (address == 0xc0DC) { return "SLOT5_ROMC"; }
    if (address == 0xc0DD) { return "SLOT5_ROMD"; }
    if (address == 0xc0DE) { return "SLOT5_ROME"; }
    if (address == 0xc0DF) { return "SLOT5_ROMF"; }

    if (address == 0xc0E0) { return "SLOT6_ROM0"; }
    if (address == 0xc0E1) { return "SLOT6_ROM1"; }
    if (address == 0xc0E2) { return "SLOT6_ROM2"; }
    if (address == 0xc0E3) { return "SLOT6_ROM3"; }
    if (address == 0xc0E4) { return "SLOT6_ROM4"; }
    if (address == 0xc0E5) { return "SLOT6_ROM5"; }
    if (address == 0xc0E6) { return "SLOT6_ROM6"; }
    if (address == 0xc0E7) { return "SLOT6_ROM7"; }
    if (address == 0xc0E8) { return "SLOT6_ROM8"; }
    if (address == 0xc0E9) { return "SLOT6_ROM9"; }
    if (address == 0xc0EA) { return "SLOT6_ROMA"; }
    if (address == 0xc0EB) { return "SLOT6_ROMB"; }
    if (address == 0xc0EC) { return "SLOT6_ROMC"; }
    if (address == 0xc0ED) { return "SLOT6_ROMD"; }
    if (address == 0xc0EE) { return "SLOT6_ROME"; }
    if (address == 0xc0EF) { return "SLOT6_ROMF"; }

    if (address == 0xc0F0) { return "SLOT7_ROM0"; }
    if (address == 0xc0F1) { return "SLOT7_ROM1"; }
    if (address == 0xc0F2) { return "SLOT7_ROM2"; }
    if (address == 0xc0F3) { return "SLOT7_ROM3"; }
    if (address == 0xc0F4) { return "SLOT7_ROM4"; }
    if (address == 0xc0F5) { return "SLOT7_ROM5"; }
    if (address == 0xc0F6) { return "SLOT7_ROM6"; }
    if (address == 0xc0F7) { return "SLOT7_ROM7"; }
    if (address == 0xc0F8) { return "SLOT7_ROM8"; }
    if (address == 0xc0F9) { return "SLOT7_ROM9"; }
    if (address == 0xc0FA) { return "SLOT7_ROMA"; }
    if (address == 0xc0FB) { return "SLOT7_ROMB"; }
    if (address == 0xc0FC) { return "SLOT7_ROMC"; }
    if (address == 0xc0FD) { return "SLOT7_ROMD"; }
    if (address == 0xc0FE) { return "SLOT7_ROME"; }
    if (address == 0xc0FF) { return "SLOT7_ROMF"; }

    if (address == 0xc100) { return "M80_BFUNCPG"; }
    if (address == 0xc107) { return "M80_B.FUNCK"; }
    if (address == 0xc10E) { return "M80_B.FUNCNE"; }
    if (address == 0xc11f) { return "M80_B.OLDFUNC"; }
    if (address == 0xc129) { return "M80_F.CLREOP"; }
    if (address == 0xc12d) { return "M80_CLEOP1"; }
    if (address == 0xc143) { return "M80_F.HOME"; }
    if (address == 0xc14d) { return "M80_F.SCROLL"; }
    if (address == 0xc153) { return "M80_SCRL1"; }
    if (address == 0xc169) { return "M80_SCRL2"; }
    if (address == 0xc172) { return "M80_SCRL3"; }
    if (address == 0xc17d) { return "M80_F.CLREOL"; }
    if (address == 0xc181) { return "M80_CLEOL2"; }
    if (address == 0xc18a) { return "M80_F.SETWND"; }
    if (address == 0xc19c) { return "M80_F.CLEOLZ"; }
    if (address == 0xc1a1) { return "M80_F.GORET"; }
    if (address == 0xc1a4) { return "M80_B.FUNCO"; }
    if (address == 0xc1c5) { return "M80_NOI"; }
    if (address == 0xc1cd) { return "M80_B.SCROLL"; }
    if (address == 0xc1d3) { return "M80_B.CLREOL"; }
    if (address == 0xc1d9) { return "M80_B.CLEOLZ"; }
    if (address == 0xc1e1) { return "M80_B.CLREOP"; }
    if (address == 0xc1e7) { return "M80_B.SETWND"; }
    if (address == 0xc1ea) { return "M80_B.RESET"; }
    if (address == 0xc1ed) { return "M80_B.HOME"; }
    if (address == 0xc1ff) { return "M80_B.VECTOR"; }
    if (address == 0xc20e) { return "M80_B.GETCH"; }
    if (address == 0xc211) { return "M80_B.FUNC1"; }
    if (address == 0xc219) { return "M80_B.SETWNDX"; }
    if (address == 0xc221) { return "M80_B.SETWND2"; }
    if (address == 0xc22e) { return "M80_GOBACK"; }
    if (address == 0xc234) { return "M80_B.RESETX"; }
    if (address == 0xc252) { return "M80_BLAST"; }
    if (address == 0xc261) { return "M80_DIAGS"; }
    if (address == 0xc264) { return "M80_RESETRET"; }
    if (address == 0xc26e) { return "M80_B.ESCFIX"; }
    if (address == 0xc272) { return "M80_B.ESCFIX2"; }
    if (address == 0xc27a) { return "M80_B.ESCFIX3"; }
    if (address == 0xc27d) { return "M80_GORETN"; }
    if (address == 0xc280) { return "M80_ESCIN"; }
    if (address == 0xc284) { return "M80_ESCOUT"; }
    if (address == 0xc288) { return "M80_B.KEYIN"; }
    if (address == 0xc29c) { return "M80_B.KEYIN2"; }
    if (address == 0xc2b5) { return "M80_GOTKEY"; }
    if (address == 0xc2c6) { return "M80_KEYDLY"; }
    if (address == 0xc2cc) { return "M80_IK1"; }
    if (address == 0xc2ce) { return "M80_IK2"; }
    if (address == 0xc2d5) { return "M80_IK2A"; }
    if (address == 0xc2db) { return "M80_IK3"; }
    if (address == 0xc2e6) { return "M80_KDRETN"; }
    if (address == 0xc2e9) { return "M80_KDRETY"; }
    if (address == 0xc2ea) { return "M80_KDRET"; }
    if (address == 0xc2eb) { return "M80_F.RETURN"; }
    if (address == 0xc2f1) { return "M80_F.RET1"; }
    if (address == 0xc2f4) { return "M80_X.CLEOLZ"; }
    if (address == 0xc2f6) { return "M80_X.CLEOL2"; }
    if (address == 0xc300) { return "M80_CN00/M80_BASICINT"; }
    if (address == 0xc305) { return "M80_BASICIN"; }
    if (address == 0xc307) { return "M80_BASICOUT"; }
    if (address == 0xc317) { return "M80_BASICENT"; }
    if (address == 0xc336) { return "M80_BASICENT2"; }
    if (address == 0xc348) { return "M80_JBASINIT"; }
    if (address == 0xc34b) { return "M80_JPINIT"; }
    if (address == 0xc351) { return "M80_JPREAD"; }
    if (address == 0xc357) { return "M80_JPWRITE"; }
    if (address == 0xc35d) { return "M80_JPSTAT"; }
    if (address == 0xc363) { return "M80_MOVE"; }
    if (address == 0xc378) { return "M80_MOVEC2M"; }
    if (address == 0xc37b) { return "M80_MOVESTRT"; }
    if (address == 0xc380) { return "M80_MOVELOOP"; }
    if (address == 0xc38a) { return "M80_NXTA1"; }
    if (address == 0xc398) { return "M80_C01"; }
    if (address == 0xc3a3) { return "M80_C03"; }
    if (address == 0xc3ac) { return "M80_MOVERET"; }
    if (address == 0xc3b0) { return "M80_XFER"; }
    if (address == 0xc3c5) { return "M80_XFERC2M"; }
    if (address == 0xc3cd) { return "M80_XFERAZP"; }
    if (address == 0xc3dc) { return "M80_XFERSZP"; }
    if (address == 0xc3eb) { return "M80_SETCB"; }
    if (address == 0xc803) { return "M80_BASICINIT"; }
    if (address == 0xc813) { return "M80_HANG"; }
    if (address == 0xc816) { return "M80_BINIT1"; }
    if (address == 0xc831) { return "M80_BINIT1A"; }
    if (address == 0xc850) { return "M80_BINIT2"; }
    if (address == 0xc85d) { return "M80_CLEARIT"; }
    if (address == 0xc866) { return "M80_C8BASIC"; }
    if (address == 0xc874) { return "M80_C8B2"; }
    if (address == 0xc87e) { return "M80_C8B3"; }
    if (address == 0xc890) { return "M80_C8B4"; }
    if (address == 0xc896) { return "M80_BOUT"; }
    if (address == 0xc8a1) { return "M80_BPRINT"; }
    if (address == 0xc8b4) { return "M80_KBDWAIT"; }
    if (address == 0xc8c0) { return "M80_NOWAIT"; }
    if (address == 0xc8cc) { return "M80_BPNCTL"; }
    if (address == 0xc8e2) { return "M80_BIORET"; }
    if (address == 0xc8f6) { return "M80_BINPUT"; }
    if (address == 0xc905) { return "M80_B.INPUT"; }
    if (address == 0xc918) { return "M80_ESCAPING"; }
    if (address == 0xc929) { return "M80_ESC1"; }
    if (address == 0xc92b) { return "M80_ESC2"; }
    if (address == 0xc935) { return "M80_ESC3"; }
    if (address == 0xc945) { return "M80_ESCSPEC"; }
    if (address == 0xc954) { return "M80_ESCSPEC2"; }
    if (address == 0xc960) { return "M80_ESCNONE"; }
    if (address == 0xc963) { return "M80_ESCSPEC3"; }
    if (address == 0xc972) { return "M80_ESCTAB"; }
    if (address == 0xc983) { return "M80_ESCCHAR"; }
    if (address == 0xc994) { return "M80_PSTATUS"; }
    if (address == 0xc99e) { return "M80_PSTATUS2"; }
    if (address == 0xc9b0) { return "M80_PSTATUS3"; }
    if (address == 0xc9b4) { return "M80_PSTATUS4"; }
    if (address == 0xc9b7) { return "M80_NOESC"; }
    if (address == 0xc9c6) { return "M80_B.NOPICK"; }
    if (address == 0xc9df) { return "M80_B.CHKCAN"; }
    if (address == 0xc9f7) { return "M80_B.FLIP"; }
    if (address == 0xca02) { return "M80_B.CANLIT"; }
    if (address == 0xca0a) { return "M80_B.FIXCHR"; }
    if (address == 0xca24) { return "M80_B.INRET"; }
    if (address == 0xca27) { return "M80_GETPRIOR"; }
    if (address == 0xca49) { return "M80_GPX"; }
    if (address == 0xca4a) { return "M80_PREAD"; }
    if (address == 0xca4f) { return "M80_PREADRET2"; }
    if (address == 0xca51) { return "M80_PWRITE"; }
    if (address == 0xca62) { return "M80_PIGOOD"; }
    if (address == 0xca74) { return "M80_PREAD"; }
    if (address == 0xca8a) { return "M80_PREADRET2"; }
    if (address == 0xca8e) { return "M80_PWRITE"; }
    if (address == 0xca9e) { return "M80_PWRITE2"; }
    if (address == 0xcaaf) { return "M80_GETY"; }
    if (address == 0xcacb) { return "M80_PWRITE3"; }
    if (address == 0xcadc) { return "M80_STARTXY"; }
    if (address == 0xcaeb) { return "M80_PWRITE4"; }
    if (address == 0xcb09) { return "M80_PWWRAP"; }
    if (address == 0xcb0f) { return "M80_PWRITERET"; }
    if (address == 0xcb15) { return "M80_GETKEY"; }
    if (address == 0xcb1b) { return "M80_GETK2"; }
    if (address == 0xcb24) { return "M80_TESTCARD"; }
    if (address == 0xcb48) { return "M80_STAY2"; }
    if (address == 0xcb4d) { return "M80_STAY80"; }
    if (address == 0xcb4e) { return "M80_TESTFAIL"; }
    if (address == 0xcb51) { return "M80_BASCALC"; }
    if (address == 0xcb54) { return "M80_BASCALCZ"; }
    if (address == 0xcb55) { return "M80_BSCLC1"; }
    if (address == 0xcb5b) { return "M80_BSCLC1A"; }
    if (address == 0xcb6d) { return "M80_BSCLC2"; }
    if (address == 0xcb7e) { return "M80_BASCLC3"; }
    if (address == 0xcb97) { return "M80_BASCLCX"; }
    if (address == 0xcb99) { return "M80_CTLCHAR"; }
    if (address == 0xcbab) { return "M80_CTLCHARX"; }
    if (address == 0xcbae) { return "M80_CTLGO"; }
    if (address == 0xcbb2) { return "M80_CTLRET"; }
    if (address == 0xcbb6) { return "M80_CTLXFER"; }
    if (address == 0xcbbc) { return "M80_X.BELL"; }
    if (address == 0xcbce) { return "M80_BELL2"; }
    if (address == 0xcbcf) { return "M80_WAIT"; }
    if (address == 0xcbd0) { return "M80_WAIT2"; }
    if (address == 0xcbd1) { return "M80_WAIT3"; }
    if (address == 0xcbdb) { return "M80_X.BS"; }
    if (address == 0xcbe2) { return "M80_BS40"; }
    if (address == 0xcbeb) { return "M80_BSDONE"; }
    if (address == 0xcbec) { return "M80_X.CR"; }
    if (address == 0xcbfd) { return "M80_X.CRPAS"; }
    if (address == 0xcc0c) { return "M80_X.CRRET"; }
    if (address == 0xcc0d) { return "M80_X.EM"; }
    if (address == 0xcc1a) { return "M80_X.SUB"; }
    if (address == 0xcc1d) { return "M80_X.SUB80"; }
    if (address == 0xcc1f) { return "M80_X.SUBLP"; }
    if (address == 0xcc26) { return "M80_X.FS"; }
    if (address == 0xcc33) { return "M80_X.FSRET"; }
    if (address == 0xcc34) { return "M80_X.US"; }
    if (address == 0xcc40) { return "M80_X.US1"; }
    if (address == 0xcc45) { return "M80_X.US2"; }
    if (address == 0xcc48) { return "M80_X.USRET"; }
    if (address == 0xcc49) { return "M80_X.SO"; }
    if (address == 0xcc52) { return "M80_X.SI"; }
    if (address == 0xcc59) { return "M80_STUFFINV"; }
    if (address == 0xcc5f) { return "M80_CTLADL"; }
    if (address == 0xcc78) { return "M80_CTLADH"; }
    if (address == 0xcc91) { return "M80_X.LF"; }
    if (address == 0xcc9e) { return "M80_X.LF2"; }
    if (address == 0xcca4) { return "M80_SCROLLUP"; }
    if (address == 0xccaa) { return "M80_SCROLLDN"; }
    if (address == 0xccae) { return "M80_SCROLL1"; }
    if (address == 0xccb8) { return "M80_SCROLL2"; }
    if (address == 0xccd1) { return "M80_SCRLSUB"; }
    if (address == 0xccdd) { return "M80_MSCROL0"; }
    if (address == 0xcce1) { return "M80_MSCROL1"; }
    if (address == 0xccf9) { return "M80_MSCRL2"; }
    if (address == 0xcd02) { return "M80_MSCRLRET"; }
    if (address == 0xcd09) { return "M80_ONEMORE"; }
    if (address == 0xcd10) { return "M80_MSCRLRTS"; }
    if (address == 0xcd11) { return "M80_X.SCRLRET"; }
    if (address == 0xcd17) { return "M80_X.SCRLRET2"; }
    if (address == 0xcd20) { return "M80_X.LFRET"; }
    if (address == 0xcd23) { return "M80_X.VT"; }
    if (address == 0xcd2c) { return "M80_X.VTLOOP"; }
    if (address == 0xcd32) { return "M80_X.VTNEXT"; }
    if (address == 0xcd42) { return "M80_X.FF"; }
    if (address == 0xcd48) { return "M80_X.GS"; }
    if (address == 0xcd4e) { return "M80_X.GSEOLZ"; }
    if (address == 0xcd54) { return "M80_X.GS2"; }
    if (address == 0xcd59) { return "M80_X.DC1"; }
    if (address == 0xcd64) { return "M80_X.DC1B"; }
    if (address == 0xcd76) { return "M80_X.DC1RTS"; }
    if (address == 0xcd77) { return "M80_X.DC2"; }
    if (address == 0xcd88) { return "M80_X.DC2B"; }
    if (address == 0xcd90) { return "M80_X.NAK"; }
    if (address == 0xcd9a) { return "M80_X.NAKRET/M80_DC2RET"; }
    if (address == 0xcd9b) { return "M80_FULL80"; }
    if (address == 0xcdaa) { return "M80_QUIT"; }
    if (address == 0xcdc0) { return "M80_QUIT2"; }
    if (address == 0xcddb) { return "M80_SCRN84"; }
    if (address == 0xcdea) { return "M80_SCR40"; }
    if (address == 0xce01) { return "M80_SCR40RET"; }
    if (address == 0xce0a) { return "M80_ATEFOR"; }
    if (address == 0xce13) { return "M80_ATEFOR1"; }
    if (address == 0xce22) { return "M80_GET84"; }
    if (address == 0xce32) { return "M80_SCRN48"; }
    if (address == 0xce3e) { return "M80_SCR80"; }
    if (address == 0xce55) { return "M80_SCR80RET"; }
    if (address == 0xce58) { return "M80_SCRNRET"; }
    if (address == 0xce63) { return "M80_FORATE"; }
    if (address == 0xce6f) { return "M80_FORATE1"; }
    if (address == 0xce91) { return "M80_CLRHALF"; }
    if (address == 0xce9b) { return "M80_CLRHALF2"; }
    if (address == 0xcea3) { return "M80_DO48"; }
    if (address == 0xceaf) { return "M80_SETCH"; }
    if (address == 0xced9) { return "M80_SETCHRTS"; }
    if (address == 0xcedd) { return "M80_INVERT"; }
    if (address == 0xcef2) { return "M80_STORCHAR"; }
    if (address == 0xcef9) { return "M80_STOR2"; }
    if (address == 0xcf00) { return "M80_SEV"; }
    if (address == 0xcf01) { return "M80_PICK"; }
    if (address == 0xcf06) { return "M80_SCREENIT"; }
    if (address == 0xcf1e) { return "M80_SCRN2"; }
    if (address == 0xcf2a) { return "M80_STOR80"; }
    if (address == 0xcf37) { return "M80_SCRN3"; }
    if (address == 0xcf40) { return "M80_SCRN40"; }
    if (address == 0xcf4a) { return "M80_STOR40"; }
    if (address == 0xcf4e) { return "M80_STPKEXIT"; }
    if (address == 0xcf52) { return "M80_ESCON"; }
    if (address == 0xcf65) { return "M80_ESCOFF"; }
    if (address == 0xcf6e) { return "M80_ESCRET"; }
    if (address == 0xcf78) { return "M80_COPYROM"; }
    if (address == 0xcf95) { return "M80_COPYROM2"; }
    if (address == 0xcfb3) { return "M80_LCB2ROM"; }
    if (address == 0xcfb9) { return "M80_LCB1"; }
    if (address == 0xcfc2) { return "M80_LCB1ROM"; }
    if (address == 0xcfc5) { return "M80_COPYRET"; }
    if (address == 0xcfc8) { return "M80_PSETUP"; }
    if (address == 0xcfd2) { return "M80_PSETUP2"; }
    if (address == 0xcfdf) { return "M80_PSETUPRET"; }
    if (address == 0xcfea) { return "M80_F.TABLE"; }
    if (address == 0xcff0) { return "M80_PLUSMINUS1"; }
    if (address == 0xcff3) { return "M80_B.TABLE"; }
    if (address == 0xcff9) { return "M80_WNDTAB"; }
    if (address == 0xcffd) { return "M80_ZZEND"; }


    if (address == 0xcfff) { return "MON.CLRROM"; }
    if (address == 0xe000) { return "MON.BASIC"; }
    if (address == 0xe003) { return "MON.BASIC2"; }

    if (address == 0xd365) { return "AS.GTFORPNT"; }
    if (address == 0xd39e) { return "AS.BLTU"; }
    if (address == 0xd39a) { return "AS.BLTU2"; }
    if (address == 0xd3d6) { return "AS.CHKMEM"; }
    if (address == 0xd3e3) { return "AS.REASON"; }
    if (address == 0xd410) { return "AS.MEMERR"; }
    if (address == 0xd412) { return "AS.ERROR"; }
    if (address == 0xd431) { return "AS.PRINT_ERROR_LINNUM"; }
    if (address == 0xd43c) { return "AS.RESTART"; }
    if (address == 0xd45c) { return "AS.NUMBERED_LINE"; }
    if (address == 0xd4b5) { return "AS.PUT_NEW_LINE"; }
    if (address == 0xd4f2) { return "AS.FIX_LINKS"; }
    if (address == 0xd52c) { return "AS.INLIN"; }
    if (address == 0xd52e) { return "AS.INLIN2"; }
    if (address == 0xd553) { return "AS.INCHR"; }
    if (address == 0xd559) { return "AS.PARSE_INPUT_LINE"; }
    if (address == 0xd56c) { return "AS.PARSE"; }
    if (address == 0xd61a) { return "AS.FNDLIN"; }
    if (address == 0xd61e) { return "AS.FL1"; }
    if (address == 0xd648) { return "AS.RTS1"; }
    if (address == 0xd649) { return "AS.NEW"; }
    if (address == 0xd64b) { return "AS.SCRTCH"; }
    if (address == 0xd665) { return "AS.SETPTRS"; }
    if (address == 0xd66a) { return "AS.CLEAR"; }
    if (address == 0xd66c) { return "AS.CLEARC"; }
    if (address == 0xd683) { return "AS.STKINI"; }
    if (address == 0xd696) { return "AS.RTS2"; }
    if (address == 0xd697) { return "AS.STXTPT"; }
    if (address == 0xd6a5) { return "AS.LIST"; }
    if (address == 0xd6da) { return "AS.LIST0"; }
    if (address == 0xd6fe) { return "AS.LIST1"; }
    if (address == 0xd702) { return "AS.LIST2"; }
    if (address == 0xd724) { return "AS.LIST3"; }
    if (address == 0xd72c) { return "AS.GETCHR"; }
    if (address == 0xd734) { return "AS.LIST4"; }
    if (address == 0xd766) { return "AS.FOR"; }
    if (address == 0xd7af) { return "AS.STEP"; }
    if (address == 0xd7d2) { return "AS.NEWSTT"; }
    if (address == 0xd805) { return "AS.TRACE_"; }
    if (address == 0xd826) { return "AS.GOEND"; }
    if (address == 0xd828) { return "AS.EXECUTE_STATEMENT"; }
    if (address == 0xd82a) { return "AS.EXECUTE_STATEMENT1"; }
    if (address == 0xd842) { return "AS.COLON_"; }
    if (address == 0xd846) { return "AS.SYNERR1"; }
    if (address == 0xd849) { return "AS.RESTORE"; }
    if (address == 0xd853) { return "AS.SETDA"; }
    if (address == 0xd857) { return "AS.RTS3"; }
    if (address == 0xd858) { return "AS.ISCNTC"; }
    if (address == 0xd863) { return "AS.CONTROL_C_TYPED"; }
    if (address == 0xd86e) { return "AS.STOP"; }
    if (address == 0xd870) { return "AS.END"; }
    if (address == 0xd871) { return "AS.END2"; }
    if (address == 0xd88a) { return "AS.END4"; }
    if (address == 0xd896) { return "AS.CONT"; }
    if (address == 0xd8af) { return "AS.RTS4"; }
    if (address == 0xd8b0) { return "AS.SAVE"; }
    if (address == 0xd8c9) { return "AS.LOAD"; }
    if (address == 0xd8F0) { return "AS.VARTIO"; }
    if (address == 0xd901) { return "AS.PROGIO"; }
    if (address == 0xd912) { return "AS.RUN"; }
    if (address == 0xd921) { return "AS.GOSUB"; }
    if (address == 0xd935) { return "AS.GO_TO_LINE"; }
    if (address == 0xd93e) { return "AS.GOTO"; }
    if (address == 0xd96a) { return "AS.RTS5"; }
    if (address == 0xd96b) { return "AS.POP"; }
    if (address == 0xd97c) { return "AS.UNDERR"; }
    if (address == 0xd981) { return "AS.SYNERR2"; }
    if (address == 0xd984) { return "AS.RETURN"; }
    if (address == 0xd995) { return "AS.DATA"; }
    if (address == 0xd998) { return "AS.ADDON"; }
    if (address == 0xd9a2) { return "AS.RTS6"; }
    if (address == 0xd9a3) { return "AS.DATAN"; }
    if (address == 0xd9a6) { return "AS.REMN"; }
    if (address == 0xd9c5) { return "AS.PULL3"; }
    if (address == 0xd9c9) { return "AS.IF"; }
    if (address == 0xd9dc) { return "AS.REM"; }
    if (address == 0xd9e1) { return "AS.IFTRUE"; }
    if (address == 0xd9ec) { return "AS.ONGOTO"; }
    if (address == 0xd9f4) { return "AS.ON1"; }
    if (address == 0xd9f8) { return "AS.ON2"; }
    if (address == 0xda0b) { return "AS.RTS7"; }
    if (address == 0xda0c) { return "AS.LINGET"; }
    if (address == 0xda46) { return "AS.LET"; }
    if (address == 0xda63) { return "AS.LET2"; }
    if (address == 0xda7a) { return "AS.LET_STRING"; }
    if (address == 0xda7b) { return "AS.PUTSTR"; }
    if (address == 0xdacf) { return "AS.PR_SPRING"; }
    if (address == 0xdad5) { return "AS.PRINT"; }
    if (address == 0xdad7) { return "AS.PRINT2"; }
    if (address == 0xdafb) { return "AS.CRDO"; }
    if (address == 0xdb00) { return "AS.NEGATE"; }
    if (address == 0xdb02) { return "AS.PR_RTS8"; }
    if (address == 0xdb03) { return "AS.PR_COMMA"; }
    if (address == 0xdb16) { return "AS.PR_TAB_OR_SPC"; }
    if (address == 0xdb2c) { return "AS.NXSPC"; }
    if (address == 0xdb2f) { return "AS.PR_NEXT_CHAR"; }
    if (address == 0xdb35) { return "AS.DOSPC"; }
    if (address == 0xdb3a) { return "AS.STROUT"; }
    if (address == 0xdb3d) { return "AS.STRPRT"; }
    if (address == 0xdb57) { return "AS.OUTSP"; }
    if (address == 0xdb5a) { return "AS.OUTQUES"; }
    if (address == 0xdb5c) { return "AS.OUTDO"; }
    if (address == 0xdb71) { return "AS.INPUTERR"; }
    if (address == 0xdb7b) { return "AS.READERR"; }
    if (address == 0xdb7f) { return "AS.ERLIN"; }
    if (address == 0xdb86) { return "AS.INPERR"; }
    if (address == 0xdb87) { return "AS.RESPERR"; }
    if (address == 0xdba0) { return "AS.GET"; }
    if (address == 0xdbb2) { return "AS.INPUT"; }
    if (address == 0xdbdc) { return "AS.NXIN"; }
    if (address == 0xdbe2) { return "AS.READ"; }
    if (address == 0xdbe9) { return "AS.INPUT_FLAG_ZERO"; }
    if (address == 0xdbeb) { return "AS.PROCESS_INPUT_LIST"; }
    if (address == 0xdbf1) { return "AS.PROCESS_INPUT_ITEM"; }
    if (address == 0xdC2b) { return "AS.INSTART"; }
    if (address == 0xdC69) { return "AS.INPUT_DATA"; }
    if (address == 0xdC72) { return "AS.INPUT_MORE"; }
    if (address == 0xdC99) { return "AS.INPFIN"; }
    if (address == 0xdCa0) { return "AS.FINDATA"; }
    if (address == 0xdCC6) { return "AS.INPDONE"; }
    if (address == 0xdcdf) { return "AS.ERR_EXTRA"; }
    if (address == 0xdcef) { return "AS.ERR_REENTRY"; }
    if (address == 0xdcf9) { return "AS.NEXT"; }
    if (address == 0xdcff) { return "AS.NEXT1"; }
    if (address == 0xdd02) { return "AS.NEXT2"; }
    if (address == 0xdd0d) { return "AS.GERR"; }
    if (address == 0xdd0f) { return "AS.NEXT3"; }
    if (address == 0xdd67) { return "AS.FRMNUM"; }
    if (address == 0xdd6a) { return "AS.CHKNUM"; }
    if (address == 0xdd6c) { return "AS.CHKSTR"; }
    if (address == 0xdd6d) { return "AS.CHKVAL"; }
    if (address == 0xdd78) { return "AS.JERROR"; }
    if (address == 0xdd7b) { return "AS.FRMEVL"; }
    if (address == 0xdd86) { return "AS.FRMEVL1"; }
    if (address == 0xdd95) { return "AS.FRMEVL2"; }
    if (address == 0xddcd) { return "AS.FRM_PRECEDENCE_TEST"; }
    if (address == 0xddd6) { return "AS.NXOP"; }
    if (address == 0xddd7) { return "AS.SAVOP"; }
    if (address == 0xdde4) { return "AS.FRM_RELATIONAL"; }
    if (address == 0xddf6) { return "AS.FRM_PREFNC"; }
    if (address == 0xddfd) { return "AS.FRM_RECURSE"; }
    if (address == 0xde0d) { return "AS.SNTXERR"; }
    if (address == 0xde10) { return "AS.FRM_STACK1"; }
    if (address == 0xde15) { return "AS.FRM_STACK2"; }
    if (address == 0xde20) { return "AS.FRM_STACK3"; }
    if (address == 0xde35) { return "AS.NOTMATH"; }
    if (address == 0xde38) { return "AS.GOEX"; }
    if (address == 0xde3a) { return "AS.FRM_PERFORM1"; }
    if (address == 0xde43) { return "AS.FRM_PERFORM2"; }
    if (address == 0xde5d) { return "AS.EXIT"; }
    if (address == 0xde60) { return "AS.FRM_ELEMENT"; }
    if (address == 0xde81) { return "AS.STRTXT"; }
    if (address == 0xde90) { return "AS.NOT_"; }
    if (address == 0xde98) { return "AS.EQUOP"; }
    if (address == 0xdea4) { return "AS.FN_"; }
    if (address == 0xdeab) { return "AS.SGN_"; }
    if (address == 0xdeb2) { return "AS.PARCHK"; }
    if (address == 0xdeb8) { return "AS.CHKCLS"; }
    if (address == 0xdebb) { return "AS.CHKOPN"; }
    if (address == 0xdebe) { return "AS.CHKCOM"; }
    if (address == 0xdec0) { return "AS.SYNCHR"; }
    if (address == 0xdec9) { return "AS.SYNERR"; }
    if (address == 0xdece) { return "AS.MIN"; }
    if (address == 0xded0) { return "AS.EQUL"; }
    if (address == 0xded5) { return "AS.FRM_VARIABLE"; }
    if (address == 0xded7) { return "AS.FRM_VARIABLE_CALL"; }
    if (address == 0xdef9) { return "AS.SCREEN"; }
    if (address == 0xdf0c) { return "AS.UNARY"; }
    if (address == 0xdf4f) { return "AS.OR"; }
    if (address == 0xdf55) { return "AS.AND"; }
    if (address == 0xdf5d) { return "AS.FALSE"; }
    if (address == 0xdf60) { return "AS.TRUE"; }
    if (address == 0xdf65) { return "AS.RELOPS"; }
    if (address == 0xdf7d) { return "AS.STRCMP"; }
    if (address == 0xdfaa) { return "AS.STRCMP1"; }
    if (address == 0xdfb0) { return "AS.NUMCMP"; }
    if (address == 0xdfb5) { return "AS.STRCMP2"; }
    if (address == 0xdfc1) { return "AS.CMPDONE"; }
    if (address == 0xdfcd) { return "AS.PDL"; }
    if (address == 0xdfd6) { return "AS.NXDIM"; }
    if (address == 0xdfd9) { return "AS.DIM"; }
    if (address == 0xdfe3) { return "AS.PTRGET"; }
    if (address == 0xdfe8) { return "AS.PTRGET2"; }
    if (address == 0xdfea) { return "AS.PTRGET3"; }
    if (address == 0xdff4) { return "AS.BADNAM"; }
    if (address == 0xdff7) { return "AS.NAMOK"; }
    if (address == 0xe007) { return "AS.PTRGET4"; }
    if (address == 0xe07d) { return "AS.ISLETC"; }
    if (address == 0xe087) { return "AS.NAME_NOT_FOUND"; }
    if (address == 0xe09c) { return "AS.MAKE_NEW_VARIABLE"; }
    if (address == 0xe0de) { return "AS.SET_VARPNT_AND_YA"; }
    if (address == 0xe0ed) { return "AS.GETARY"; }
    if (address == 0xe0ef) { return "AS.GETARY2"; }
    if (address == 0xe102) { return "AS.MAKINT"; }
    if (address == 0xe108) { return "AS.MKINT"; }
    if (address == 0xe10c) { return "AS.AYINT"; }
    if (address == 0xe119) { return "AS.MI1"; }
    if (address == 0xe11b) { return "AS.MI2"; }
    if (address == 0xe11e) { return "AS.ARRAY"; }
    if (address == 0xe196) { return "AS.SUBERR"; }
    if (address == 0xe199) { return "AS.IQERR"; }
    if (address == 0xe19b) { return "AS.JER"; }
    if (address == 0xe19e) { return "AS.USE_OLD_ARRAY"; }
    if (address == 0xe1b8) { return "AS.MAKE_NEW_ARRAY"; }
    if (address == 0xe24b) { return "AS.FIND_ARRAY_ELEMENT"; }
    if (address == 0xe253) { return "AS.FAE1"; }
    if (address == 0xe269) { return "AS.GSE"; }
    if (address == 0xe26c) { return "AS.GME"; }
    if (address == 0xe26f) { return "AS.FAE2"; }
    if (address == 0xe270) { return "AS.FAE3"; }
    if (address == 0xe2ac) { return "AS.RTS9"; }
    if (address == 0xe2ad) { return "AS.MULTIPLY_SUBSCRIPT"; }
    if (address == 0xe2b6) { return "AS.MULTIPLY_SUBS1"; }
    if (address == 0xe2de) { return "AS.FRE"; }
    if (address == 0xe2f2) { return "AS.GIVAYF"; }
    if (address == 0xe2ff) { return "AS.POS"; }
    if (address == 0xe301) { return "AS.SNGFLT"; }
    if (address == 0xe306) { return "AS.ERRDIR"; }
    if (address == 0xe30e) { return "AS.UNDFNC"; }
    if (address == 0xe313) { return "AS.DEF"; }
    if (address == 0xe341) { return "AS.FNC_"; }
    if (address == 0xe354) { return "AS.FUNCT"; }
    if (address == 0xe3af) { return "AS.FNCDATA"; }
    if (address == 0xe3c5) { return "AS.STR"; }
    if (address == 0xe3d5) { return "AS.STRINI"; }
    if (address == 0xe3dd) { return "AS.STRSPA"; }
    if (address == 0xe3e7) { return "AS.STRLIT"; }
    if (address == 0xe3ed) { return "AS.STRLT2"; }
    if (address == 0xe42a) { return "AS.PUTNEW"; }
    if (address == 0xe432) { return "AS.JERR"; }
    if (address == 0xe435) { return "AS.PUTEMP"; }
    if (address == 0xe452) { return "AS.GETSPA"; }
    if (address == 0xe484) { return "AS.GARBAG"; }
    if (address == 0xe488) { return "AS.FIND_HIGHEST_STRING"; }
    if (address == 0xe519) { return "AS.CHECK_SIMPLE_VARIABLE"; }
    if (address == 0xe523) { return "AS.CHECK_VARIABLE"; }
    if (address == 0xe552) { return "AS.CHECK_BUMP"; }
    if (address == 0xe55d) { return "AS.CHECK_EXIT"; }
    if (address == 0xe562) { return "AS.MOVE_HIGHEST_STRING_TO_TOP"; }
    if (address == 0xe597) { return "AS.CAT"; }
    if (address == 0xe5d4) { return "AS.MOVINS"; }
    if (address == 0xe5e2) { return "AS.MOVSTR"; }
    if (address == 0xe5e6) { return "AS.MOVSTR1"; }
    if (address == 0xe5fd) { return "AS.FRESTR"; }
    if (address == 0xe600) { return "AS.FREFAC"; }
    if (address == 0xe604) { return "AS.FRETMP"; }
    if (address == 0xe635) { return "AS.FRETMS"; }
    if (address == 0xe646) { return "AS.CHRSTR"; }
    if (address == 0xe65a) { return "AS.LEFTSTR"; }
    if (address == 0xe660) { return "AS.SUBSTRING1"; }
    if (address == 0xe667) { return "AS.SUBSTRING2"; }
    if (address == 0xe668) { return "AS.SUBSTRING3"; }
    if (address == 0xe686) { return "AS.RIGHTSTR"; }
    if (address == 0xe691) { return "AS.MIDSTR"; }
    if (address == 0xe6b9) { return "AS.SUBSTRING_SETUP"; }
    if (address == 0xe6d6) { return "AS.LEN"; }
    if (address == 0xe6dc) { return "AS.GETSTR"; }
    if (address == 0xe6e5) { return "AS.ASC"; }
    if (address == 0xe6f2) { return "AS.GOIQ"; }
    if (address == 0xe6f5) { return "AS.GTBYTC"; }
    if (address == 0xe6f8) { return "AS.GETBYT"; }
    if (address == 0xe6fb) { return "AS.CONINT"; }
    if (address == 0xe707) { return "AS.VAL"; }
    if (address == 0xe73d) { return "AS.POINT"; }
    if (address == 0xe746) { return "AS.GTNUM"; }
    if (address == 0xe74c) { return "AS.COMBYTE"; }
    if (address == 0xe752) { return "AS.GETADR"; }
    if (address == 0xe764) { return "AS.PEEK"; }
    if (address == 0xe77b) { return "AS.POKE"; }
    if (address == 0xe784) { return "AS.WAIT"; }
    if (address == 0xe79f) { return "AS.RTS10"; }
    if (address == 0xe7a0) { return "AS.FADDH"; }
    if (address == 0xe7a7) { return "AS.FSUB"; }
    if (address == 0xe7aa) { return "AS.FSUBT"; }
    if (address == 0xe7b9) { return "AS.FADD1"; }
    if (address == 0xe7be) { return "AS.FADD"; }
    if (address == 0xe7c1) { return "AS.FADDT"; }
    if (address == 0xe7ce) { return "AS.FADD2"; }
    if (address == 0xe7fa) { return "AS.FADD3"; }
    if (address == 0xe829) { return "AS.NORMALIZE_FAC1"; }
    if (address == 0xe82e) { return "AS.NORMALIZE_FAC2"; }
    if (address == 0xe84e) { return "AS.ZERO_FAC"; }
    if (address == 0xe850) { return "AS.STA_IN_FAC_SIGN_AND_EXP"; }
    if (address == 0xe852) { return "AS.STA_IN_FAC_SIGN"; }
    if (address == 0xe855) { return "AS.FADD4"; }
    if (address == 0xe874) { return "AS.NORMALIZE_FAC3"; }
    if (address == 0xe880) { return "AS.NORMALIZE_FAC4"; }
    if (address == 0xe88d) { return "AS.NORMALIZE_FAC5"; }
    if (address == 0xe88f) { return "AS.NORMALIZE_FAC6"; }
    if (address == 0xe89d) { return "AS.RTS11"; }
    if (address == 0xe89e) { return "AS.COMPLEMENT_FAC"; }
    if (address == 0xe8a4) { return "AS.COMPLEMENT_FAC_MANTISSA"; }
    if (address == 0xe8c6) { return "AS.INCREMENT_FAC_MANTISSA"; }
    if (address == 0xe8d4) { return "AS.RTS12"; }
    if (address == 0xe8d5) { return "AS.OVERFLOW"; }
    if (address == 0xe8da) { return "AS.SHIFT_RIGHT1"; }
    if (address == 0xe8dc) { return "AS.SHIFT_RIGHT2"; }
    if (address == 0xe8f0) { return "AS.SHIFT_RIGHT"; }
    if (address == 0xe8fd) { return "AS.SHIFT_RIGHT3"; }
    if (address == 0xe907) { return "AS.SHIFT_RIGHT4"; }
    if (address == 0xe911) { return "AS.SHIFT_RIGHT5"; }
    if (address == 0xe941) { return "AS.LOG"; }
    if (address == 0xe948) { return "AS.GIQ"; }
    if (address == 0xe94b) { return "AS.LOG2"; }
    if (address == 0xe97f) { return "AS.FMULT"; }
    if (address == 0xe982) { return "AS.FMULTT"; }
    if (address == 0xe9b0) { return "AS.MULTIPLY1"; }
    if (address == 0xe9b5) { return "AS.MULTIPLY2"; }
    if (address == 0xe9e2) { return "AS.RTS13"; }
    if (address == 0xe9e3) { return "AS.LOAD_ARG_FROM_YA"; }
    if (address == 0xea0e) { return "AS.ADD_EXPONENTS"; }
    if (address == 0xea10) { return "AS.ADD_EXPONENTS1"; }
    if (address == 0xea2b) { return "AS.OUTOFRNG"; }
    if (address == 0xea31) { return "AS.ZERO"; }
    if (address == 0xea36) { return "AS.JOV"; }
    if (address == 0xeae9) { return "AS.MUL10"; }
    if (address == 0xea55) { return "AS.DIV10"; }
    if (address == 0xea5e) { return "AS.DIV"; }
    if (address == 0xea66) { return "AS.FDIV"; }
    if (address == 0xeae6) { return "AS.COPY_RESULT_INTO_FAC"; }
    if (address == 0xeaf9) { return "AS.LOAD_FAC_FROM_YA"; }
    if (address == 0xeb1e) { return "AS.STORE_FAC_IN_TEMP2_ROUNDED"; }
    if (address == 0xeb21) { return "AS.STORE_FAC_IN_TEMP1_ROUNDED"; }
    if (address == 0xeb27) { return "AS.SETFOR"; }
    if (address == 0xeb2b) { return "AS.STORE_FAC_AT_YX_ROUNDED"; }
    if (address == 0xeb53) { return "AS.COPY_FAC_TO_ARG"; }
    if (address == 0xeb63) { return "AS.COPY_FAC_TO_ARG_ROUNDED"; }
    if (address == 0xeb71) { return "AS.RTS14"; }
    if (address == 0xeb72) { return "AS.ROUND_FAC"; }
    if (address == 0xeb7a) { return "AS.INCREMENT_MANTISSA"; }
    if (address == 0xeb82) { return "AS.SIGN"; }
    if (address == 0xeb86) { return "AS.SIGN1"; }
    if (address == 0xeb88) { return "AS.SIGN2"; }
    if (address == 0xeb8f) { return "AS.RTS15"; }
    if (address == 0xeb90) { return "AS.SGN"; }
    if (address == 0xeb9e) { return "AS.FLOAT"; }
    if (address == 0xeb9b) { return "AS.FLOAT1"; }
    if (address == 0xeba0) { return "AS.FLOAT2"; }
    if (address == 0xebaf) { return "AS.ABS"; }
    if (address == 0xebb2) { return "AS.FCOMP"; }
    if (address == 0xebb4) { return "AS.FCOMP2"; }
    if (address == 0xebf2) { return "AS.QINT"; }
    if (address == 0xec11) { return "AS.RTS16"; }
    if (address == 0xec12) { return "AS.QINT2"; }
    if (address == 0xec23) { return "AS.INT"; }
    if (address == 0xec40) { return "AS.QINT3"; }
    if (address == 0xec49) { return "AS.RTS17"; }
    if (address == 0xec4a) { return "AS.FIN"; }
    if (address == 0xec61) { return "AS.FIN1"; }
    if (address == 0xec64) { return "AS.FIN2"; }
    if (address == 0xec66) { return "AS.FIN3"; }
    if (address == 0xec87) { return "AS.FIN4"; }
    if (address == 0xec8a) { return "AS.FIN5"; }
    if (address == 0xec8c) { return "AS.FIN6"; }
    if (address == 0xec98) { return "AS.FIN10"; }
    if (address == 0xec9e) { return "AS.FIN7"; }
    if (address == 0xeca0) { return "AS.FIN8"; }
    if (address == 0xecc1) { return "AS.FIN9"; }
    if (address == 0xecd5) { return "AS.ADDACC"; }
    if (address == 0xece8) { return "AS.GETEXP"; }
    if (address == 0xed19) { return "AS.INPRT"; }
    if (address == 0xed24) { return "AS.LINPRT"; }
    if (address == 0xed2e) { return "AS.PRINT_FAC"; }
    if (address == 0xed31) { return "AS.GO_STROUT"; }
    if (address == 0xed34) { return "AS.FOUT"; }
    if (address == 0xed36) { return "AS.FOUT1"; }
    if (address == 0xed8c) { return "AS.FOUT2"; }
    if (address == 0xee17) { return "AS.FOUT3"; }
    if (address == 0xee57) { return "AS.FOUT4"; }
    if (address == 0xee5a) { return "AS.FOUT5"; }
    if (address == 0xee5f) { return "AS.FOUT6"; }
    if (address == 0xee8d) { return "AS.SQR"; }
    if (address == 0xee97) { return "AS.FPWRT"; }
    if (address == 0xeed0) { return "AS.NEGOP"; }
    if (address == 0xeeda) { return "AS.RTS18"; }
    if (address == 0xef09) { return "AS.EXP"; }
    if (address == 0xef5c) { return "AS.POLYNOMIAL_ODD"; }
    if (address == 0xef72) { return "AS.POLYNOMIAL"; }
    if (address == 0xef76) { return "AS.SERMAIN"; }
    if (address == 0xefa5) { return "AS.RTS19"; }
    if (address == 0xefae) { return "AS.RND"; }
    if (address == 0xefe7) { return "AS.GO_MOVMF"; }
    if (address == 0xefea) { return "AS.COS"; }
    if (address == 0xeff1) { return "AS.SIN"; }
    if (address == 0xf023) { return "AS.SIN1"; }
    if (address == 0xf026) { return "AS.SIN2"; }
    if (address == 0xf03a) { return "AS.TAN"; }
    if (address == 0xf062) { return "AS.TAN1"; }
    if (address == 0xf094) { return "AS.MS_EASTER_EGG_DATA"; }
    if (address == 0xf09e) { return "AS.ATN"; }
    if (address == 0xf0cd) { return "AS.RTS20"; }
    if (address == 0xf10b) { return "AS.GENERIC_CHRGET"; }
    if (address == 0xf128) { return "AS.COLD_START"; }
    if (address == 0xf1d5) { return "AS.CALL"; }
    if (address == 0xf1de) { return "AS.IN_NUMBER"; }
    if (address == 0xf1e5) { return "AS.PR_NUMBER"; }
    if (address == 0xf1ec) { return "AS.PLOTFNS"; }
    if (address == 0xf206) { return "AS.GOERR"; }
    if (address == 0xf209) { return "AS.LINCOOR"; }
    if (address == 0xf225) { return "AS.PLOT"; }
    if (address == 0xf232) { return "AS.HLIN"; }
    if (address == 0xf241) { return "AS.VLIN"; }
    if (address == 0xf24f) { return "AS.COLOR"; }
    if (address == 0xf256) { return "AS.VTAB"; }
    if (address == 0xf262) { return "AS.SPEED"; }
    if (address == 0xf26d) { return "AS.TRACE"; }
    if (address == 0xf26f) { return "AS.NOTRACE"; }
    if (address == 0xf273) { return "AS.NORMAL"; }
    if (address == 0xf277) { return "AS.INVERSE"; }
    if (address == 0xf280) { return "AS.FLASH"; }
    if (address == 0xf286) { return "AS.HIMEM"; }
    if (address == 0xf296) { return "AS.JMM"; }
    if (address == 0xf299) { return "AS.SETHI"; }
    if (address == 0xf2a6) { return "AS.LOMEM"; }
    if (address == 0xf2cb) { return "AS.ONERR"; }
    if (address == 0xf2e9) { return "AS.HANDLERR"; }
    if (address == 0xf318) { return "AS.RESUME"; }
    if (address == 0xf32e) { return "AS.JSYN"; }
    if (address == 0xf331) { return "AS.DEL"; }
    if (address == 0xf390) { return "AS.GR"; }
    if (address == 0xf399) { return "AS.TEXT"; }
    if (address == 0xf39f) { return "AS.STORE"; }
    if (address == 0xf3bc) { return "AS.RECALL"; }
    if (address == 0xf3d8) { return "AS.HGR2"; }
    if (address == 0xf3e2) { return "AS.HGR"; }
    if (address == 0xf3ea) { return "AS.SETHPG"; }
    if (address == 0xf3f2) { return "AS.HCLR"; }
    if (address == 0xf3f6) { return "AS.BKGND"; }
    if (address == 0xf411) { return "AS.HPOSN"; }
    if (address == 0xf457) { return "AS.HPLOT0"; }
    if (address == 0xf465) { return "AS.MOVE_LEFT_OR_RIGHT"; }
    if (address == 0xf47e) { return "AS.COLOR_SHIFT"; }
    if (address == 0xf48a) { return "AS.MOVE_RIGHT"; }
    if (address == 0xf49c) { return "AS.LRUDX1"; }
    if (address == 0xf49d) { return "AS.LRUDX2"; }
    if (address == 0xf4b3) { return "AS.LRUD1"; }
    if (address == 0xf4b4) { return "AS.LRUD2"; }
    if (address == 0xf4c4) { return "AS.LRUD3"; }
    if (address == 0xf4c8) { return "AS.LRUD4"; }
    if (address == 0xf4d3) { return "AS.MOVE_UP_OR_DOWN"; }
    if (address == 0xf505) { return "AS.MOVE_DOWN"; }
    if (address == 0xf530) { return "AS.HLINRL"; }
    if (address == 0xf53a) { return "AS.HGLN"; }
    if (address == 0xf57c) { return "AS.MOVEX"; }
    if (address == 0xf581) { return "AS.MOVEX2"; }
    if (address == 0xf5cb) { return "AS.HFIND"; }
    if (address == 0xf600) { return "AS.RTS22"; }
    if (address == 0xf601) { return "AS.DRAW0"; }
    if (address == 0xf605) { return "AS.DRAW1"; }
    if (address == 0xf65d) { return "AS.XDRAW0"; }
    if (address == 0xf661) { return "AS.XDRAW1"; }
    if (address == 0xf6b9) { return "AS.HFNS"; }
    if (address == 0xf6e6) { return "AS.GGERR"; }
    if (address == 0xf6e9) { return "AS.HCOLOR"; }
    if (address == 0xf6f5) { return "AS.RTS23"; }
    if (address == 0xf6fc) { return "AS.COLORTBL"; }
    if (address == 0xf6fe) { return "AS.HPLOT"; }
    if (address == 0xf721) { return "AS.ROT"; }
    if (address == 0xf727) { return "AS.SCALE"; }
    if (address == 0xf72d) { return "AS.DRAWPNT"; }
    if (address == 0xf769) { return "AS.DRAW"; }
    if (address == 0xf76f) { return "AS.XDRAW"; }
    if (address == 0xf775) { return "AS.SHLOAD"; }
    if (address == 0xf7bc) { return "AS.TAPEPNT"; }
    if (address == 0xf7d9) { return "AS.GETARYPT"; }
    if (address == 0xf7e7) { return "AS.HTAB"; }

    if (address == 0xf800) { return "MON.PLOT"; }
    if (address == 0xf80c) { return "MON.RTMASK"; }
    if (address == 0xf80e) { return "MON.PLOT1"; }
    if (address == 0xf819) { return "MON.HLINE"; }
    if (address == 0xf828) { return "MON.VLINE"; }
    if (address == 0xf831) { return "MON.RTS1"; }
    if (address == 0xf832) { return "MON.CLRSCR"; }
    if (address == 0xf836) { return "MON.CLRTOP"; }
    if (address == 0xf838) { return "MON.CLRSC2"; }
    if (address == 0xf83c) { return "MON.CLRSC3"; }
    if (address == 0xf847) { return "MON.GBASCALC"; }
    if (address == 0xf856) { return "MON.GBCALC"; }
    if (address == 0xf85f) { return "MON.NXTCOL"; }
    if (address == 0xf864) { return "MON.SETCOL"; }
    if (address == 0xf871) { return "MON.SCRN"; }
    if (address == 0xf879) { return "MON.SCRN2"; }
    if (address == 0xf87f) { return "MON.RTMSKZ"; }
    if (address == 0xf882) { return "MON.INDS1"; }
    if (address == 0xf88c) { return "MON.INDS2"; }
    if (address == 0xf89b) { return "MON.IEVEN"; }
    if (address == 0xf8a5) { return "MON.ERR"; }
    if (address == 0xf8a9) { return "MON.GETFMT"; }
    if (address == 0xf8be) { return "MON.MNNDX1"; }
    if (address == 0xf8c2) { return "MON.MNNDX2"; }
    if (address == 0xf8c9) { return "MON.NBBDX3"; }
    if (address == 0xf8d0) { return "MON.UBSDSP"; }
    if (address == 0xf8d4) { return "MON.PRINTOP"; }
    if (address == 0xf8db) { return "MON.PRNTBL"; }
    if (address == 0xf8f5) { return "MON.PRMN1"; }
    if (address == 0xf8f9) { return "MON.PRMN2"; }
    if (address == 0xf910) { return "MON.PRADR1"; }
    if (address == 0xf914) { return "MON.PRADR2"; }
    if (address == 0xf926) { return "MON.PRADR3"; }
    if (address == 0xf92a) { return "MON.PRADR4"; }
    if (address == 0xf930) { return "MON.PRADR5"; }
    if (address == 0xf938) { return "MON.RELADR"; }
    if (address == 0xf940) { return "MON.PRNTYX"; }
    if (address == 0xf941) { return "MON.PRNTAX"; }
    if (address == 0xf944) { return "MON.PRNTX"; }
    if (address == 0xf948) { return "MON.PRBLNK"; }
    if (address == 0xf94a) { return "MON.PRBL2"; }
    if (address == 0xf94c) { return "MON.PRBL3"; }
    if (address == 0xf953) { return "MON.PCADJ"; }
    if (address == 0xf954) { return "MON.PCADJ2"; }
    if (address == 0xf956) { return "MON.PCADJ3"; }
    if (address == 0xf95c) { return "MON.PCADJ4"; }
    if (address == 0xf961) { return "MON.RTS2"; }
    if (address == 0xf962) { return "MON.FMT1"; }
    if (address == 0xf9a6) { return "MON.FMT2"; }
    if (address == 0xf9b4) { return "MON.CHAR1"; }
    if (address == 0xf9ba) { return "MON.CHAR2"; }
    if (address == 0xf9c0) { return "MON.MNEML"; }
    if (address == 0xfa00) { return "MON.MNEMR"; }
    if (address == 0xfa40) { return "MON.IRQ"; }
    if (address == 0xfa4c) { return "MON.BREAK"; }
    if (address == 0xfa59) { return "MON.OLDBRK"; }
    if (address == 0xfa62) { return "MON.RESET"; }
    if (address == 0xfa6f) { return "MON.INITAN"; }
    if (address == 0xfa81) { return "MON.NEWMON"; }
    if (address == 0xfa9b) { return "MON.FIXSEV"; }
    if (address == 0xfaa3) { return "MON.NOFIX"; }
    if (address == 0xfaa6) { return "MON.PWRUP"; }
    if (address == 0xfaa9) { return "MON.SETPG3"; }
    if (address == 0xfaab) { return "MON.SETPLP"; }
    if (address == 0xfaba) { return "MON.SLOOP"; }
    if (address == 0xfac7) { return "MON.NXTBYT"; }
    if (address == 0xfad7) { return "MON.REGDSP"; }
    if (address == 0xfada) { return "MON.RGDPS1"; }
    if (address == 0xfae4) { return "MON.RDSP1"; }
    if (address == 0xfafd) { return "MON.PWRCON"; }
    if (address == 0xfb02) { return "MON.DISKID"; }
    if (address == 0xfb09) { return "MON.TITLE"; }
    if (address == 0xfb11) { return "MON.XLTBL"; }
    if (address == 0xfb19) { return "MON.RTBL"; }
    if (address == 0xfb1e) { return "MON.PREAD"; }
    if (address == 0xfb25) { return "MON.PREAD2"; }
    if (address == 0xfb2e) { return "MON.RTS2D"; }
    if (address == 0xfb2f) { return "MON.INIT"; }
    if (address == 0xfb39) { return "MON.SETTXT"; }
    if (address == 0xfb40) { return "MON.SETGR"; }
    if (address == 0xfb4b) { return "MON.SETWND"; }
    if (address == 0xfb5b) { return "MON.TABV"; }
    if (address == 0xfb60) { return "MON.APPLEII"; }
    if (address == 0xfb65) { return "MON.STITLE"; }
    if (address == 0xfb6A) { return "MON.GETLN"; }
    if (address == 0xfb6f) { return "MON.SETPWRC"; }
    if (address == 0xfb78) { return "MON.VIDWAIT"; }
    if (address == 0xfb88) { return "MON.KBDWAIT"; }

    if (address == 0xfbb3) { return "M80_F8VERSION"; }

    if (address == 0xfb94) { return "MON.NOWAIT"; }
    if (address == 0xfb97) { return "MON.ESCOLD"; }
    if (address == 0xfb9b) { return "MON.ESCNOW"; }
    if (address == 0xfba5) { return "MON.ESCNEW"; }
    if (address == 0xfbb3) { return "MON.VERSION"; }
    if (address == 0xfbb4) { return "MON.GOTOCX"; }
    if (address == 0xfbc1) { return "MON.BASCALC"; }

    if (address == 0xfbd0) { return "MON.BASCLC2"; }
    if (address == 0xfbd9) { return "MON.BELL1"; }
    if (address == 0xfbe4) { return "MON.BELL2"; }
    if (address == 0xfbef) { return "MON.RTS2B"; }
    if (address == 0xfbf0) { return "MON.STORADV"; }
    if (address == 0xfbf4) { return "MON.ADVANCE"; }
    if (address == 0xfbfc) { return "MON.RTS3"; }
    if (address == 0xfbfd) { return "MON.VIDOUT"; }
    if (address == 0xfc10) { return "MON.BS"; }
    if (address == 0xfc1a) { return "MON.UP"; }
    if (address == 0xfc22) { return "MON.VTAB"; }
    if (address == 0xfc24) { return "MON.VTABZ"; }
    if (address == 0xfc2b) { return "MON.RTS4"; }
    if (address == 0xfc2c) { return "MON.ESC1"; }
    if (address == 0xfc42) { return "MON.CLREOP"; }
    if (address == 0xfc58) { return "MON.HOME"; }
    if (address == 0xfc62) { return "MON.CR"; }
    if (address == 0xfc66) { return "MON.LF"; }
    if (address == 0xfc70) { return "MON.SCROLL"; }
    if (address == 0xfc72) { return "MON.XGOTOCX"; }

    if (address == 0xfc75) { return "M80_SNIFFIRQ"; }

    if (address == 0xfc84) { return "MON.RDCX"; }
    if (address == 0xfc91) { return "MON.ISSLOTS"; }
    if (address == 0xfc99) { return "MON.ISPAGE1"; }
    if (address == 0xfc9c) { return "MON.CLREOL"; }
    if (address == 0xfc9e) { return "MON.CLREOLZ"; }
    if (address == 0xfca8) { return "MON.WAIT"; }
    if (address == 0xfca9) { return "MON.WAIT2"; }
    if (address == 0xfcaa) { return "MON.WAIT3"; }
    if (address == 0xfcb4) { return "MON.NXTA4"; }
    if (address == 0xfcba) { return "MON.NXTA1"; }
    if (address == 0xfcc8) { return "MON.RTS4B"; }
    if (address == 0xfcc9) { return "MON.HEADR"; }
    if (address == 0xfcd6) { return "MON.WRBIT"; }
    if (address == 0xfcdb) { return "MON.ZERDLY"; }
    if (address == 0xfce2) { return "MON.ONEDLY"; }
    if (address == 0xfce5) { return "MON.WRTAPE"; }
    if (address == 0xfcec) { return "MON.RDBYTE"; }
    if (address == 0xfcee) { return "MON.RDBYT2"; }
    if (address == 0xfcfa) { return "MON.READ2BIT"; }
    if (address == 0xfcfd) { return "MON.RDBIT"; }
    if (address == 0xfd0c) { return "MON.RDKEY"; }

    if (address == 0xfd18) { return "MON.KEYIN"; }
    if (address == 0xfd21) { return "MON.RDESC"; }

    if (address == 0xfd29) { return "M80_FUNCEXIT"; }

    if (address == 0xfd2f) { return "MON.ESC"; }
    if (address == 0xfd35) { return "MON.RDCHAR"; }
    if (address == 0xfd3d) { return "MON.NOTCR"; }
    if (address == 0xfd5f) { return "MON.NOTCR1"; }
    if (address == 0xfd62) { return "MON.CANCEL"; }
    if (address == 0xfd67) { return "MON.GETLNZ"; }
    if (address == 0xfd6a) { return "MON.GETLN"; }
    if (address == 0xfd71) { return "MON.BCKSPC"; }
    if (address == 0xfd75) { return "MON.NXTCHAR"; }
    if (address == 0xfd7e) { return "MON.CAPTST"; }
    if (address == 0xfd84) { return "MON.ADDINP"; }
    if (address == 0xfd8e) { return "MON.CROUT"; }
    if (address == 0xfd92) { return "MON.PRA1"; }
    if (address == 0xfd96) { return "MON.PRYX2"; }
    if (address == 0xfda3) { return "MON.XAMB"; }
    if (address == 0xfdad) { return "MON.MOD8CHK"; }
    if (address == 0xfdb3) { return "MON.XAM"; }
    if (address == 0xfdb6) { return "MON.DATAOUT"; }
    if (address == 0xfdc5) { return "MON.RTS4C"; }
    if (address == 0xfdc6) { return "MON.XAMPM"; }
    if (address == 0xfdd1) { return "MON.ADD"; }
    if (address == 0xfdda) { return "MON.PRBYTE"; }
    if (address == 0xfde3) { return "MON.PRHEX"; }
    if (address == 0xfde5) { return "MON.PRHEXZ"; }
    if (address == 0xfded) { return "MON.COUT"; }
    if (address == 0xfdf0) { return "MON.COUT1"; }
    if (address == 0xfdf6) { return "MON.COUTZ"; }
    if (address == 0xfe00) { return "MON.BLI"; }
    if (address == 0xfe04) { return "MON.BLANK"; }
    if (address == 0xfe0b) { return "MON.STOR"; }
    if (address == 0xfe17) { return "MON.RTS5"; }
    if (address == 0xfe18) { return "MON.SETMODE"; }
    if (address == 0xfe1d) { return "MON.SETMDZ"; }
    if (address == 0xfe20) { return "MON.LT"; }
    if (address == 0xfe22) { return "MON.LT2"; }
    if (address == 0xfe2c) { return "MON.MOVE"; }
    if (address == 0xfe36) { return "MON.VFY"; }
    if (address == 0xfe58) { return "MON.VFYOK"; }
    if (address == 0xfe5e) { return "MON.LIST"; }
    if (address == 0xfe63) { return "MON.LIST2"; }
    if (address == 0xfe75) { return "MON.A1PC"; }
    if (address == 0xfe78) { return "MON.A1PCLP"; }
    if (address == 0xfe7f) { return "MON.A1PCRTS"; }
    if (address == 0xfe80) { return "MON.SETINV"; }
    if (address == 0xfe84) { return "MON.SETNORM"; }
    if (address == 0xfe86) { return "MON.SETIFLG"; }
    if (address == 0xfe89) { return "MON.SETKBD"; }
    if (address == 0xfe8b) { return "MON.INPORT"; }
    if (address == 0xfe8d) { return "MON.INPRT"; }
    if (address == 0xfe93) { return "MON.SETVID"; }
    if (address == 0xfe95) { return "MON.OUTPORT"; }
    if (address == 0xfe97) { return "MON.OUTPRT"; }
    if (address == 0xfe9b) { return "MON.IOPRT"; }
    if (address == 0xfea7) { return "MON.IOPRT1"; }
    if (address == 0xfea9) { return "MON.IOPRT2"; }
    if (address == 0xfeaf) { return "MON.CKSUMFIX"; }
    if (address == 0xfeb0) { return "MON.XBASIC"; }
    if (address == 0xfeb3) { return "MON.BASCONT"; }
    if (address == 0xfeb6) { return "MON.GO"; }
    if (address == 0xfebf) { return "MON.REGZ"; }
    if (address == 0xfec2) { return "MON.TRACE"; }
    if (address == 0xfec4) { return "MON.STEPZ"; }
    if (address == 0xfeca) { return "MON.USR"; }
    if (address == 0xfecd) { return "MON.WRITE"; }
    if (address == 0xfed4) { return "MON.WR1"; }
    if (address == 0xfeed) { return "MON.WRBYTE"; }
    if (address == 0xfeef) { return "MON.WRBYT2"; }
    if (address == 0xfef6) { return "MON.CRMON"; }
    if (address == 0xfefd) { return "MON.READ"; }
    if (address == 0xff02) { return "MON.READ2"; }
    if (address == 0xff0a) { return "MON.RD2"; }
    if (address == 0xff16) { return "MON.RD3"; }
    if (address == 0xff2d) { return "MON.PRERR"; }
    if (address == 0xff3a) { return "MON.BELL"; }
    if (address == 0xff3f) { return "MON.RESTORE"; }
    if (address == 0xff44) { return "MON.RESTR1"; }
    if (address == 0xff4a) { return "MON.SAVE"; }
    if (address == 0xff4c) { return "MON.SAV1"; }

    if (address == 0xff58) { return "M80_IORTS"; }

    if (address == 0xff59) { return "MON.OLDRST"; }
    if (address == 0xff65) { return "MON.MON"; }
    if (address == 0xff69) { return "MON.MONZ"; }
    if (address == 0xff73) { return "MON.NXTITM"; }
    if (address == 0xff7a) { return "MON.CRSRCH"; }
    if (address == 0xff8a) { return "MON.DIG"; }
    if (address == 0xff90) { return "MON.NXTBIT"; }
    if (address == 0xff98) { return "MON.NXTBAS"; }
    if (address == 0xffa2) { return "MON.NXTBS2"; }
    if (address == 0xffa7) { return "MON.GETNUM"; }
    if (address == 0xffad) { return "MON.NXTCHR"; }
    if (address == 0xffbe) { return "MON.TOSUB"; }
    if (address == 0xffc7) { return "MON.ZMODE"; }
    if (address == 0xffcc) { return "MON.CHRTBL"; }
    if (address == 0xffe3) { return "MON.SUBTBL"; }

    return QString::Null();
}

bool DisassemblerViewer::optionsMenuItems(QMenu *menu)
{
    QSettings settings;

    if (!m_wordWrapAction) {
        m_wordWrapAction = new QAction("&Word Wrap");
        m_wordWrapAction->setCheckable(true);
        m_wordWrapAction->setChecked(settings.value("DisassemblerViewer.WordWrap",true).toBool());
        connect(m_wordWrapAction, &QAction::toggled,
                this, &DisassemblerViewer::toggleWordWrap);
    }
    menu->addAction(m_wordWrapAction);

    menu->addSeparator();

    if (!m_showMetadataAction) {
        m_showMetadataAction = new QAction("&Dissassembler Metadata");
        connect(m_showMetadataAction, &QAction::triggered,
                this, &DisassemblerViewer::showMetadataDialog);
    }
    menu->addAction(m_showMetadataAction);

    menu->addSeparator();

    if (!m_setFontAction) {
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
            fontToSettings("DisassemblerViewer.textFont", font);
        }
    });


    return true;
}

void DisassemblerViewer::showMetadataDialog()
{
    if (!m_dmd) {
        m_dmd = new DisassemblerMetadataDialog(m_bfm, this);
    }
    m_dmd->show();
    m_dmd->raise();
}

void DisassemblerViewer::setData(QByteArray data)
{
    ui->textArea->setText(data);
    ui->textArea->setJumpLines(&m_jumpLines);
}


bool DisassemblerViewer::canPrint() const { return true; }

void DisassemblerViewer::doPrint()
{
    QPrinter printer;

    QPrintDialog dialog(&printer, this);
    dialog.setWindowTitle(tr("Print Dissassembly"));
    if (ui->textArea->textCursor().hasSelection())
        dialog.addEnabledOption(QAbstractPrintDialog::PrintSelection);
    if (dialog.exec() != QDialog::Accepted) {
        //      qDebug() << "Cancelled";

        return;
    }

    ui->textArea->print(&printer);
}

bool DisassemblerViewer::canExport() const { return true; }

void DisassemblerViewer::doExport()
{
    QString defaultPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QDir savename = QDir(defaultPath).filePath(m_file->filename()+".txt");

    QString saveName = QFileDialog::getSaveFileName(this,
                                                    tr("Export Disassembly"), savename.path(), tr("Text Files (*.txt)"));

    if (saveName == "") return;  // User cancelled

    //   qDebug() << "Set filename: " << saveName;

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

QString DisassemblerViewer::makeDescriptorStringForVal(quint8 val)
{
    QString retval;

    //    QString zone;
    //    if (val <= 0x3f) zone = "Inverse";
    //    else if (val <= 0x7f) zone = "Flash";
    //    else if (val <= 0x9f) zone = "(Alt) Normal";
    //    else zone = "Normal";

    //    quint8 baseascii = val;
    //    if (val <= 0x1f) baseascii += 0x40;
    //    else if (val <= 0x5f) baseascii += 0;
    //    else if (val <= 0xbf) baseascii -= 0x40;
    //    else  baseascii -= 80;

    //    QString ch = QChar(baseascii);
    //    if (val == 0xff) ch = "[DEL]";

    //    QString appleAscii = QString("%1 %2").arg(ch).arg(zone);

    //    if (val < 0x20)
    //    {
    //        QString ctrl = QString(" / (^%1)").arg(QChar(val+0x40));
    //        appleAscii.append(ctrl);
    //    }

    //    return QString("; %1 / %2").arg(val).arg(appleAscii);
    retval =  QString("; %1").arg(val);
    return retval;
}
