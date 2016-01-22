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

    static const quint16 ASEnd;      static const quint16 ASFor;
    static const quint16 ASNext;     static const quint16 ASData;
    static const quint16 ASInput;    static const quint16 ASDel;
    static const quint16 ASDim;      static const quint16 ASRead;
    static const quint16 ASGr;       static const quint16 ASText;
    static const quint16 ASPr;       static const quint16 ASIn;
    static const quint16 ASCall;     static const quint16 ASPlot;
    static const quint16 ASHlin;     static const quint16 ASVlin;

    static const quint16 ASHgr2;     static const quint16 ASHgr;
    static const quint16 ASHcolor;   static const quint16 ASHplot;
    static const quint16 ASDraw;     static const quint16 ASXdraw;
    static const quint16 ASHtab;     static const quint16 ASHome;
    static const quint16 ASRot;      static const quint16 ASScale;
    static const quint16 ASShload;   static const quint16 ASTrace;
    static const quint16 ASNotrace;  static const quint16 ASNormal;
    static const quint16 ASInverse;  static const quint16 ASFlash;

    static const quint16 ASColor;    static const quint16 ASPop;
    static const quint16 ASVtab;     static const quint16 ASHimem;
    static const quint16 ASLomem;    static const quint16 ASOnerr;
    static const quint16 ASResume;   static const quint16 ASRecall;
    static const quint16 ASStore;    static const quint16 ASSpeed;
    static const quint16 ASLet;      static const quint16 ASGoto;
    static const quint16 ASRun;      static const quint16 ASIf;
    static const quint16 ASRestore;  static const quint16 ASAMP;

    static const quint16 ASGosub;    static const quint16 ASReturn;
    static const quint16 ASRem;      static const quint16 ASStop;
    static const quint16 ASOn;       static const quint16 ASWait;
    static const quint16 ASLoad;     static const quint16 ASSave;
    static const quint16 ASDef;      static const quint16 ASPoke;
    static const quint16 ASPrint;    static const quint16 ASCont;
    static const quint16 ASList;     static const quint16 ASClear;
    static const quint16 ASGet;      static const quint16 ASNew;

    static const quint16 ASTab;      static const quint16 ASTo;
    static const quint16 ASFn;       static const quint16 ASSpc;
    static const quint16 ASThen;     static const quint16 ASAt;
    static const quint16 ASNot;      static const quint16 ASStep;
    static const quint16 ASPLUS;     static const quint16 ASMINUS;
    static const quint16 ASASTERISK; static const quint16 ASSLASH;
    static const quint16 ASCARET;    static const quint16 ASAnd;
    static const quint16 ASOr;       static const quint16 ASGREATER;

    static const quint16 ASEQUAL;    static const quint16 ASLESS;
    static const quint16 ASSgn;      static const quint16 ASInt;
    static const quint16 ASAbs;      static const quint16 ASUsr;
    static const quint16 ASFre;      static const quint16 ASScrn;
    static const quint16 ASPdl;      static const quint16 ASPos;
    static const quint16 ASSqr;      static const quint16 ASRnd;
    static const quint16 ASLog;      static const quint16 ASExp;
    static const quint16 ASCos;      static const quint16 ASSin;

    static const quint16 ASTan;      static const quint16 ASAtn;
    static const quint16 ASPeek;     static const quint16 ASLen;
    static const quint16 ASStr;      static const quint16 ASVal;
    static const quint16 ASAsc;      static const quint16 ASChr;
    static const quint16 ASLeft;     static const quint16 ASRight;
    static const quint16 ASMid;      static const quint16 AS_EB;
    static const quint16 AS_EC;      static const quint16 AS_ED;
    static const quint16 AS_EE;      static const quint16 AS_EF;

    static const quint16 AS_F0;      static const quint16 AS_F1;
    static const quint16 AS_F2;      static const quint16 AS_F3;
    static const quint16 AS_F4;      static const quint16 AS_F5;
    static const quint16 AS_F6;      static const quint16 AS_F7;
    static const quint16 AS_F8;      static const quint16 AS_F9;
    static const quint16 AS_FA;      static const quint16 AS_FB;
    static const quint16 AS_FC;      static const quint16 AS_FD;
    static const quint16 AS_FE;      static const quint16 AS_FF;

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
