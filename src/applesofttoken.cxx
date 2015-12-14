#include "applesofttoken.h"

QMap<quint16, QString> ApplesoftToken::m_tokens = QMap<quint16, QString>();

ApplesoftToken::ApplesoftToken()
{
    if (m_tokens.size() == 0) { initializeTokenTable(); }
    setTokenId(0xFFFF);
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

    if (id <= 0xff) {
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
    } else if (id == ApplesoftToken::StringTokenVal) {
        m_token_type = STRING_TOKEN;
        m_command_type = NONE;
    } else if (id == ApplesoftToken::RemStringTokenVal) {
        m_token_type = REM_TOKEN;
        m_command_type = NONE;
    } else if (id == ApplesoftToken::DataStringTokenVal) {
        m_token_type = DATASTRING_TOKEN;
        m_command_type = NONE;
    } else if (id == ApplesoftToken::IntegerTokenVal) {
        m_token_type = INTEGER_TOKEN;
        m_command_type = NONE;
    } else if (id == ApplesoftToken::FloatTokenVal) {
        m_token_type = FLOAT_TOKEN;
        m_command_type = NONE;
    } else if (id == ApplesoftToken::IntVarTokenVal) {
        m_token_type = INT_VARIABLE_TOKEN;
        m_command_type = NONE;
    } else if (id == ApplesoftToken::IntAryVarTokenVal) {
        m_token_type = INT_ARY_VARIABLE_TOKEN;
        m_command_type = NONE;
    } else if (id == ApplesoftToken::FloatVarTokenVal) {
        m_token_type = FLOAT_VARIABLE_TOKEN;
        m_command_type = NONE;
    } else if (id == ApplesoftToken::FloatAryVarTokenVal) {
        m_token_type = FLOAT_ARY_VARIABLE_TOKEN;
        m_command_type = NONE;
    } else if (id == ApplesoftToken::StringVarTokenVal) {
        m_token_type = STRING_VARIABLE_TOKEN;
        m_command_type = NONE;
    } else if (id == ApplesoftToken::StringAryVarTokenVal) {
        m_token_type = STRING_ARY_VARIABLE_TOKEN;
        m_command_type = NONE;
    }

}

void ApplesoftToken::setValue(QVariant value)
{
    m_payload = value;
}

QString ApplesoftToken::getRawPrintableString()
{
    if (m_token_id == 0x00) {
        return "";
    } else if (m_token_id <= 0x7f) {
        return QString(QChar(m_token_id));
    } else if (m_token_id <= 0xff) {
        return m_tokens[m_token_id];
    } else
        return "[temp undefined]";
}

void ApplesoftToken::initializeTokenTable()
{
    m_tokens[0x80] = " END ";     m_tokens[0x81] = " FOR ";     m_tokens[0x82] = " NEXT ";
    m_tokens[0x83] = " DATA ";    m_tokens[0x84] = " INPUT ";   m_tokens[0x85] = " DEL ";
    m_tokens[0x86] = " DIM ";     m_tokens[0x87] = " READ ";    m_tokens[0x88] = " GR ";
    m_tokens[0x89] = " TEXT ";    m_tokens[0x8A] = " PR# ";     m_tokens[0x8B] = " IN# ";
    m_tokens[0x8C] = " CALL ";    m_tokens[0x8D] = " PLOT ";    m_tokens[0x8E] = " HLIN ";
    m_tokens[0x8F] = " VLIN ";    m_tokens[0x90] = " HGR2 ";    m_tokens[0x91] = " HGR ";
    m_tokens[0x92] = " HCOLOR= "; m_tokens[0x93] = " HPLOT ";   m_tokens[0x94] = " DRAW ";
    m_tokens[0x95] = " XDRAW ";   m_tokens[0x96] = " HTAB ";    m_tokens[0x97] = " HOME ";
    m_tokens[0x98] = " ROT= ";    m_tokens[0x99] = " SCALE= ";  m_tokens[0x9A] = " SHLOAD ";
    m_tokens[0x9B] = " TRACE ";   m_tokens[0x9C] = " NOTRACE "; m_tokens[0x9D] = " NORMAL ";
    m_tokens[0x9E] = " INVERSE "; m_tokens[0x9F] = " FLASH ";   m_tokens[0xA0] = " COLOR= ";
    m_tokens[0xA1] = " POP ";     m_tokens[0xA2] = " VTAB ";    m_tokens[0xA3] = " HIMEM: ";
    m_tokens[0xA4] = " LOMEM: ";  m_tokens[0xA5] = " ONERR ";   m_tokens[0xA6] = " RESUME ";
    m_tokens[0xA7] = " RECALL ";  m_tokens[0xA8] = " STORE ";   m_tokens[0xA9] = " SPEED= ";
    m_tokens[0xAA] = " LET ";     m_tokens[0xAB] = " GOTO ";    m_tokens[0xAC] = " RUN ";
    m_tokens[0xAD] = " IF ";      m_tokens[0xAE] = " RESTORE "; m_tokens[0xAF] = " & ";
    m_tokens[0xB0] = " GOSUB ";   m_tokens[0xB1] = " RETURN ";  m_tokens[0xB2] = " REM ";
    m_tokens[0xB3] = " STOP ";    m_tokens[0xB4] = " ON ";      m_tokens[0xB5] = " WAIT ";
    m_tokens[0xB6] = " LOAD ";    m_tokens[0xB7] = " SAVE ";    m_tokens[0xB8] = " DEF ";
    m_tokens[0xB9] = " POKE ";    m_tokens[0xBA] = " PRINT ";   m_tokens[0xBB] = " CONT ";
    m_tokens[0xBC] = " LIST ";    m_tokens[0xBD] = " CLEAR ";   m_tokens[0xBE] = " GET ";
    m_tokens[0xBF] = " NEW ";     m_tokens[0xC0] = " TAB(";     m_tokens[0xC1] = " TO ";
    m_tokens[0xC2] = " FN ";      m_tokens[0xC3] = " SPC( ";    m_tokens[0xC4] = " THEN ";
    m_tokens[0xC5] = " AT ";      m_tokens[0xC6] = " NOT ";     m_tokens[0xC7] = " STEP ";
    m_tokens[0xC8] = " + ";       m_tokens[0xC9] = " - ";       m_tokens[0xCA] = " * ";
    m_tokens[0xCB] = " / ";       m_tokens[0xCC] = " ^ ";       m_tokens[0xCD] = " AND ";
    m_tokens[0xCE] = "  OR ";     m_tokens[0xCF] = " > ";       m_tokens[0xD0] = " = ";
    m_tokens[0xD1] = " < ";       m_tokens[0xD2] = " SGN";      m_tokens[0xD3] = " INT";
    m_tokens[0xD4] = " ABS";      m_tokens[0xD5] = " USR";      m_tokens[0xD6] = " FRE";
    m_tokens[0xD7] = " SCRN( ";   m_tokens[0xD8] = " PDL";      m_tokens[0xD9] = " POS";
    m_tokens[0xDA] = " SQR";      m_tokens[0xDB] = " RND";      m_tokens[0xDC] = " LOG";
    m_tokens[0xDD] = " EXP";      m_tokens[0xDE] = " COS";      m_tokens[0xDF] = " SIN";
    m_tokens[0xE0] = " TAN";      m_tokens[0xE1] = " ATN";      m_tokens[0xE2] = " PEEK";
    m_tokens[0xE3] = " LEN";      m_tokens[0xE4] = " STR$";     m_tokens[0xE5] = " VAL";
    m_tokens[0xE6] = " ASC";      m_tokens[0xE7] = " CHR$";     m_tokens[0xE8] = " LEFT$ ";
    m_tokens[0xE9] = " RIGHT$ ";  m_tokens[0xEA] = " MID$ ";
    m_tokens[0xEB] = "{Token 0xEB} ";
    m_tokens[0xEC] = "{Token 0xEC} ";
    m_tokens[0xED] = "{Token 0xED} ";
    m_tokens[0xEE] = "{Token 0xEE} ";
    m_tokens[0xEF] = "{Token 0xEF} ";
    m_tokens[0xF0] = "{Token 0xF0} ";
    m_tokens[0xF1] = "{Token 0xF1} ";
    m_tokens[0xF2] = "{Token 0xF2} ";
    m_tokens[0xF3] = "{Token 0xF3} ";
    m_tokens[0xF4] = "{Token 0xF4} ";
    m_tokens[0xF5] = "{Token 0xF5} ";
    m_tokens[0xF6] = "{Token 0xF6} ";
    m_tokens[0xF7] = "{Token 0xF7} ";
    m_tokens[0xF8] = "{Token 0xF8} ";
    m_tokens[0xF9] = "{Token 0xF9} ";
    m_tokens[0xFA] = "{Token 0xFA} ";
    m_tokens[0xFB] = "{Token 0xFB} ";
    m_tokens[0xFC] = "{Token 0xFC} ";
    m_tokens[0xFD] = "{Token 0xFD} ";
    m_tokens[0xFE] = "{num}";
    m_tokens[0xFF] = "{string}";
}


