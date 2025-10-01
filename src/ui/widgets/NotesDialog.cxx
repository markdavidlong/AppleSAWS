#include "NotesDialog.h"
#include "ui_NotesDialog.h"
#include <QFile>

NotesDialog::NotesDialog(QWidget *parent) :
    QDialog(parent),
    ui(std::make_unique<Ui::NotesDialog>())
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    QByteArray text;
    QFile notes(":/notes/notes.txt");
    if (notes.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        text = notes.readAll();
        notes.close();
    }
    else
    {
        text = "No notes available right now! Maybe next version!";
    }
    ui->notesArea->setMarkdown(text);
}

NotesDialog::~NotesDialog() = default;
