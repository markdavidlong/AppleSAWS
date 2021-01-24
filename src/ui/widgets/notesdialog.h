#ifndef NOTESDIALOG_H
#define NOTESDIALOG_H

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

#endif // NOTESDIALOG_H
