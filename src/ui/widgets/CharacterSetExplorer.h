#ifndef CHARACTERSETEXPLORER_H
#define CHARACTERSETEXPLORER_H

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

private:
    Ui::CharacterSetExplorer *ui;
};

#endif // CHARACTERSETEXPLORER_H
