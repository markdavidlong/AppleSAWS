#pragma once

#include <QString>
#include <QVariant>
#include <QMap>
#include <QTextCharFormat>

enum class TextCharFormatType {
    Default,
    CtrlChar,
    Ascii,
    Function,
    Operator,
    UnusedToken,
    Number,
    String,
    Variable,
    DataString,
    RemString,
    Unknown
};


class ApplesoftToken
{
public:

    static constexpr quint16 StringTokenVal       = 0x100;
    static constexpr quint16 RemStringTokenVal    = 0x101;
    static constexpr quint16 DataStringTokenVal   = 0x102;

    static constexpr quint16 IntegerTokenVal      = 0x103;
    static constexpr quint16 FloatTokenVal        = 0x104;

    static constexpr quint16 IntVarTokenVal       = 0x105;
    static constexpr quint16 IntAryVarTokenVal    = 0x106;

    static constexpr quint16 FloatVarTokenVal     = 0x107;
    static constexpr quint16 FloatAryVarTokenVal  = 0x108;

    static constexpr quint16 StringVarTokenVal    = 0x109;
    static constexpr quint16 StringAryVarTokenVal = 0x10A;

    static constexpr quint16 OptFmtLeadingSpaceTokenValue       = 0xe000;
    static constexpr quint16 OptFmtIndentLineBreakTokenValue    = 0xe001;
    static constexpr quint16 OptFmtIndentTabTokenValue          = 0xe002;
    static constexpr quint16 OptFmtIndentSpaceTokenValue        = 0xe003;
    static constexpr quint16 OptFmtFlagFlowTargetNextTokenValue = 0xe004;
    static constexpr quint16 OptFmtReturnLineBreakTokenValue    = 0xe005;

    static constexpr quint16 ControlCharTokenVal  = 0xfffd;
    static constexpr quint16 LineNumberTokenVal   = 0xfffe;
    static constexpr quint16 DefaultTokenVal      = 0xffff;



    static constexpr quint16 ASEnd = 0x80;       static constexpr quint16 ASFor = 0x81;
    static constexpr quint16 ASNext = 0x82;      static constexpr quint16 ASData = 0x83;
    static constexpr quint16 ASInput = 0x84;     static constexpr quint16 ASDel = 0x85;
    static constexpr quint16 ASDim = 0x86;       static constexpr quint16 ASRead = 0x87;
    static constexpr quint16 ASGr = 0x88;        static constexpr quint16 ASText = 0x89;
    static constexpr quint16 ASPr = 0x8A;        static constexpr quint16 ASIn = 0x8B;
    static constexpr quint16 ASCall = 0x8C;      static constexpr quint16 ASPlot = 0x8D;
    static constexpr quint16 ASHlin = 0x8E;      static constexpr quint16 ASVlin = 0x8F;

    static constexpr quint16 ASHgr2 = 0x90;      static constexpr quint16 ASHgr = 0x91;
    static constexpr quint16 ASHcolor = 0x92;    static constexpr quint16 ASHplot = 0x93;
    static constexpr quint16 ASDraw = 0x94;      static constexpr quint16 ASXdraw = 0x95;
    static constexpr quint16 ASHtab = 0x96;      static constexpr quint16 ASHome = 0x97;
    static constexpr quint16 ASRot = 0x98;       static constexpr quint16 ASScale = 0x99;
    static constexpr quint16 ASShload = 0x9A;    static constexpr quint16 ASTrace = 0x9B;
    static constexpr quint16 ASNotrace = 0x9C;   static constexpr quint16 ASNormal = 0x9D;
    static constexpr quint16 ASInverse = 0x9E;   static constexpr quint16 ASFlash = 0x9F;

    static constexpr quint16 ASColor = 0xA0;     static constexpr quint16 ASPop = 0xA1;
    static constexpr quint16 ASVtab = 0xA2;      static constexpr quint16 ASHimem = 0xA3;
    static constexpr quint16 ASLomem = 0xA4;     static constexpr quint16 ASOnerr = 0xA5;
    static constexpr quint16 ASResume = 0xA6;    static constexpr quint16 ASRecall = 0xA7;
    static constexpr quint16 ASStore = 0xA8;     static constexpr quint16 ASSpeed = 0xA9;
    static constexpr quint16 ASLet = 0xAA;       static constexpr quint16 ASGoto = 0xAB;
    static constexpr quint16 ASRun = 0xAC;       static constexpr quint16 ASIf = 0xAD;
    static constexpr quint16 ASRestore = 0xAE;   static constexpr quint16 ASAMP = 0xAF;

    static constexpr quint16 ASGosub = 0xB0;     static constexpr quint16 ASReturn = 0xB1;
    static constexpr quint16 ASRem = 0xB2;       static constexpr quint16 ASStop = 0xB3;
    static constexpr quint16 ASOn = 0xB4;        static constexpr quint16 ASWait = 0xB5;
    static constexpr quint16 ASLoad = 0xB6;      static constexpr quint16 ASSave = 0xB7;
    static constexpr quint16 ASDef = 0xB8;       static constexpr quint16 ASPoke = 0xB9;
    static constexpr quint16 ASPrint = 0xBA;     static constexpr quint16 ASCont = 0xBB;
    static constexpr quint16 ASList = 0xBC;      static constexpr quint16 ASClear = 0xBD;
    static constexpr quint16 ASGet = 0xBE;       static constexpr quint16 ASNew = 0xBF;

    static constexpr quint16 ASTab = 0xC0;       static constexpr quint16 ASTo = 0xC1;
    static constexpr quint16 ASFn = 0xC2;        static constexpr quint16 ASSpc = 0xC3;
    static constexpr quint16 ASThen = 0xC4;      static constexpr quint16 ASAt = 0xC5;
    static constexpr quint16 ASNot = 0xC6;       static constexpr quint16 ASStep = 0xC7;
    static constexpr quint16 ASPLUS = 0xC8;      static constexpr quint16 ASMINUS = 0xC9;
    static constexpr quint16 ASASTERISK = 0xCA;  static constexpr quint16 ASSLASH = 0xCB;
    static constexpr quint16 ASCARET = 0xCC;     static constexpr quint16 ASAnd = 0xCD;
    static constexpr quint16 ASOr = 0xCE;        static constexpr quint16 ASGREATER = 0xCF;

    static constexpr quint16 ASEQUAL = 0xD0;     static constexpr quint16 ASLESS = 0xD1;
    static constexpr quint16 ASSgn = 0xD2;       static constexpr quint16 ASInt = 0xD3;
    static constexpr quint16 ASAbs = 0xD4;       static constexpr quint16 ASUsr = 0xD5;
    static constexpr quint16 ASFre = 0xD6;       static constexpr quint16 ASScrn = 0xD7;
    static constexpr quint16 ASPdl = 0xD8;       static constexpr quint16 ASPos = 0xD9;
    static constexpr quint16 ASSqr = 0xDA;       static constexpr quint16 ASRnd = 0xDB;
    static constexpr quint16 ASLog = 0xDC;       static constexpr quint16 ASExp = 0xDD;
    static constexpr quint16 ASCos = 0xDE;       static constexpr quint16 ASSin = 0xDF;

    static constexpr quint16 ASTan = 0xE0;       static constexpr quint16 ASAtn = 0xE1;
    static constexpr quint16 ASPeek = 0xE2;      static constexpr quint16 ASLen = 0xE3;
    static constexpr quint16 ASStr = 0xE4;       static constexpr quint16 ASVal = 0xE5;
    static constexpr quint16 ASAsc = 0xE6;       static constexpr quint16 ASChr = 0xE7;
    static constexpr quint16 ASLeft = 0xE8;      static constexpr quint16 ASRight = 0xE9;
    static constexpr quint16 ASMid = 0xEA;       static constexpr quint16 AS_EB = 0xEB;
    static constexpr quint16 AS_EC = 0xEC;       static constexpr quint16 AS_ED = 0xED;
    static constexpr quint16 AS_EE = 0xEE;       static constexpr quint16 AS_EF = 0xEF;

    static constexpr quint16 AS_F0 = 0xF0;       static constexpr quint16 AS_F1 = 0xF1;
    static constexpr quint16 AS_F2 = 0xF2;       static constexpr quint16 AS_F3 = 0xF3;
    static constexpr quint16 AS_F4 = 0xF4;       static constexpr quint16 AS_F5 = 0xF5;
    static constexpr quint16 AS_F6 = 0xF6;       static constexpr quint16 AS_F7 = 0xF7;
    static constexpr quint16 AS_F8 = 0xF8;       static constexpr quint16 AS_F9 = 0xF9;
    static constexpr quint16 AS_FA = 0xFA;       static constexpr quint16 AS_FB = 0xFB;
    static constexpr quint16 AS_FC = 0xFC;       static constexpr quint16 AS_FD = 0xFD;
    static constexpr quint16 AS_FE = 0xFE;       static constexpr quint16 AS_FF = 0xFF;

private:
    static inline const QMap<quint16, QString> m_tokens = {
        {ASEnd, " END "},        {ASFor, " FOR "},
        {ASNext, " NEXT "},      {ASData, " DATA "},
        {ASInput, " INPUT "},    {ASDel, " DEL "},
        {ASDim, " DIM "},       {ASRead, " READ "},
        {ASGr, " GR "},         {ASText, " TEXT "},
        {ASPr, " PR# "},        {ASIn, " IN# "},
        {ASCall, " CALL "},     {ASPlot, " PLOT "},
        {ASHlin, " HLIN "},     {ASVlin, " VLIN "},
        {ASHgr2, " HGR2 "},     {ASHgr, " HGR "},
        {ASHcolor, " HCOLOR= "}, {ASHplot, " HPLOT "},
        {ASDraw, " DRAW "},     {ASXdraw, " XDRAW "},
        {ASHtab, " HTAB "},     {ASHome, " HOME "},
        {ASRot, " ROT= "},      {ASScale, " SCALE= "},
        {ASShload, " SHLOAD "}, {ASTrace, " TRACE "},
        {ASNotrace, " NOTRACE "}, {ASNormal, " NORMAL "},
        {ASInverse, " INVERSE "}, {ASFlash, " FLASH "},
        {ASColor, " COLOR= "},  {ASPop, " POP "},
        {ASVtab, " VTAB "},     {ASHimem, " HIMEM: "},
        {ASLomem, " LOMEM: "},  {ASOnerr, " ONERR "},
        {ASResume, " RESUME "}, {ASRecall, " RECALL "},
        {ASStore, " STORE "},   {ASSpeed, " SPEED= "},
        {ASLet, " LET "},       {ASGoto, " GOTO "},
        {ASRun, " RUN "},       {ASIf, " IF "},
        {ASRestore, " RESTORE "}, {ASAMP, " & "},
        {ASGosub, " GOSUB "},   {ASReturn, " RETURN "},
        {ASRem, " REM "},       {ASStop, " STOP "},
        {ASOn, " ON "},         {ASWait, " WAIT "},
        {ASLoad, " LOAD "},     {ASSave, " SAVE "},
        {ASDef, " DEF "},       {ASPoke, " POKE "},
        {ASPrint, " PRINT "},   {ASCont, " CONT "},
        {ASList, " LIST "},     {ASClear, " CLEAR "},
        {ASGet, " GET "},       {ASNew, " NEW "},
        {ASTab, " TAB("},       {ASTo, " TO "},
        {ASFn, " FN "},         {ASSpc, " SPC( "},
        {ASThen, " THEN "},     {ASAt, " AT "},
        {ASNot, " NOT "},       {ASStep, " STEP "},
        {ASPLUS, " + "},        {ASMINUS, " - "},
        {ASASTERISK, " * "},    {ASSLASH, " / "},
        {ASCARET, " ^ "},       {ASAnd, " AND "},
        {ASOr, "  OR "},        {ASGREATER, " > "},
        {ASEQUAL, " = "},       {ASLESS, " < "},
        {ASSgn, " SGN"},        {ASInt, " INT"},
        {ASAbs, " ABS"},        {ASUsr, " USR"},
        {ASFre, " FRE"},        {ASScrn, " SCRN( "},
        {ASPdl, " PDL"},        {ASPos, " POS"},
        {ASSqr, " SQR"},        {ASRnd, " RND"},
        {ASLog, " LOG"},        {ASExp, " EXP"},
        {ASCos, " COS"},        {ASSin, " SIN"},
        {ASTan, " TAN"},        {ASAtn, " ATN"},
        {ASPeek, " PEEK"},      {ASLen, " LEN"},
        {ASStr, " STR$"},       {ASVal, " VAL"},
        {ASAsc, " ASC"},        {ASChr, " CHR$"},
        {ASLeft, " LEFT$ "},    {ASRight, " RIGHT$ "},
        {ASMid, " MID$ "},
        {AS_EB, "{Token 0xEB} "}, {AS_EC, "{Token 0xEC} "},
        {AS_ED, "{Token 0xED} "}, {AS_EE, "{Token 0xEE} "},
        {AS_EF, "{Token 0xEF} "}, {AS_F0, "{Token 0xF0} "},
        {AS_F1, "{Token 0xF1} "}, {AS_F2, "{Token 0xF2} "},
        {AS_F3, "{Token 0xF3} "}, {AS_F4, "{Token 0xF4} "},
        {AS_F5, "{Token 0xF5} "}, {AS_F6, "{Token 0xF6} "},
        {AS_F7, "{Token 0xF7} "}, {AS_F8, "{Token 0xF8} "},
        {AS_F9, "{Token 0xF9} "}, {AS_FA, "{Token 0xFA} "},
        {AS_FB, "{Token 0xFB} "}, {AS_FC, "{Token 0xFC} "},
        {AS_FD, "{Token 0xFD} "}, {AS_FE, "{Token 0xFE} "},
        {AS_FF, "{Token 0xFF} "}
    };

    static constexpr quint16 NullChar             = 0x00;
    static constexpr quint16 MaxAsciiChar         = 0x7f;
    static constexpr quint16 MaxApplesoftCommand  = 0xC7;
    static constexpr quint16 MaxApplesoftOperator = 0xD1;
    static constexpr quint16 MaxApplesoftFunction = 0xEA;
    static constexpr quint16 MaxApplesoftToken    = 0xFF;
    static constexpr quint16 OptFmtRangeStart     = 0xe000;
    static constexpr quint16 OptFmtRangeEnd       = 0xf000;


public:
    enum class TokenType {
        Unknown              = 0x0,
        AsciiChar            = 0x1,
        ApplesoftToken       = 0x2,
        UndefinedApplesoft   = 0x3,
        String               = 0x4,
        Integer              = 0x5,
        Float                = 0x6,
        DataString           = 0x7,
        Rem                  = 0x8,
        IntVariable          = 0x9,
        IntAryVariable       = 0xA,
        FloatVariable        = 0xB,
        FloatAryVariable     = 0xC,
        StringVariable       = 0xD,
        StringAryVariable    = 0xE,
        OptionalFormat       = 0xF
    };

    enum class CommandType {
        None,
        Command,
        Operator,
        Function,
        UndefinedCommand,
        OptionalFormat
    };

    ApplesoftToken();
    explicit ApplesoftToken(quint16 id);
    ApplesoftToken(quint16 id, QVariant payload);

    void setTokenId(quint16 id);
    [[nodiscard]] inline quint16 getTokenId() const noexcept { return m_token_id; }

    void setValue(QVariant value);
    [[nodiscard]] inline QVariant getValue() const noexcept { return m_payload; }

    [[nodiscard]] inline QByteArray getByteStringValue() const noexcept { return m_payload.toByteArray(); }
    [[nodiscard]] QString getStringValue() const;
    [[nodiscard]] inline quint32 getUnsignedIntegerValue() const noexcept { return m_payload.toUInt(); }
    [[nodiscard]] inline qint32 getIntegerValue() const noexcept { return m_payload.toInt(); }

    [[nodiscard]] inline quint16 getWordValue() const noexcept { return quint16(m_payload.toUInt() & 0xFFFF); }
    [[nodiscard]] inline quint8 getByteValue() const noexcept { return quint8(m_payload.toUInt() & 0xFF); }

    [[nodiscard]] inline TokenType getTokenType() const noexcept { return m_token_type; }
    [[nodiscard]] inline CommandType getCommandType() const noexcept { return m_command_type; }

    [[nodiscard]] QString getRawPrintableString() const;

    [[nodiscard]] QTextCharFormat textFormat();

    [[nodiscard]] static QTextCharFormat defaultTextFormat();
    [[nodiscard]] static QTextCharFormat defaultInverseTextFormat();
    [[nodiscard]] static QTextCharFormat textFormat(quint16 tokentype);

    [[nodiscard]] QString getStringForToken(quint8 token);

    [[nodiscard]] inline bool isOptFmtToken() const noexcept { return (m_token_id >= OptFmtRangeStart && m_token_id < OptFmtRangeEnd); }

private:
    quint16 m_token_id{0};
    QString m_tokenname{};
    QVariant m_payload{};
    TokenType m_token_type{TokenType::Unknown};
    CommandType m_command_type{CommandType::None};
};


