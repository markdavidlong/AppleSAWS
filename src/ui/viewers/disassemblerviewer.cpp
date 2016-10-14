#include "disassemblerviewer.h"
#include "ui_disassemblerviewer.h"
#include "disassembler.h"
#include "memory.h"

#include <QDebug>

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

    QList<DisassembledItem> lines = dis.disassemble(file->address(), file->address()+file->length());

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
        QString newline = QString("%1:  %2 %3").arg(di.hexAddress()).arg(di.hexString()).arg(ds);
        formattedLines.append(newline);
    }

    QByteArray joinedlines = qPrintable(formattedLines.join("\n"));
    setData(joinedlines);
}


void DisassemblerViewer::setFile(RelocatableFile *file) {
    m_file = file;

    QString title = QString("Disassembler Viewer: %1 (Relocatable)").arg(m_file->filename());
    setWindowTitle(title);

    quint16 address = file->address() + 6 ; // Handle offset for relocatable metadata

    Memory mem;
    mem.addFile(file->getBinaryCodeImage(), address);
    Disassembler dis(mem.values());

    QList<DisassembledItem> lines = dis.disassemble(address, address+file->codeImageLength()-1);

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
        QString newline = QString("%1:  %2 %3").arg(di.hexAddress()).arg(di.hexString()).arg(ds);
        formattedLines.append(newline);
    }

    QByteArray joinedlines = qPrintable(formattedLines.join("\n"));
    QStringList rd = file->decodeRelocatableDict();
    QByteArray rdlines = qPrintable(rd.join("\n"));
    setData(joinedlines + '\n' + rdlines);
}


QString DisassemblerViewer::getPotentialLabel(quint16 address) {
    QString retval = QString();

    if (address == 0x0A) { retval = "ASBASIC_USRVEC0"; }
    else if (address == 0x0B) { retval = "ASBASIC_USRVEC1"; }
    else if (address == 0x0C) { retval = "ASBASIC_USRVEC2"; }
    else if (address == 0x0D) { retval = "ASBASIC_GENPURPOSE0"; }
    else if (address == 0x0E) { retval = "ASBASIC_GENPURPOSE1"; }
    else if (address == 0x0F) { retval = "ASBASIC_GENPURPOSE2"; }
    else if (address == 0x10) { retval = "ASBASIC_GENPURPOSE3"; }
    else if (address == 0x11) { retval = "ASBASIC_GENPURPOSE4"; }
    else if (address == 0x12) { retval = "ASBASIC_GENPURPOSE5"; }
    else if (address == 0x13) { retval = "ASBASIC_GENPURPOSE6"; }
    else if (address == 0x14) { retval = "ASBASIC_GENPURPOSE7"; }
    else if (address == 0x15) { retval = "ASBASIC_GENPURPOSE8"; }
    else if (address == 0x16) { retval = "ASBASIC_GENPURPOSE9"; }
    else if (address == 0x17) { retval = "ASBASIC_GENPURPOSE10"; }


    else if (address == 0x24) { retval = "DOS_CURSOR_HORIZONTAL"; }
    else if (address == 0x28) { retval = "DOS_BASL"; }
    else if (address == 0x29) { retval = "DOS_BASH"; }
    else if (address == 0x33) { retval = "DOS_PROMPT_CHAR"; }
    else if (address == 0x36) { retval = "DOS_CSWL"; }
    else if (address == 0x37) { retval = "DOS_CSWH"; }
    else if (address == 0x38) { retval = "DOS_KSWL"; }
    else if (address == 0x39) { retval = "DOS_KSWH"; }
    else if (address == 0x40) { retval = "DOS_FILE_BUFFER_L"; }
    else if (address == 0x41) { retval = "DOS_FILE_BUFFER_H"; }
    else if (address == 0x42) { retval = "DOS_BUFFER_ADDR_L"; }
    else if (address == 0x43) { retval = "DOS_BUFFER_ADDR_H"; }
    else if (address == 0x44) { retval = "DOS_NUMERIC_OPERAND_L"; }
    else if (address == 0x45) { retval = "DOS_NUMERIC_OPERAND_H"; }

    else if (address == 0x50) { retval = "ASBASIC_PTR_0L"; }
    else if (address == 0x51) { retval = "ASBASIC_PTR_0H"; }
    else if (address == 0x52) { retval = "ASBASIC_PTR_1L"; }
    else if (address == 0x53) { retval = "ASBASIC_PTR_1H"; }
    else if (address == 0x54) { retval = "ASBASIC_PTR_2L"; }
    else if (address == 0x55) { retval = "ASBASIC_PTR_2H"; }
    else if (address == 0x56) { retval = "ASBASIC_PTR_3L"; }
    else if (address == 0x57) { retval = "ASBASIC_PTR_3H"; }
    else if (address == 0x58) { retval = "ASBASIC_PTR_4L"; }
    else if (address == 0x59) { retval = "ASBASIC_PTR_4H"; }
    else if (address == 0x5A) { retval = "ASBASIC_PTR_5L"; }
    else if (address == 0x5B) { retval = "ASBASIC_PTR_5H"; }
    else if (address == 0x5C) { retval = "ASBASIC_PTR_6L"; }
    else if (address == 0x5D) { retval = "ASBASIC_PTR_6H"; }
    else if (address == 0x5E) { retval = "ASBASIC_PTR_7L"; }
    else if (address == 0x5F) { retval = "ASBASIC_PTR_7H"; }
    else if (address == 0x60) { retval = "ASBASIC_PTR_8L"; }
    else if (address == 0x61) { retval = "ASBASIC_PTR_8H"; }

    else if (address == 0x62) { retval = "ASBASIC_MULT_DIV_RESULT0"; }
    else if (address == 0x63) { retval = "ASBASIC_MULT_DIV_RESULT1"; }
    else if (address == 0x64) { retval = "ASBASIC_MULT_DIV_RESULT2"; }
    else if (address == 0x65) { retval = "ASBASIC_MULT_DIV_RESULT3"; }
    else if (address == 0x66) { retval = "ASBASIC_MULT_DIV_RESULT4"; }


    else if (address == 0x67) { retval = "ASBASIC_PROG_STARTL"; }
    else if (address == 0x68) { retval = "ASBASIC_PROG_STARTH"; }

    else if (address == 0x69) { retval = "ASBASIC_VAR_STARTL"; }
    else if (address == 0x6A) { retval = "ASBASIC_VAR_STARTH"; }

    else if (address == 0x6B) { retval = "ASBASIC_ARRAY_STARTL"; }
    else if (address == 0x6C) { retval = "ASBASIC_ARRAY_STARTH"; }

    else if (address == 0x6D) { retval = "ASBASIC_NUMSTORE_ENDL"; }
    else if (address == 0x6E) { retval = "ASBASIC_NUMSTORE_ENDH"; }

    else if (address == 0x6F) { retval = "ASBASIC_STRING_STARTL"; }
    else if (address == 0x70) { retval = "ASBASIC_STRING_STARTH"; }

    else if (address == 0x71) { retval = "ASBASIC_PTR_9L"; }
    else if (address == 0x72) { retval = "ASBASIC_PTR_9H"; }

    else if (address == 0x73) { retval = "ASBASIC_HIMEM_L"; }
    else if (address == 0x74) { retval = "ASBASIC_HIMEM_H"; }

    else if (address == 0x75) { retval = "ASBASIC_CURR_LINENUM_L"; }
    else if (address == 0x76) { retval = "ASBASIC_CURR_LINENUM_H"; }

    else if (address == 0x77) { retval = "ASBASIC_INTR_LINENUM_L"; }
    else if (address == 0x78) { retval = "ASBASIC_INTR_LINENUM_H"; }

    else if (address == 0x79) { retval = "ASBASIC_NEXT_STATEMENT_L"; }
    else if (address == 0x7A) { retval = "ASBASIC_NEXT_STATEMENT_H"; }

    else if (address == 0x7B) { retval = "ASBASIC_DATA_LINENUM_L"; }
    else if (address == 0x7C) { retval = "ASBASIC_DATA_LINENUM_H"; }

    else if (address == 0x7D) { retval = "ASBASIC_DATA_ADDR_L"; }
    else if (address == 0x7E) { retval = "ASBASIC_DATA_ADDR_H"; }

    else if (address == 0x7F) { retval = "ASBASIC_INPUT_SRC_L"; }
    else if (address == 0x80) { retval = "ASBASIC_INPUT_SRC_H"; }

    else if (address == 0x81) { retval = "ASBASIC_LAST_VARNAME_L"; }
    else if (address == 0x82) { retval = "ASBASIC_LAST_VARNAME_H"; }

    else if (address == 0x83) { retval = "ASBASIC_LAST_VARVAL_L"; }
    else if (address == 0x84) { retval = "ASBASIC_LAST_VARVAL_H"; }

    else if (address == 0x85) { retval = "ASBASIC_GENPURPOSE10"; }
    else if (address == 0x86) { retval = "ASBASIC_GENPURPOSE11"; }
    else if (address == 0x87) { retval = "ASBASIC_GENPURPOSE12"; }
    else if (address == 0x88) { retval = "ASBASIC_GENPURPOSE13"; }
    else if (address == 0x89) { retval = "ASBASIC_GENPURPOSE14"; }
    else if (address == 0x8A) { retval = "ASBASIC_GENPURPOSE15"; }
    else if (address == 0x8B) { retval = "ASBASIC_GENPURPOSE16"; }
    else if (address == 0x8C) { retval = "ASBASIC_GENPURPOSE17"; }
    else if (address == 0x8D) { retval = "ASBASIC_GENPURPOSE18"; }
    else if (address == 0x8E) { retval = "ASBASIC_GENPURPOSE19"; }
    else if (address == 0x8F) { retval = "ASBASIC_GENPURPOSE20"; }
    else if (address == 0x90) { retval = "ASBASIC_GENPURPOSE21"; }
    else if (address == 0x91) { retval = "ASBASIC_GENPURPOSE22"; }
    else if (address == 0x92) { retval = "ASBASIC_GENPURPOSE23"; }
    else if (address == 0x93) { retval = "ASBASIC_GENPURPOSE24"; }
    else if (address == 0x94) { retval = "ASBASIC_GENPURPOSE25"; }
    else if (address == 0x95) { retval = "ASBASIC_GENPURPOSE26"; }
    else if (address == 0x96) { retval = "ASBASIC_GENPURPOSE27"; }
    else if (address == 0x97) { retval = "ASBASIC_GENPURPOSE28"; }
    else if (address == 0x98) { retval = "ASBASIC_GENPURPOSE29"; }
    else if (address == 0x99) { retval = "ASBASIC_GENPURPOSE30"; }
    else if (address == 0x9A) { retval = "ASBASIC_GENPURPOSE31"; }
    else if (address == 0x9B) { retval = "ASBASIC_GENPURPOSE32"; }
    else if (address == 0x9C) { retval = "ASBASIC_GENPURPOSE33"; }

    else if (address == 0x9D) { retval = "ASBASIC_FPACCUM_0"; }
    else if (address == 0x9E) { retval = "ASBASIC_FPACCUM_1"; }
    else if (address == 0x9F) { retval = "ASBASIC_FPACCUM_2"; }
    else if (address == 0xA0) { retval = "ASBASIC_FPACCUM_3"; }
    else if (address == 0xA1) { retval = "ASBASIC_FPACCUM_4"; }
    else if (address == 0xA2) { retval = "ASBASIC_FPACCUM_5"; }
    else if (address == 0xA3) { retval = "ASBASIC_FPACCUM_6"; }

    else if (address == 0xA4) { retval = "ASBASIC_FP_GENUSE"; }

    else if (address == 0xA5) { retval = "ASBASIC_SEC_FPACCUM_0"; }
    else if (address == 0xA6) { retval = "ASBASIC_SEC_FPACCUM_1"; }
    else if (address == 0xA7) { retval = "ASBASIC_SEC_FPACCUM_2"; }
    else if (address == 0xA8) { retval = "ASBASIC_SEC_FPACCUM_3"; }
    else if (address == 0xA9) { retval = "ASBASIC_SEC_FPACCUM_4"; }
    else if (address == 0xAA) { retval = "ASBASIC_SEC_FPACCUM_5"; }
    else if (address == 0xAB) { retval = "ASBASIC_SEC_FPACCUM_6"; }

    else if (address == 0xAC) { retval = "ASBASIC_GENUSE_FLAGS0"; }
    else if (address == 0xAD) { retval = "ASBASIC_GENUSE_FLAGS1"; }
    else if (address == 0xAE) { retval = "ASBASIC_GENUSE_FLAGS2"; }

    else if (address == 0xAF) { retval = "ASBASIC_PROGEND_L"; }
    else if (address == 0xB0) { retval = "ASBASIC_PROGEND_H"; }

    else if (address == 0xB1) { retval = "ASBASIC_CHRGET0"; }
    else if (address == 0xB2) { retval = "ASBASIC_CHRGET1"; }
    else if (address == 0xB3) { retval = "ASBASIC_CHRGET2"; }
    else if (address == 0xB4) { retval = "ASBASIC_CHRGET3"; }
    else if (address == 0xB5) { retval = "ASBASIC_CHRGET4"; }
    else if (address == 0xB6) { retval = "ASBASIC_CHRGET5"; }
    else if (address == 0xB7) { retval = "ASBASIC_CHRGET6"; }
    else if (address == 0xB8) { retval = "ASBASIC_CHRGET7"; }
    else if (address == 0xB9) { retval = "ASBASIC_CHRGET8"; }
    else if (address == 0xBA) { retval = "ASBASIC_CHRGET9"; }
    else if (address == 0xBB) { retval = "ASBASIC_CHRGET10"; }
    else if (address == 0xBC) { retval = "ASBASIC_CHRGET11"; }
    else if (address == 0xBD) { retval = "ASBASIC_CHRGET12"; }
    else if (address == 0xBE) { retval = "ASBASIC_CHRGET13"; }
    else if (address == 0xBF) { retval = "ASBASIC_CHRGET14"; }
    else if (address == 0xC0) { retval = "ASBASIC_CHRGET15"; }
    else if (address == 0xC1) { retval = "ASBASIC_CHRGET16"; }
    else if (address == 0xC2) { retval = "ASBASIC_CHRGET17"; }
    else if (address == 0xC3) { retval = "ASBASIC_CHRGET18"; }
    else if (address == 0xC4) { retval = "ASBASIC_CHRGET19"; }
    else if (address == 0xC5) { retval = "ASBASIC_CHRGET20"; }
    else if (address == 0xC6) { retval = "ASBASIC_CHRGET21"; }
    else if (address == 0xC7) { retval = "ASBASIC_CHRGET22"; }
    else if (address == 0xC8) { retval = "ASBASIC_CHRGET23"; }

    else if (address == 0xC9) { retval = "ASBASIC_RNDNUM0"; }
    else if (address == 0xCA) { retval = "ASBASIC_RNDNUM1"; }
    else if (address == 0xCB) { retval = "ASBASIC_RNDNUM2"; }
    else if (address == 0xCC) { retval = "ASBASIC_RNDNUM3"; }
    else if (address == 0xCD) { retval = "ASBASIC_RNDNUM4"; }

    else if (address == 0xD0) { retval = "ASBASIC_HGRSCRTCH_0L"; }
    else if (address == 0xD1) { retval = "ASBASIC_HGRSCRTCH_0H"; }
    else if (address == 0xD2) { retval = "ASBASIC_HGRSCRTCH_1L"; }
    else if (address == 0xD3) { retval = "ASBASIC_HGRSCRTCH_1H"; }
    else if (address == 0xD4) { retval = "ASBASIC_HGRSCRTCH_2L"; }
    else if (address == 0xD5) { retval = "ASBASIC_HGRSCRTCH_2H"; }


    else if (address == 0xD6) { retval = "DOS_ASBASIC_PROG_PROT_FLAG"; }

    else if (address == 0xD8) { retval = "ASBASIC_ONERR_0L"; }
    else if (address == 0xD9) { retval = "ASBASIC_ONERR_0H"; }
    else if (address == 0xDA) { retval = "ASBASIC_ONERR_1L"; }
    else if (address == 0xDB) { retval = "ASBASIC_ONERR_1H"; }
    else if (address == 0xDC) { retval = "ASBASIC_ONERR_2L"; }
    else if (address == 0xDD) { retval = "ASBASIC_ONERR_2H"; }
    else if (address == 0xDE) { retval = "ASBASIC_ONERR_3L"; }
    else if (address == 0xDF) { retval = "ASBASIC_ONERR_3H"; }

    else if (address == 0xE0) { retval = "ASBASIC_HGR_XCOORD_L"; }
    else if (address == 0xE1) { retval = "ASBASIC_HGR_XCOORD_H"; }
    else if (address == 0xE2) { retval = "ASBASIC_HGR_YCOORD"; }

    else if (address == 0xE4) { retval = "ASBASIC_HGR_COLORBYTE"; }

    else if (address == 0xE5) { retval = "ASBASIC_HGR_GENUSE0"; }
    else if (address == 0xE6) { retval = "ASBASIC_HGR_GENUSE1"; }
    else if (address == 0xE7) { retval = "ASBASIC_HGR_GENUSE2"; }

    else if (address == 0xE8) { retval = "ASBASIC_SHAPETBL_L"; }
    else if (address == 0xE9) { retval = "ASBASIC_SHAPETBL_H"; }

    else if (address == 0xEA) { retval = "ASBASIC_HGR_COLLISION_CTR"; }

    else if (address == 0xF0) { retval = "ASBASIC_GENUSE_FLAGS3"; }
    else if (address == 0xF1) { retval = "ASBASIC_GENUSE_FLAGS4"; }
    else if (address == 0xF2) { retval = "ASBASIC_GENUSE_FLAGS5"; }
    else if (address == 0xF3) { retval = "ASBASIC_GENUSE_FLAGS6"; }

    else if (address == 0xF4) { retval = "ASBASIC_ONERR4"; }
    else if (address == 0xF5) { retval = "ASBASIC_ONERR5"; }
    else if (address == 0xF6) { retval = "ASBASIC_ONERR6"; }
    else if (address == 0xF7) { retval = "ASBASIC_ONERR7"; }
    else if (address == 0xF8) { retval = "ASBASIC_ONERR8"; }


    else if (address == 0x03d0) { retval = "DOS_WARMSTART"; }
    else if (address == 0x03d3) { retval = "DOS_COLDSTART"; }
    else if (address == 0x03d6) { retval = "DOS_FILE_MANAGER"; }
    else if (address == 0x03d9) { retval = "DOS_RWTS"; }
    else if (address == 0x03dc) { retval = "DOS_FM_PARAM_LIST_LOCATE"; }
    else if (address == 0x03e3) { retval = "DOS_RWTS_PARAM_LIST_LOCATE"; }
    else if (address == 0x03ea) { retval = "DOS_REPLACE_DOS_INTERCEPTS"; }
    else if (address == 0x03ef) { retval = "DOS_AUTOSTART_BRK_HANDLER"; }
    else if (address == 0x03f2) { retval = "DOS_AUTOSTART_RESET_HANDLER"; }
    else if (address == 0x03f4) { retval = "DOS_POWERUP_BYTE"; }
    else if (address == 0x03f5) { retval = "DOS_AMPERSAND_HANDLER"; }
    else if (address == 0x03f8) { retval = "DOS_CTRL_Y_HANDLER"; }
    else if (address == 0x03fb) { retval = "DOS_NMI_HANDLER"; }
    else if (address == 0x03fe) { retval = "DOS_IRQ_HANDLER"; }

    else if (address == 0x0400) { retval = "MON_LINE1"; }

    else if (address == 0x07f8) { retval = "MON_MSLOT"; }
    else if (address == 0xc000) { retval = "MON_IOADR/MON_KBD"; }

    else if (address == 0xc001) { retval = "M80_SET80COL"; }
    else if (address == 0xc002) { retval = "M80_RDMAINRAM"; }
    else if (address == 0xc003) { retval = "M80_RDCARDRAM"; }
    else if (address == 0xc004) { retval = "M80_WRMAINRAM"; }
    else if (address == 0xc005) { retval = "M80_WRCARDRAM"; }

    else if (address == 0xc006) { retval = "MON_SETSLOTCXROM"; }
    else if (address == 0xc007) { retval = "MON_SETINTCXROM"; }

    else if (address == 0xc008) { retval = "M80_SETSTDSZ"; }
    else if (address == 0xc009) { retval = "M80_SETALTZP"; }
    else if (address == 0xc00B) { retval = "M80_SETSLOTC3ROM"; }
    else if (address == 0xc00C) { retval = "M80_CLR80VID"; }
    else if (address == 0xc00D) { retval = "M80_SET80VID"; }
    else if (address == 0xc00E) { retval = "M80_CLRALTCHAR"; }
    else if (address == 0xc00F) { retval = "M80_SETALTCHAR"; }

    else if (address == 0xc010) { retval = "MON_KBDSTRB"; }

    else if (address == 0xc011) { retval = "M80_RDLCBNK2"; }
    else if (address == 0xc012) { retval = "M80_RDLCRAM"; }
    else if (address == 0xc013) { retval = "M80_RDRAMRD"; }
    else if (address == 0xc014) { retval = "M80_RDRAMWRT"; }

    else if (address == 0xc015) { retval = "MON_RDCXROM"; }
    else if (address == 0xc018) { retval = "MON_RD80STORE"; }

    else if (address == 0xc019) { retval = "M80_RDVBLBAR"; }
    else if (address == 0xc01A) { retval = "M80_RDTEXT"; }

    else if (address == 0xc01c) { retval = "MON_RDPAGE2"; }

    else if (address == 0xc01f) { retval = "M80_RD80VID"; }

    else if (address == 0xc020) { retval = "MON_TAPEOUT"; }
    else if (address == 0xc030) { retval = "MON_SPKR"; }
    else if (address == 0xc050) { retval = "MON_TXTCLR"; }
    else if (address == 0xc051) { retval = "MON_TXTSET"; }
    else if (address == 0xc052) { retval = "MON_MIXCLR"; }
    else if (address == 0xc053) { retval = "MON_MIXSET"; }
    else if (address == 0xc054) { retval = "MON_LOWSCR"; }
    else if (address == 0xc055) { retval = "MON_HISCR"; }
    else if (address == 0xc056) { retval = "MON_LORES"; }
    else if (address == 0xc057) { retval = "MON_HIRES"; }
    else if (address == 0xc058) { retval = "MON_SETAN0"; }
    else if (address == 0xc059) { retval = "MON_CLRAN0"; }
    else if (address == 0xc05a) { retval = "MON_SETAN1"; }
    else if (address == 0xc05b) { retval = "MON_CLRAN1"; }
    else if (address == 0xc05c) { retval = "MON_SETAN2"; }
    else if (address == 0xc05d) { retval = "MON_CLRAN2"; }
    else if (address == 0xc05e) { retval = "MON_SETAN3"; }
    else if (address == 0xc05f) { retval = "MON_CLRAN3"; }
    else if (address == 0xc060) { retval = "MON_TAPEIN"; }
    else if (address == 0xc064) { retval = "MON_PADDL0"; }
    else if (address == 0xc070) { retval = "MON_PTRIG"; }

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


    else if (address == 0xcfff) { retval = "MON_CLRROM"; }
    else if (address == 0xe000) { retval = "MON_BASIC"; }
    else if (address == 0xe003) { retval = "MON_BASIC2"; }

    else if (address == 0xd365) { retval = "AS_GTFORPNT"; }
    else if (address == 0xd39e) { retval = "AS_BLTU"; }
    else if (address == 0xd39a) { retval = "AS_BLTU2"; }
    else if (address == 0xd3d6) { retval = "AS_CHKMEM"; }
    else if (address == 0xd3e3) { retval = "AS_REASON"; }
    else if (address == 0xd410) { retval = "AS_MEMERR"; }
    else if (address == 0xd412) { retval = "AS_ERROR"; }
    else if (address == 0xd431) { retval = "AS_PRINT_ERROR_LINNUM"; }
    else if (address == 0xd43c) { retval = "AS_RESTART"; }
    else if (address == 0xd45c) { retval = "AS_NUMBERED_LINE"; }
    else if (address == 0xd4b5) { retval = "AS_PUT_NEW_LINE"; }
    else if (address == 0xd4f2) { retval = "AS_FIX_LINKS"; }
    else if (address == 0xd52c) { retval = "AS_INLIN"; }
    else if (address == 0xd52e) { retval = "AS_INLIN2"; }
    else if (address == 0xd553) { retval = "AS_INCHR"; }
    else if (address == 0xd559) { retval = "AS_PARSE_INPUT_LINE"; }
    else if (address == 0xd56c) { retval = "AS_PARSE"; }
    else if (address == 0xd61a) { retval = "AS_FNDLIN"; }
    else if (address == 0xd61e) { retval = "AS_FL1"; }
    else if (address == 0xd648) { retval = "AS_RTS1"; }
    else if (address == 0xd649) { retval = "AS_NEW"; }
    else if (address == 0xd64b) { retval = "AS_SCRTCH"; }
    else if (address == 0xd665) { retval = "AS_SETPTRS"; }
    else if (address == 0xd66a) { retval = "AS_CLEAR"; }
    else if (address == 0xd66c) { retval = "AS_CLEARC"; }
    else if (address == 0xd683) { retval = "AS_STKINI"; }
    else if (address == 0xd696) { retval = "AS_RTS2"; }
    else if (address == 0xd697) { retval = "AS_STXTPT"; }
    else if (address == 0xd6a5) { retval = "AS_LIST"; }
    else if (address == 0xd6da) { retval = "AS_LIST0"; }
    else if (address == 0xd6fe) { retval = "AS_LIST1"; }
    else if (address == 0xd702) { retval = "AS_LIST2"; }
    else if (address == 0xd724) { retval = "AS_LIST3"; }
    else if (address == 0xd72c) { retval = "AS_GETCHR"; }
    else if (address == 0xd734) { retval = "AS_LIST4"; }
    else if (address == 0xd766) { retval = "AS_FOR"; }
    else if (address == 0xd7af) { retval = "AS_STEP"; }
    else if (address == 0xd7d2) { retval = "AS_NEWSTT"; }
    else if (address == 0xd805) { retval = "AS_TRACE_"; }
    else if (address == 0xd826) { retval = "AS_GOEND"; }
    else if (address == 0xd828) { retval = "AS_EXECUTE_STATEMENT"; }
    else if (address == 0xd82a) { retval = "AS_EXECUTE_STATEMENT1"; }
    else if (address == 0xd842) { retval = "AS_COLON_"; }
    else if (address == 0xd846) { retval = "AS_SYNERR1"; }
    else if (address == 0xd849) { retval = "AS_RESTORE"; }
    else if (address == 0xd853) { retval = "AS_SETDA"; }
    else if (address == 0xd857) { retval = "AS_RTS3"; }
    else if (address == 0xd858) { retval = "AS_ISCNTC"; }
    else if (address == 0xd863) { retval = "AS_CONTROL_C_TYPED"; }
    else if (address == 0xd86e) { retval = "AS_STOP"; }
    else if (address == 0xd870) { retval = "AS_END"; }
    else if (address == 0xd871) { retval = "AS_END2"; }
    else if (address == 0xd88a) { retval = "AS_END4"; }
    else if (address == 0xd896) { retval = "AS_CONT"; }
    else if (address == 0xd8af) { retval = "AS_RTS4"; }
    else if (address == 0xd8b0) { retval = "AS_SAVE"; }
    else if (address == 0xd8c9) { retval = "AS_LOAD"; }
    else if (address == 0xd8F0) { retval = "AS_VARTIO"; }
    else if (address == 0xd901) { retval = "AS_PROGIO"; }
    else if (address == 0xd912) { retval = "AS_RUN"; }
    else if (address == 0xd921) { retval = "AS_GOSUB"; }
    else if (address == 0xd935) { retval = "AS_GO_TO_LINE"; }
    else if (address == 0xd93e) { retval = "AS_GOTO"; }
    else if (address == 0xd96a) { retval = "AS_RTS5"; }
    else if (address == 0xd96b) { retval = "AS_POP"; }
    else if (address == 0xd97c) { retval = "AS_UNDERR"; }
    else if (address == 0xd981) { retval = "AS_SYNERR2"; }
    else if (address == 0xd984) { retval = "AS_RETURN"; }
    else if (address == 0xd995) { retval = "AS_DATA"; }
    else if (address == 0xd998) { retval = "AS_ADDON"; }
    else if (address == 0xd9a2) { retval = "AS_RTS6"; }
    else if (address == 0xd9a3) { retval = "AS_DATAN"; }
    else if (address == 0xd9a6) { retval = "AS_REMN"; }
    else if (address == 0xd9c5) { retval = "AS_PULL3"; }
    else if (address == 0xd9c9) { retval = "AS_IF"; }
    else if (address == 0xd9dc) { retval = "AS_REM"; }
    else if (address == 0xd9e1) { retval = "AS_IFTRUE"; }
    else if (address == 0xd9ec) { retval = "AS_ONGOTO"; }
    else if (address == 0xd9f4) { retval = "AS_ON1"; }
    else if (address == 0xd9f8) { retval = "AS_ON2"; }
    else if (address == 0xda0b) { retval = "AS_RTS7"; }
    else if (address == 0xda0c) { retval = "AS_LINGET"; }
    else if (address == 0xda46) { retval = "AS_LET"; }
    else if (address == 0xda63) { retval = "AS_LET2"; }
    else if (address == 0xda7a) { retval = "AS_LET_STRING"; }
    else if (address == 0xda7b) { retval = "AS_PUTSTR"; }
    else if (address == 0xdacf) { retval = "AS_PR_SPRING"; }
    else if (address == 0xdad5) { retval = "AS_PRINT"; }
    else if (address == 0xdad7) { retval = "AS_PRINT2"; }
    else if (address == 0xdafb) { retval = "AS_CRDO"; }
    else if (address == 0xdb00) { retval = "AS_NEGATE"; }
    else if (address == 0xdb02) { retval = "AS_PR_RTS8"; }
    else if (address == 0xdb03) { retval = "AS_PR_COMMA"; }
    else if (address == 0xdb16) { retval = "AS_PR_TAB_OR_SPC"; }
    else if (address == 0xdb2c) { retval = "AS_NXSPC"; }
    else if (address == 0xdb2f) { retval = "AS_PR_NEXT_CHAR"; }
    else if (address == 0xdb35) { retval = "AS_DOSPC"; }
    else if (address == 0xdb3a) { retval = "AS_STROUT"; }
    else if (address == 0xdb3d) { retval = "AS_STRPRT"; }
    else if (address == 0xdb57) { retval = "AS_OUTSP"; }
    else if (address == 0xdb5a) { retval = "AS_OUTQUES"; }
    else if (address == 0xdb5c) { retval = "AS_OUTDO"; }
    else if (address == 0xdb71) { retval = "AS_INPUTERR"; }
    else if (address == 0xdb7b) { retval = "AS_READERR"; }
    else if (address == 0xdb7f) { retval = "AS_ERLIN"; }
    else if (address == 0xdb86) { retval = "AS_INPERR"; }
    else if (address == 0xdb87) { retval = "AS_RESPERR"; }
    else if (address == 0xdba0) { retval = "AS_GET"; }
    else if (address == 0xdbb2) { retval = "AS_INPUT"; }
    else if (address == 0xdbdc) { retval = "AS_NXIN"; }
    else if (address == 0xdbe2) { retval = "AS_READ"; }
    else if (address == 0xdbe9) { retval = "AS_INPUT_FLAG_ZERO"; }
    else if (address == 0xdbeb) { retval = "AS_PROCESS_INPUT_LIST"; }
    else if (address == 0xdbf1) { retval = "AS_PROCESS_INPUT_ITEM"; }
    else if (address == 0xdC2b) { retval = "AS_INSTART"; }
    else if (address == 0xdC69) { retval = "AS_INPUT_DATA"; }
    else if (address == 0xdC72) { retval = "AS_INPUT_MORE"; }
    else if (address == 0xdC99) { retval = "AS_INPFIN"; }
    else if (address == 0xdCa0) { retval = "AS_FINDATA"; }
    else if (address == 0xdCC6) { retval = "AS_INPDONE"; }
    else if (address == 0xdcdf) { retval = "AS_ERR_EXTRA"; }
    else if (address == 0xdcef) { retval = "AS_ERR_REENTRY"; }
    else if (address == 0xdcf9) { retval = "AS_NEXT"; }
    else if (address == 0xdcff) { retval = "AS_NEXT1"; }
    else if (address == 0xdd02) { retval = "AS_NEXT2"; }
    else if (address == 0xdd0d) { retval = "AS_GERR"; }
    else if (address == 0xdd0f) { retval = "AS_NEXT3"; }
    else if (address == 0xdd67) { retval = "AS_FRMNUM"; }
    else if (address == 0xdd6a) { retval = "AS_CHKNUM"; }
    else if (address == 0xdd6c) { retval = "AS_CHKSTR"; }
    else if (address == 0xdd6d) { retval = "AS_CHKVAL"; }
    else if (address == 0xdd78) { retval = "AS_JERROR"; }
    else if (address == 0xdd7b) { retval = "AS_FRMEVL"; }
    else if (address == 0xdd86) { retval = "AS_FRMEVL1"; }
    else if (address == 0xdd95) { retval = "AS_FRMEVL2"; }
    else if (address == 0xddcd) { retval = "AS_FRM_PRECEDENCE_TEST"; }
    else if (address == 0xddd6) { retval = "AS_NXOP"; }
    else if (address == 0xddd7) { retval = "AS_SAVOP"; }
    else if (address == 0xdde4) { retval = "AS_FRM_RELATIONAL"; }
    else if (address == 0xddf6) { retval = "AS_FRM_PREFNC"; }
    else if (address == 0xddfd) { retval = "AS_FRM_RECURSE"; }
    else if (address == 0xde0d) { retval = "AS_SNTXERR"; }
    else if (address == 0xde10) { retval = "AS_FRM_STACK1"; }
    else if (address == 0xde15) { retval = "AS_FRM_STACK2"; }
    else if (address == 0xde20) { retval = "AS_FRM_STACK3"; }
    else if (address == 0xde35) { retval = "AS_NOTMATH"; }
    else if (address == 0xde38) { retval = "AS_GOEX"; }
    else if (address == 0xde3a) { retval = "AS_FRM_PERFORM1"; }
    else if (address == 0xde43) { retval = "AS_FRM_PERFORM2"; }
    else if (address == 0xde5d) { retval = "AS_EXIT"; }
    else if (address == 0xde60) { retval = "AS_FRM_ELEMENT"; }
    else if (address == 0xde81) { retval = "AS_STRTXT"; }
    else if (address == 0xde90) { retval = "AS_NOT_"; }
    else if (address == 0xde98) { retval = "AS_EQUOP"; }
    else if (address == 0xdea4) { retval = "AS_FN_"; }
    else if (address == 0xdeab) { retval = "AS_SGN_"; }
    else if (address == 0xdeb2) { retval = "AS_PARCHK"; }
    else if (address == 0xdeb8) { retval = "AS_CHKCLS"; }
    else if (address == 0xdebb) { retval = "AS_CHKOPN"; }
    else if (address == 0xdebe) { retval = "AS_CHKCOM"; }
    else if (address == 0xdec0) { retval = "AS_SYNCHR"; }
    else if (address == 0xdec9) { retval = "AS_SYNERR"; }
    else if (address == 0xdece) { retval = "AS_MIN"; }
    else if (address == 0xded0) { retval = "AS_EQUL"; }
    else if (address == 0xded5) { retval = "AS_FRM_VARIABLE"; }
    else if (address == 0xded7) { retval = "AS_FRM_VARIABLE_CALL"; }
    else if (address == 0xdef9) { retval = "AS_SCREEN"; }
    else if (address == 0xdf0c) { retval = "AS_UNARY"; }
    else if (address == 0xdf4f) { retval = "AS_OR"; }
    else if (address == 0xdf55) { retval = "AS_AND"; }
    else if (address == 0xdf5d) { retval = "AS_FALSE"; }
    else if (address == 0xdf60) { retval = "AS_TRUE"; }
    else if (address == 0xdf65) { retval = "AS_RELOPS"; }
    else if (address == 0xdf7d) { retval = "AS_STRCMP"; }
    else if (address == 0xdfaa) { retval = "AS_STRCMP1"; }
    else if (address == 0xdfb0) { retval = "AS_NUMCMP"; }
    else if (address == 0xdfb5) { retval = "AS_STRCMP2"; }
    else if (address == 0xdfc1) { retval = "AS_CMPDONE"; }
    else if (address == 0xdfcd) { retval = "AS_PDL"; }
    else if (address == 0xdfd6) { retval = "AS_NXDIM"; }
    else if (address == 0xdfd9) { retval = "AS_DIM"; }
    else if (address == 0xdfe3) { retval = "AS_PTRGET"; }
    else if (address == 0xdfe8) { retval = "AS_PTRGET2"; }
    else if (address == 0xdfea) { retval = "AS_PTRGET3"; }
    else if (address == 0xdff4) { retval = "AS_BADNAM"; }
    else if (address == 0xdff7) { retval = "AS_NAMOK"; }
    else if (address == 0xe007) { retval = "AS_PTRGET4"; }
    else if (address == 0xe07d) { retval = "AS_ISLETC"; }
    else if (address == 0xe087) { retval = "AS_NAME_NOT_FOUND"; }
    else if (address == 0xe09c) { retval = "AS_MAKE_NEW_VARIABLE"; }
    else if (address == 0xe0de) { retval = "AS_SET_VARPNT_AND_YA"; }
    else if (address == 0xe0ed) { retval = "AS_GETARY"; }
    else if (address == 0xe0ef) { retval = "AS_GETARY2"; }
    else if (address == 0xe102) { retval = "AS_MAKINT"; }
    else if (address == 0xe108) { retval = "AS_MKINT"; }
    else if (address == 0xe10c) { retval = "AS_AYINT"; }
    else if (address == 0xe119) { retval = "AS_MI1"; }
    else if (address == 0xe11b) { retval = "AS_MI2"; }
    else if (address == 0xe11e) { retval = "AS_ARRAY"; }
    else if (address == 0xe196) { retval = "AS_SUBERR"; }
    else if (address == 0xe199) { retval = "AS_IQERR"; }
    else if (address == 0xe19b) { retval = "AS_JER"; }
    else if (address == 0xe19e) { retval = "AS_USE_OLD_ARRAY"; }
    else if (address == 0xe1b8) { retval = "AS_MAKE_NEW_ARRAY"; }
    else if (address == 0xe24b) { retval = "AS_FIND_ARRAY_ELEMENT"; }
    else if (address == 0xe253) { retval = "AS_FAE1"; }
    else if (address == 0xe269) { retval = "AS_GSE"; }
    else if (address == 0xe26c) { retval = "AS_GME"; }
    else if (address == 0xe26f) { retval = "AS_FAE2"; }
    else if (address == 0xe270) { retval = "AS_FAE3"; }
    else if (address == 0xe2ac) { retval = "AS_RTS9"; }
    else if (address == 0xe2ad) { retval = "AS_MULTIPLY_SUBSCRIPT"; }
    else if (address == 0xe2b6) { retval = "AS_MULTIPLY_SUBS1"; }
    else if (address == 0xe2de) { retval = "AS_FRE"; }
    else if (address == 0xe2f2) { retval = "AS_GIVAYF"; }
    else if (address == 0xe2ff) { retval = "AS_POS"; }
    else if (address == 0xe301) { retval = "AS_SNGFLT"; }
    else if (address == 0xe306) { retval = "AS_ERRDIR"; }
    else if (address == 0xe30e) { retval = "AS_UNDFNC"; }
    else if (address == 0xe313) { retval = "AS_DEF"; }
    else if (address == 0xe341) { retval = "AS_FNC_"; }
    else if (address == 0xe354) { retval = "AS_FUNCT"; }
    else if (address == 0xe3af) { retval = "AS_FNCDATA"; }
    else if (address == 0xe3c5) { retval = "AS_STR"; }
    else if (address == 0xe3d5) { retval = "AS_STRINI"; }
    else if (address == 0xe3dd) { retval = "AS_STRSPA"; }
    else if (address == 0xe3e7) { retval = "AS_STRLIT"; }
    else if (address == 0xe3ed) { retval = "AS_STRLT2"; }
    else if (address == 0xe42a) { retval = "AS_PUTNEW"; }
    else if (address == 0xe432) { retval = "AS_JERR"; }
    else if (address == 0xe435) { retval = "AS_PUTEMP"; }
    else if (address == 0xe452) { retval = "AS_GETSPA"; }
    else if (address == 0xe484) { retval = "AS_GARBAG"; }
    else if (address == 0xe488) { retval = "AS_FIND_HIGHEST_STRING"; }
    else if (address == 0xe519) { retval = "AS_CHECK_SIMPLE_VARIABLE"; }
    else if (address == 0xe523) { retval = "AS_CHECK_VARIABLE"; }
    else if (address == 0xe552) { retval = "AS_CHECK_BUMP"; }
    else if (address == 0xe55d) { retval = "AS_CHECK_EXIT"; }
    else if (address == 0xe562) { retval = "AS_MOVE_HIGHEST_STRING_TO_TOP"; }
    else if (address == 0xe597) { retval = "AS_CAT"; }
    else if (address == 0xe5d4) { retval = "AS_MOVINS"; }
    else if (address == 0xe5e2) { retval = "AS_MOVSTR"; }
    else if (address == 0xe5e6) { retval = "AS_MOVSTR1"; }
    else if (address == 0xe5fd) { retval = "AS_FRESTR"; }
    else if (address == 0xe600) { retval = "AS_FREFAC"; }
    else if (address == 0xe604) { retval = "AS_FRETMP"; }
    else if (address == 0xe635) { retval = "AS_FRETMS"; }
    else if (address == 0xe646) { retval = "AS_CHRSTR"; }
    else if (address == 0xe65a) { retval = "AS_LEFTSTR"; }
    else if (address == 0xe660) { retval = "AS_SUBSTRING1"; }
    else if (address == 0xe667) { retval = "AS_SUBSTRING2"; }
    else if (address == 0xe668) { retval = "AS_SUBSTRING3"; }
    else if (address == 0xe686) { retval = "AS_RIGHTSTR"; }
    else if (address == 0xe691) { retval = "AS_MIDSTR"; }
    else if (address == 0xe6b9) { retval = "AS_SUBSTRING_SETUP"; }
    else if (address == 0xe6d6) { retval = "AS_LEN"; }
    else if (address == 0xe6dc) { retval = "AS_GETSTR"; }
    else if (address == 0xe6e5) { retval = "AS_ASC"; }
    else if (address == 0xe6f2) { retval = "AS_GOIQ"; }
    else if (address == 0xe6f5) { retval = "AS_GTBYTC"; }
    else if (address == 0xe6f8) { retval = "AS_GETBYT"; }
    else if (address == 0xe6fb) { retval = "AS_CONINT"; }
    else if (address == 0xe707) { retval = "AS_VAL"; }
    else if (address == 0xe73d) { retval = "AS_POINT"; }
    else if (address == 0xe746) { retval = "AS_GTNUM"; }
    else if (address == 0xe74c) { retval = "AS_COMBYTE"; }
    else if (address == 0xe752) { retval = "AS_GETADR"; }
    else if (address == 0xe764) { retval = "AS_PEEK"; }
    else if (address == 0xe77b) { retval = "AS_POKE"; }
    else if (address == 0xe784) { retval = "AS_WAIT"; }
    else if (address == 0xe79f) { retval = "AS_RTS10"; }
    else if (address == 0xe7a0) { retval = "AS_FADDH"; }
    else if (address == 0xe7a7) { retval = "AS_FSUB"; }
    else if (address == 0xe7aa) { retval = "AS_FSUBT"; }
    else if (address == 0xe7b9) { retval = "AS_FADD1"; }
    else if (address == 0xe7be) { retval = "AS_FADD"; }
    else if (address == 0xe7c1) { retval = "AS_FADDT"; }
    else if (address == 0xe7ce) { retval = "AS_FADD2"; }
    else if (address == 0xe7fa) { retval = "AS_FADD3"; }
    else if (address == 0xe829) { retval = "AS_NORMALIZE_FAC1"; }
    else if (address == 0xe82e) { retval = "AS_NORMALIZE_FAC2"; }
    else if (address == 0xe84e) { retval = "AS_ZERO_FAC"; }
    else if (address == 0xe850) { retval = "AS_STA_IN_FAC_SIGN_AND_EXP"; }
    else if (address == 0xe852) { retval = "AS_STA_IN_FAC_SIGN"; }
    else if (address == 0xe855) { retval = "AS_FADD4"; }
    else if (address == 0xe874) { retval = "AS_NORMALIZE_FAC3"; }
    else if (address == 0xe880) { retval = "AS_NORMALIZE_FAC4"; }
    else if (address == 0xe88d) { retval = "AS_NORMALIZE_FAC5"; }
    else if (address == 0xe88f) { retval = "AS_NORMALIZE_FAC6"; }
    else if (address == 0xe89d) { retval = "AS_RTS11"; }
    else if (address == 0xe89e) { retval = "AS_COMPLEMENT_FAC"; }
    else if (address == 0xe8a4) { retval = "AS_COMPLEMENT_FAC_MANTISSA"; }
    else if (address == 0xe8c6) { retval = "AS_INCREMENT_FAC_MANTISSA"; }
    else if (address == 0xe8d4) { retval = "AS_RTS12"; }
    else if (address == 0xe8d5) { retval = "AS_OVERFLOW"; }
    else if (address == 0xe8da) { retval = "AS_SHIFT_RIGHT1"; }
    else if (address == 0xe8dc) { retval = "AS_SHIFT_RIGHT2"; }
    else if (address == 0xe8f0) { retval = "AS_SHIFT_RIGHT"; }
    else if (address == 0xe8fd) { retval = "AS_SHIFT_RIGHT3"; }
    else if (address == 0xe907) { retval = "AS_SHIFT_RIGHT4"; }
    else if (address == 0xe911) { retval = "AS_SHIFT_RIGHT5"; }
    else if (address == 0xe941) { retval = "AS_LOG"; }
    else if (address == 0xe948) { retval = "AS_GIQ"; }
    else if (address == 0xe94b) { retval = "AS_LOG2"; }
    else if (address == 0xe97f) { retval = "AS_FMULT"; }
    else if (address == 0xe982) { retval = "AS_FMULTT"; }
    else if (address == 0xe9b0) { retval = "AS_MULTIPLY1"; }
    else if (address == 0xe9b5) { retval = "AS_MULTIPLY2"; }
    else if (address == 0xe9e2) { retval = "AS_RTS13"; }
    else if (address == 0xe9e3) { retval = "AS_LOAD_ARG_FROM_YA"; }
    else if (address == 0xea0e) { retval = "AS_ADD_EXPONENTS"; }
    else if (address == 0xea10) { retval = "AS_ADD_EXPONENTS1"; }
    else if (address == 0xea2b) { retval = "AS_OUTOFRNG"; }
    else if (address == 0xea31) { retval = "AS_ZERO"; }
    else if (address == 0xea36) { retval = "AS_JOV"; }
    else if (address == 0xeae9) { retval = "AS_MUL10"; }
    else if (address == 0xea55) { retval = "AS_DIV10"; }
    else if (address == 0xea5e) { retval = "AS_DIV"; }
    else if (address == 0xea66) { retval = "AS_FDIV"; }
    else if (address == 0xeae6) { retval = "AS_COPY_RESULT_INTO_FAC"; }
    else if (address == 0xeaf9) { retval = "AS_LOAD_FAC_FROM_YA"; }
    else if (address == 0xeb1e) { retval = "AS_STORE_FAC_IN_TEMP2_ROUNDED"; }
    else if (address == 0xeb21) { retval = "AS_STORE_FAC_IN_TEMP1_ROUNDED"; }
    else if (address == 0xeb27) { retval = "AS_SETFOR"; }
    else if (address == 0xeb2b) { retval = "AS_STORE_FAC_AT_YX_ROUNDED"; }
    else if (address == 0xeb53) { retval = "AS_COPY_FAC_TO_ARG"; }
    else if (address == 0xeb63) { retval = "AS_COPY_FAC_TO_ARG_ROUNDED"; }
    else if (address == 0xeb71) { retval = "AS_RTS14"; }
    else if (address == 0xeb72) { retval = "AS_ROUND_FAC"; }
    else if (address == 0xeb7a) { retval = "AS_INCREMENT_MANTISSA"; }
    else if (address == 0xeb82) { retval = "AS_SIGN"; }
    else if (address == 0xeb86) { retval = "AS_SIGN1"; }
    else if (address == 0xeb88) { retval = "AS_SIGN2"; }
    else if (address == 0xeb8f) { retval = "AS_RTS15"; }
    else if (address == 0xeb90) { retval = "AS_SGN"; }
    else if (address == 0xeb9e) { retval = "AS_FLOAT"; }
    else if (address == 0xeb9b) { retval = "AS_FLOAT1"; }
    else if (address == 0xeba0) { retval = "AS_FLOAT2"; }
    else if (address == 0xebaf) { retval = "AS_ABS"; }
    else if (address == 0xebb2) { retval = "AS_FCOMP"; }
    else if (address == 0xebb4) { retval = "AS_FCOMP2"; }
    else if (address == 0xebf2) { retval = "AS_QINT"; }
    else if (address == 0xec11) { retval = "AS_RTS16"; }
    else if (address == 0xec12) { retval = "AS_QINT2"; }
    else if (address == 0xec23) { retval = "AS_INT"; }
    else if (address == 0xec40) { retval = "AS_QINT3"; }
    else if (address == 0xec49) { retval = "AS_RTS17"; }
    else if (address == 0xec4a) { retval = "AS_FIN"; }
    else if (address == 0xec61) { retval = "AS_FIN1"; }
    else if (address == 0xec64) { retval = "AS_FIN2"; }
    else if (address == 0xec66) { retval = "AS_FIN3"; }
    else if (address == 0xec87) { retval = "AS_FIN4"; }
    else if (address == 0xec8a) { retval = "AS_FIN5"; }
    else if (address == 0xec8c) { retval = "AS_FIN6"; }
    else if (address == 0xec98) { retval = "AS_FIN10"; }
    else if (address == 0xec9e) { retval = "AS_FIN7"; }
    else if (address == 0xeca0) { retval = "AS_FIN8"; }
    else if (address == 0xecc1) { retval = "AS_FIN9"; }
    else if (address == 0xecd5) { retval = "AS_ADDACC"; }
    else if (address == 0xece8) { retval = "AS_GETEXP"; }
    else if (address == 0xed19) { retval = "AS_INPRT"; }
    else if (address == 0xed24) { retval = "AS_LINPRT"; }
    else if (address == 0xed2e) { retval = "AS_PRINT_FAC"; }
    else if (address == 0xed31) { retval = "AS_GO_STROUT"; }
    else if (address == 0xed34) { retval = "AS_FOUT"; }
    else if (address == 0xed36) { retval = "AS_FOUT1"; }
    else if (address == 0xed8c) { retval = "AS_FOUT2"; }
    else if (address == 0xee17) { retval = "AS_FOUT3"; }
    else if (address == 0xee57) { retval = "AS_FOUT4"; }
    else if (address == 0xee5a) { retval = "AS_FOUT5"; }
    else if (address == 0xee5f) { retval = "AS_FOUT6"; }
    else if (address == 0xee8d) { retval = "AS_SQR"; }
    else if (address == 0xee97) { retval = "AS_FPWRT"; }
    else if (address == 0xeed0) { retval = "AS_NEGOP"; }
    else if (address == 0xeeda) { retval = "AS_RTS18"; }
    else if (address == 0xef09) { retval = "AS_EXP"; }
    else if (address == 0xef5c) { retval = "AS_POLYNOMIAL_ODD"; }
    else if (address == 0xef72) { retval = "AS_POLYNOMIAL"; }
    else if (address == 0xef76) { retval = "AS_SERMAIN"; }
    else if (address == 0xefa5) { retval = "AS_RTS19"; }
    else if (address == 0xefae) { retval = "AS_RND"; }
    else if (address == 0xefe7) { retval = "AS_GO_MOVMF"; }
    else if (address == 0xefea) { retval = "AS_COS"; }
    else if (address == 0xeff1) { retval = "AS_SIN"; }
    else if (address == 0xf023) { retval = "AS_SIN1"; }
    else if (address == 0xf026) { retval = "AS_SIN2"; }
    else if (address == 0xf03a) { retval = "AS_TAN"; }
    else if (address == 0xf062) { retval = "AS_TAN1"; }
    else if (address == 0xf094) { retval = "AS_MS_EASTER_EGG_DATA"; }
    else if (address == 0xf09e) { retval = "AS_ATN"; }
    else if (address == 0xf0cd) { retval = "AS_RTS20"; }
    else if (address == 0xf10b) { retval = "AS_GENERIC_CHRGET"; }
    else if (address == 0xf128) { retval = "AS_COLD_START"; }
    else if (address == 0xf1d5) { retval = "AS_CALL"; }
    else if (address == 0xf1de) { retval = "AS_IN_NUMBER"; }
    else if (address == 0xf1e5) { retval = "AS_PR_NUMBER"; }
    else if (address == 0xf1ec) { retval = "AS_PLOTFNS"; }
    else if (address == 0xf206) { retval = "AS_GOERR"; }
    else if (address == 0xf209) { retval = "AS_LINCOOR"; }
    else if (address == 0xf225) { retval = "AS_PLOT"; }
    else if (address == 0xf232) { retval = "AS_HLIN"; }
    else if (address == 0xf241) { retval = "AS_VLIN"; }
    else if (address == 0xf24f) { retval = "AS_COLOR"; }
    else if (address == 0xf256) { retval = "AS_VTAB"; }
    else if (address == 0xf262) { retval = "AS_SPEED"; }
    else if (address == 0xf26d) { retval = "AS_TRACE"; }
    else if (address == 0xf26f) { retval = "AS_NOTRACE"; }
    else if (address == 0xf273) { retval = "AS_NORMAL"; }
    else if (address == 0xf277) { retval = "AS_INVERSE"; }
    else if (address == 0xf280) { retval = "AS_FLASH"; }
    else if (address == 0xf286) { retval = "AS_HIMEM"; }
    else if (address == 0xf296) { retval = "AS_JMM"; }
    else if (address == 0xf299) { retval = "AS_SETHI"; }
    else if (address == 0xf2a6) { retval = "AS_LOMEM"; }
    else if (address == 0xf2cb) { retval = "AS_ONERR"; }
    else if (address == 0xf2e9) { retval = "AS_HANDLERR"; }
    else if (address == 0xf318) { retval = "AS_RESUME"; }
    else if (address == 0xf32e) { retval = "AS_JSYN"; }
    else if (address == 0xf331) { retval = "AS_DEL"; }
    else if (address == 0xf390) { retval = "AS_GR"; }
    else if (address == 0xf399) { retval = "AS_TEXT"; }
    else if (address == 0xf39f) { retval = "AS_STORE"; }
    else if (address == 0xf3bc) { retval = "AS_RECALL"; }
    else if (address == 0xf3d8) { retval = "AS_HGR2"; }
    else if (address == 0xf3e2) { retval = "AS_HGR"; }
    else if (address == 0xf3ea) { retval = "AS_SETHPG"; }
    else if (address == 0xf3f2) { retval = "AS_HCLR"; }
    else if (address == 0xf3f6) { retval = "AS_BKGND"; }
    else if (address == 0xf411) { retval = "AS_HPOSN"; }
    else if (address == 0xf457) { retval = "AS_HPLOT0"; }
    else if (address == 0xf465) { retval = "AS_MOVE_LEFT_OR_RIGHT"; }
    else if (address == 0xf47e) { retval = "AS_COLOR_SHIFT"; }
    else if (address == 0xf48a) { retval = "AS_MOVE_RIGHT"; }
    else if (address == 0xf49c) { retval = "AS_LRUDX1"; }
    else if (address == 0xf49d) { retval = "AS_LRUDX2"; }
    else if (address == 0xf4b3) { retval = "AS_LRUD1"; }
    else if (address == 0xf4b4) { retval = "AS_LRUD2"; }
    else if (address == 0xf4c4) { retval = "AS_LRUD3"; }
    else if (address == 0xf4c8) { retval = "AS_LRUD4"; }
    else if (address == 0xf4d3) { retval = "AS_MOVE_UP_OR_DOWN"; }
    else if (address == 0xf505) { retval = "AS_MOVE_DOWN"; }
    else if (address == 0xf530) { retval = "AS_HLINRL"; }
    else if (address == 0xf53a) { retval = "AS_HGLN"; }
    else if (address == 0xf57c) { retval = "AS_MOVEX"; }
    else if (address == 0xf581) { retval = "AS_MOVEX2"; }
    else if (address == 0xf5cb) { retval = "AS_HFIND"; }
    else if (address == 0xf600) { retval = "AS_RTS22"; }
    else if (address == 0xf601) { retval = "AS_DRAW0"; }
    else if (address == 0xf605) { retval = "AS_DRAW1"; }
    else if (address == 0xf65d) { retval = "AS_XDRAW0"; }
    else if (address == 0xf661) { retval = "AS_XDRAW1"; }
    else if (address == 0xf6b9) { retval = "AS_HFNS"; }
    else if (address == 0xf6e6) { retval = "AS_GGERR"; }
    else if (address == 0xf6e9) { retval = "AS_HCOLOR"; }
    else if (address == 0xf6f5) { retval = "AS_RTS23"; }
    else if (address == 0xf6fc) { retval = "AS_COLORTBL"; }
    else if (address == 0xf6fe) { retval = "AS_HPLOT"; }
    else if (address == 0xf721) { retval = "AS_ROT"; }
    else if (address == 0xf727) { retval = "AS_SCALE"; }
    else if (address == 0xf72d) { retval = "AS_DRAWPNT"; }
    else if (address == 0xf769) { retval = "AS_DRAW"; }
    else if (address == 0xf76f) { retval = "AS_XDRAW"; }
    else if (address == 0xf775) { retval = "AS_SHLOAD"; }
    else if (address == 0xf7bc) { retval = "AS_TAPEPNT"; }
    else if (address == 0xf7d9) { retval = "AS_GETARYPT"; }
    else if (address == 0xf7e7) { retval = "AS_HTAB"; }

    else if (address == 0xf800) { retval = "MON_PLOT"; }
    else if (address == 0xf80c) { retval = "MON_RTMASK"; }
    else if (address == 0xf80e) { retval = "MON_PLOT1"; }
    else if (address == 0xf831) { retval = "MON_RTS1"; }
    else if (address == 0xf832) { retval = "MON_CLRSCR"; }
    else if (address == 0xf836) { retval = "MON_CLRTOP"; }
    else if (address == 0xf838) { retval = "MON_CLRSC2"; }
    else if (address == 0xf83c) { retval = "MON_CLRSC3"; }
    else if (address == 0xf847) { retval = "MON_GBASCALC"; }
    else if (address == 0xf856) { retval = "MON_GBCALC"; }
    else if (address == 0xf85f) { retval = "MON_NXTCOL"; }
    else if (address == 0xf864) { retval = "MON_SETCOL"; }
    else if (address == 0xf871) { retval = "MON_SCRN"; }
    else if (address == 0xf879) { retval = "MON_SCRN2"; }
    else if (address == 0xf87f) { retval = "MON_RTMSKZ"; }
    else if (address == 0xf882) { retval = "MON_INDS1"; }
    else if (address == 0xf88c) { retval = "MON_INDS2"; }
    else if (address == 0xf89b) { retval = "MON_IEVEN"; }
    else if (address == 0xf8a5) { retval = "MON_ERR"; }
    else if (address == 0xf8a9) { retval = "MON_GETFMT"; }
    else if (address == 0xf8be) { retval = "MON_MNNDX1"; }
    else if (address == 0xf8c2) { retval = "MON_MNNDX2"; }
    else if (address == 0xf8c9) { retval = "MON_NBBDX3"; }
    else if (address == 0xf8d0) { retval = "MON_UBSDSP"; }
    else if (address == 0xf8d4) { retval = "MON_PRINTOP"; }
    else if (address == 0xf8db) { retval = "MON_PRNTBL"; }
    else if (address == 0xf8f5) { retval = "MON_PRMN1"; }
    else if (address == 0xf8f9) { retval = "MON_PRMN2"; }
    else if (address == 0xf910) { retval = "MON_PRADR1"; }
    else if (address == 0xf914) { retval = "MON_PRADR2"; }
    else if (address == 0xf926) { retval = "MON_PRADR3"; }
    else if (address == 0xf92a) { retval = "MON_PRADR4"; }
    else if (address == 0xf930) { retval = "MON_PRADR5"; }
    else if (address == 0xf938) { retval = "MON_RELADR"; }
    else if (address == 0xf940) { retval = "MON_PRNTYX"; }
    else if (address == 0xf941) { retval = "MON_PRNTAX"; }
    else if (address == 0xf944) { retval = "MON_PRNTX"; }
    else if (address == 0xf948) { retval = "MON_PRBLNK"; }
    else if (address == 0xf94a) { retval = "MON_PRBL2"; }
    else if (address == 0xf94c) { retval = "MON_PRBL3"; }
    else if (address == 0xf953) { retval = "MON_PCADJ"; }
    else if (address == 0xf954) { retval = "MON_PCADJ2"; }
    else if (address == 0xf956) { retval = "MON_PCADJ3"; }
    else if (address == 0xf95c) { retval = "MON_PCADJ4"; }
    else if (address == 0xf961) { retval = "MON_RTS2"; }
    else if (address == 0xf962) { retval = "MON_FMT1"; }
    else if (address == 0xf9a6) { retval = "MON_FMT2"; }
    else if (address == 0xf9b4) { retval = "MON_CHAR1"; }
    else if (address == 0xf9ba) { retval = "MON_CHAR2"; }
    else if (address == 0xf9c0) { retval = "MON_MNEML"; }
    else if (address == 0xfa00) { retval = "MON_MNEMR"; }
    else if (address == 0xfa40) { retval = "MON_IRQ"; }
    else if (address == 0xfa4c) { retval = "MON_BREAK"; }
    else if (address == 0xfa59) { retval = "MON_OLDBRK"; }
    else if (address == 0xfa62) { retval = "MON_RESET"; }
    else if (address == 0xfa6f) { retval = "MON_INITAN"; }
    else if (address == 0xfa81) { retval = "MON_NEWMON"; }
    else if (address == 0xfa9b) { retval = "MON_FIXSEV"; }
    else if (address == 0xfaa3) { retval = "MON_NOFIX"; }
    else if (address == 0xfaa6) { retval = "MON_PWRUP"; }
    else if (address == 0xfaa9) { retval = "MON_SETPG3"; }
    else if (address == 0xfaab) { retval = "MON_SETPLP"; }
    else if (address == 0xfaba) { retval = "MON_SLOOP"; }
    else if (address == 0xfac7) { retval = "MON_NXTBYT"; }
    else if (address == 0xfad7) { retval = "MON_REGDSP"; }
    else if (address == 0xfada) { retval = "MON_RGDPS1"; }
    else if (address == 0xfae4) { retval = "MON_RDSP1"; }
    else if (address == 0xfafd) { retval = "MON_PWRCON"; }
    else if (address == 0xfb02) { retval = "MON_DISKID"; }
    else if (address == 0xfb09) { retval = "MON_TITLE"; }
    else if (address == 0xfb11) { retval = "MON_XLTBL"; }
    else if (address == 0xfb19) { retval = "MON_RTBL"; }
    else if (address == 0xfb1e) { retval = "MON_PREAD"; }
    else if (address == 0xfb25) { retval = "MON_PREAD2"; }
    else if (address == 0xfb2e) { retval = "MON_RTS2D"; }
    else if (address == 0xfb2f) { retval = "MON_INIT"; }
    else if (address == 0xfb39) { retval = "MON_SETTXT"; }
    else if (address == 0xfb40) { retval = "MON_SETGR"; }
    else if (address == 0xfb4b) { retval = "MON_SETWND"; }
    else if (address == 0xfb5b) { retval = "MON_TABV"; }
    else if (address == 0xfb60) { retval = "MON_APPLEII"; }
    else if (address == 0xfb65) { retval = "MON_STITLE"; }
    else if (address == 0xfb6f) { retval = "MON_SETPWRC"; }
    else if (address == 0xfb78) { retval = "MON_VIDWAIT"; }
    else if (address == 0xfb88) { retval = "MON_KBDWAIT"; }

    else if (address == 0xfbb3) { retval = "M80_F8VERSION"; }

    else if (address == 0xfb94) { retval = "MON_NOWAIT"; }
    else if (address == 0xfb97) { retval = "MON_ESCOLD"; }
    else if (address == 0xfb9b) { retval = "MON_ESCNOW"; }
    else if (address == 0xfba5) { retval = "MON_ESCNEW"; }
    else if (address == 0xfbb3) { retval = "MON_VERSION"; }
    else if (address == 0xfbb4) { retval = "MON_GOTOCX"; }
    else if (address == 0xfbc1) { retval = "MON_BASCALC"; }

    else if (address == 0xfbd0) { retval = "MON_BASCLC2"; }
    else if (address == 0xfbd9) { retval = "MON_BELL1"; }
    else if (address == 0xfbe4) { retval = "MON_BELL2"; }
    else if (address == 0xfbef) { retval = "MON_RTS2B"; }
    else if (address == 0xfbf0) { retval = "MON_STORADV"; }
    else if (address == 0xfbf4) { retval = "MON_ADVANCE"; }
    else if (address == 0xfbfc) { retval = "MON_RTS3"; }
    else if (address == 0xfbfd) { retval = "MON_VIDOUT"; }
    else if (address == 0xfc10) { retval = "MON_BS"; }
    else if (address == 0xfc1a) { retval = "MON_UP"; }
    else if (address == 0xfc22) { retval = "MON_VTAB"; }
    else if (address == 0xfc24) { retval = "MON_VTABZ"; }
    else if (address == 0xfc2b) { retval = "MON_RTS4"; }
    else if (address == 0xfc2c) { retval = "MON_ESC1"; }
    else if (address == 0xfc42) { retval = "MON_CLREOP"; }
    else if (address == 0xfc58) { retval = "MON_HOME"; }
    else if (address == 0xfc62) { retval = "MON_CR"; }
    else if (address == 0xfc66) { retval = "MON_LF"; }
    else if (address == 0xfc70) { retval = "MON_SCROLL"; }
    else if (address == 0xfc72) { retval = "MON_XGOTOCX"; }

    else if (address == 0xfc75) { retval = "M80_SNIFFIRQ"; }

    else if (address == 0xfc84) { retval = "MON_RDCX"; }
    else if (address == 0xfc91) { retval = "MON_ISSLOTS"; }
    else if (address == 0xfc99) { retval = "MON_ISPAGE1"; }
    else if (address == 0xfc9c) { retval = "MON_CLREOL"; }
    else if (address == 0xfc9e) { retval = "MON_CLREOLZ"; }
    else if (address == 0xfca8) { retval = "MON_WAIT"; }
    else if (address == 0xfca9) { retval = "MON_WAIT2"; }
    else if (address == 0xfcaa) { retval = "MON_WAIT3"; }
    else if (address == 0xfcb4) { retval = "MON_NXTA4"; }
    else if (address == 0xfcba) { retval = "MON_NXTA1"; }
    else if (address == 0xfcc8) { retval = "MON_RTS4B"; }
    else if (address == 0xfcc9) { retval = "MON_HEADR"; }
    else if (address == 0xfcd6) { retval = "MON_WRBIT"; }
    else if (address == 0xfcdb) { retval = "MON_ZERDLY"; }
    else if (address == 0xfce2) { retval = "MON_ONEDLY"; }
    else if (address == 0xfce5) { retval = "MON_WRTAPE"; }
    else if (address == 0xfcec) { retval = "MON_RDBYTE"; }
    else if (address == 0xfcee) { retval = "MON_RDBYT2"; }
    else if (address == 0xfcfa) { retval = "MON_READ2BIT"; }
    else if (address == 0xfcfd) { retval = "MON_RDBIT"; }
    else if (address == 0xfd0c) { retval = "MON_RDKEY"; }


    else if (address == 0xfd18) { retval = "MON_KEYIN"; }
    else if (address == 0xfd21) { retval = "MON_RDESC"; }

    else if (address == 0xfd29) { retval = "M80_FUNCEXIT"; }

    else if (address == 0xfd2f) { retval = "MON_ESC"; }
    else if (address == 0xfd35) { retval = "MON_RDCHAR"; }
    else if (address == 0xfd3d) { retval = "MON_NOTCR"; }
    else if (address == 0xfd5f) { retval = "MON_NOTCR1"; }
    else if (address == 0xfd62) { retval = "MON_CANCEL"; }
    else if (address == 0xfd67) { retval = "MON_GETLNZ"; }
    else if (address == 0xfd6a) { retval = "MON_GETLN"; }
    else if (address == 0xfd71) { retval = "MON_BCKSPC"; }
    else if (address == 0xfd75) { retval = "MON_NXTCHAR"; }
    else if (address == 0xfd7e) { retval = "MON_CAPTST"; }
    else if (address == 0xfd84) { retval = "MON_ADDINP"; }
    else if (address == 0xfd8e) { retval = "MON_CROUT"; }
    else if (address == 0xfd92) { retval = "MON_PRA1"; }
    else if (address == 0xfd96) { retval = "MON_PRYX2"; }
    else if (address == 0xfda3) { retval = "MON_XAMB"; }
    else if (address == 0xfdad) { retval = "MON_MOD8CHK"; }
    else if (address == 0xfdb3) { retval = "MON_XAM"; }
    else if (address == 0xfdb6) { retval = "MON_DATAOUT"; }
    else if (address == 0xfdc5) { retval = "MON_RTS4C"; }
    else if (address == 0xfdc6) { retval = "MON_XAMPM"; }
    else if (address == 0xfdd1) { retval = "MON_ADD"; }
    else if (address == 0xfdda) { retval = "MON_PRBYTE"; }
    else if (address == 0xfde3) { retval = "MON_PRHEX"; }
    else if (address == 0xfde5) { retval = "MON_PRHEXZ"; }
    else if (address == 0xfded) { retval = "MON_COUT"; }
    else if (address == 0xfdf0) { retval = "MON_COUT1"; }
    else if (address == 0xfdf6) { retval = "MON_COUTZ"; }
    else if (address == 0xfe00) { retval = "MON_BLI"; }
    else if (address == 0xfe04) { retval = "MON_BLANK"; }
    else if (address == 0xfe0b) { retval = "MON_STOR"; }
    else if (address == 0xfe17) { retval = "MON_RTS5"; }
    else if (address == 0xfe18) { retval = "MON_SETMODE"; }
    else if (address == 0xfe1d) { retval = "MON_SETMDZ"; }
    else if (address == 0xfe20) { retval = "MON_LT"; }
    else if (address == 0xfe22) { retval = "MON_LT2"; }
    else if (address == 0xfe2c) { retval = "MON_MOVE"; }
    else if (address == 0xfe36) { retval = "MON_VFY"; }
    else if (address == 0xfe58) { retval = "MON_VFYOK"; }
    else if (address == 0xfe5e) { retval = "MON_LIST"; }
    else if (address == 0xfe63) { retval = "MON_LIST2"; }
    else if (address == 0xfe75) { retval = "MON_A1PC"; }
    else if (address == 0xfe78) { retval = "MON_A1PCLP"; }
    else if (address == 0xfe7f) { retval = "MON_A1PCRTS"; }
    else if (address == 0xfe80) { retval = "MON_SETINV"; }
    else if (address == 0xfe84) { retval = "MON_SETNORM"; }
    else if (address == 0xfe86) { retval = "MON_SETIFLG"; }
    else if (address == 0xfe89) { retval = "MON_SETKBD"; }
    else if (address == 0xfe8b) { retval = "MON_INPORT"; }
    else if (address == 0xfe8d) { retval = "MON_INPRT"; }
    else if (address == 0xfe93) { retval = "MON_SETVID"; }
    else if (address == 0xfe95) { retval = "MON_OUTPORT"; }
    else if (address == 0xfe97) { retval = "MON_OUTPRT"; }
    else if (address == 0xfe9b) { retval = "MON_IOPRT"; }
    else if (address == 0xfea7) { retval = "MON_IOPRT1"; }
    else if (address == 0xfea9) { retval = "MON_IOPRT2"; }
    else if (address == 0xfeaf) { retval = "MON_CKSUMFIX"; }
    else if (address == 0xfeb0) { retval = "MON_XBASIC"; }
    else if (address == 0xfeb3) { retval = "MON_BASCONT"; }
    else if (address == 0xfeb6) { retval = "MON_GO"; }
    else if (address == 0xfebf) { retval = "MON_REGZ"; }
    else if (address == 0xfec2) { retval = "MON_TRACE"; }
    else if (address == 0xfec4) { retval = "MON_STEPZ"; }
    else if (address == 0xfeca) { retval = "MON_USR"; }
    else if (address == 0xfecd) { retval = "MON_WRITE"; }
    else if (address == 0xfed4) { retval = "MON_WR1"; }
    else if (address == 0xfeed) { retval = "MON_WRBYTE"; }
    else if (address == 0xfeef) { retval = "MON_WRBYT2"; }
    else if (address == 0xfef6) { retval = "MON_CRMON"; }
    else if (address == 0xfffd) { retval = "MON_READ"; }
    else if (address == 0xff0a) { retval = "MON_RD2"; }
    else if (address == 0xff16) { retval = "MON_RD3"; }
    else if (address == 0xff2d) { retval = "MON_PRERR"; }
    else if (address == 0xff3a) { retval = "MON_BELL"; }
    else if (address == 0xff3f) { retval = "MON_RESTORE"; }
    else if (address == 0xff44) { retval = "MON_RESTR1"; }
    else if (address == 0xff4a) { retval = "MON_SAVE"; }
    else if (address == 0xff4c) { retval = "MON_SAV1"; }

    else if (address == 0xff58) { retval = "M80_IORTS"; }

    else if (address == 0xff59) { retval = "MON_OLDRST"; }
    else if (address == 0xff65) { retval = "MON_MON"; }
    else if (address == 0xff69) { retval = "MON_MONZ"; }
    else if (address == 0xff73) { retval = "MON_NXTITM"; }
    else if (address == 0xff7a) { retval = "MON_CRSRCH"; }
    else if (address == 0xff8a) { retval = "MON_DIG"; }
    else if (address == 0xff90) { retval = "MON_NXTBIT"; }
    else if (address == 0xff98) { retval = "MON_NXTBAS"; }
    else if (address == 0xffa2) { retval = "MON_NXTBS2"; }
    else if (address == 0xffa7) { retval = "MON_GETNUM"; }
    else if (address == 0xffad) { retval = "MON_NXTCHR"; }
    else if (address == 0xffbe) { retval = "MON_TOSUB"; }
    else if (address == 0xffc7) { retval = "MON_ZMODE"; }
    else if (address == 0xffcc) { retval = "MON_CHRTBL"; }
    else if (address == 0xffe3) { retval = "MON_SUBTBL"; }


    return retval;
}

void DisassemblerViewer::setData(QByteArray data)
{
    ui->textArea->setText(data);
}

void DisassemblerViewer::setText(QString text)
{
    ui->textArea->setHtml(text);
}
