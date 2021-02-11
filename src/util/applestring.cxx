#include "applestring.h"



QString AppleString::printable() const
{
    QString retval;
    foreach (quint8 ch, *this) {
        retval.append(AppleChar::printable(ch));
    }
    return retval;
}

QString AppleString::appleFontPrintable() const
{
    QString retval;
    foreach (quint8 ch, *this) {

         retval.append(AppleChar::getAppleFontChar(ch));
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
    case TextSet::SetInvUC:
        newval = val+0x40;
        break;
    case TextSet::SetInvSp:
        newval = val;
        break;
    case TextSet::SetFlUC:
        newval = val;
        break;
    case TextSet::SetFlSp:
        newval = val-0x40;
        break;
    case TextSet::SetNormUC:
        newval = val-0x40;
        break;
    case TextSet::SetNormSp:
        newval = val-0x80;
        break;
    case TextSet::SetNormAltUC:
        newval = val-0x80;
        break;
    case TextSet::SetNormLC:
    default:
        newval = val-0x80;
        break;
    }
    return QChar(newval);
}

TextAttribute AppleChar::getAttribute(quint8 val)
{
    if (val <= 0x3f) { return TextAttribute::Inverse; }
    if (val <= 0x7f) { return TextAttribute::Flash; }
    if (val <= 0xbf) { return TextAttribute::NormalLow; }
    if (val <= 0xdf) { return TextAttribute::NormalHigh; }
    return TextAttribute::NormalHigh;
}

TextSet AppleChar::getTextSet(quint8 val)  {
    if (val < 0x20) { return TextSet::SetInvUC; }
    if (val < 0x40) { return TextSet::SetInvSp; }
    if (val < 0x60) { return TextSet::SetFlUC; }
    if (val < 0x80) { return TextSet::SetFlSp; }
    if (val < 0xA0) { return TextSet::SetNormUC; }
    if (val < 0xC0) { return TextSet::SetNormSp; }
    if (val < 0xE0) { return TextSet::SetNormAltUC; }
    return TextSet::SetNormLC;
}

QString AppleChar::getHtmlString(quint8 val)
{
    TextAttribute attribute = getAttribute(val);
    QString htmlstring = printable(val);
    htmlstring = htmlstring.toHtmlEscaped();

    QString retval;
    if (attribute == TextAttribute::Inverse) { retval = QString("<font color=\"blue\"><b>%1</b></font>").arg(htmlstring); }
    else if (attribute == TextAttribute::Flash) { retval = QString("<font color=\"green\"><b><i>%1</i></b></font>").arg(htmlstring);}
    else if (attribute == TextAttribute::NormalLow) { retval = QString("<font color=\"red\"><i>%1</i></font>").arg(htmlstring);}
    else   { retval = QString("%1").arg(htmlstring);}

    return retval;
}

QChar AppleChar::getAppleFontChar(quint8 val)
{
    if (getAttribute(val) == TextAttribute::Inverse)
    {
        return QChar(printable(val).unicode() + 0xe100);
    }
    else if (getAttribute(val) == TextAttribute::Flash)
    {
        return QChar(printable(val).unicode() + 0xe100);
    }
    else if (val >= 0x80 && val < 0xA0)
    {
        return QChar(val - 0x80 + 0xe800);
    }
    else
    {
        return printable(val);
    }
}



