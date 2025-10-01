#pragma once

#include <QDialog>
#include <memory>

namespace Ui {
class NotesDialog;
}

class NotesDialog final : public QDialog
{
    Q_OBJECT

public:
    explicit NotesDialog(QWidget *parent = nullptr);
    ~NotesDialog() override;
    
    // Rule of Five - Qt QObject classes cannot be copied or moved
    NotesDialog(const NotesDialog&) = delete;
    NotesDialog& operator=(const NotesDialog&) = delete;
    NotesDialog(NotesDialog&&) = delete;
    NotesDialog& operator=(NotesDialog&&) = delete;

private:
    std::unique_ptr<Ui::NotesDialog> ui;
};

