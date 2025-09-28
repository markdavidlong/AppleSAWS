#pragma once

#include "fileviewerinterface.h"
#include "IntBasicFile.h"

namespace Ui {
class IntBasicFileViewer;
}

class IntBasicFileViewer : public FileViewerInterface
{
    Q_OBJECT

public:
    explicit IntBasicFileViewer(QWidget *parent = nullptr);
    ~IntBasicFileViewer();

    bool optionsMenuItems(QMenu * /*menu*/) { return false; }

public slots:
    void setFile(GenericFile *file);

private:
    Ui::IntBasicFileViewer *ui;
};

