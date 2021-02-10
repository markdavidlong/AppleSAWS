#ifndef SEQUENCEVIEWER_H
#define SEQUENCEVIEWER_H

#include <QWidget>

namespace Ui {
class SequenceViewer;
}

class SequenceViewer : public QWidget
{
    Q_OBJECT

public:
    explicit SequenceViewer(QWidget *parent = nullptr);
    ~SequenceViewer();

private:
    Ui::SequenceViewer *ui;
};

#endif // SEQUENCEVIEWER_H
