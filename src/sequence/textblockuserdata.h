#ifndef TEXTBLOCKUSERDATA_H
#define TEXTBLOCKUSERDATA_H

#include <QTextBlockUserData>
#include <QSyntaxHighlighter>

class TextBlockUserData : public QTextBlockUserData
{
public:
    TextBlockUserData();
    virtual ~TextBlockUserData() { }

    virtual void applySyntaxHighlighting(QSyntaxHighlighter &highlighter) = 0;
};

#endif // TEXTBLOCKUSERDATA_H
