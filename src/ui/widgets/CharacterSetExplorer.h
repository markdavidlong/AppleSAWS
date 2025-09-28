#pragma once

#include "charset.h"

#include <QDialog>

namespace Ui {
class CharacterSetExplorer;
}

class CharacterSetExplorer : public QDialog
{
    Q_OBJECT

public:
    explicit CharacterSetExplorer(QWidget *parent = 0);
    ~CharacterSetExplorer();

    void setCharSet(CharacterSet &charset);

public slots:
    void handleInsertCharButton();
    void handleTextChanged(QString string);

private:
    Ui::CharacterSetExplorer *ui;

    QByteArray m_unpackedScreen;

    CharacterSet m_charset;
};
