#pragma once

#include "CharSet.h"

#include <QDialog>
#include <memory>

namespace Ui {
class CharacterSetExplorer;
}

class CharacterSetExplorer final : public QDialog
{
    Q_OBJECT

public:
    explicit CharacterSetExplorer(QWidget *parent = nullptr);
    ~CharacterSetExplorer() override;
    
    // Rule of Five - Qt QObject classes cannot be copied or moved
    CharacterSetExplorer(const CharacterSetExplorer&) = delete;
    CharacterSetExplorer& operator=(const CharacterSetExplorer&) = delete;
    CharacterSetExplorer(CharacterSetExplorer&&) = delete;
    CharacterSetExplorer& operator=(CharacterSetExplorer&&) = delete;

    void setCharSet(CharacterSet &charset);

public slots:
    void handleInsertCharButton();
    void handleTextChanged(QString string);

private:
    std::unique_ptr<Ui::CharacterSetExplorer> ui;
    QByteArray m_unpackedScreen;
    CharacterSet m_charset;
};
