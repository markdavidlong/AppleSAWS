#include "sequencetool.h"

SequenceTool::SequenceTool(SequenceEvent *event, QWidget *parent) : QWidget(parent)
{
    setSequenceEvent(event);
}

SequenceTool::~SequenceTool()
{

}

void SequenceTool::setSequenceEvent(SequenceEvent *event)
{
    m_seqevent = event;


}

void SequenceTool::mousePressEvent(QMouseEvent *event)
{

}
