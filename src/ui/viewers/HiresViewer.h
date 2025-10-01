#pragma once

#include "BinaryFile.h"
#include "FileViewerInterface.h"
#include "HiresScreenWidget.h"

#include <QWidget>
#include <QPixmap>
#include <QByteArray>
#include <QPair>
#include <QMap>
#include <QBitArray>
#include <QAction>
#include <QMenu>
#include <QLabel>


class HiresViewer final : public FileViewerInterface
{
    Q_OBJECT
public:
    explicit HiresViewer(QWidget *parent = nullptr);
    ~HiresViewer() override = default;
    
    // Rule of Five - Qt QObject classes cannot be copied or moved
    HiresViewer(const HiresViewer&) = delete;
    HiresViewer& operator=(const HiresViewer&) = delete;
    HiresViewer(HiresViewer&&) = delete;
    HiresViewer& operator=(HiresViewer&&) = delete;
    
    [[nodiscard]] bool optionsMenuItems(QMenu *) override;
    [[nodiscard]] bool canPrint() const override;
    [[nodiscard]] bool canExport() const override;

public slots:
    void setFile(GenericFile *file) override;
    void setFile(BinaryFile *file);

    void doPrint() override;
    void doExport() override;

    void handleNewOffset(quint16 offset);

private:
    HiresScreenWidget *hrsw{nullptr};
    QLabel *m_offsetLabel{nullptr};

    BinaryFile *m_file{nullptr};
};
