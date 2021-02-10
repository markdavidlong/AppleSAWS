#ifndef SEQUENCEEVENTLIST_H
#define SEQUENCEEVENTLIST_H

#include "sequenceevent.h"

#include <QAbstractListModel>

class SequenceEventList : public QAbstractListModel
{
    Q_OBJECT

public:
    SequenceEventList(QObject *parent = nullptr);
    virtual ~SequenceEventList();

};

#endif // SEQUENCEEVENTLIST_H
