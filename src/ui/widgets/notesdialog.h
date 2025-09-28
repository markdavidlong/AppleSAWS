#pragma once

#include <QDialog>

namespace Ui {
class NotesDialog;
}

class NotesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NotesDialog(QWidget *parent = nullptr);
    ~NotesDialog();

private:
    Ui::NotesDialog *ui;
};

