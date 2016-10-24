#include "CharacterSetExplorer.h"
#include "ui_CharacterSetExplorer.h"

CharacterSetExplorer::CharacterSetExplorer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CharacterSetExplorer)
{
    m_unpackedScreen.fill(0,8192);
    ui->setupUi(this);
    connect(ui->insertChar, SIGNAL(clicked(bool)), SLOT(handleInsertCharButton()));
    connect(ui->inputText, SIGNAL(textChanged(QString)), SLOT(handleTextChanged(QString)));
}

void CharacterSetExplorer::setCharSet(CharacterSet &charset)
{
    m_charset = charset;
}

CharacterSetExplorer::~CharacterSetExplorer()
{
    delete ui;
}

void CharacterSetExplorer::handleInsertCharButton()
{
    //TODO
}

void CharacterSetExplorer::handleTextChanged(QString string)
{
    int currCursorX = 0;
    int currCursorY = 0;
    m_unpackedScreen.fill(0x00,8192);

    foreach (QChar character, string)
    {
        if (character.unicode() < 128)
        {
            if (character.unicode() < 32)
            {
                // Handle control character
            }
            else
            {
                // Handle ASCII value
                if (m_charset.contains(character.unicode()))
                {
                    QByteArray chardata = m_charset[character.unicode()].data();

                    for (int idx = 0; idx < 8; idx++)
                    {
                        int baseY = (currCursorY * 8 + idx) * 40;
                        quint8 chr = chardata[idx];
                        m_unpackedScreen[baseY + currCursorX] = chr;
                    }
                    currCursorX++;

                    if (currCursorX >= 40) {
                        currCursorY++;
                        currCursorX = 0;
                    }
                    if (currCursorY >= 24)
                    {
                        currCursorX = 0;
                        currCursorY = 0;
                    }
                }
                else
                {
                    qDebug() << "Character" << character.unicode() << "not defined in char set";
                }
            }
        }
        else
        {
            qDebug() << "Unhandled character: " << character.unicode();
        }
    }
    ui->drawWidget->setUnpackedData(m_unpackedScreen);
}
