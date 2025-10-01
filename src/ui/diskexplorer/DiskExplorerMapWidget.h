#pragma once

#include "DiskFile.h"
#include "DEButton.h"
#include "DiskExplorerTypes.h"

#include <QWidget>
#include <QMap>
#include <QColor>
#include <QGroupBox>
#include <QLabel>

using namespace DiskExplorerTypes;


class DiskExplorerMapWidget final : public QWidget
{
    Q_OBJECT

public:
    explicit DiskExplorerMapWidget(int numtracks, int numsectors, QWidget* parent = nullptr);

    void setButtonBgColor(int track, int sector, const QColor& color);
    void setButtonText(int track, int sector, const QString& text) {
        buttonAt(track, sector)->setText(text);
    }

    void setDisk(DiskFile* disk);
    void unloadDisk();

    void setAllButtonsEnabled(bool enabled);

    [[nodiscard]] QGroupBox* makeKeyWidget();
    [[nodiscard]] QWidget* getStatusWidget() const noexcept { return m_statusWidget; }

signals:
    void showSectorData(const QByteArray& data, int track, int sector, const QVariant& metadata);

public slots:
    void handleButtonCheck(int track, int sector, bool checked);

protected:
    void mapDiskToButtons();
    [[nodiscard]] DEButton* buttonAt(int track, int sector);

    void initColors();

    [[nodiscard]] static QColor determineFgColor(const QColor& bgColor) noexcept
    {
        return (qGray(bgColor.rgb()) > 128) ? QColor(Qt::black) : QColor(Qt::white);
    }

    [[nodiscard]] QLabel* makeKeyLabel(QWidget* parent, const QString& name, const QColor& color);
    void showEvent(QShowEvent* event) override;
    void makeStatusWidget();
    [[nodiscard]] QString getSectorDescription(int track, int sector) const;

private:
    QMap<int, QMap<int, DEButton*>> m_buttons;
    DEButton* m_currentChecked{nullptr};

    int m_numtracks{0};
    int m_numsectors{0};

    QColor m_defaultColor{Qt::lightGray};
    QColor m_bootSectorColor{Qt::red};
    QColor m_dosImageColor{Qt::blue};
    QColor m_vtocColor{Qt::yellow};
    QColor m_dirEntryColor{Qt::green};
    QColor m_tsListColor{Qt::cyan};
    QColor m_applesoftFileColor{Qt::magenta};
    QColor m_intBasicFileColor{Qt::darkRed};
    QColor m_binaryFileColor{Qt::darkBlue};
    QColor m_textFileColor{Qt::darkGreen};
    QColor m_reloFileColor{Qt::darkCyan};
    QColor m_typeAFileColor{Qt::darkMagenta};
    QColor m_typeBFileColor{Qt::darkYellow};
    QColor m_typeSFileColor{Qt::gray};

    QButtonGroup* m_bgroup{nullptr};
    DiskFile* m_disk{nullptr};

    bool m_deferredSetup{false};

    QLabel* m_diskLabel{nullptr};
    QLabel* m_trackSectorLabel{nullptr};
    QWidget* m_statusWidget{nullptr};

    QMap<DETSPair, QString> m_sectorDescriptions;
};

