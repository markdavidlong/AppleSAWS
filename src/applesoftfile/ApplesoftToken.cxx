#include "ApplesoftToken.h"

ApplesoftToken::ApplesoftToken()
{
    setTokenId(DefaultTokenVal);
}

ApplesoftToken::ApplesoftToken(quint16 id)
{
    setTokenId(id);
}

ApplesoftToken::ApplesoftToken(quint16 id, QVariant payload)
{
    setTokenId(id);
    setValue(payload);
}

void ApplesoftToken::setTokenId(quint16 id)
{
    m_token_id = id;
    m_token_type = TokenType::Unknown;
    m_command_type = CommandType::None;

    if (id <= MaxAsciiChar) {
        setValue(id);
        m_token_type = TokenType::AsciiChar;
        m_command_type = CommandType::None;
    } else if (id <= MaxApplesoftCommand) {
        setValue(id);
        m_token_type = TokenType::ApplesoftToken;
        m_command_type = CommandType::Command;
    } else if (id <= MaxApplesoftOperator) {
        setValue(id);
        m_token_type = TokenType::ApplesoftToken;
        m_command_type = CommandType::Operator;
    } else if (id <= MaxApplesoftFunction) {
        setValue(id);
        m_token_type = TokenType::ApplesoftToken;
        m_command_type = CommandType::Function;
    } else if (id <= MaxApplesoftToken) {
        setValue(id);
        m_token_type = TokenType::UndefinedApplesoft;
        m_command_type = CommandType::None;
    } else if (id == StringTokenVal) {
        m_token_type = TokenType::String;
        m_command_type = CommandType::None;
    } else if (id == RemStringTokenVal) {
        m_token_type = TokenType::Rem;
        m_command_type = CommandType::None;
    } else if (id == DataStringTokenVal) {
        m_token_type = TokenType::DataString;
        m_command_type = CommandType::None;
    } else if (id == IntegerTokenVal) {
        m_token_type = TokenType::Integer;
        m_command_type = CommandType::None;
    } else if (id == FloatTokenVal) {
        m_token_type = TokenType::Float;
        m_command_type = CommandType::None;
    } else if (id == IntVarTokenVal) {
        m_token_type = TokenType::IntVariable;
        m_command_type = CommandType::None;
    } else if (id == IntAryVarTokenVal) {
        m_token_type = TokenType::IntAryVariable;
        m_command_type = CommandType::None;
    } else if (id == FloatVarTokenVal) {
        m_token_type = TokenType::FloatVariable;
        m_command_type = CommandType::None;
    } else if (id == FloatAryVarTokenVal) {
        m_token_type = TokenType::FloatAryVariable;
        m_command_type = CommandType::None;
    } else if (id == StringVarTokenVal) {
        m_token_type = TokenType::StringVariable;
        m_command_type = CommandType::None;
    } else if (id == StringAryVarTokenVal) {
        m_token_type = TokenType::StringAryVariable;
        m_command_type = CommandType::None;
    } else if (id >= OptFmtRangeStart && id < OptFmtRangeEnd) {
        m_token_type = TokenType::OptionalFormat;
        m_command_type = CommandType::OptionalFormat;
    }
}

void ApplesoftToken::setValue(QVariant value)
{
    m_payload = value;
}

QString ApplesoftToken::getStringValue() const
{
    QString str = m_payload.toString();
    // Remove trailing NUL character if present
    if (!str.isEmpty() && str.back() == QChar(0)) {
        str.chop(1);
    }
    return str;
}

QTextCharFormat ApplesoftToken::textFormat()
{
    return textFormat(m_token_id);
}

QString ApplesoftToken::getStringForToken(quint8 token)
{
    return m_tokens.value(token);
}



QString ApplesoftToken::getRawPrintableString() const
{
    if (m_token_id == DefaultTokenVal) {
        return "";
    } else if (m_token_id == NullChar) {
        return "";
    } else if (m_token_id <= MaxAsciiChar) {
        return QString(static_cast<char>(m_token_id));
    } else if (m_token_id <= MaxApplesoftToken) {
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
    } else if (m_token_id >= OptFmtRangeStart && m_token_id < OptFmtRangeEnd) {
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

    if (tokenType < MaxAsciiChar + 1) // Ascii
    {
        tf.setForeground(Qt::black);
    }
    else if (tokenType < StringTokenVal) // Applesoft Tokens
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

