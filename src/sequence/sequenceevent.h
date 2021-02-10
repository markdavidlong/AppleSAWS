#ifndef SEQUENCEEVENT_H
#define SEQUENCEEVENT_H

#include <QString>

class SequenceEvent
{
public:
    SequenceEvent();
    virtual ~SequenceEvent() { }

    virtual QString basename() = 0;
    virtual QString extendedName() { return basename(); }
};

#endif // SEQUENCEEVENT_H
