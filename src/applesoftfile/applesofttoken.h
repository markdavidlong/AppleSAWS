#ifndef APPLESOFTTOKEN_H
#define APPLESOFTTOKEN_H

#include <QString>
#include <QVariant>
#include <QMap>

class ApplesoftToken
{
public:

    static const quint16 StringTokenVal       = 0x100;
    static const quint16 RemStringTokenVal    = 0x101;
    static const quint16 DataStringTokenVal   = 0x102;
    static const quint16 IntegerTokenVal      = 0x103;
    static const quint16 FloatTokenVal        = 0x104;
    static const quint16 IntVarTokenVal       = 0x105;
    static const quint16 IntAryVarTokenVal    = 0x106;
    static const quint16 FloatVarTokenVal     = 0x107;
    static const quint16 FloatAryVarTokenVal  = 0x108;
    static const quint16 StringVarTokenVal    = 0x109;
    static const quint16 StringAryVarTokenVal = 0x10A;


    typedef enum {
        UNKNOWN_TOKEN              = 0x0,
        ASCIICHAR_TOKEN            = 0x1,
        APPLESOFT_TOKEN            = 0x2,
        UNDEFINED_APPLESOFT_TOKEN  = 0x3,
        STRING_TOKEN               = 0x4,
        INTEGER_TOKEN              = 0x5,
        FLOAT_TOKEN                = 0x6,
        DATASTRING_TOKEN           = 0x7,
        REM_TOKEN                  = 0x8,
        INT_VARIABLE_TOKEN         = 0x9,
        INT_ARY_VARIABLE_TOKEN     = 0xA,
        FLOAT_VARIABLE_TOKEN       = 0xB,
        FLOAT_ARY_VARIABLE_TOKEN   = 0xC,
        STRING_VARIABLE_TOKEN      = 0xD,
        STRING_ARY_VARIABLE_TOKEN  = 0xE
    } TokenType;

    typedef enum {
        NONE,
        COMMAND,
        OPERATOR,
        FUNCTION,
        UNDEFINED_COMMAND
    } CommandType;

    ApplesoftToken();
    ApplesoftToken(quint16 id);
    ApplesoftToken(quint16 id, QVariant payload);

    void setTokenId(quint16 id);
    quint16 getTokenId() const { return m_token_id; }

    void setValue(QVariant value);

    QVariant getValue() const { return m_payload; }
    QByteArray getByteStringValue() const { return m_payload.toByteArray(); }
    quint16 getWordValue() const { return (quint16) (m_payload.toUInt() & 0xFFFF); }
    quint8 getByteValue() const { return (quint8) (m_payload.toUInt() & 0xFF); }

    TokenType getTokenType() const { return m_token_type; }
    CommandType getCommandType() const { return m_command_type; }

    QString getRawPrintableString();

    static QString getStringForToken(quint8 token) {
        if (m_tokens.size() == 0) { initializeTokenTable(); }
        return m_tokens[token];
    }

private:

    static QMap<quint16, QString> m_tokens;

    quint16 m_token_id;
    QString m_tokenname;
    QVariant m_payload;
    TokenType m_token_type;
    CommandType m_command_type;

    static void initializeTokenTable();
};


#endif // APPLESOFTTOKEN_H
