#ifndef SEQUENCETOOLBOX_H
#define SEQUENCETOOLBOX_H

#include "sequencetool.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMap>
#include <QList>
#include <QCheckBox>

class SequenceToolBoxHeader : public QCheckBox
{
    Q_OBJECT
public:
    SequenceToolBoxHeader(const QString &text,
                          QWidget *parent = 0) : QCheckBox(text,parent)
    {
        setCheckable(true);
        setChecked(true);
    }
};

class SequenceToolBox : public QWidget
{
    Q_OBJECT

public:
    explicit SequenceToolBox(QWidget *parent = nullptr);
    void addTool(SequenceTool *tool);

public slots:
    void updateItems();

protected:
    QList<SequenceTool *> toolsInCategory(QString category);
    SequenceToolBoxHeader *getHeader(const QString &text);

protected:
    QVBoxLayout *m_layout;
    QStringList m_categories;
    QMap<QString, QList<SequenceTool *> > m_category;
    QMap<QString, SequenceToolBoxHeader *> m_headers;
};

#endif // SEQUENCETOOLBOX_H
