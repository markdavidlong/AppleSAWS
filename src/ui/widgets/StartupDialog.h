#pragma once

#include <QDialog>
#include <memory>

class HexConverter;
class HRCGControlsInfo;
class NotesDialog;
class AsciiInfoDialog;

namespace Ui {
class StartupDialog;
}

class StartupDialog final : public QDialog
{
    Q_OBJECT

public:
    explicit StartupDialog(QWidget *parent = nullptr);
    ~StartupDialog() override;
    
    // Rule of Five - Qt QObject classes cannot be copied or moved
    StartupDialog(const StartupDialog&) = delete;
    StartupDialog& operator=(const StartupDialog&) = delete;
    StartupDialog(StartupDialog&&) = delete;
    StartupDialog& operator=(StartupDialog&&) = delete;

private slots:
    void on_exploreContentsBtn_clicked();
    void on_exploreDiskImageBtn_clicked();
    void on_bootSectorCompareBtn_clicked();
    void on_infoBtn_clicked();

private:
    std::unique_ptr<Ui::StartupDialog> ui;
    NotesDialog *m_notes{nullptr};
    HRCGControlsInfo *m_hrcgDialog{nullptr};
    HexConverter *m_hexConverter{nullptr};
    AsciiInfoDialog *m_AsciiInfoDialog{nullptr};
};
