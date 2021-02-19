#ifndef SEQUENCETOOLBOX_H
#define SEQUENCETOOLBOX_H

#include "sequencetool.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMap>
#include <QList>
#include <QCheckBox>

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
