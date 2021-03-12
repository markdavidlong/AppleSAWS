/*****************************************************************************
*    AppleSAWS - The Apple Software Analysis WorkShop                        *
*    Copyright (C) 2015-2021  Mark D. Long                                   *
*                                                                            *
*    This program is free software: you can redistribute it and/or modify    *
*    it under the terms of the GNU General Public License as published by    *
*    the Free Software Foundation, either version 3 of the License, or       *
*    (at your option) any later version.                                     *
*                                                                            *
*    This program is distributed in the hope that it will be useful,         *
*    but WITHOUT ANY WARRANTY; without even the implied warranty of          *
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
*    GNU General Public License for more details.                            *
*                                                                            *
*    You should have received a copy of the GNU General Public License       *
*    along with this program.  If not, see <https://www.gnu.org/licenses/>.  *
*****************************************************************************/

#include "tslistview.h"
#include "sector.h"

TSListView::TSListView(QWidget *parent) : QTextBrowser(parent)
{

}

void TSListView::setSector(pSector sec)
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
