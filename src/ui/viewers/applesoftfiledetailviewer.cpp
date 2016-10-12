#include "applesoftfiledetailviewer.h"
#include "ui_applesoftfiledetailviewer.h"

#include <QMap>
#include <QString>
#include <QStringList>


ApplesoftFileDetailViewer::ApplesoftFileDetailViewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ApplesoftFileDetailViewer)
{
    ui->setupUi(this);
}

ApplesoftFileDetailViewer::~ApplesoftFileDetailViewer()
{
    delete ui;
}

void ApplesoftFileDetailViewer::process()
{
     QMap<QString,QStringList> vardata;

    foreach (ApplesoftLine line, m_lines)
    {
        quint16 linenum = line.linenum;
        foreach(ApplesoftToken token, line.tokens)
        {
            quint16 tid = token.getTokenId();
            if (tid == ApplesoftToken::IntVarTokenVal ||
                tid == ApplesoftToken::IntAryVarTokenVal ||
                tid == ApplesoftToken::FloatVarTokenVal ||
                tid == ApplesoftToken::FloatAryVarTokenVal ||
                tid == ApplesoftToken::StringVarTokenVal ||
                tid == ApplesoftToken::StringAryVarTokenVal)
            {
                QString varname = token.getStringValue();
                if (varname.contains("(")) { varname.append(")"); }
                vardata[varname].append(QString("%1").arg(linenum));
            }
        }
    }
    QStringList keys = vardata.keys();
    ui->m_varView->setRowCount(keys.count());
    qSort(keys);
    int idx = 0;
    foreach (QString key, keys)
    {
        QString linenums = vardata[key].join(",");
        ui->m_varView->setItem(idx,1,new QTableWidgetItem(key));
        ui->m_varView->setItem(idx,2,new QTableWidgetItem(linenums));
        idx++;
    }

}
