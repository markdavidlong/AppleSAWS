#include "applestring.h"



QString AppleString::printable() const
{
    QString retval;
    foreach (quint8 ch, *this) {
        retval.append(AppleChar(ch).printable());
    }
    return retval;
}

QVector<TextAttribute> AppleString::attributes() const
{
    QVector<TextAttribute> retval(this->length());
    int idx = 0;
    foreach (quint8 ch, *this) {
        retval[idx++] = AppleChar(ch).getAttribute();
    }
    return retval;
}



QChar AppleChar::printable() const
{
    quint8 newval;
    switch (getTextSet()) {
        case SetInvUC:
            newval = m_val+0x40;
            break;
        case SetInvSp:
            newval = m_val;
            break;
        case SetFlUC:
            newval = m_val;
            break;
        case SetFlSp:
            newval = m_val-0x40;
            break;
        case SetNormUC:
            newval = m_val-0x40;
            break;
        case SetNormSp:
            newval = m_val-0x80;
            break;
        case SetNormAltUC:
            newval = m_val-0x80;
            break;
        case SetNormLC:
        default:
            newval = m_val-0x80;
            break;
    }
    return QChar(newval);
}

TextAttribute AppleChar::getAttribute() const
{
    if (m_val <= 0x3f) { return Inverse; }
    if (m_val <= 0x7f) { return Flash; }
    if (m_val <= 0xbf) { return Normal; }
    if (m_val <= 0xdf) { return NormalHigh; }
    return Normal;
}

TextSet AppleChar::getTextSet() const {
    if (m_val < 0x20) { return SetInvUC; }
    if (m_val < 0x40) { return SetInvSp; }
    if (m_val < 0x60) { return SetFlUC; }
    if (m_val < 0x80) { return SetFlSp; }
    if (m_val < 0xA0) { return SetNormUC; }
    if (m_val < 0xC0) { return SetNormSp; }
    if (m_val < 0xE0) { return SetNormAltUC; }
    return SetNormLC;
}

