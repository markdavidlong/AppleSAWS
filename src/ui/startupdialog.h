#ifndef STARTUPDIALOG_H
#define STARTUPDIALOG_H

#include "notesdialog.h"
#include "hrcgcontrolsinfo.h"
#include "hexconverter.h"
#include "hexdumpviewer.h"
#include <QDialog>
#include "asciiinfodialog.h"

namespace Ui {
class StartupDialog;
}

class StartupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StartupDialog(QWidget *parent = nullptr);
    ~StartupDialog();

private slots:
    void on_exploreContentsBtn_clicked();

    void on_exploreDiskImageBtn_clicked();

    void on_bootSectorCompareBtn_clicked();

    void on_infoBtn_clicked();

private:
    Ui::StartupDialog *ui;

    NotesDialog *m_notes;
    HRCGControlsInfo *m_hrcgDialog;
    HexConverter *m_hexConverter;
    AsciiInfoDialog *m_AsciiInfoDialog;
};

#endif // STARTUPDIALOG_H
