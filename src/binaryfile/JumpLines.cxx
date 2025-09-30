#include "JumpLines.h"

QList<JumpLine> JumpLines::jumpLinesAtAddress(quint16 addrs) const
{
    QList<JumpLine> list;
    foreach (const JumpLine& jl, jumpLines)
    {
        if (addrs >= jl.min() && addrs <= jl.max())
        {
            list.append(jl);
        }
    }
    return list;
}