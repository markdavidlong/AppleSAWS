#include "charsetviewer.h"
#include <QGridLayout>
#include <QSettings>
#include <QAction>
#include <QMenu>

CharSetViewer::CharSetViewer(QWidget *parent) : FileViewerInterface(parent)
{
    m_file = Q_NULLPTR;

    QGridLayout *qgl = new QGridLayout(this);
    setLayout(qgl);
    QString title = QString("Character Set Viewer");
    setWindowTitle(title);
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

    QAction *action = new QAction("Show &Grid",menu);
    action->setCheckable(true);
    action->setChecked(settings.value("CharSetViewer.ShowGrid",true).toBool());
    showGrid(settings.value("CharSetViewer.ShowGrid",true).toBool());
    connect(action, SIGNAL(toggled(bool)),SLOT(showGrid(bool)));
    menu->addAction(action);

    action = new QAction("&Enable Bit Shift",menu);
    action->setCheckable(true);
    action->setChecked(settings.value("CharSetViewer.EnableBitShift",true).toBool());
    enableBitShift(settings.value("CharSetViewer.EnableBitShift",true).toBool());
    connect(action, SIGNAL(toggled(bool)),SLOT(enableBitShift(bool)));
    menu->addAction(action);

    return true;
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
