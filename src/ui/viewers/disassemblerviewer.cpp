#include "disassemblerviewer.h"
#include "ui_disassemblerviewer.h"
#include "disassembler.h"
#include "memory.h"
#include "util.h"
#include "relocatablefile.h"

#include <QSettings>
#include <QMenu>
#include <QAction>
#include <QDebug>


DisassemblerViewer::DisassemblerViewer(QWidget *parent) :
    FileViewerInterface(parent),
    ui(new Ui::DisassemblerViewer)
{
    ui->setupUi(this);
    m_isRelo = false;
    m_dmd = Q_NULLPTR;
    m_wordWrapAction = Q_NULLPTR;
    m_showMetadataAction = Q_NULLPTR;

    QString title = QString("Disassembly Viewer");
    setWindowTitle(title);

    QSettings settings;
    toggleWordWrap(settings.value("DisassemblerViewer.WordWrap",true).toBool());
}

DisassemblerViewer::~DisassemblerViewer()
{
    delete ui;
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
}

void DisassemblerViewer::setFile(RelocatableFile *file) {
    m_file = file;
    m_isRelo = true;

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
}

void DisassemblerViewer::handleDisassembleRequest(QList<quint16> addresses)
{
    QStringList strings;

    disassemble(addresses);
    strings += getDisassemblyStrings();
    qSort(strings);
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

    Disassembler dis(m_mem.values());

    int length = m_file->length();

    QList<DisassembledItem> lines = dis.disassemble(m_file->address(),
                                                    m_file->address()+length,
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
                else if (ds.contains("_ARG8_")) { ds.replace("_ARG8_",potentialLabel); }
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



        QString newline = QString("%1:  %2 %3").arg(di.hexAddress()).arg(di.hexString()).arg(ds);
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
            newline = QString("%1:  %2                       %3 (%4)").arg(uint16ToHex(idx))
                              .arg(uint8ToHex(m_mem.at(idx)))
                              .arg(makeDescriptorStringForVal(m_mem.at(idx)))
                              .arg(dis.getMnemonicForOp(m_mem.at(idx)));
            }
            formattedLines.append(newline);
        }
    }
    qSort(formattedLines);

    m_disassemblyStrings = formattedLines;
}



QString DisassemblerViewer::getPotentialLabel(quint16 address) {
    QString retval = QString();

    if      (address == 0x24) { retval = "MON.CURSORHORIZ"; }
    else if (address == 0x28) { retval = "MON.BASL"; }
    else if (address == 0x29) { retval = "MON.BASH"; }
    else if (address == 0x33) { retval = "MON.PROMPTCHAR"; }
    else if (address == 0x36) { retval = "DOS.CSWL"; }
    else if (address == 0x37) { retval = "DOS.CSWH"; }
    else if (address == 0x38) { retval = "DOS.KSWL"; }
    else if (address == 0x39) { retval = "DOS.KSWH"; }
    else if (address == 0x40) { retval = "DOS.FILE_BUFFER_L"; }
    else if (address == 0x41) { retval = "DOS.FILE_BUFFER_H"; }
    else if (address == 0x42) { retval = "DOS.BUFFER_ADDR_L"; }
    else if (address == 0x43) { retval = "DOS.BUFFER_ADDR_H"; }
    else if (address == 0x44) { retval = "DOS.NUMERIC_OPERAND_L"; }
    else if (address == 0x45) { retval = "DOS.NUMERIC_OPERAND_H"; }

    else if (address == 0x67) { retval = "AS.PROG_STARTL"; }
    else if (address == 0x68) { retval = "AS.PROG_STARTH"; }

    else if (address == 0x69) { retval = "AS.VAR_STARTL"; }
    else if (address == 0x6A) { retval = "AS.VAR_STARTH"; }

    else if (address == 0x6B) { retval = "AS.ARRAY_STARTL"; }
    else if (address == 0x6C) { retval = "AS.ARRAY_STARTH"; }

    else if (address == 0x6D) { retval = "AS.NUMSTORE_ENDL"; }
    else if (address == 0x6E) { retval = "AS.NUMSTORE_ENDH"; }

    else if (address == 0x6F) { retval = "AS.STRING_STARTL"; }
    else if (address == 0x70) { retval = "AS.STRING_STARTH"; }

    else if (address == 0x71) { retval = "AS.PTR_9L"; }
    else if (address == 0x72) { retval = "AS.PTR_9H"; }

    else if (address == 0x73) { retval = "AS.HIMEM_L"; }
    else if (address == 0x74) { retval = "AS.HIMEM_H"; }

    else if (address == 0x75) { retval = "AS.CURR_LINENUM_L"; }
    else if (address == 0x76) { retval = "AS.CURR_LINENUM_H"; }

    else if (address == 0x77) { retval = "AS.INTR_LINENUM_L"; }
    else if (address == 0x78) { retval = "AS.INTR_LINENUM_H"; }

    else if (address == 0x79) { retval = "AS.NEXT_STATEMENT_L"; }
    else if (address == 0x7A) { retval = "AS.NEXT_STATEMENT_H"; }

    else if (address == 0x7B) { retval = "AS.DATA_LINENUM_L"; }
    else if (address == 0x7C) { retval = "AS.DATA_LINENUM_H"; }

    else if (address == 0x7D) { retval = "AS.DATA_ADDR_L"; }
    else if (address == 0x7E) { retval = "AS.DATA_ADDR_H"; }

    else if (address == 0x7F) { retval = "AS.INPUT_SRC_L"; }
    else if (address == 0x80) { retval = "AS.INPUT_SRC_H"; }

    else if (address == 0x81) { retval = "AS.LAST_VARNAME_L"; }
    else if (address == 0x82) { retval = "AS.LAST_VARNAME_H"; }

    else if (address == 0x83) { retval = "AS.LAST_VARVAL_L"; }
    else if (address == 0x84) { retval = "AS.LAST_VARVAL_H"; }

    else if (address == 0xAF) { retval = "AS.PROGEND_L"; }
    else if (address == 0xB0) { retval = "AS.PROGEND_H"; }

    else if (address == 0xD6) { retval = "DOS.AS.LOCK"; }

    else if (address == 0xE0) { retval = "AS.HGR_X_L"; }
    else if (address == 0xE1) { retval = "AS.HGR_X_H"; }
    else if (address == 0xE2) { retval = "AS.HGR_Y"; }

    else if (address == 0xE4) { retval = "AS.HGR_COLOR"; }

    else if (address == 0xE8) { retval = "AS.SHAPETBL_L"; }
    else if (address == 0xE9) { retval = "AS.SHAPETBL_H"; }

    else if (address == 0xEA) { retval = "AS.HGR_COLLISION_CTR"; }

    else if (address == 0x03d0) { retval = "DOS.WARMSTART"; }
    else if (address == 0x03d3) { retval = "DOS.COLDSTART"; }
    else if (address == 0x03d6) { retval = "DOS.FILE_MANAGER"; }
    else if (address == 0x03d9) { retval = "DOS.RWTS"; }
    else if (address == 0x03dc) { retval = "DOS.FM_PARAM_LIST_LOCATE"; }
    else if (address == 0x03e3) { retval = "DOS.RWTS_PARAM_LIST_LOCATE"; }
    else if (address == 0x03ea) { retval = "DOS.REPLACE_DOS_INTERCEPTS"; }
    else if (address == 0x03ef) { retval = "DOS.AUTOSTART_BRK_HANDLER"; }
    else if (address == 0x03f2) { retval = "DOS.AUTOSTART_RESET_HANDLER"; }
    else if (address == 0x03f4) { retval = "DOS.POWERUP_BYTE"; }
    else if (address == 0x03f5) { retval = "AMPR_VEC"; }
    else if (address == 0x03f8) { retval = "MON.CTRL_Y_VEC"; }
    else if (address == 0x03fb) { retval = "DOS.NMI_HANDLER"; }
    else if (address == 0x03fe) { retval = "DOS.IRQ_HANDLER"; }

    else if (address == 0x0400) { retval = "MON.LINE1"; }

    else if (address == 0x07f8) { retval = "MON.MSLOT"; }
    else if (address == 0xc000) { retval = "KEYBOARD"; }

    else if (address == 0xc001) { retval = "M80_SET80COL"; }
    else if (address == 0xc002) { retval = "M80_RDMAINRAM"; }
    else if (address == 0xc003) { retval = "M80_RDCARDRAM"; }
    else if (address == 0xc004) { retval = "M80_WRMAINRAM"; }
    else if (address == 0xc005) { retval = "M80_WRCARDRAM"; }

    else if (address == 0xc006) { retval = "MON.SETSLOTCXROM"; }
    else if (address == 0xc007) { retval = "MON.SETINTCXROM"; }

    else if (address == 0xc008) { retval = "M80_SETSTDSZ"; }
    else if (address == 0xc009) { retval = "M80_SETALTZP"; }
    else if (address == 0xc00A) { retval = "M80_SETINTC3ROM"; }
    else if (address == 0xc00B) { retval = "M80_SETSLOTC3ROM"; }
    else if (address == 0xc00C) { retval = "M80_CLR80VID"; }
    else if (address == 0xc00D) { retval = "M80_SET80VID"; }
    else if (address == 0xc00E) { retval = "M80_CLRALTCHAR"; }
    else if (address == 0xc00F) { retval = "M80_SETALTCHAR"; }

    else if (address == 0xc010) { retval = "MON.KBDSTRB"; }

    else if (address == 0xc011) { retval = "M80_RDLCBNK2"; }
    else if (address == 0xc012) { retval = "M80_RDLCRAM"; }
    else if (address == 0xc013) { retval = "M80_RDRAMRD"; }
    else if (address == 0xc014) { retval = "M80_RDRAMWRT"; }

    else if (address == 0xc015) { retval = "RDCXROM"; }
    else if (address == 0xc016) { retval = "RDALTZP"; }
    else if (address == 0xc017) { retval = "RDC3ROM"; }

    else if (address == 0xc018) { retval = "MON.RD80STORE"; }

    else if (address == 0xc019) { retval = "RDVBLBAR"; }
    else if (address == 0xc01A) { retval = "RDTEXT"; }
    else if (address == 0xc01B) { retval = "RDMIXED"; }

    else if (address == 0xc01c) { retval = "RDPAGE2"; }
    else if (address == 0xc01D) { retval = "RDHIRES"; }
    else if (address == 0xc01E) { retval = "RDALTCHAR"; }

    else if (address == 0xc01f) { retval = "RD80VID"; }

    else if (address == 0xc020) { retval = "TAPEOUT"; }

    else if (address == 0xc030) { retval = "SPKR"; }

    else if (address == 0xc040) { retval = "STROBE"; }

    else if (address == 0xc050) { retval = "SW.TXTCLR"; }
    else if (address == 0xc051) { retval = "SW.TXTSET"; }
    else if (address == 0xc052) { retval = "SW.MIXCLR"; }
    else if (address == 0xc053) { retval = "SW.MIXSET"; }
    else if (address == 0xc054) { retval = "SW.LOWSCR"; }
    else if (address == 0xc055) { retval = "SW.HISCR"; }
    else if (address == 0xc056) { retval = "SW.LORES"; }
    else if (address == 0xc057) { retval = "SW.HIRES"; }
    else if (address == 0xc058) { retval = "SW.SETAN0"; }
    else if (address == 0xc059) { retval = "SW.CLRAN0"; }
    else if (address == 0xc05a) { retval = "SW.SETAN1"; }
    else if (address == 0xc05b) { retval = "SW.CLRAN1"; }
    else if (address == 0xc05c) { retval = "SW.SETAN2"; }
    else if (address == 0xc05d) { retval = "SW.CLRAN2"; }
    else if (address == 0xc05e) { retval = "SW.SETAN3"; }
    else if (address == 0xc05f) { retval = "SW.CLRAN3"; }
    else if (address == 0xc060) { retval = "MON.TAPEIN"; }
    else if (address == 0xc064) { retval = "MON.PADDL0"; }
    else if (address == 0xc070) { retval = "MON.PTRIG"; }

    else if (address == 0xc080) { retval = "RDRAMBANK2_NOWRITE"; }
    else if (address == 0xc081) { retval = "RDROM_WRBANK2"; }
    else if (address == 0xc082) { retval = "RDROM_NOWRITE"; }
    else if (address == 0xc083) { retval = "RDWRBANK2"; }
    else if (address == 0xc084) { retval = "RDRAM_NOWRITE"; }
    else if (address == 0xc085) { retval = "READROM_WRBANK2"; }
    else if (address == 0xc086) { retval = "RDROM_NOWRITE"; }
    else if (address == 0xc087) { retval = "RDWRBANK2"; }
    else if (address == 0xc088) { retval = "RDBANK1_NOWRITE"; }
    else if (address == 0xc089) { retval = "RDROM_WRBANK1"; }
    else if (address == 0xc08A) { retval = "RDROM_NOWRITE"; }
    else if (address == 0xc08B) { retval = "RDWRBANK1"; }
    else if (address == 0xc08C) { retval = "RDBANK1_NOWRITE"; }
    else if (address == 0xc08D) { retval = "RDROM_WRBANK1"; }
    else if (address == 0xc08E) { retval = "RDROM_NOWRITE"; }
    else if (address == 0xc08F) { retval = "RDWR_BANK1"; }

    else if (address == 0xc090) { retval = "SLOT1_ROM0"; }
    else if (address == 0xc091) { retval = "SLOT1_ROM1"; }
    else if (address == 0xc092) { retval = "SLOT1_ROM2"; }
    else if (address == 0xc093) { retval = "SLOT1_ROM3"; }
    else if (address == 0xc094) { retval = "SLOT1_ROM4"; }
    else if (address == 0xc095) { retval = "SLOT1_ROM5"; }
    else if (address == 0xc096) { retval = "SLOT1_ROM6"; }
    else if (address == 0xc097) { retval = "SLOT1_ROM7"; }
    else if (address == 0xc098) { retval = "SLOT1_ROM8"; }
    else if (address == 0xc099) { retval = "SLOT1_ROM9"; }
    else if (address == 0xc09A) { retval = "SLOT1_ROMA"; }
    else if (address == 0xc09B) { retval = "SLOT1_ROMB"; }
    else if (address == 0xc09C) { retval = "SLOT1_ROMC"; }
    else if (address == 0xc09D) { retval = "SLOT1_ROMD"; }
    else if (address == 0xc09E) { retval = "SLOT1_ROME"; }
    else if (address == 0xc09F) { retval = "SLOT1_ROMF"; }

    else if (address == 0xc0A0) { retval = "SLOT2_ROM0"; }
    else if (address == 0xc0A1) { retval = "SLOT2_ROM1"; }
    else if (address == 0xc0A2) { retval = "SLOT2_ROM2"; }
    else if (address == 0xc0A3) { retval = "SLOT2_ROM3"; }
    else if (address == 0xc0A4) { retval = "SLOT2_ROM4"; }
    else if (address == 0xc0A5) { retval = "SLOT2_ROM5"; }
    else if (address == 0xc0A6) { retval = "SLOT2_ROM6"; }
    else if (address == 0xc0A7) { retval = "SLOT2_ROM7"; }
    else if (address == 0xc0A8) { retval = "SLOT2_ROM8"; }
    else if (address == 0xc0A9) { retval = "SLOT2_ROM9"; }
    else if (address == 0xc0AA) { retval = "SLOT2_ROMA"; }
    else if (address == 0xc0AB) { retval = "SLOT2_ROMB"; }
    else if (address == 0xc0AC) { retval = "SLOT2_ROMC"; }
    else if (address == 0xc0AD) { retval = "SLOT2_ROMD"; }
    else if (address == 0xc0AE) { retval = "SLOT2_ROME"; }
    else if (address == 0xc0AF) { retval = "SLOT2_ROMF"; }

    else if (address == 0xc0B0) { retval = "SLOT3_ROM0"; }
    else if (address == 0xc0B1) { retval = "SLOT3_ROM1"; }
    else if (address == 0xc0B2) { retval = "SLOT3_ROM2"; }
    else if (address == 0xc0B3) { retval = "SLOT3_ROM3"; }
    else if (address == 0xc0B4) { retval = "SLOT3_ROM4"; }
    else if (address == 0xc0B5) { retval = "SLOT3_ROM5"; }
    else if (address == 0xc0B6) { retval = "SLOT3_ROM6"; }
    else if (address == 0xc0B7) { retval = "SLOT3_ROM7"; }
    else if (address == 0xc0B8) { retval = "SLOT3_ROM8"; }
    else if (address == 0xc0B9) { retval = "SLOT3_ROM9"; }
    else if (address == 0xc0BA) { retval = "SLOT3_ROMA"; }
    else if (address == 0xc0BB) { retval = "SLOT3_ROMB"; }
    else if (address == 0xc0BC) { retval = "SLOT3_ROMC"; }
    else if (address == 0xc0BD) { retval = "SLOT3_ROMD"; }
    else if (address == 0xc0BE) { retval = "SLOT3_ROME"; }
    else if (address == 0xc0BF) { retval = "SLOT3_ROMF"; }

    else if (address == 0xc0C0) { retval = "SLOT4_ROM0"; }
    else if (address == 0xc0C1) { retval = "SLOT4_ROM1"; }
    else if (address == 0xc0C2) { retval = "SLOT4_ROM2"; }
    else if (address == 0xc0C3) { retval = "SLOT4_ROM3"; }
    else if (address == 0xc0C4) { retval = "SLOT4_ROM4"; }
    else if (address == 0xc0C5) { retval = "SLOT4_ROM5"; }
    else if (address == 0xc0C6) { retval = "SLOT4_ROM6"; }
    else if (address == 0xc0C7) { retval = "SLOT4_ROM7"; }
    else if (address == 0xc0C8) { retval = "SLOT4_ROM8"; }
    else if (address == 0xc0C9) { retval = "SLOT4_ROM9"; }
    else if (address == 0xc0CA) { retval = "SLOT4_ROMA"; }
    else if (address == 0xc0CB) { retval = "SLOT4_ROMB"; }
    else if (address == 0xc0CC) { retval = "SLOT4_ROMC"; }
    else if (address == 0xc0CD) { retval = "SLOT4_ROMD"; }
    else if (address == 0xc0CE) { retval = "SLOT4_ROME"; }
    else if (address == 0xc0CF) { retval = "SLOT4_ROMF"; }

    else if (address == 0xc0D0) { retval = "SLOT5_ROM0"; }
    else if (address == 0xc0D1) { retval = "SLOT5_ROM1"; }
    else if (address == 0xc0D2) { retval = "SLOT5_ROM2"; }
    else if (address == 0xc0D3) { retval = "SLOT5_ROM3"; }
    else if (address == 0xc0D4) { retval = "SLOT5_ROM4"; }
    else if (address == 0xc0D5) { retval = "SLOT5_ROM5"; }
    else if (address == 0xc0D6) { retval = "SLOT5_ROM6"; }
    else if (address == 0xc0D7) { retval = "SLOT5_ROM7"; }
    else if (address == 0xc0D8) { retval = "SLOT5_ROM8"; }
    else if (address == 0xc0D9) { retval = "SLOT5_ROM9"; }
    else if (address == 0xc0DA) { retval = "SLOT5_ROMA"; }
    else if (address == 0xc0DB) { retval = "SLOT5_ROMB"; }
    else if (address == 0xc0DC) { retval = "SLOT5_ROMC"; }
    else if (address == 0xc0DD) { retval = "SLOT5_ROMD"; }
    else if (address == 0xc0DE) { retval = "SLOT5_ROME"; }
    else if (address == 0xc0DF) { retval = "SLOT5_ROMF"; }

    else if (address == 0xc0E0) { retval = "SLOT6_ROM0"; }
    else if (address == 0xc0E1) { retval = "SLOT6_ROM1"; }
    else if (address == 0xc0E2) { retval = "SLOT6_ROM2"; }
    else if (address == 0xc0E3) { retval = "SLOT6_ROM3"; }
    else if (address == 0xc0E4) { retval = "SLOT6_ROM4"; }
    else if (address == 0xc0E5) { retval = "SLOT6_ROM5"; }
    else if (address == 0xc0E6) { retval = "SLOT6_ROM6"; }
    else if (address == 0xc0E7) { retval = "SLOT6_ROM7"; }
    else if (address == 0xc0E8) { retval = "SLOT6_ROM8"; }
    else if (address == 0xc0E9) { retval = "SLOT6_ROM9"; }
    else if (address == 0xc0EA) { retval = "SLOT6_ROMA"; }
    else if (address == 0xc0EB) { retval = "SLOT6_ROMB"; }
    else if (address == 0xc0EC) { retval = "SLOT6_ROMC"; }
    else if (address == 0xc0ED) { retval = "SLOT6_ROMD"; }
    else if (address == 0xc0EE) { retval = "SLOT6_ROME"; }
    else if (address == 0xc0EF) { retval = "SLOT6_ROMF"; }

    else if (address == 0xc0F0) { retval = "SLOT7_ROM0"; }
    else if (address == 0xc0F1) { retval = "SLOT7_ROM1"; }
    else if (address == 0xc0F2) { retval = "SLOT7_ROM2"; }
    else if (address == 0xc0F3) { retval = "SLOT7_ROM3"; }
    else if (address == 0xc0F4) { retval = "SLOT7_ROM4"; }
    else if (address == 0xc0F5) { retval = "SLOT7_ROM5"; }
    else if (address == 0xc0F6) { retval = "SLOT7_ROM6"; }
    else if (address == 0xc0F7) { retval = "SLOT7_ROM7"; }
    else if (address == 0xc0F8) { retval = "SLOT7_ROM8"; }
    else if (address == 0xc0F9) { retval = "SLOT7_ROM9"; }
    else if (address == 0xc0FA) { retval = "SLOT7_ROMA"; }
    else if (address == 0xc0FB) { retval = "SLOT7_ROMB"; }
    else if (address == 0xc0FC) { retval = "SLOT7_ROMC"; }
    else if (address == 0xc0FD) { retval = "SLOT7_ROMD"; }
    else if (address == 0xc0FE) { retval = "SLOT7_ROME"; }
    else if (address == 0xc0FF) { retval = "SLOT7_ROMF"; }

    else if (address == 0xc100) { retval = "M80_BFUNCPG"; }
    else if (address == 0xc107) { retval = "M80_B.FUNCK"; }
    else if (address == 0xc10E) { retval = "M80_B.FUNCNE"; }
    else if (address == 0xc11f) { retval = "M80_B.OLDFUNC"; }
    else if (address == 0xc129) { retval = "M80_F.CLREOP"; }
    else if (address == 0xc12d) { retval = "M80_CLEOP1"; }
    else if (address == 0xc143) { retval = "M80_F.HOME"; }
    else if (address == 0xc14d) { retval = "M80_F.SCROLL"; }
    else if (address == 0xc153) { retval = "M80_SCRL1"; }
    else if (address == 0xc169) { retval = "M80_SCRL2"; }
    else if (address == 0xc172) { retval = "M80_SCRL3"; }
    else if (address == 0xc17d) { retval = "M80_F.CLREOL"; }
    else if (address == 0xc181) { retval = "M80_CLEOL2"; }
    else if (address == 0xc18a) { retval = "M80_F.SETWND"; }
    else if (address == 0xc19c) { retval = "M80_F.CLEOLZ"; }
    else if (address == 0xc1a1) { retval = "M80_F.GORET"; }
    else if (address == 0xc1a4) { retval = "M80_B.FUNCO"; }
    else if (address == 0xc1c5) { retval = "M80_NOI"; }
    else if (address == 0xc1cd) { retval = "M80_B.SCROLL"; }
    else if (address == 0xc1d3) { retval = "M80_B.CLREOL"; }
    else if (address == 0xc1d9) { retval = "M80_B.CLEOLZ"; }
    else if (address == 0xc1e1) { retval = "M80_B.CLREOP"; }
    else if (address == 0xc1e7) { retval = "M80_B.SETWND"; }
    else if (address == 0xc1ea) { retval = "M80_B.RESET"; }
    else if (address == 0xc1ed) { retval = "M80_B.HOME"; }
    else if (address == 0xc1ff) { retval = "M80_B.VECTOR"; }
    else if (address == 0xc20e) { retval = "M80_B.GETCH"; }
    else if (address == 0xc211) { retval = "M80_B.FUNC1"; }
    else if (address == 0xc219) { retval = "M80_B.SETWNDX"; }
    else if (address == 0xc221) { retval = "M80_B.SETWND2"; }
    else if (address == 0xc22e) { retval = "M80_GOBACK"; }
    else if (address == 0xc234) { retval = "M80_B.RESETX"; }
    else if (address == 0xc252) { retval = "M80_BLAST"; }
    else if (address == 0xc261) { retval = "M80_DIAGS"; }
    else if (address == 0xc264) { retval = "M80_RESETRET"; }
    else if (address == 0xc26e) { retval = "M80_B.ESCFIX"; }
    else if (address == 0xc272) { retval = "M80_B.ESCFIX2"; }
    else if (address == 0xc27a) { retval = "M80_B.ESCFIX3"; }
    else if (address == 0xc27d) { retval = "M80_GORETN"; }
    else if (address == 0xc280) { retval = "M80_ESCIN"; }
    else if (address == 0xc284) { retval = "M80_ESCOUT"; }
    else if (address == 0xc288) { retval = "M80_B.KEYIN"; }
    else if (address == 0xc29c) { retval = "M80_B.KEYIN2"; }
    else if (address == 0xc2b5) { retval = "M80_GOTKEY"; }
    else if (address == 0xc2c6) { retval = "M80_KEYDLY"; }
    else if (address == 0xc2cc) { retval = "M80_IK1"; }
    else if (address == 0xc2ce) { retval = "M80_IK2"; }
    else if (address == 0xc2d5) { retval = "M80_IK2A"; }
    else if (address == 0xc2db) { retval = "M80_IK3"; }
    else if (address == 0xc2e6) { retval = "M80_KDRETN"; }
    else if (address == 0xc2e9) { retval = "M80_KDRETY"; }
    else if (address == 0xc2ea) { retval = "M80_KDRET"; }
    else if (address == 0xc2eb) { retval = "M80_F.RETURN"; }
    else if (address == 0xc2f1) { retval = "M80_F.RET1"; }
    else if (address == 0xc2f4) { retval = "M80_X.CLEOLZ"; }
    else if (address == 0xc2f6) { retval = "M80_X.CLEOL2"; }
    else if (address == 0xc300) { retval = "M80_CN00/M80_BASICINT"; }
    else if (address == 0xc305) { retval = "M80_BASICIN"; }
    else if (address == 0xc307) { retval = "M80_BASICOUT"; }
    else if (address == 0xc317) { retval = "M80_BASICENT"; }
    else if (address == 0xc336) { retval = "M80_BASICENT2"; }
    else if (address == 0xc348) { retval = "M80_JBASINIT"; }
    else if (address == 0xc34b) { retval = "M80_JPINIT"; }
    else if (address == 0xc351) { retval = "M80_JPREAD"; }
    else if (address == 0xc357) { retval = "M80_JPWRITE"; }
    else if (address == 0xc35d) { retval = "M80_JPSTAT"; }
    else if (address == 0xc363) { retval = "M80_MOVE"; }
    else if (address == 0xc378) { retval = "M80_MOVEC2M"; }
    else if (address == 0xc37b) { retval = "M80_MOVESTRT"; }
    else if (address == 0xc380) { retval = "M80_MOVELOOP"; }
    else if (address == 0xc38a) { retval = "M80_NXTA1"; }
    else if (address == 0xc398) { retval = "M80_C01"; }
    else if (address == 0xc3a3) { retval = "M80_C03"; }
    else if (address == 0xc3ac) { retval = "M80_MOVERET"; }
    else if (address == 0xc3b0) { retval = "M80_XFER"; }
    else if (address == 0xc3c5) { retval = "M80_XFERC2M"; }
    else if (address == 0xc3cd) { retval = "M80_XFERAZP"; }
    else if (address == 0xc3dc) { retval = "M80_XFERSZP"; }
    else if (address == 0xc3eb) { retval = "M80_SETCB"; }
    else if (address == 0xc803) { retval = "M80_BASICINIT"; }
    else if (address == 0xc813) { retval = "M80_HANG"; }
    else if (address == 0xc816) { retval = "M80_BINIT1"; }
    else if (address == 0xc831) { retval = "M80_BINIT1A"; }
    else if (address == 0xc850) { retval = "M80_BINIT2"; }
    else if (address == 0xc85d) { retval = "M80_CLEARIT"; }
    else if (address == 0xc866) { retval = "M80_C8BASIC"; }
    else if (address == 0xc874) { retval = "M80_C8B2"; }
    else if (address == 0xc87e) { retval = "M80_C8B3"; }
    else if (address == 0xc890) { retval = "M80_C8B4"; }
    else if (address == 0xc896) { retval = "M80_BOUT"; }
    else if (address == 0xc8a1) { retval = "M80_BPRINT"; }
    else if (address == 0xc8b4) { retval = "M80_KBDWAIT"; }
    else if (address == 0xc8c0) { retval = "M80_NOWAIT"; }
    else if (address == 0xc8cc) { retval = "M80_BPNCTL"; }
    else if (address == 0xc8e2) { retval = "M80_BIORET"; }
    else if (address == 0xc8f6) { retval = "M80_BINPUT"; }
    else if (address == 0xc905) { retval = "M80_B.INPUT"; }
    else if (address == 0xc918) { retval = "M80_ESCAPING"; }
    else if (address == 0xc929) { retval = "M80_ESC1"; }
    else if (address == 0xc92b) { retval = "M80_ESC2"; }
    else if (address == 0xc935) { retval = "M80_ESC3"; }
    else if (address == 0xc945) { retval = "M80_ESCSPEC"; }
    else if (address == 0xc954) { retval = "M80_ESCSPEC2"; }
    else if (address == 0xc960) { retval = "M80_ESCNONE"; }
    else if (address == 0xc963) { retval = "M80_ESCSPEC3"; }
    else if (address == 0xc972) { retval = "M80_ESCTAB"; }
    else if (address == 0xc983) { retval = "M80_ESCCHAR"; }
    else if (address == 0xc994) { retval = "M80_PSTATUS"; }
    else if (address == 0xc99e) { retval = "M80_PSTATUS2"; }
    else if (address == 0xc9b0) { retval = "M80_PSTATUS3"; }
    else if (address == 0xc9b4) { retval = "M80_PSTATUS4"; }
    else if (address == 0xc9b7) { retval = "M80_NOESC"; }
    else if (address == 0xc9c6) { retval = "M80_B.NOPICK"; }
    else if (address == 0xc9df) { retval = "M80_B.CHKCAN"; }
    else if (address == 0xc9f7) { retval = "M80_B.FLIP"; }
    else if (address == 0xca02) { retval = "M80_B.CANLIT"; }
    else if (address == 0xca0a) { retval = "M80_B.FIXCHR"; }
    else if (address == 0xca24) { retval = "M80_B.INRET"; }
    else if (address == 0xca27) { retval = "M80_GETPRIOR"; }
    else if (address == 0xca49) { retval = "M80_GPX"; }
    else if (address == 0xca4a) { retval = "M80_PREAD"; }
    else if (address == 0xca4f) { retval = "M80_PREADRET2"; }
    else if (address == 0xca51) { retval = "M80_PWRITE"; }
    else if (address == 0xca62) { retval = "M80_PIGOOD"; }
    else if (address == 0xca74) { retval = "M80_PREAD"; }
    else if (address == 0xca8a) { retval = "M80_PREADRET2"; }
    else if (address == 0xca8e) { retval = "M80_PWRITE"; }
    else if (address == 0xca9e) { retval = "M80_PWRITE2"; }
    else if (address == 0xcaaf) { retval = "M80_GETY"; }
    else if (address == 0xcacb) { retval = "M80_PWRITE3"; }
    else if (address == 0xcadc) { retval = "M80_STARTXY"; }
    else if (address == 0xcaeb) { retval = "M80_PWRITE4"; }
    else if (address == 0xcb09) { retval = "M80_PWWRAP"; }
    else if (address == 0xcb0f) { retval = "M80_PWRITERET"; }
    else if (address == 0xcb15) { retval = "M80_GETKEY"; }
    else if (address == 0xcb1b) { retval = "M80_GETK2"; }
    else if (address == 0xcb24) { retval = "M80_TESTCARD"; }
    else if (address == 0xcb48) { retval = "M80_STAY2"; }
    else if (address == 0xcb4d) { retval = "M80_STAY80"; }
    else if (address == 0xcb4e) { retval = "M80_TESTFAIL"; }
    else if (address == 0xcb51) { retval = "M80_BASCALC"; }
    else if (address == 0xcb54) { retval = "M80_BASCALCZ"; }
    else if (address == 0xcb55) { retval = "M80_BSCLC1"; }
    else if (address == 0xcb5b) { retval = "M80_BSCLC1A"; }
    else if (address == 0xcb6d) { retval = "M80_BSCLC2"; }
    else if (address == 0xcb7e) { retval = "M80_BASCLC3"; }
    else if (address == 0xcb97) { retval = "M80_BASCLCX"; }
    else if (address == 0xcb99) { retval = "M80_CTLCHAR"; }
    else if (address == 0xcbab) { retval = "M80_CTLCHARX"; }
    else if (address == 0xcbae) { retval = "M80_CTLGO"; }
    else if (address == 0xcbb2) { retval = "M80_CTLRET"; }
    else if (address == 0xcbb6) { retval = "M80_CTLXFER"; }
    else if (address == 0xcbbc) { retval = "M80_X.BELL"; }
    else if (address == 0xcbce) { retval = "M80_BELL2"; }
    else if (address == 0xcbcf) { retval = "M80_WAIT"; }
    else if (address == 0xcbd0) { retval = "M80_WAIT2"; }
    else if (address == 0xcbd1) { retval = "M80_WAIT3"; }
    else if (address == 0xcbdb) { retval = "M80_X.BS"; }
    else if (address == 0xcbe2) { retval = "M80_BS40"; }
    else if (address == 0xcbeb) { retval = "M80_BSDONE"; }
    else if (address == 0xcbec) { retval = "M80_X.CR"; }
    else if (address == 0xcbfd) { retval = "M80_X.CRPAS"; }
    else if (address == 0xcc0c) { retval = "M80_X.CRRET"; }
    else if (address == 0xcc0d) { retval = "M80_X.EM"; }
    else if (address == 0xcc1a) { retval = "M80_X.SUB"; }
    else if (address == 0xcc1d) { retval = "M80_X.SUB80"; }
    else if (address == 0xcc1f) { retval = "M80_X.SUBLP"; }
    else if (address == 0xcc26) { retval = "M80_X.FS"; }
    else if (address == 0xcc33) { retval = "M80_X.FSRET"; }
    else if (address == 0xcc34) { retval = "M80_X.US"; }
    else if (address == 0xcc40) { retval = "M80_X.US1"; }
    else if (address == 0xcc45) { retval = "M80_X.US2"; }
    else if (address == 0xcc48) { retval = "M80_X.USRET"; }
    else if (address == 0xcc49) { retval = "M80_X.SO"; }
    else if (address == 0xcc52) { retval = "M80_X.SI"; }
    else if (address == 0xcc59) { retval = "M80_STUFFINV"; }
    else if (address == 0xcc5f) { retval = "M80_CTLADL"; }
    else if (address == 0xcc78) { retval = "M80_CTLADH"; }
    else if (address == 0xcc91) { retval = "M80_X.LF"; }
    else if (address == 0xcc9e) { retval = "M80_X.LF2"; }
    else if (address == 0xcca4) { retval = "M80_SCROLLUP"; }
    else if (address == 0xccaa) { retval = "M80_SCROLLDN"; }
    else if (address == 0xccae) { retval = "M80_SCROLL1"; }
    else if (address == 0xccb8) { retval = "M80_SCROLL2"; }
    else if (address == 0xccd1) { retval = "M80_SCRLSUB"; }
    else if (address == 0xccdd) { retval = "M80_MSCROL0"; }
    else if (address == 0xcce1) { retval = "M80_MSCROL1"; }
    else if (address == 0xccf9) { retval = "M80_MSCRL2"; }
    else if (address == 0xcd02) { retval = "M80_MSCRLRET"; }
    else if (address == 0xcd09) { retval = "M80_ONEMORE"; }
    else if (address == 0xcd10) { retval = "M80_MSCRLRTS"; }
    else if (address == 0xcd11) { retval = "M80_X.SCRLRET"; }
    else if (address == 0xcd17) { retval = "M80_X.SCRLRET2"; }
    else if (address == 0xcd20) { retval = "M80_X.LFRET"; }
    else if (address == 0xcd23) { retval = "M80_X.VT"; }
    else if (address == 0xcd2c) { retval = "M80_X.VTLOOP"; }
    else if (address == 0xcd32) { retval = "M80_X.VTNEXT"; }
    else if (address == 0xcd42) { retval = "M80_X.FF"; }
    else if (address == 0xcd48) { retval = "M80_X.GS"; }
    else if (address == 0xcd4e) { retval = "M80_X.GSEOLZ"; }
    else if (address == 0xcd54) { retval = "M80_X.GS2"; }
    else if (address == 0xcd59) { retval = "M80_X.DC1"; }
    else if (address == 0xcd64) { retval = "M80_X.DC1B"; }
    else if (address == 0xcd76) { retval = "M80_X.DC1RTS"; }
    else if (address == 0xcd77) { retval = "M80_X.DC2"; }
    else if (address == 0xcd88) { retval = "M80_X.DC2B"; }
    else if (address == 0xcd90) { retval = "M80_X.NAK"; }
    else if (address == 0xcd9a) { retval = "M80_X.NAKRET/M80_DC2RET"; }
    else if (address == 0xcd9b) { retval = "M80_FULL80"; }
    else if (address == 0xcdaa) { retval = "M80_QUIT"; }
    else if (address == 0xcdc0) { retval = "M80_QUIT2"; }
    else if (address == 0xcddb) { retval = "M80_SCRN84"; }
    else if (address == 0xcdea) { retval = "M80_SCR40"; }
    else if (address == 0xce01) { retval = "M80_SCR40RET"; }
    else if (address == 0xce0a) { retval = "M80_ATEFOR"; }
    else if (address == 0xce13) { retval = "M80_ATEFOR1"; }
    else if (address == 0xce22) { retval = "M80_GET84"; }
    else if (address == 0xce32) { retval = "M80_SCRN48"; }
    else if (address == 0xce3e) { retval = "M80_SCR80"; }
    else if (address == 0xce55) { retval = "M80_SCR80RET"; }
    else if (address == 0xce58) { retval = "M80_SCRNRET"; }
    else if (address == 0xce63) { retval = "M80_FORATE"; }
    else if (address == 0xce6f) { retval = "M80_FORATE1"; }
    else if (address == 0xce91) { retval = "M80_CLRHALF"; }
    else if (address == 0xce9b) { retval = "M80_CLRHALF2"; }
    else if (address == 0xcea3) { retval = "M80_DO48"; }
    else if (address == 0xceaf) { retval = "M80_SETCH"; }
    else if (address == 0xced9) { retval = "M80_SETCHRTS"; }
    else if (address == 0xcedd) { retval = "M80_INVERT"; }
    else if (address == 0xcef2) { retval = "M80_STORCHAR"; }
    else if (address == 0xcef9) { retval = "M80_STOR2"; }
    else if (address == 0xcf00) { retval = "M80_SEV"; }
    else if (address == 0xcf01) { retval = "M80_PICK"; }
    else if (address == 0xcf06) { retval = "M80_SCREENIT"; }
    else if (address == 0xcf1e) { retval = "M80_SCRN2"; }
    else if (address == 0xcf2a) { retval = "M80_STOR80"; }
    else if (address == 0xcf37) { retval = "M80_SCRN3"; }
    else if (address == 0xcf40) { retval = "M80_SCRN40"; }
    else if (address == 0xcf4a) { retval = "M80_STOR40"; }
    else if (address == 0xcf4e) { retval = "M80_STPKEXIT"; }
    else if (address == 0xcf52) { retval = "M80_ESCON"; }
    else if (address == 0xcf65) { retval = "M80_ESCOFF"; }
    else if (address == 0xcf6e) { retval = "M80_ESCRET"; }
    else if (address == 0xcf78) { retval = "M80_COPYROM"; }
    else if (address == 0xcf95) { retval = "M80_COPYROM2"; }
    else if (address == 0xcfb3) { retval = "M80_LCB2ROM"; }
    else if (address == 0xcfb9) { retval = "M80_LCB1"; }
    else if (address == 0xcfc2) { retval = "M80_LCB1ROM"; }
    else if (address == 0xcfc5) { retval = "M80_COPYRET"; }
    else if (address == 0xcfc8) { retval = "M80_PSETUP"; }
    else if (address == 0xcfd2) { retval = "M80_PSETUP2"; }
    else if (address == 0xcfdf) { retval = "M80_PSETUPRET"; }
    else if (address == 0xcfea) { retval = "M80_F.TABLE"; }
    else if (address == 0xcff0) { retval = "M80_PLUSMINUS1"; }
    else if (address == 0xcff3) { retval = "M80_B.TABLE"; }
    else if (address == 0xcff9) { retval = "M80_WNDTAB"; }
    else if (address == 0xcffd) { retval = "M80_ZZEND"; }


    else if (address == 0xcfff) { retval = "MON.CLRROM"; }
    else if (address == 0xe000) { retval = "MON.BASIC"; }
    else if (address == 0xe003) { retval = "MON.BASIC2"; }

    else if (address == 0xd365) { retval = "AS.GTFORPNT"; }
    else if (address == 0xd39e) { retval = "AS.BLTU"; }
    else if (address == 0xd39a) { retval = "AS.BLTU2"; }
    else if (address == 0xd3d6) { retval = "AS.CHKMEM"; }
    else if (address == 0xd3e3) { retval = "AS.REASON"; }
    else if (address == 0xd410) { retval = "AS.MEMERR"; }
    else if (address == 0xd412) { retval = "AS.ERROR"; }
    else if (address == 0xd431) { retval = "AS.PRINT_ERROR_LINNUM"; }
    else if (address == 0xd43c) { retval = "AS.RESTART"; }
    else if (address == 0xd45c) { retval = "AS.NUMBERED_LINE"; }
    else if (address == 0xd4b5) { retval = "AS.PUT_NEW_LINE"; }
    else if (address == 0xd4f2) { retval = "AS.FIX_LINKS"; }
    else if (address == 0xd52c) { retval = "AS.INLIN"; }
    else if (address == 0xd52e) { retval = "AS.INLIN2"; }
    else if (address == 0xd553) { retval = "AS.INCHR"; }
    else if (address == 0xd559) { retval = "AS.PARSE_INPUT_LINE"; }
    else if (address == 0xd56c) { retval = "AS.PARSE"; }
    else if (address == 0xd61a) { retval = "AS.FNDLIN"; }
    else if (address == 0xd61e) { retval = "AS.FL1"; }
    else if (address == 0xd648) { retval = "AS.RTS1"; }
    else if (address == 0xd649) { retval = "AS.NEW"; }
    else if (address == 0xd64b) { retval = "AS.SCRTCH"; }
    else if (address == 0xd665) { retval = "AS.SETPTRS"; }
    else if (address == 0xd66a) { retval = "AS.CLEAR"; }
    else if (address == 0xd66c) { retval = "AS.CLEARC"; }
    else if (address == 0xd683) { retval = "AS.STKINI"; }
    else if (address == 0xd696) { retval = "AS.RTS2"; }
    else if (address == 0xd697) { retval = "AS.STXTPT"; }
    else if (address == 0xd6a5) { retval = "AS.LIST"; }
    else if (address == 0xd6da) { retval = "AS.LIST0"; }
    else if (address == 0xd6fe) { retval = "AS.LIST1"; }
    else if (address == 0xd702) { retval = "AS.LIST2"; }
    else if (address == 0xd724) { retval = "AS.LIST3"; }
    else if (address == 0xd72c) { retval = "AS.GETCHR"; }
    else if (address == 0xd734) { retval = "AS.LIST4"; }
    else if (address == 0xd766) { retval = "AS.FOR"; }
    else if (address == 0xd7af) { retval = "AS.STEP"; }
    else if (address == 0xd7d2) { retval = "AS.NEWSTT"; }
    else if (address == 0xd805) { retval = "AS.TRACE_"; }
    else if (address == 0xd826) { retval = "AS.GOEND"; }
    else if (address == 0xd828) { retval = "AS.EXECUTE_STATEMENT"; }
    else if (address == 0xd82a) { retval = "AS.EXECUTE_STATEMENT1"; }
    else if (address == 0xd842) { retval = "AS.COLON_"; }
    else if (address == 0xd846) { retval = "AS.SYNERR1"; }
    else if (address == 0xd849) { retval = "AS.RESTORE"; }
    else if (address == 0xd853) { retval = "AS.SETDA"; }
    else if (address == 0xd857) { retval = "AS.RTS3"; }
    else if (address == 0xd858) { retval = "AS.ISCNTC"; }
    else if (address == 0xd863) { retval = "AS.CONTROL_C_TYPED"; }
    else if (address == 0xd86e) { retval = "AS.STOP"; }
    else if (address == 0xd870) { retval = "AS.END"; }
    else if (address == 0xd871) { retval = "AS.END2"; }
    else if (address == 0xd88a) { retval = "AS.END4"; }
    else if (address == 0xd896) { retval = "AS.CONT"; }
    else if (address == 0xd8af) { retval = "AS.RTS4"; }
    else if (address == 0xd8b0) { retval = "AS.SAVE"; }
    else if (address == 0xd8c9) { retval = "AS.LOAD"; }
    else if (address == 0xd8F0) { retval = "AS.VARTIO"; }
    else if (address == 0xd901) { retval = "AS.PROGIO"; }
    else if (address == 0xd912) { retval = "AS.RUN"; }
    else if (address == 0xd921) { retval = "AS.GOSUB"; }
    else if (address == 0xd935) { retval = "AS.GO_TO_LINE"; }
    else if (address == 0xd93e) { retval = "AS.GOTO"; }
    else if (address == 0xd96a) { retval = "AS.RTS5"; }
    else if (address == 0xd96b) { retval = "AS.POP"; }
    else if (address == 0xd97c) { retval = "AS.UNDERR"; }
    else if (address == 0xd981) { retval = "AS.SYNERR2"; }
    else if (address == 0xd984) { retval = "AS.RETURN"; }
    else if (address == 0xd995) { retval = "AS.DATA"; }
    else if (address == 0xd998) { retval = "AS.ADDON"; }
    else if (address == 0xd9a2) { retval = "AS.RTS6"; }
    else if (address == 0xd9a3) { retval = "AS.DATAN"; }
    else if (address == 0xd9a6) { retval = "AS.REMN"; }
    else if (address == 0xd9c5) { retval = "AS.PULL3"; }
    else if (address == 0xd9c9) { retval = "AS.IF"; }
    else if (address == 0xd9dc) { retval = "AS.REM"; }
    else if (address == 0xd9e1) { retval = "AS.IFTRUE"; }
    else if (address == 0xd9ec) { retval = "AS.ONGOTO"; }
    else if (address == 0xd9f4) { retval = "AS.ON1"; }
    else if (address == 0xd9f8) { retval = "AS.ON2"; }
    else if (address == 0xda0b) { retval = "AS.RTS7"; }
    else if (address == 0xda0c) { retval = "AS.LINGET"; }
    else if (address == 0xda46) { retval = "AS.LET"; }
    else if (address == 0xda63) { retval = "AS.LET2"; }
    else if (address == 0xda7a) { retval = "AS.LET_STRING"; }
    else if (address == 0xda7b) { retval = "AS.PUTSTR"; }
    else if (address == 0xdacf) { retval = "AS.PR_SPRING"; }
    else if (address == 0xdad5) { retval = "AS.PRINT"; }
    else if (address == 0xdad7) { retval = "AS.PRINT2"; }
    else if (address == 0xdafb) { retval = "AS.CRDO"; }
    else if (address == 0xdb00) { retval = "AS.NEGATE"; }
    else if (address == 0xdb02) { retval = "AS.PR_RTS8"; }
    else if (address == 0xdb03) { retval = "AS.PR_COMMA"; }
    else if (address == 0xdb16) { retval = "AS.PR_TAB_OR_SPC"; }
    else if (address == 0xdb2c) { retval = "AS.NXSPC"; }
    else if (address == 0xdb2f) { retval = "AS.PR_NEXT_CHAR"; }
    else if (address == 0xdb35) { retval = "AS.DOSPC"; }
    else if (address == 0xdb3a) { retval = "AS.STROUT"; }
    else if (address == 0xdb3d) { retval = "AS.STRPRT"; }
    else if (address == 0xdb57) { retval = "AS.OUTSP"; }
    else if (address == 0xdb5a) { retval = "AS.OUTQUES"; }
    else if (address == 0xdb5c) { retval = "AS.OUTDO"; }
    else if (address == 0xdb71) { retval = "AS.INPUTERR"; }
    else if (address == 0xdb7b) { retval = "AS.READERR"; }
    else if (address == 0xdb7f) { retval = "AS.ERLIN"; }
    else if (address == 0xdb86) { retval = "AS.INPERR"; }
    else if (address == 0xdb87) { retval = "AS.RESPERR"; }
    else if (address == 0xdba0) { retval = "AS.GET"; }
    else if (address == 0xdbb2) { retval = "AS.INPUT"; }
    else if (address == 0xdbdc) { retval = "AS.NXIN"; }
    else if (address == 0xdbe2) { retval = "AS.READ"; }
    else if (address == 0xdbe9) { retval = "AS.INPUT_FLAG_ZERO"; }
    else if (address == 0xdbeb) { retval = "AS.PROCESS_INPUT_LIST"; }
    else if (address == 0xdbf1) { retval = "AS.PROCESS_INPUT_ITEM"; }
    else if (address == 0xdC2b) { retval = "AS.INSTART"; }
    else if (address == 0xdC69) { retval = "AS.INPUT_DATA"; }
    else if (address == 0xdC72) { retval = "AS.INPUT_MORE"; }
    else if (address == 0xdC99) { retval = "AS.INPFIN"; }
    else if (address == 0xdCa0) { retval = "AS.FINDATA"; }
    else if (address == 0xdCC6) { retval = "AS.INPDONE"; }
    else if (address == 0xdcdf) { retval = "AS.ERR_EXTRA"; }
    else if (address == 0xdcef) { retval = "AS.ERR_REENTRY"; }
    else if (address == 0xdcf9) { retval = "AS.NEXT"; }
    else if (address == 0xdcff) { retval = "AS.NEXT1"; }
    else if (address == 0xdd02) { retval = "AS.NEXT2"; }
    else if (address == 0xdd0d) { retval = "AS.GERR"; }
    else if (address == 0xdd0f) { retval = "AS.NEXT3"; }
    else if (address == 0xdd67) { retval = "AS.FRMNUM"; }
    else if (address == 0xdd6a) { retval = "AS.CHKNUM"; }
    else if (address == 0xdd6c) { retval = "AS.CHKSTR"; }
    else if (address == 0xdd6d) { retval = "AS.CHKVAL"; }
    else if (address == 0xdd78) { retval = "AS.JERROR"; }
    else if (address == 0xdd7b) { retval = "AS.FRMEVL"; }
    else if (address == 0xdd86) { retval = "AS.FRMEVL1"; }
    else if (address == 0xdd95) { retval = "AS.FRMEVL2"; }
    else if (address == 0xddcd) { retval = "AS.FRM_PRECEDENCE_TEST"; }
    else if (address == 0xddd6) { retval = "AS.NXOP"; }
    else if (address == 0xddd7) { retval = "AS.SAVOP"; }
    else if (address == 0xdde4) { retval = "AS.FRM_RELATIONAL"; }
    else if (address == 0xddf6) { retval = "AS.FRM_PREFNC"; }
    else if (address == 0xddfd) { retval = "AS.FRM_RECURSE"; }
    else if (address == 0xde0d) { retval = "AS.SNTXERR"; }
    else if (address == 0xde10) { retval = "AS.FRM_STACK1"; }
    else if (address == 0xde15) { retval = "AS.FRM_STACK2"; }
    else if (address == 0xde20) { retval = "AS.FRM_STACK3"; }
    else if (address == 0xde35) { retval = "AS.NOTMATH"; }
    else if (address == 0xde38) { retval = "AS.GOEX"; }
    else if (address == 0xde3a) { retval = "AS.FRM_PERFORM1"; }
    else if (address == 0xde43) { retval = "AS.FRM_PERFORM2"; }
    else if (address == 0xde5d) { retval = "AS.EXIT"; }
    else if (address == 0xde60) { retval = "AS.FRM_ELEMENT"; }
    else if (address == 0xde81) { retval = "AS.STRTXT"; }
    else if (address == 0xde90) { retval = "AS.NOT_"; }
    else if (address == 0xde98) { retval = "AS.EQUOP"; }
    else if (address == 0xdea4) { retval = "AS.FN_"; }
    else if (address == 0xdeab) { retval = "AS.SGN_"; }
    else if (address == 0xdeb2) { retval = "AS.PARCHK"; }
    else if (address == 0xdeb8) { retval = "AS.CHKCLS"; }
    else if (address == 0xdebb) { retval = "AS.CHKOPN"; }
    else if (address == 0xdebe) { retval = "AS.CHKCOM"; }
    else if (address == 0xdec0) { retval = "AS.SYNCHR"; }
    else if (address == 0xdec9) { retval = "AS.SYNERR"; }
    else if (address == 0xdece) { retval = "AS.MIN"; }
    else if (address == 0xded0) { retval = "AS.EQUL"; }
    else if (address == 0xded5) { retval = "AS.FRM_VARIABLE"; }
    else if (address == 0xded7) { retval = "AS.FRM_VARIABLE_CALL"; }
    else if (address == 0xdef9) { retval = "AS.SCREEN"; }
    else if (address == 0xdf0c) { retval = "AS.UNARY"; }
    else if (address == 0xdf4f) { retval = "AS.OR"; }
    else if (address == 0xdf55) { retval = "AS.AND"; }
    else if (address == 0xdf5d) { retval = "AS.FALSE"; }
    else if (address == 0xdf60) { retval = "AS.TRUE"; }
    else if (address == 0xdf65) { retval = "AS.RELOPS"; }
    else if (address == 0xdf7d) { retval = "AS.STRCMP"; }
    else if (address == 0xdfaa) { retval = "AS.STRCMP1"; }
    else if (address == 0xdfb0) { retval = "AS.NUMCMP"; }
    else if (address == 0xdfb5) { retval = "AS.STRCMP2"; }
    else if (address == 0xdfc1) { retval = "AS.CMPDONE"; }
    else if (address == 0xdfcd) { retval = "AS.PDL"; }
    else if (address == 0xdfd6) { retval = "AS.NXDIM"; }
    else if (address == 0xdfd9) { retval = "AS.DIM"; }
    else if (address == 0xdfe3) { retval = "AS.PTRGET"; }
    else if (address == 0xdfe8) { retval = "AS.PTRGET2"; }
    else if (address == 0xdfea) { retval = "AS.PTRGET3"; }
    else if (address == 0xdff4) { retval = "AS.BADNAM"; }
    else if (address == 0xdff7) { retval = "AS.NAMOK"; }
    else if (address == 0xe007) { retval = "AS.PTRGET4"; }
    else if (address == 0xe07d) { retval = "AS.ISLETC"; }
    else if (address == 0xe087) { retval = "AS.NAME_NOT_FOUND"; }
    else if (address == 0xe09c) { retval = "AS.MAKE_NEW_VARIABLE"; }
    else if (address == 0xe0de) { retval = "AS.SET_VARPNT_AND_YA"; }
    else if (address == 0xe0ed) { retval = "AS.GETARY"; }
    else if (address == 0xe0ef) { retval = "AS.GETARY2"; }
    else if (address == 0xe102) { retval = "AS.MAKINT"; }
    else if (address == 0xe108) { retval = "AS.MKINT"; }
    else if (address == 0xe10c) { retval = "AS.AYINT"; }
    else if (address == 0xe119) { retval = "AS.MI1"; }
    else if (address == 0xe11b) { retval = "AS.MI2"; }
    else if (address == 0xe11e) { retval = "AS.ARRAY"; }
    else if (address == 0xe196) { retval = "AS.SUBERR"; }
    else if (address == 0xe199) { retval = "AS.IQERR"; }
    else if (address == 0xe19b) { retval = "AS.JER"; }
    else if (address == 0xe19e) { retval = "AS.USE_OLD_ARRAY"; }
    else if (address == 0xe1b8) { retval = "AS.MAKE_NEW_ARRAY"; }
    else if (address == 0xe24b) { retval = "AS.FIND_ARRAY_ELEMENT"; }
    else if (address == 0xe253) { retval = "AS.FAE1"; }
    else if (address == 0xe269) { retval = "AS.GSE"; }
    else if (address == 0xe26c) { retval = "AS.GME"; }
    else if (address == 0xe26f) { retval = "AS.FAE2"; }
    else if (address == 0xe270) { retval = "AS.FAE3"; }
    else if (address == 0xe2ac) { retval = "AS.RTS9"; }
    else if (address == 0xe2ad) { retval = "AS.MULTIPLY_SUBSCRIPT"; }
    else if (address == 0xe2b6) { retval = "AS.MULTIPLY_SUBS1"; }
    else if (address == 0xe2de) { retval = "AS.FRE"; }
    else if (address == 0xe2f2) { retval = "AS.GIVAYF"; }
    else if (address == 0xe2ff) { retval = "AS.POS"; }
    else if (address == 0xe301) { retval = "AS.SNGFLT"; }
    else if (address == 0xe306) { retval = "AS.ERRDIR"; }
    else if (address == 0xe30e) { retval = "AS.UNDFNC"; }
    else if (address == 0xe313) { retval = "AS.DEF"; }
    else if (address == 0xe341) { retval = "AS.FNC_"; }
    else if (address == 0xe354) { retval = "AS.FUNCT"; }
    else if (address == 0xe3af) { retval = "AS.FNCDATA"; }
    else if (address == 0xe3c5) { retval = "AS.STR"; }
    else if (address == 0xe3d5) { retval = "AS.STRINI"; }
    else if (address == 0xe3dd) { retval = "AS.STRSPA"; }
    else if (address == 0xe3e7) { retval = "AS.STRLIT"; }
    else if (address == 0xe3ed) { retval = "AS.STRLT2"; }
    else if (address == 0xe42a) { retval = "AS.PUTNEW"; }
    else if (address == 0xe432) { retval = "AS.JERR"; }
    else if (address == 0xe435) { retval = "AS.PUTEMP"; }
    else if (address == 0xe452) { retval = "AS.GETSPA"; }
    else if (address == 0xe484) { retval = "AS.GARBAG"; }
    else if (address == 0xe488) { retval = "AS.FIND_HIGHEST_STRING"; }
    else if (address == 0xe519) { retval = "AS.CHECK_SIMPLE_VARIABLE"; }
    else if (address == 0xe523) { retval = "AS.CHECK_VARIABLE"; }
    else if (address == 0xe552) { retval = "AS.CHECK_BUMP"; }
    else if (address == 0xe55d) { retval = "AS.CHECK_EXIT"; }
    else if (address == 0xe562) { retval = "AS.MOVE_HIGHEST_STRING_TO_TOP"; }
    else if (address == 0xe597) { retval = "AS.CAT"; }
    else if (address == 0xe5d4) { retval = "AS.MOVINS"; }
    else if (address == 0xe5e2) { retval = "AS.MOVSTR"; }
    else if (address == 0xe5e6) { retval = "AS.MOVSTR1"; }
    else if (address == 0xe5fd) { retval = "AS.FRESTR"; }
    else if (address == 0xe600) { retval = "AS.FREFAC"; }
    else if (address == 0xe604) { retval = "AS.FRETMP"; }
    else if (address == 0xe635) { retval = "AS.FRETMS"; }
    else if (address == 0xe646) { retval = "AS.CHRSTR"; }
    else if (address == 0xe65a) { retval = "AS.LEFTSTR"; }
    else if (address == 0xe660) { retval = "AS.SUBSTRING1"; }
    else if (address == 0xe667) { retval = "AS.SUBSTRING2"; }
    else if (address == 0xe668) { retval = "AS.SUBSTRING3"; }
    else if (address == 0xe686) { retval = "AS.RIGHTSTR"; }
    else if (address == 0xe691) { retval = "AS.MIDSTR"; }
    else if (address == 0xe6b9) { retval = "AS.SUBSTRING_SETUP"; }
    else if (address == 0xe6d6) { retval = "AS.LEN"; }
    else if (address == 0xe6dc) { retval = "AS.GETSTR"; }
    else if (address == 0xe6e5) { retval = "AS.ASC"; }
    else if (address == 0xe6f2) { retval = "AS.GOIQ"; }
    else if (address == 0xe6f5) { retval = "AS.GTBYTC"; }
    else if (address == 0xe6f8) { retval = "AS.GETBYT"; }
    else if (address == 0xe6fb) { retval = "AS.CONINT"; }
    else if (address == 0xe707) { retval = "AS.VAL"; }
    else if (address == 0xe73d) { retval = "AS.POINT"; }
    else if (address == 0xe746) { retval = "AS.GTNUM"; }
    else if (address == 0xe74c) { retval = "AS.COMBYTE"; }
    else if (address == 0xe752) { retval = "AS.GETADR"; }
    else if (address == 0xe764) { retval = "AS.PEEK"; }
    else if (address == 0xe77b) { retval = "AS.POKE"; }
    else if (address == 0xe784) { retval = "AS.WAIT"; }
    else if (address == 0xe79f) { retval = "AS.RTS10"; }
    else if (address == 0xe7a0) { retval = "AS.FADDH"; }
    else if (address == 0xe7a7) { retval = "AS.FSUB"; }
    else if (address == 0xe7aa) { retval = "AS.FSUBT"; }
    else if (address == 0xe7b9) { retval = "AS.FADD1"; }
    else if (address == 0xe7be) { retval = "AS.FADD"; }
    else if (address == 0xe7c1) { retval = "AS.FADDT"; }
    else if (address == 0xe7ce) { retval = "AS.FADD2"; }
    else if (address == 0xe7fa) { retval = "AS.FADD3"; }
    else if (address == 0xe829) { retval = "AS.NORMALIZE_FAC1"; }
    else if (address == 0xe82e) { retval = "AS.NORMALIZE_FAC2"; }
    else if (address == 0xe84e) { retval = "AS.ZERO_FAC"; }
    else if (address == 0xe850) { retval = "AS.STA_IN_FAC_SIGN_AND_EXP"; }
    else if (address == 0xe852) { retval = "AS.STA_IN_FAC_SIGN"; }
    else if (address == 0xe855) { retval = "AS.FADD4"; }
    else if (address == 0xe874) { retval = "AS.NORMALIZE_FAC3"; }
    else if (address == 0xe880) { retval = "AS.NORMALIZE_FAC4"; }
    else if (address == 0xe88d) { retval = "AS.NORMALIZE_FAC5"; }
    else if (address == 0xe88f) { retval = "AS.NORMALIZE_FAC6"; }
    else if (address == 0xe89d) { retval = "AS.RTS11"; }
    else if (address == 0xe89e) { retval = "AS.COMPLEMENT_FAC"; }
    else if (address == 0xe8a4) { retval = "AS.COMPLEMENT_FAC_MANTISSA"; }
    else if (address == 0xe8c6) { retval = "AS.INCREMENT_FAC_MANTISSA"; }
    else if (address == 0xe8d4) { retval = "AS.RTS12"; }
    else if (address == 0xe8d5) { retval = "AS.OVERFLOW"; }
    else if (address == 0xe8da) { retval = "AS.SHIFT_RIGHT1"; }
    else if (address == 0xe8dc) { retval = "AS.SHIFT_RIGHT2"; }
    else if (address == 0xe8f0) { retval = "AS.SHIFT_RIGHT"; }
    else if (address == 0xe8fd) { retval = "AS.SHIFT_RIGHT3"; }
    else if (address == 0xe907) { retval = "AS.SHIFT_RIGHT4"; }
    else if (address == 0xe911) { retval = "AS.SHIFT_RIGHT5"; }
    else if (address == 0xe941) { retval = "AS.LOG"; }
    else if (address == 0xe948) { retval = "AS.GIQ"; }
    else if (address == 0xe94b) { retval = "AS.LOG2"; }
    else if (address == 0xe97f) { retval = "AS.FMULT"; }
    else if (address == 0xe982) { retval = "AS.FMULTT"; }
    else if (address == 0xe9b0) { retval = "AS.MULTIPLY1"; }
    else if (address == 0xe9b5) { retval = "AS.MULTIPLY2"; }
    else if (address == 0xe9e2) { retval = "AS.RTS13"; }
    else if (address == 0xe9e3) { retval = "AS.LOAD_ARG_FROM_YA"; }
    else if (address == 0xea0e) { retval = "AS.ADD_EXPONENTS"; }
    else if (address == 0xea10) { retval = "AS.ADD_EXPONENTS1"; }
    else if (address == 0xea2b) { retval = "AS.OUTOFRNG"; }
    else if (address == 0xea31) { retval = "AS.ZERO"; }
    else if (address == 0xea36) { retval = "AS.JOV"; }
    else if (address == 0xeae9) { retval = "AS.MUL10"; }
    else if (address == 0xea55) { retval = "AS.DIV10"; }
    else if (address == 0xea5e) { retval = "AS.DIV"; }
    else if (address == 0xea66) { retval = "AS.FDIV"; }
    else if (address == 0xeae6) { retval = "AS.COPY_RESULT_INTO_FAC"; }
    else if (address == 0xeaf9) { retval = "AS.LOAD_FAC_FROM_YA"; }
    else if (address == 0xeb1e) { retval = "AS.STORE_FAC_IN_TEMP2_ROUNDED"; }
    else if (address == 0xeb21) { retval = "AS.STORE_FAC_IN_TEMP1_ROUNDED"; }
    else if (address == 0xeb27) { retval = "AS.SETFOR"; }
    else if (address == 0xeb2b) { retval = "AS.STORE_FAC_AT_YX_ROUNDED"; }
    else if (address == 0xeb53) { retval = "AS.COPY_FAC_TO_ARG"; }
    else if (address == 0xeb63) { retval = "AS.COPY_FAC_TO_ARG_ROUNDED"; }
    else if (address == 0xeb71) { retval = "AS.RTS14"; }
    else if (address == 0xeb72) { retval = "AS.ROUND_FAC"; }
    else if (address == 0xeb7a) { retval = "AS.INCREMENT_MANTISSA"; }
    else if (address == 0xeb82) { retval = "AS.SIGN"; }
    else if (address == 0xeb86) { retval = "AS.SIGN1"; }
    else if (address == 0xeb88) { retval = "AS.SIGN2"; }
    else if (address == 0xeb8f) { retval = "AS.RTS15"; }
    else if (address == 0xeb90) { retval = "AS.SGN"; }
    else if (address == 0xeb9e) { retval = "AS.FLOAT"; }
    else if (address == 0xeb9b) { retval = "AS.FLOAT1"; }
    else if (address == 0xeba0) { retval = "AS.FLOAT2"; }
    else if (address == 0xebaf) { retval = "AS.ABS"; }
    else if (address == 0xebb2) { retval = "AS.FCOMP"; }
    else if (address == 0xebb4) { retval = "AS.FCOMP2"; }
    else if (address == 0xebf2) { retval = "AS.QINT"; }
    else if (address == 0xec11) { retval = "AS.RTS16"; }
    else if (address == 0xec12) { retval = "AS.QINT2"; }
    else if (address == 0xec23) { retval = "AS.INT"; }
    else if (address == 0xec40) { retval = "AS.QINT3"; }
    else if (address == 0xec49) { retval = "AS.RTS17"; }
    else if (address == 0xec4a) { retval = "AS.FIN"; }
    else if (address == 0xec61) { retval = "AS.FIN1"; }
    else if (address == 0xec64) { retval = "AS.FIN2"; }
    else if (address == 0xec66) { retval = "AS.FIN3"; }
    else if (address == 0xec87) { retval = "AS.FIN4"; }
    else if (address == 0xec8a) { retval = "AS.FIN5"; }
    else if (address == 0xec8c) { retval = "AS.FIN6"; }
    else if (address == 0xec98) { retval = "AS.FIN10"; }
    else if (address == 0xec9e) { retval = "AS.FIN7"; }
    else if (address == 0xeca0) { retval = "AS.FIN8"; }
    else if (address == 0xecc1) { retval = "AS.FIN9"; }
    else if (address == 0xecd5) { retval = "AS.ADDACC"; }
    else if (address == 0xece8) { retval = "AS.GETEXP"; }
    else if (address == 0xed19) { retval = "AS.INPRT"; }
    else if (address == 0xed24) { retval = "AS.LINPRT"; }
    else if (address == 0xed2e) { retval = "AS.PRINT_FAC"; }
    else if (address == 0xed31) { retval = "AS.GO_STROUT"; }
    else if (address == 0xed34) { retval = "AS.FOUT"; }
    else if (address == 0xed36) { retval = "AS.FOUT1"; }
    else if (address == 0xed8c) { retval = "AS.FOUT2"; }
    else if (address == 0xee17) { retval = "AS.FOUT3"; }
    else if (address == 0xee57) { retval = "AS.FOUT4"; }
    else if (address == 0xee5a) { retval = "AS.FOUT5"; }
    else if (address == 0xee5f) { retval = "AS.FOUT6"; }
    else if (address == 0xee8d) { retval = "AS.SQR"; }
    else if (address == 0xee97) { retval = "AS.FPWRT"; }
    else if (address == 0xeed0) { retval = "AS.NEGOP"; }
    else if (address == 0xeeda) { retval = "AS.RTS18"; }
    else if (address == 0xef09) { retval = "AS.EXP"; }
    else if (address == 0xef5c) { retval = "AS.POLYNOMIAL_ODD"; }
    else if (address == 0xef72) { retval = "AS.POLYNOMIAL"; }
    else if (address == 0xef76) { retval = "AS.SERMAIN"; }
    else if (address == 0xefa5) { retval = "AS.RTS19"; }
    else if (address == 0xefae) { retval = "AS.RND"; }
    else if (address == 0xefe7) { retval = "AS.GO_MOVMF"; }
    else if (address == 0xefea) { retval = "AS.COS"; }
    else if (address == 0xeff1) { retval = "AS.SIN"; }
    else if (address == 0xf023) { retval = "AS.SIN1"; }
    else if (address == 0xf026) { retval = "AS.SIN2"; }
    else if (address == 0xf03a) { retval = "AS.TAN"; }
    else if (address == 0xf062) { retval = "AS.TAN1"; }
    else if (address == 0xf094) { retval = "AS.MS_EASTER_EGG_DATA"; }
    else if (address == 0xf09e) { retval = "AS.ATN"; }
    else if (address == 0xf0cd) { retval = "AS.RTS20"; }
    else if (address == 0xf10b) { retval = "AS.GENERIC_CHRGET"; }
    else if (address == 0xf128) { retval = "AS.COLD_START"; }
    else if (address == 0xf1d5) { retval = "AS.CALL"; }
    else if (address == 0xf1de) { retval = "AS.IN_NUMBER"; }
    else if (address == 0xf1e5) { retval = "AS.PR_NUMBER"; }
    else if (address == 0xf1ec) { retval = "AS.PLOTFNS"; }
    else if (address == 0xf206) { retval = "AS.GOERR"; }
    else if (address == 0xf209) { retval = "AS.LINCOOR"; }
    else if (address == 0xf225) { retval = "AS.PLOT"; }
    else if (address == 0xf232) { retval = "AS.HLIN"; }
    else if (address == 0xf241) { retval = "AS.VLIN"; }
    else if (address == 0xf24f) { retval = "AS.COLOR"; }
    else if (address == 0xf256) { retval = "AS.VTAB"; }
    else if (address == 0xf262) { retval = "AS.SPEED"; }
    else if (address == 0xf26d) { retval = "AS.TRACE"; }
    else if (address == 0xf26f) { retval = "AS.NOTRACE"; }
    else if (address == 0xf273) { retval = "AS.NORMAL"; }
    else if (address == 0xf277) { retval = "AS.INVERSE"; }
    else if (address == 0xf280) { retval = "AS.FLASH"; }
    else if (address == 0xf286) { retval = "AS.HIMEM"; }
    else if (address == 0xf296) { retval = "AS.JMM"; }
    else if (address == 0xf299) { retval = "AS.SETHI"; }
    else if (address == 0xf2a6) { retval = "AS.LOMEM"; }
    else if (address == 0xf2cb) { retval = "AS.ONERR"; }
    else if (address == 0xf2e9) { retval = "AS.HANDLERR"; }
    else if (address == 0xf318) { retval = "AS.RESUME"; }
    else if (address == 0xf32e) { retval = "AS.JSYN"; }
    else if (address == 0xf331) { retval = "AS.DEL"; }
    else if (address == 0xf390) { retval = "AS.GR"; }
    else if (address == 0xf399) { retval = "AS.TEXT"; }
    else if (address == 0xf39f) { retval = "AS.STORE"; }
    else if (address == 0xf3bc) { retval = "AS.RECALL"; }
    else if (address == 0xf3d8) { retval = "AS.HGR2"; }
    else if (address == 0xf3e2) { retval = "AS.HGR"; }
    else if (address == 0xf3ea) { retval = "AS.SETHPG"; }
    else if (address == 0xf3f2) { retval = "AS.HCLR"; }
    else if (address == 0xf3f6) { retval = "AS.BKGND"; }
    else if (address == 0xf411) { retval = "AS.HPOSN"; }
    else if (address == 0xf457) { retval = "AS.HPLOT0"; }
    else if (address == 0xf465) { retval = "AS.MOVE_LEFT_OR_RIGHT"; }
    else if (address == 0xf47e) { retval = "AS.COLOR_SHIFT"; }
    else if (address == 0xf48a) { retval = "AS.MOVE_RIGHT"; }
    else if (address == 0xf49c) { retval = "AS.LRUDX1"; }
    else if (address == 0xf49d) { retval = "AS.LRUDX2"; }
    else if (address == 0xf4b3) { retval = "AS.LRUD1"; }
    else if (address == 0xf4b4) { retval = "AS.LRUD2"; }
    else if (address == 0xf4c4) { retval = "AS.LRUD3"; }
    else if (address == 0xf4c8) { retval = "AS.LRUD4"; }
    else if (address == 0xf4d3) { retval = "AS.MOVE_UP_OR_DOWN"; }
    else if (address == 0xf505) { retval = "AS.MOVE_DOWN"; }
    else if (address == 0xf530) { retval = "AS.HLINRL"; }
    else if (address == 0xf53a) { retval = "AS.HGLN"; }
    else if (address == 0xf57c) { retval = "AS.MOVEX"; }
    else if (address == 0xf581) { retval = "AS.MOVEX2"; }
    else if (address == 0xf5cb) { retval = "AS.HFIND"; }
    else if (address == 0xf600) { retval = "AS.RTS22"; }
    else if (address == 0xf601) { retval = "AS.DRAW0"; }
    else if (address == 0xf605) { retval = "AS.DRAW1"; }
    else if (address == 0xf65d) { retval = "AS.XDRAW0"; }
    else if (address == 0xf661) { retval = "AS.XDRAW1"; }
    else if (address == 0xf6b9) { retval = "AS.HFNS"; }
    else if (address == 0xf6e6) { retval = "AS.GGERR"; }
    else if (address == 0xf6e9) { retval = "AS.HCOLOR"; }
    else if (address == 0xf6f5) { retval = "AS.RTS23"; }
    else if (address == 0xf6fc) { retval = "AS.COLORTBL"; }
    else if (address == 0xf6fe) { retval = "AS.HPLOT"; }
    else if (address == 0xf721) { retval = "AS.ROT"; }
    else if (address == 0xf727) { retval = "AS.SCALE"; }
    else if (address == 0xf72d) { retval = "AS.DRAWPNT"; }
    else if (address == 0xf769) { retval = "AS.DRAW"; }
    else if (address == 0xf76f) { retval = "AS.XDRAW"; }
    else if (address == 0xf775) { retval = "AS.SHLOAD"; }
    else if (address == 0xf7bc) { retval = "AS.TAPEPNT"; }
    else if (address == 0xf7d9) { retval = "AS.GETARYPT"; }
    else if (address == 0xf7e7) { retval = "AS.HTAB"; }

    else if (address == 0xf800) { retval = "MON.PLOT"; }
    else if (address == 0xf80c) { retval = "MON.RTMASK"; }
    else if (address == 0xf80e) { retval = "MON.PLOT1"; }
    else if (address == 0xf819) { retval = "MON.HLINE"; }
    else if (address == 0xf828) { retval = "MON.VLINE"; }
    else if (address == 0xf831) { retval = "MON.RTS1"; }
    else if (address == 0xf832) { retval = "MON.CLRSCR"; }
    else if (address == 0xf836) { retval = "MON.CLRTOP"; }
    else if (address == 0xf838) { retval = "MON.CLRSC2"; }
    else if (address == 0xf83c) { retval = "MON.CLRSC3"; }
    else if (address == 0xf847) { retval = "MON.GBASCALC"; }
    else if (address == 0xf856) { retval = "MON.GBCALC"; }
    else if (address == 0xf85f) { retval = "MON.NXTCOL"; }
    else if (address == 0xf864) { retval = "MON.SETCOL"; }
    else if (address == 0xf871) { retval = "MON.SCRN"; }
    else if (address == 0xf879) { retval = "MON.SCRN2"; }
    else if (address == 0xf87f) { retval = "MON.RTMSKZ"; }
    else if (address == 0xf882) { retval = "MON.INDS1"; }
    else if (address == 0xf88c) { retval = "MON.INDS2"; }
    else if (address == 0xf89b) { retval = "MON.IEVEN"; }
    else if (address == 0xf8a5) { retval = "MON.ERR"; }
    else if (address == 0xf8a9) { retval = "MON.GETFMT"; }
    else if (address == 0xf8be) { retval = "MON.MNNDX1"; }
    else if (address == 0xf8c2) { retval = "MON.MNNDX2"; }
    else if (address == 0xf8c9) { retval = "MON.NBBDX3"; }
    else if (address == 0xf8d0) { retval = "MON.UBSDSP"; }
    else if (address == 0xf8d4) { retval = "MON.PRINTOP"; }
    else if (address == 0xf8db) { retval = "MON.PRNTBL"; }
    else if (address == 0xf8f5) { retval = "MON.PRMN1"; }
    else if (address == 0xf8f9) { retval = "MON.PRMN2"; }
    else if (address == 0xf910) { retval = "MON.PRADR1"; }
    else if (address == 0xf914) { retval = "MON.PRADR2"; }
    else if (address == 0xf926) { retval = "MON.PRADR3"; }
    else if (address == 0xf92a) { retval = "MON.PRADR4"; }
    else if (address == 0xf930) { retval = "MON.PRADR5"; }
    else if (address == 0xf938) { retval = "MON.RELADR"; }
    else if (address == 0xf940) { retval = "MON.PRNTYX"; }
    else if (address == 0xf941) { retval = "MON.PRNTAX"; }
    else if (address == 0xf944) { retval = "MON.PRNTX"; }
    else if (address == 0xf948) { retval = "MON.PRBLNK"; }
    else if (address == 0xf94a) { retval = "MON.PRBL2"; }
    else if (address == 0xf94c) { retval = "MON.PRBL3"; }
    else if (address == 0xf953) { retval = "MON.PCADJ"; }
    else if (address == 0xf954) { retval = "MON.PCADJ2"; }
    else if (address == 0xf956) { retval = "MON.PCADJ3"; }
    else if (address == 0xf95c) { retval = "MON.PCADJ4"; }
    else if (address == 0xf961) { retval = "MON.RTS2"; }
    else if (address == 0xf962) { retval = "MON.FMT1"; }
    else if (address == 0xf9a6) { retval = "MON.FMT2"; }
    else if (address == 0xf9b4) { retval = "MON.CHAR1"; }
    else if (address == 0xf9ba) { retval = "MON.CHAR2"; }
    else if (address == 0xf9c0) { retval = "MON.MNEML"; }
    else if (address == 0xfa00) { retval = "MON.MNEMR"; }
    else if (address == 0xfa40) { retval = "MON.IRQ"; }
    else if (address == 0xfa4c) { retval = "MON.BREAK"; }
    else if (address == 0xfa59) { retval = "MON.OLDBRK"; }
    else if (address == 0xfa62) { retval = "MON.RESET"; }
    else if (address == 0xfa6f) { retval = "MON.INITAN"; }
    else if (address == 0xfa81) { retval = "MON.NEWMON"; }
    else if (address == 0xfa9b) { retval = "MON.FIXSEV"; }
    else if (address == 0xfaa3) { retval = "MON.NOFIX"; }
    else if (address == 0xfaa6) { retval = "MON.PWRUP"; }
    else if (address == 0xfaa9) { retval = "MON.SETPG3"; }
    else if (address == 0xfaab) { retval = "MON.SETPLP"; }
    else if (address == 0xfaba) { retval = "MON.SLOOP"; }
    else if (address == 0xfac7) { retval = "MON.NXTBYT"; }
    else if (address == 0xfad7) { retval = "MON.REGDSP"; }
    else if (address == 0xfada) { retval = "MON.RGDPS1"; }
    else if (address == 0xfae4) { retval = "MON.RDSP1"; }
    else if (address == 0xfafd) { retval = "MON.PWRCON"; }
    else if (address == 0xfb02) { retval = "MON.DISKID"; }
    else if (address == 0xfb09) { retval = "MON.TITLE"; }
    else if (address == 0xfb11) { retval = "MON.XLTBL"; }
    else if (address == 0xfb19) { retval = "MON.RTBL"; }
    else if (address == 0xfb1e) { retval = "MON.PREAD"; }
    else if (address == 0xfb25) { retval = "MON.PREAD2"; }
    else if (address == 0xfb2e) { retval = "MON.RTS2D"; }
    else if (address == 0xfb2f) { retval = "MON.INIT"; }
    else if (address == 0xfb39) { retval = "MON.SETTXT"; }
    else if (address == 0xfb40) { retval = "MON.SETGR"; }
    else if (address == 0xfb4b) { retval = "MON.SETWND"; }
    else if (address == 0xfb5b) { retval = "MON.TABV"; }
    else if (address == 0xfb60) { retval = "MON.APPLEII"; }
    else if (address == 0xfb65) { retval = "MON.STITLE"; }
    else if (address == 0xfb6A) { retval = "MON.GETLN"; }
    else if (address == 0xfb6f) { retval = "MON.SETPWRC"; }
    else if (address == 0xfb78) { retval = "MON.VIDWAIT"; }
    else if (address == 0xfb88) { retval = "MON.KBDWAIT"; }

    else if (address == 0xfbb3) { retval = "M80_F8VERSION"; }

    else if (address == 0xfb94) { retval = "MON.NOWAIT"; }
    else if (address == 0xfb97) { retval = "MON.ESCOLD"; }
    else if (address == 0xfb9b) { retval = "MON.ESCNOW"; }
    else if (address == 0xfba5) { retval = "MON.ESCNEW"; }
    else if (address == 0xfbb3) { retval = "MON.VERSION"; }
    else if (address == 0xfbb4) { retval = "MON.GOTOCX"; }
    else if (address == 0xfbc1) { retval = "MON.BASCALC"; }

    else if (address == 0xfbd0) { retval = "MON.BASCLC2"; }
    else if (address == 0xfbd9) { retval = "MON.BELL1"; }
    else if (address == 0xfbe4) { retval = "MON.BELL2"; }
    else if (address == 0xfbef) { retval = "MON.RTS2B"; }
    else if (address == 0xfbf0) { retval = "MON.STORADV"; }
    else if (address == 0xfbf4) { retval = "MON.ADVANCE"; }
    else if (address == 0xfbfc) { retval = "MON.RTS3"; }
    else if (address == 0xfbfd) { retval = "MON.VIDOUT"; }
    else if (address == 0xfc10) { retval = "MON.BS"; }
    else if (address == 0xfc1a) { retval = "MON.UP"; }
    else if (address == 0xfc22) { retval = "MON.VTAB"; }
    else if (address == 0xfc24) { retval = "MON.VTABZ"; }
    else if (address == 0xfc2b) { retval = "MON.RTS4"; }
    else if (address == 0xfc2c) { retval = "MON.ESC1"; }
    else if (address == 0xfc42) { retval = "MON.CLREOP"; }
    else if (address == 0xfc58) { retval = "MON.HOME"; }
    else if (address == 0xfc62) { retval = "MON.CR"; }
    else if (address == 0xfc66) { retval = "MON.LF"; }
    else if (address == 0xfc70) { retval = "MON.SCROLL"; }
    else if (address == 0xfc72) { retval = "MON.XGOTOCX"; }

    else if (address == 0xfc75) { retval = "M80_SNIFFIRQ"; }

    else if (address == 0xfc84) { retval = "MON.RDCX"; }
    else if (address == 0xfc91) { retval = "MON.ISSLOTS"; }
    else if (address == 0xfc99) { retval = "MON.ISPAGE1"; }
    else if (address == 0xfc9c) { retval = "MON.CLREOL"; }
    else if (address == 0xfc9e) { retval = "MON.CLREOLZ"; }
    else if (address == 0xfca8) { retval = "MON.WAIT"; }
    else if (address == 0xfca9) { retval = "MON.WAIT2"; }
    else if (address == 0xfcaa) { retval = "MON.WAIT3"; }
    else if (address == 0xfcb4) { retval = "MON.NXTA4"; }
    else if (address == 0xfcba) { retval = "MON.NXTA1"; }
    else if (address == 0xfcc8) { retval = "MON.RTS4B"; }
    else if (address == 0xfcc9) { retval = "MON.HEADR"; }
    else if (address == 0xfcd6) { retval = "MON.WRBIT"; }
    else if (address == 0xfcdb) { retval = "MON.ZERDLY"; }
    else if (address == 0xfce2) { retval = "MON.ONEDLY"; }
    else if (address == 0xfce5) { retval = "MON.WRTAPE"; }
    else if (address == 0xfcec) { retval = "MON.RDBYTE"; }
    else if (address == 0xfcee) { retval = "MON.RDBYT2"; }
    else if (address == 0xfcfa) { retval = "MON.READ2BIT"; }
    else if (address == 0xfcfd) { retval = "MON.RDBIT"; }
    else if (address == 0xfd0c) { retval = "MON.RDKEY"; }

    else if (address == 0xfd18) { retval = "MON.KEYIN"; }
    else if (address == 0xfd21) { retval = "MON.RDESC"; }

    else if (address == 0xfd29) { retval = "M80_FUNCEXIT"; }

    else if (address == 0xfd2f) { retval = "MON.ESC"; }
    else if (address == 0xfd35) { retval = "MON.RDCHAR"; }
    else if (address == 0xfd3d) { retval = "MON.NOTCR"; }
    else if (address == 0xfd5f) { retval = "MON.NOTCR1"; }
    else if (address == 0xfd62) { retval = "MON.CANCEL"; }
    else if (address == 0xfd67) { retval = "MON.GETLNZ"; }
    else if (address == 0xfd6a) { retval = "MON.GETLN"; }
    else if (address == 0xfd71) { retval = "MON.BCKSPC"; }
    else if (address == 0xfd75) { retval = "MON.NXTCHAR"; }
    else if (address == 0xfd7e) { retval = "MON.CAPTST"; }
    else if (address == 0xfd84) { retval = "MON.ADDINP"; }
    else if (address == 0xfd8e) { retval = "MON.CROUT"; }
    else if (address == 0xfd92) { retval = "MON.PRA1"; }
    else if (address == 0xfd96) { retval = "MON.PRYX2"; }
    else if (address == 0xfda3) { retval = "MON.XAMB"; }
    else if (address == 0xfdad) { retval = "MON.MOD8CHK"; }
    else if (address == 0xfdb3) { retval = "MON.XAM"; }
    else if (address == 0xfdb6) { retval = "MON.DATAOUT"; }
    else if (address == 0xfdc5) { retval = "MON.RTS4C"; }
    else if (address == 0xfdc6) { retval = "MON.XAMPM"; }
    else if (address == 0xfdd1) { retval = "MON.ADD"; }
    else if (address == 0xfdda) { retval = "MON.PRBYTE"; }
    else if (address == 0xfde3) { retval = "MON.PRHEX"; }
    else if (address == 0xfde5) { retval = "MON.PRHEXZ"; }
    else if (address == 0xfded) { retval = "MON.COUT"; }
    else if (address == 0xfdf0) { retval = "MON.COUT1"; }
    else if (address == 0xfdf6) { retval = "MON.COUTZ"; }
    else if (address == 0xfe00) { retval = "MON.BLI"; }
    else if (address == 0xfe04) { retval = "MON.BLANK"; }
    else if (address == 0xfe0b) { retval = "MON.STOR"; }
    else if (address == 0xfe17) { retval = "MON.RTS5"; }
    else if (address == 0xfe18) { retval = "MON.SETMODE"; }
    else if (address == 0xfe1d) { retval = "MON.SETMDZ"; }
    else if (address == 0xfe20) { retval = "MON.LT"; }
    else if (address == 0xfe22) { retval = "MON.LT2"; }
    else if (address == 0xfe2c) { retval = "MON.MOVE"; }
    else if (address == 0xfe36) { retval = "MON.VFY"; }
    else if (address == 0xfe58) { retval = "MON.VFYOK"; }
    else if (address == 0xfe5e) { retval = "MON.LIST"; }
    else if (address == 0xfe63) { retval = "MON.LIST2"; }
    else if (address == 0xfe75) { retval = "MON.A1PC"; }
    else if (address == 0xfe78) { retval = "MON.A1PCLP"; }
    else if (address == 0xfe7f) { retval = "MON.A1PCRTS"; }
    else if (address == 0xfe80) { retval = "MON.SETINV"; }
    else if (address == 0xfe84) { retval = "MON.SETNORM"; }
    else if (address == 0xfe86) { retval = "MON.SETIFLG"; }
    else if (address == 0xfe89) { retval = "MON.SETKBD"; }
    else if (address == 0xfe8b) { retval = "MON.INPORT"; }
    else if (address == 0xfe8d) { retval = "MON.INPRT"; }
    else if (address == 0xfe93) { retval = "MON.SETVID"; }
    else if (address == 0xfe95) { retval = "MON.OUTPORT"; }
    else if (address == 0xfe97) { retval = "MON.OUTPRT"; }
    else if (address == 0xfe9b) { retval = "MON.IOPRT"; }
    else if (address == 0xfea7) { retval = "MON.IOPRT1"; }
    else if (address == 0xfea9) { retval = "MON.IOPRT2"; }
    else if (address == 0xfeaf) { retval = "MON.CKSUMFIX"; }
    else if (address == 0xfeb0) { retval = "MON.XBASIC"; }
    else if (address == 0xfeb3) { retval = "MON.BASCONT"; }
    else if (address == 0xfeb6) { retval = "MON.GO"; }
    else if (address == 0xfebf) { retval = "MON.REGZ"; }
    else if (address == 0xfec2) { retval = "MON.TRACE"; }
    else if (address == 0xfec4) { retval = "MON.STEPZ"; }
    else if (address == 0xfeca) { retval = "MON.USR"; }
    else if (address == 0xfecd) { retval = "MON.WRITE"; }
    else if (address == 0xfed4) { retval = "MON.WR1"; }
    else if (address == 0xfeed) { retval = "MON.WRBYTE"; }
    else if (address == 0xfeef) { retval = "MON.WRBYT2"; }
    else if (address == 0xfef6) { retval = "MON.CRMON"; }
    else if (address == 0xfefd) { retval = "MON.READ"; }
    else if (address == 0xff02) { retval = "MON.READ2"; }
    else if (address == 0xff0a) { retval = "MON.RD2"; }
    else if (address == 0xff16) { retval = "MON.RD3"; }
    else if (address == 0xff2d) { retval = "MON.PRERR"; }
    else if (address == 0xff3a) { retval = "MON.BELL"; }
    else if (address == 0xff3f) { retval = "MON.RESTORE"; }
    else if (address == 0xff44) { retval = "MON.RESTR1"; }
    else if (address == 0xff4a) { retval = "MON.SAVE"; }
    else if (address == 0xff4c) { retval = "MON.SAV1"; }

    else if (address == 0xff58) { retval = "M80_IORTS"; }

    else if (address == 0xff59) { retval = "MON.OLDRST"; }
    else if (address == 0xff65) { retval = "MON.MON"; }
    else if (address == 0xff69) { retval = "MON.MONZ"; }
    else if (address == 0xff73) { retval = "MON.NXTITM"; }
    else if (address == 0xff7a) { retval = "MON.CRSRCH"; }
    else if (address == 0xff8a) { retval = "MON.DIG"; }
    else if (address == 0xff90) { retval = "MON.NXTBIT"; }
    else if (address == 0xff98) { retval = "MON.NXTBAS"; }
    else if (address == 0xffa2) { retval = "MON.NXTBS2"; }
    else if (address == 0xffa7) { retval = "MON.GETNUM"; }
    else if (address == 0xffad) { retval = "MON.NXTCHR"; }
    else if (address == 0xffbe) { retval = "MON.TOSUB"; }
    else if (address == 0xffc7) { retval = "MON.ZMODE"; }
    else if (address == 0xffcc) { retval = "MON.CHRTBL"; }
    else if (address == 0xffe3) { retval = "MON.SUBTBL"; }

    return retval;
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

    //    retval = QString("; %1 / %2").arg(val).arg(appleAscii);
    retval = QString("; %1").arg(val);
    return retval;
}
