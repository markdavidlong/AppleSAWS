#pragma once

#include "FileViewerInterface.h"
#include "CharacterSetExplorer.h"

#include <QWidget>

class BinaryFile;
class CharacterWidget;

class CharSetViewer final : public FileViewerInterface
{
    Q_OBJECT
public:
    explicit CharSetViewer(QWidget *parent = nullptr);
    ~CharSetViewer() override;
    
    // Rule of Five - Qt QObject classes cannot be copied or moved
    CharSetViewer(const CharSetViewer&) = delete;
    CharSetViewer& operator=(const CharSetViewer&) = delete;
    CharSetViewer(CharSetViewer&&) = delete;
    CharSetViewer& operator=(CharSetViewer&&) = delete;

    void setFile(GenericFile *file) override;
    void setFile(BinaryFile *file);

    [[nodiscard]] bool optionsMenuItems(QMenu *menu) override;

public slots:
    void setFgColor(QColor color);
    void setBgColor(QColor color);
    void setGridColor(QColor color);

    void showGrid(bool show);
    void enableBitShift(bool enable);

    void showExplorer();

signals:

protected:
    [[nodiscard]] QList<CharacterWidget*> getChildren();

protected slots:
    void cleanupExplorer();

private:
    BinaryFile *m_file{nullptr};

    QByteArray m_data;

    CharacterSet m_charset;

    CharacterSetExplorer *m_cse{nullptr};

    QAction *m_showGridAction{nullptr};
    QAction *m_enableBitShiftAction{nullptr};
    QAction *m_charSetEncoderAction{nullptr};
};

