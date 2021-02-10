#ifndef SEQUENCETOOL_H
#define SEQUENCETOOL_H

#include "sequenceevent.h"

#include <QWidget>

class SequenceTool : public QWidget
{
    Q_OBJECT
public:
    explicit SequenceTool(SequenceEvent *event = nullptr, QWidget *parent = nullptr);
    virtual ~SequenceTool();

    virtual void setSequenceEvent(SequenceEvent *event);

    virtual void setCategory(QString category) { m_category = category; }
    virtual QString category() const { return m_category; }


protected:
    void mousePressEvent(QMouseEvent *event);

protected:
    SequenceEvent *m_seqevent;
    QString m_category;

};

#endif // SEQUENCETOOL_H
