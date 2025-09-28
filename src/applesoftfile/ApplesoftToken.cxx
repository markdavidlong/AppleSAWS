#include "ApplesoftToken.h"

QMap<quint16, QString> ApplesoftToken::m_tokens = QMap<quint16, QString>();

ApplesoftToken::ApplesoftToken()
{
    if (m_tokens.size() == 0) { initializeTokenTable(); }
    setTokenId(DefaultTokenVal);
}

ApplesoftToken::ApplesoftToken(quint16 id)
{
    if (m_tokens.size() == 0) { initializeTokenTable(); }
    setTokenId(id);
}

ApplesoftToken::ApplesoftToken(quint16 id, QVariant payload)
{
    if (m_tokens.size() == 0) { initializeTokenTable(); }
    setTokenId(id);
    setValue(payload);
}

void ApplesoftToken::setTokenId(quint16 id)
{
    m_token_id = id;
    m_token_type = UNKNOWN_TOKEN;
    m_command_type = NONE;

    if (id <= 0x7f) {
        setValue(id);
        m_token_type = ASCIICHAR_TOKEN;
        m_command_type = NONE;
    } else if (id <= 0xC7) {
        setValue(id);
        m_token_type = APPLESOFT_TOKEN;
        m_command_type = COMMAND;
    } else if (id <= 0xD1) {
        setValue(id);
        m_token_type = APPLESOFT_TOKEN;
        m_command_type = OPERATOR;
    } else if (id <= 0xEA) {
        setValue(id);
        m_token_type = APPLESOFT_TOKEN;
        m_command_type = FUNCTION;
    } else if (id <= 0xFF) {
        setValue(id);
        m_token_type = UNDEFINED_APPLESOFT_TOKEN;
        m_command_type = NONE;
    } else if (id == StringTokenVal) {
        m_token_type = STRING_TOKEN;
        m_command_type = NONE;
    } else if (id == RemStringTokenVal) {
        m_token_type = REM_TOKEN;
        m_command_type = NONE;
    } else if (id == DataStringTokenVal) {
        m_token_type = DATASTRING_TOKEN;
        m_command_type = NONE;
    } else if (id == IntegerTokenVal) {
        m_token_type = INTEGER_TOKEN;
        m_command_type = NONE;
    } else if (id == FloatTokenVal) {
        m_token_type = FLOAT_TOKEN;
        m_command_type = NONE;
    } else if (id == IntVarTokenVal) {
        m_token_type = INT_VARIABLE_TOKEN;
        m_command_type = NONE;
    } else if (id == IntAryVarTokenVal) {
        m_token_type = INT_ARY_VARIABLE_TOKEN;
        m_command_type = NONE;
    } else if (id == FloatVarTokenVal) {
        m_token_type = FLOAT_VARIABLE_TOKEN;
        m_command_type = NONE;
    } else if (id == FloatAryVarTokenVal) {
        m_token_type = FLOAT_ARY_VARIABLE_TOKEN;
        m_command_type = NONE;
    } else if (id == StringVarTokenVal) {
        m_token_type = STRING_VARIABLE_TOKEN;
        m_command_type = NONE;
    } else if (id == StringAryVarTokenVal) {
        m_token_type = STRING_ARY_VARIABLE_TOKEN;
        m_command_type = NONE;
    } else if (id >= 0xe000 && id < 0xf000) {
        m_token_type = OPTIONAL_FORMAT_TOKEN;
        m_command_type = OPTIONAL_FORMAT;
    }
}

void ApplesoftToken::setValue(QVariant value)
{
    m_payload = value;
}



QString ApplesoftToken::getRawPrintableString() const
{
    if (m_token_id == DefaultTokenVal) {
        return "";
    } else if (m_token_id == 0x00) {
        return "";
    } else if (m_token_id <= 0x7f) {
        return QString((m_token_id));
    } else if (m_token_id <= 0xff) {
        return m_tokens[m_token_id];
    } else if (m_token_id == StringTokenVal) {
        return getStringValue();
    } else if (m_token_id == RemStringTokenVal) {
        return getStringValue();
    } else if (m_token_id == DataStringTokenVal) {
        return getStringValue();
    } else if (m_token_id == IntegerTokenVal) {
        return getStringValue();
    } else if (m_token_id == FloatTokenVal) {
        return getStringValue();
    } else if (m_token_id == IntVarTokenVal) {
        return getStringValue();
    } else if (m_token_id == IntAryVarTokenVal) {
        return getStringValue();
    } else if (m_token_id == FloatVarTokenVal) {
        return getStringValue();
    } else if (m_token_id == FloatAryVarTokenVal) {
        return getStringValue();
    } else if (m_token_id == StringVarTokenVal) {
        return getStringValue();
    } else if (m_token_id == StringAryVarTokenVal) {
        return getStringValue();
    } else if (m_token_id >= 0xe000 && m_token_id < 0xf000) {
        return "";
    } else {
        return "[temp undefined]";
    }
}

QTextCharFormat ApplesoftToken::defaultTextFormat()
{
    QTextCharFormat tf;  // Default
    tf.setForeground(Qt::black);
    return tf;
}

QTextCharFormat ApplesoftToken::defaultInverseTextFormat()
{
    QTextCharFormat tf;
    tf.setForeground(Qt::white);
    tf.setBackground(Qt::black);
    return tf;
}

QTextCharFormat ApplesoftToken::textFormat(quint16 tokenType)
{
    QTextCharFormat tf = defaultTextFormat();

    if (tokenType < 0x80) // Ascii
    {
        tf.setForeground(Qt::black);
    }
    else if (tokenType < 0x100) // Applesoft Tokens
    {
        tf.setForeground(Qt::black);
    }
    else if (tokenType == StringTokenVal)
    {
        tf.setForeground(Qt::blue);
        tf.setFontWeight(QFont::Bold);
    }
    else if (tokenType == IntegerTokenVal || tokenType == FloatTokenVal)
    {
        tf.setForeground(Qt::darkGreen);
        tf.setFontWeight(QFont::Bold);
    }
    else if (tokenType == StringVarTokenVal ||
             tokenType == StringAryVarTokenVal ||
             tokenType == IntVarTokenVal ||
             tokenType == IntAryVarTokenVal ||
             tokenType == FloatVarTokenVal ||
             tokenType == FloatAryVarTokenVal)
    {
        tf.setFontWeight(QFont::Bold);
        tf.setForeground(Qt::darkMagenta);
    }
    else if (tokenType == RemStringTokenVal)
    {
        tf.setForeground(Qt::darkGray);
        tf.setFontUnderline(true);
    }
    else if (tokenType == DataStringTokenVal)
    {
        tf.setForeground(Qt::darkRed);
    }
    else if (tokenType == ControlCharTokenVal)
    {
        // Inverse of StringTokenVal
        tf.setForeground(Qt::white);
        tf.setBackground(Qt::blue);
    }

    return tf;
}



void ApplesoftToken::makeTextCharFormats()
{
//    TCFDefault,
//    TCFCtrlChar,
//    TCFAscii,
//    TCFFunction,
//    TCFOperator,
//    TCFUnusedToken,
//    TCFNumber,
//    TCFString,
//    TCFVariable,
//    TCFDataString,
//    TCFRemString,
//    TCFUnknown


}

void ApplesoftToken::initializeTokenTable()
{
    m_tokens[ASEnd]      = " END ";     m_tokens[ASFor]     = " FOR ";
    m_tokens[ASNext]     = " NEXT ";    m_tokens[ASData]    = " DATA ";
    m_tokens[ASInput]    = " INPUT ";   m_tokens[ASDel]     = " DEL ";
    m_tokens[ASDim]      = " DIM ";     m_tokens[ASRead]    = " READ ";
    m_tokens[ASGr]       = " GR ";      m_tokens[ASText]    = " TEXT ";
    m_tokens[ASPr]       = " PR# ";     m_tokens[ASIn]      = " IN# ";
    m_tokens[ASCall]     = " CALL ";    m_tokens[ASPlot]    = " PLOT ";
    m_tokens[ASHlin]     = " HLIN ";    m_tokens[ASVlin]    = " VLIN ";
    m_tokens[ASHgr2]     = " HGR2 ";    m_tokens[ASHgr]     = " HGR ";
    m_tokens[ASHcolor]   = " HCOLOR= "; m_tokens[ASHplot]   = " HPLOT ";
    m_tokens[ASDraw]     = " DRAW ";    m_tokens[ASXdraw]   = " XDRAW ";
    m_tokens[ASHtab]     = " HTAB ";    m_tokens[ASHome]    = " HOME ";
    m_tokens[ASRot]      = " ROT= ";    m_tokens[ASScale]   = " SCALE= ";
    m_tokens[ASShload]   = " SHLOAD ";  m_tokens[ASTrace]   = " TRACE ";
    m_tokens[ASNotrace]  = " NOTRACE "; m_tokens[ASNormal]  = " NORMAL ";
    m_tokens[ASInverse]  = " INVERSE "; m_tokens[ASFlash]   = " FLASH ";
    m_tokens[ASColor]    = " COLOR= ";  m_tokens[ASPop]     = " POP ";
    m_tokens[ASVtab]     = " VTAB ";    m_tokens[ASHimem]   = " HIMEM: ";
    m_tokens[ASLomem]    = " LOMEM: ";  m_tokens[ASOnerr]   = " ONERR ";
    m_tokens[ASResume]   = " RESUME ";  m_tokens[ASRecall]  = " RECALL ";
    m_tokens[ASStore]    = " STORE ";   m_tokens[ASSpeed]   = " SPEED= ";
    m_tokens[ASLet]      = " LET ";     m_tokens[ASGoto]    = " GOTO ";
    m_tokens[ASRun]      = " RUN ";     m_tokens[ASIf]      = " IF ";
    m_tokens[ASRestore]  = " RESTORE "; m_tokens[ASAMP]     = " & ";
    m_tokens[ASGosub]    = " GOSUB ";   m_tokens[ASReturn]  = " RETURN ";
    m_tokens[ASRem]      = " REM ";     m_tokens[ASStop]    = " STOP ";
    m_tokens[ASOn]       = " ON ";      m_tokens[ASWait]    = " WAIT ";
    m_tokens[ASLoad]     = " LOAD ";    m_tokens[ASSave]    = " SAVE ";
    m_tokens[ASDef]      = " DEF ";     m_tokens[ASPoke]    = " POKE ";
    m_tokens[ASPrint]    = " PRINT ";   m_tokens[ASCont]    = " CONT ";
    m_tokens[ASList]     = " LIST ";    m_tokens[ASClear]   = " CLEAR ";
    m_tokens[ASGet]      = " GET ";     m_tokens[ASNew]     = " NEW ";
    m_tokens[ASTab]      = " TAB(";     m_tokens[ASTo]      = " TO ";
    m_tokens[ASFn]       = " FN ";      m_tokens[ASSpc]     = " SPC( ";
    m_tokens[ASThen]     = " THEN ";    m_tokens[ASAt]      = " AT ";
    m_tokens[ASNot]      = " NOT ";     m_tokens[ASStep]    = " STEP ";
    m_tokens[ASPLUS]     = " + ";       m_tokens[ASMINUS]   = " - ";
    m_tokens[ASASTERISK] = " * ";       m_tokens[ASSLASH]   = " / ";
    m_tokens[ASCARET]    = " ^ ";       m_tokens[ASAnd]     = " AND ";
    m_tokens[ASOr]       = "  OR ";     m_tokens[ASGREATER] = " > ";
    m_tokens[ASEQUAL]    = " = ";       m_tokens[ASLESS]    = " < ";
    m_tokens[ASSgn]      = " SGN";      m_tokens[ASInt]     = " INT";
    m_tokens[ASAbs]      = " ABS";      m_tokens[ASUsr]     = " USR";
    m_tokens[ASFre]      = " FRE";      m_tokens[ASScrn]    = " SCRN( ";
    m_tokens[ASPdl]      = " PDL";      m_tokens[ASPos]     = " POS";
    m_tokens[ASSqr]      = " SQR";      m_tokens[ASRnd]     = " RND";
    m_tokens[ASLog]      = " LOG";      m_tokens[ASExp]     = " EXP";
    m_tokens[ASCos]      = " COS";      m_tokens[ASSin]     = " SIN";
    m_tokens[ASTan]      = " TAN";      m_tokens[ASAtn]     = " ATN";
    m_tokens[ASPeek]     = " PEEK";     m_tokens[ASLen]     = " LEN";
    m_tokens[ASStr]      = " STR$";     m_tokens[ASVal]     = " VAL";
    m_tokens[ASAsc]      = " ASC";      m_tokens[ASChr]     = " CHR$";
    m_tokens[ASLeft]     = " LEFT$ ";   m_tokens[ASRight]   = " RIGHT$ ";
    m_tokens[ASMid]      = " MID$ ";
    m_tokens[AS_EB] = "{Token 0xEB} ";  m_tokens[AS_EC] = "{Token 0xEC} ";
    m_tokens[AS_ED] = "{Token 0xED} ";  m_tokens[AS_EE] = "{Token 0xEE} ";
    m_tokens[AS_EF] = "{Token 0xEF} ";  m_tokens[AS_F0] = "{Token 0xF0} ";
    m_tokens[AS_F1] = "{Token 0xF1} ";  m_tokens[AS_F2] = "{Token 0xF2} ";
    m_tokens[AS_F3] = "{Token 0xF3} ";  m_tokens[AS_F4] = "{Token 0xF4} ";
    m_tokens[AS_F5] = "{Token 0xF5} ";  m_tokens[AS_F6] = "{Token 0xF6} ";
    m_tokens[AS_F7] = "{Token 0xF7} ";  m_tokens[AS_F8] = "{Token 0xF8} ";
    m_tokens[AS_F9] = "{Token 0xF9} ";  m_tokens[AS_FA] = "{Token 0xFA} ";
    m_tokens[AS_FB] = "{Token 0xFB} ";  m_tokens[AS_FC] = "{Token 0xFC} ";
    m_tokens[AS_FD] = "{Token 0xFD} ";  m_tokens[AS_FE] = "{Token 0xFE} ";
    m_tokens[AS_FF] = "{Token 0xFF} ";
}

const quint16 ApplesoftToken::ASEnd = 0x80;      const quint16 ApplesoftToken::ASFor = 0x81;
const quint16 ApplesoftToken::ASNext = 0x82;     const quint16 ApplesoftToken::ASData = 0x83;
const quint16 ApplesoftToken::ASInput = 0x84;    const quint16 ApplesoftToken::ASDel = 0x85;
const quint16 ApplesoftToken::ASDim = 0x86;      const quint16 ApplesoftToken::ASRead = 0x87;
const quint16 ApplesoftToken::ASGr = 0x88;       const quint16 ApplesoftToken::ASText = 0x89;
const quint16 ApplesoftToken::ASPr = 0x8A;       const quint16 ApplesoftToken::ASIn = 0x8B;
const quint16 ApplesoftToken::ASCall = 0x8C;     const quint16 ApplesoftToken::ASPlot = 0x8D;
const quint16 ApplesoftToken::ASHlin = 0x8E;     const quint16 ApplesoftToken::ASVlin = 0x8F;

const quint16 ApplesoftToken::ASHgr2 = 0x90;     const quint16 ApplesoftToken::ASHgr = 0x91;
const quint16 ApplesoftToken::ASHcolor = 0x92;   const quint16 ApplesoftToken::ASHplot = 0x93;
const quint16 ApplesoftToken::ASDraw = 0x94;     const quint16 ApplesoftToken::ASXdraw = 0x95;
const quint16 ApplesoftToken::ASHtab = 0x96;     const quint16 ApplesoftToken::ASHome = 0x97;
const quint16 ApplesoftToken::ASRot = 0x98;      const quint16 ApplesoftToken::ASScale = 0x99;
const quint16 ApplesoftToken::ASShload = 0x9A;   const quint16 ApplesoftToken::ASTrace = 0x9B;
const quint16 ApplesoftToken::ASNotrace = 0x9C;  const quint16 ApplesoftToken::ASNormal = 0x9D;
const quint16 ApplesoftToken::ASInverse = 0x9E;  const quint16 ApplesoftToken::ASFlash = 0x9F;

const quint16 ApplesoftToken::ASColor = 0xA0;    const quint16 ApplesoftToken::ASPop = 0xA1;
const quint16 ApplesoftToken::ASVtab = 0xA2;     const quint16 ApplesoftToken::ASHimem = 0xA3;
const quint16 ApplesoftToken::ASLomem = 0xA4;    const quint16 ApplesoftToken::ASOnerr = 0xA5;
const quint16 ApplesoftToken::ASResume = 0xA6;   const quint16 ApplesoftToken::ASRecall = 0xA7;
const quint16 ApplesoftToken::ASStore = 0xA8;    const quint16 ApplesoftToken::ASSpeed = 0xA9;
const quint16 ApplesoftToken::ASLet = 0xAA;      const quint16 ApplesoftToken::ASGoto = 0xAB;
const quint16 ApplesoftToken::ASRun = 0xAC;      const quint16 ApplesoftToken::ASIf = 0xAD;
const quint16 ApplesoftToken::ASRestore = 0xAE;  const quint16 ApplesoftToken::ASAMP = 0xAF;

const quint16 ApplesoftToken::ASGosub = 0xB0;    const quint16 ApplesoftToken::ASReturn = 0xB1;
const quint16 ApplesoftToken::ASRem = 0xB2;      const quint16 ApplesoftToken::ASStop = 0xB3;
const quint16 ApplesoftToken::ASOn = 0xB4;       const quint16 ApplesoftToken::ASWait = 0xB5;
const quint16 ApplesoftToken::ASLoad = 0xB6;     const quint16 ApplesoftToken::ASSave = 0xB7;
const quint16 ApplesoftToken::ASDef = 0xB8;      const quint16 ApplesoftToken::ASPoke = 0xB9;
const quint16 ApplesoftToken::ASPrint = 0xBA;    const quint16 ApplesoftToken::ASCont = 0xBB;
const quint16 ApplesoftToken::ASList = 0xBC;     const quint16 ApplesoftToken::ASClear = 0xBD;
const quint16 ApplesoftToken::ASGet = 0xBE;      const quint16 ApplesoftToken::ASNew = 0xBF;

const quint16 ApplesoftToken::ASTab = 0xC0;      const quint16 ApplesoftToken::ASTo = 0xC1;
const quint16 ApplesoftToken::ASFn = 0xC2;       const quint16 ApplesoftToken::ASSpc = 0xC3;
const quint16 ApplesoftToken::ASThen = 0xC4;     const quint16 ApplesoftToken::ASAt = 0xC5;
const quint16 ApplesoftToken::ASNot = 0xC6;      const quint16 ApplesoftToken::ASStep = 0xC7;
const quint16 ApplesoftToken::ASPLUS = 0xC8;     const quint16 ApplesoftToken::ASMINUS = 0xC9;
const quint16 ApplesoftToken::ASASTERISK = 0xCA; const quint16 ApplesoftToken::ASSLASH = 0xCB;
const quint16 ApplesoftToken::ASCARET = 0xCC;    const quint16 ApplesoftToken::ASAnd = 0xCD;
const quint16 ApplesoftToken::ASOr = 0xCE;       const quint16 ApplesoftToken::ASGREATER = 0xCF;

const quint16 ApplesoftToken::ASEQUAL = 0xD0;    const quint16 ApplesoftToken::ASLESS = 0xD1;
const quint16 ApplesoftToken::ASSgn = 0xD2;      const quint16 ApplesoftToken::ASInt = 0xD3;
const quint16 ApplesoftToken::ASAbs = 0xD4;      const quint16 ApplesoftToken::ASUsr = 0xD5;
const quint16 ApplesoftToken::ASFre = 0xD6;      const quint16 ApplesoftToken::ASScrn = 0xD7;
const quint16 ApplesoftToken::ASPdl = 0xD8;      const quint16 ApplesoftToken::ASPos = 0xD9;
const quint16 ApplesoftToken::ASSqr = 0xDA;      const quint16 ApplesoftToken::ASRnd = 0xDB;
const quint16 ApplesoftToken::ASLog = 0xDC;      const quint16 ApplesoftToken::ASExp = 0xDD;
const quint16 ApplesoftToken::ASCos = 0xDE;      const quint16 ApplesoftToken::ASSin = 0xDF;

const quint16 ApplesoftToken::ASTan = 0xE0;      const quint16 ApplesoftToken::ASAtn = 0xE1;
const quint16 ApplesoftToken::ASPeek = 0xE2;     const quint16 ApplesoftToken::ASLen = 0xE3;
const quint16 ApplesoftToken::ASStr = 0xE4;      const quint16 ApplesoftToken::ASVal = 0xE5;
const quint16 ApplesoftToken::ASAsc = 0xE6;      const quint16 ApplesoftToken::ASChr = 0xE7;
const quint16 ApplesoftToken::ASLeft = 0xE8;     const quint16 ApplesoftToken::ASRight = 0xE9;
const quint16 ApplesoftToken::ASMid = 0xEA;      const quint16 ApplesoftToken::AS_EB = 0xEB;
const quint16 ApplesoftToken::AS_EC = 0xEC;      const quint16 ApplesoftToken::AS_ED = 0xED;
const quint16 ApplesoftToken::AS_EE = 0xEE;      const quint16 ApplesoftToken::AS_EF = 0xEF;

const quint16 ApplesoftToken::AS_F0 = 0xF0;      const quint16 ApplesoftToken::AS_F1 = 0xF1;
const quint16 ApplesoftToken::AS_F2 = 0xF2;      const quint16 ApplesoftToken::AS_F3 = 0xF3;
const quint16 ApplesoftToken::AS_F4 = 0xF4;      const quint16 ApplesoftToken::AS_F5 = 0xF5;
const quint16 ApplesoftToken::AS_F6 = 0xF6;      const quint16 ApplesoftToken::AS_F7 = 0xF7;
const quint16 ApplesoftToken::AS_F8 = 0xF8;      const quint16 ApplesoftToken::AS_F9 = 0xF9;
const quint16 ApplesoftToken::AS_FA = 0xFA;      const quint16 ApplesoftToken::AS_FB = 0xFB;
const quint16 ApplesoftToken::AS_FC = 0xFC;      const quint16 ApplesoftToken::AS_FD = 0xFD;
const quint16 ApplesoftToken::AS_FE = 0xFE;      const quint16 ApplesoftToken::AS_FF = 0xFF;
