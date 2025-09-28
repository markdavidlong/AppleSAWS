#include "charsetviewer.h"
#include "BinaryFile.h"
#include <QGridLayout>
#include <QSettings>
#include <QAction>
#include <QMenu>

CharSetViewer::CharSetViewer(QWidget *parent) : FileViewerInterface(parent)
{
    m_file = nullptr;
    m_cse = nullptr;
    m_showGridAction = nullptr;
    m_enableBitShiftAction = nullptr;
    m_charSetEncoderAction = nullptr;

    QGridLayout *qgl = new QGridLayout(this);
    setLayout(qgl);
    QString title = QString("Character Set Viewer");
    setWindowTitle(title);
}

CharSetViewer::~CharSetViewer()
{
}

void CharSetViewer::setFile(GenericFile *file)
{
    if (dynamic_cast<BinaryFile*>(file))
    {
        BinaryFile *bf = dynamic_cast<BinaryFile*>(file);
        setFile(bf);
    }
}

void CharSetViewer::setFile(BinaryFile *file)
{
    m_file = file;
    QString title = QString("Character Set Viewer: %1").arg(file->filename());
    setWindowTitle(title);
    m_data = file->data();
    m_data.resize(file->length());
    m_charset.buildSetFromSetBlob(m_data);

    int xpos = 0;
    int ypos = 0;
    foreach (CharSetCharacter csc, m_charset.allCharacters())
    {
        CharacterWidget *cw = new CharacterWidget(this,csc);
        cw->showGrid(true);
        cw->enableBitShift(true);
        cw->setBgColor(Qt::white);
        cw->setFgColor(Qt::black);

        QGridLayout *qgl = qobject_cast<QGridLayout*>(this->layout());
        qgl->addWidget(cw,ypos,xpos);
        //cw->move(xpos,ypos);
        cw->show();

        xpos+= cw->width();
        if (xpos/cw->width() > 11) {
            xpos = 0;
            ypos += cw->height();
        }
    }
}

bool CharSetViewer::optionsMenuItems(QMenu *menu)
{
    QSettings settings;

    if (!m_showGridAction)
    {
        m_showGridAction = new QAction("Show &Grid",menu);
        m_showGridAction->setCheckable(true);
        m_showGridAction->setChecked(settings.value("CharSetViewer.ShowGrid",true).toBool());
        showGrid(settings.value("CharSetViewer.ShowGrid",true).toBool());
        connect(m_showGridAction, &QAction::toggled, this, &CharSetViewer::showGrid);
        menu->addAction(m_showGridAction);
    }

    if (!m_enableBitShiftAction)
    {
        m_enableBitShiftAction = new QAction("&Enable Bit Shift",menu);
        m_enableBitShiftAction->setCheckable(true);
        m_enableBitShiftAction->setChecked(settings.value("CharSetViewer.EnableBitShift",true).toBool());
        enableBitShift(settings.value("CharSetViewer.EnableBitShift",true).toBool());
        connect(m_enableBitShiftAction, &QAction::toggled, this, &CharSetViewer::enableBitShift);
        menu->addAction(m_enableBitShiftAction);
    }

    if (!m_charSetEncoderAction)
    {
        menu->addSeparator();

        m_charSetEncoderAction = new QAction("&Character Set Explorer...");
        connect(m_charSetEncoderAction, &QAction::triggered, this, &CharSetViewer::showExplorer);
        menu->addAction(m_charSetEncoderAction);
    }
    return true;
}

void CharSetViewer::showExplorer()
{
    if (!m_cse) {
        m_cse = new CharacterSetExplorer(this);
        connect(m_cse, &CharacterSetExplorer::destroyed, this, &CharSetViewer::cleanupExplorer);
        m_cse->setCharSet(m_charset);
    }
    m_cse->show();
    m_cse->raise();
}

void CharSetViewer::cleanupExplorer()
{
    m_cse = Q_NULLPTR;
}

QList<CharacterWidget *> CharSetViewer::getChildren()
{
    QList<CharacterWidget*> retval;
    foreach (QObject *child, children())
    {
        if (dynamic_cast<CharacterWidget*>(child))
        {
            retval.append(dynamic_cast<CharacterWidget*>(child));
        }
    }
    return retval;
}

void CharSetViewer::setFgColor(QColor color)
{
    foreach (auto child, getChildren())
    {
        child->setFgColor(color);
    }
}

void CharSetViewer::setBgColor(QColor color)
{
    foreach (auto child, getChildren())
    {
        child->setBgColor(color);
    }
}

void CharSetViewer::setGridColor(QColor color)
{
    foreach (auto child, getChildren())
    {
        child->setGridColor(color);
    }
}

void CharSetViewer::showGrid(bool show)
{
    QSettings settings;
    settings.setValue("CharSetViewer.ShowGrid",show);

    foreach (auto child, getChildren())
    {
        child->showGrid(show);
    }
}

void CharSetViewer::enableBitShift(bool enable)
{
    QSettings settings;
    settings.setValue("CharSetViewer.EnableBitShift",enable);

    foreach (auto child, getChildren())
    {
        child->enableBitShift(enable);
    }
}
