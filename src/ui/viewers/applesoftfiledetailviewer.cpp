#include "applesoftfiledetailviewer.h"
#include "ui_applesoftfiledetailviewer.h"

#include <QMap>
#include <QString>
#include <QStringList>
#include <QTableWidgetItem>


ApplesoftFileDetailViewer::ApplesoftFileDetailViewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ApplesoftFileDetailViewer)
{
    ui->setupUi(this);
    ui->m_varView->setSortingEnabled(true);
}

ApplesoftFileDetailViewer::~ApplesoftFileDetailViewer()
{
    delete ui;
}

void ApplesoftFileDetailViewer::process()
{
    QMap<QString,QStringList> vardata;
    QMap<QString,quint16> vartypes;

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
                vartypes[varname] = tid;
            }
        }
    }

    QMap<quint16,QString> typemap;

    QStringList keys = vardata.keys();
    ui->m_varView->setRowCount(keys.count());
    qSort(keys);
    int idx = 0;
    foreach (QString key, keys)
    {
        QString linenums = vardata[key].join(",");
        quint16 vtype = vartypes[key];
        QString vtname;
        switch (vtype) {
            case ApplesoftToken::IntVarTokenVal: vtname = "Int"; break;
            case ApplesoftToken::FloatVarTokenVal: vtname = "Float"; break;
            case ApplesoftToken::StringVarTokenVal: vtname = "String"; break;
            case ApplesoftToken::IntAryVarTokenVal: vtname = "Int Array"; break;
            case ApplesoftToken::FloatAryVarTokenVal: vtname = "Float Array"; break;
            case ApplesoftToken::StringAryVarTokenVal: vtname = "String Array"; break;
            default: vtname = "Unknown";
        }

        QTableWidgetItem *twi = new QTableWidgetItem(vtname);
        twi->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        ui->m_varView->setItem(idx,0,twi);

        twi = new QTableWidgetItem(key);
        twi->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        ui->m_varView->setItem(idx,1,twi);

        twi = new QTableWidgetItem(linenums);
        twi->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        ui->m_varView->setItem(idx,2,twi);
        idx++;
    }

}
