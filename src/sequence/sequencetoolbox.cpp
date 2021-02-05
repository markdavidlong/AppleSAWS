#include "sequencetoolbox.h"

SequenceToolBox::SequenceToolBox(QWidget *parent) : QWidget(parent)
{
    m_layout = new QVBoxLayout(this);
}

void SequenceToolBox::addTool(SequenceTool *tool)
{
    QString catname = tool->category();
    if (!m_category.contains(catname))
    {
        QList<SequenceTool *> list;
        list.append(tool);
        m_categories.append(catname);
        m_category.insert(catname, list);
    }
    else
    {
        m_category[catname].append(tool);
    }

    int count = m_layout->count();
    for (int idx = 0; idx < count; idx++)
    {
        auto item = m_layout->itemAt(0);
        m_layout->removeItem(item);
    }

    foreach (QString cat, m_categories)
    {
        m_layout->addWidget(getHeader(cat));
        auto catitems = m_category[cat];
        bool headerchecked = m_headers[cat]->isChecked();
        foreach (auto item, catitems)
        {
            m_layout->addWidget(item);
            item->setVisible(headerchecked);
        };
    }
    m_layout->addStretch();
}

void SequenceToolBox::updateItems()
{
    foreach (auto item, m_layout->children())
    {
        auto widget = qobject_cast<SequenceTool *>(item);
        if (widget)
        {
            widget->setVisible(m_headers[widget->category()]->isChecked());
        }
    }
}

QList<SequenceTool *> SequenceToolBox::toolsInCategory(QString category)
{
    if (m_category.contains(category))
    {
        return m_category[category];
    }
    else
    {
        return QList<SequenceTool *>();
    }
}

SequenceToolBoxHeader *SequenceToolBox::getHeader(const QString &text)
{
    SequenceToolBoxHeader *header;

    if (m_headers.contains(text))
    {
        header = m_headers[text];
    }
    else
    {
        header = new SequenceToolBoxHeader(text);
        m_headers.insert(text,header);
        connect(header,SIGNAL(toggled(bool)),
                this, SLOT(updateItems()));
    }
    return header;
}
