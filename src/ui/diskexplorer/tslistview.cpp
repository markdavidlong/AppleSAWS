#include "tslistview.h"
#include "sector.h"

TSListView::TSListView(QWidget *parent) : QTextBrowser(parent)
{

}

void TSListView::setSector(Sector *sec)
{
        m_sector = sec;
        if (sec)
        {
            QString text;
            auto tslist = sec->asTrackSectorList();
            int count = 0;
            text += QString("%1 T/S pair(s) (%3 valid)  Next T/S List @ (%4/%5)\n\n")
                    .arg(tslist.getDataTSPairs().count())
                    .arg(tslist.getValidTSPairs().count())
                    .arg(tslist.getNextTSList().track())
                    .arg(tslist.getNextTSList().sector());
            foreach(auto ts, tslist.getDataTSPairs())
            {
                text += QString("T/S %1: (%2/%3)\n")
                        .arg(count++)
                        .arg(ts.track())
                        .arg(ts.sector());
            }
            setPlainText(text);
        }
        else
        {
            setText("");
        }
}
