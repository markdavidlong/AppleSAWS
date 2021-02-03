#include "applestring.h"



QString AppleString::printable() const
{
    QString retval;
    foreach (quint8 ch, *this) {
        retval.append(AppleChar::printable(ch));
//        if (ch > 0x80)
//            retval.append(QChar(ch-0x80));
//        else
//            retval.append(QChar(ch));
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


QChar AppleChar::printable(quint8 val)
{
    quint8 newval;
    switch (getTextSet(val)) {
    case SetInvUC:
        newval = val+0x40;
        break;
    case SetInvSp:
        newval = val;
        break;
    case SetFlUC:
        newval = val;
        break;
    case SetFlSp:
        newval = val-0x40;
        break;
    case SetNormUC:
        newval = val-0x40;
        break;
    case SetNormSp:
        newval = val-0x80;
        break;
    case SetNormAltUC:
        newval = val-0x80;
        break;
    case SetNormLC:
    default:
        newval = val-0x80;
        break;
    }
    return QChar(newval);
}

TextAttribute AppleChar::getAttribute(quint8 val)
{
    if (val <= 0x3f) { return Inverse; }
    if (val <= 0x7f) { return Flash; }
    if (val <= 0xbf) { return NormalLow; }
    if (val <= 0xdf) { return NormalHigh; }
    return NormalHigh;
}

TextSet AppleChar::getTextSet(quint8 val)  {
    if (val < 0x20) { return SetInvUC; }
    if (val < 0x40) { return SetInvSp; }
    if (val < 0x60) { return SetFlUC; }
    if (val < 0x80) { return SetFlSp; }
    if (val < 0xA0) { return SetNormUC; }
    if (val < 0xC0) { return SetNormSp; }
    if (val < 0xE0) { return SetNormAltUC; }
    return SetNormLC;
}

QString AppleChar::getHtmlString(quint8 val)
{
    TextAttribute attribute = getAttribute(val);
    QString htmlstring = printable(val);
    htmlstring = htmlstring.toHtmlEscaped();

    QString retval;
    if (attribute == Inverse) { retval = QString("<font color=\"blue\"><b>%1</b></font>").arg(htmlstring); }
    else if (attribute == Flash) { retval = QString("<font color=\"green\"><b><i>%1</i></b></font>").arg(htmlstring);}
    else if (attribute == NormalLow) { retval = QString("<font color=\"red\"><i>%1</i></font>").arg(htmlstring);}
    else   { retval = QString("%1").arg(htmlstring);}

    return retval;
}

