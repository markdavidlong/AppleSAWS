#include "ApplesoftFileDetailViewer.h"
#include "ui_ApplesoftFileDetailViewer.h"

#include <QMap>
#include <QString>
#include <QStringList>
#include <QTableWidgetItem>
#include <QFile>


ApplesoftFileDetailViewer::ApplesoftFileDetailViewer(ApplesoftFile *file, QWidget *parent) :
    QWidget(parent),
    ui(std::make_unique<Ui::ApplesoftFileDetailViewer>())
{

    ui->setupUi(this);

    ui->m_varView->setSortingEnabled(true);
    m_file = file;
    load();
    setLineData(m_file->getLines());
}

ApplesoftFileDetailViewer::~ApplesoftFileDetailViewer()
{
    save();
}

void ApplesoftFileDetailViewer::setLineData(QList<ApplesoftLine> lineData)
{
    m_lines = lineData;
    process();
}

bool ApplesoftFileDetailViewer::save()
{

    if (ui->gridLayout->rowCount() == 0) { return false; }

    QMap <QString,QString> map;
    for (int idx = 0; idx < ui->m_varView->rowCount(); idx++)
    {
        QString var = ui->m_varView->item(idx,1)->text();
        if (var.contains(","))
        {
            var.truncate(var.indexOf(","));
        }
        var = var.trimmed();
        QString note = ui->m_varView->item(idx,2)->text().trimmed();
        if (note.length())
        {
            map.insert(var,note);
        }
    }

    QFile outfile(QString("%1%2%3")
                 .arg(m_file->diskFile()->getMetaDataPath())
                 .arg(m_file->filename())
                 .arg(".asvar"));
    if (outfile.open(QIODevice::WriteOnly))
    {
        QDataStream ds(&outfile);
        ds << map;
        outfile.close();
        return true;
    }
    return false;
}

bool ApplesoftFileDetailViewer::load()
{
    QFile infile(QString("%1%2%3")
                 .arg(m_file->diskFile()->getMetaDataPath())
                 .arg(m_file->filename())
                 .arg(".asvar"));
    if (infile.open(QIODevice::ReadOnly))
    {
        QDataStream ds(&infile);
        ds >> m_notes;
        infile.close();
        return true;
    }
    return false;
}


void ApplesoftFileDetailViewer::process()
{
    QMap<QString,QStringList> vardata;
    QMap<QString,quint16> vartypes;
    QMap<QString,QStringList> varalias;


    foreach (ApplesoftLine line, m_lines)
    {
        quint16 linenum = line.linenum;
        foreach(ApplesoftToken token, line.tokens)
        {
            quint16 tid = token.getTokenId();
            if (tid == ApplesoftToken::IntVarTokenVal      ||
                tid == ApplesoftToken::IntAryVarTokenVal   ||
                tid == ApplesoftToken::FloatVarTokenVal    ||
                tid == ApplesoftToken::FloatAryVarTokenVal ||
                tid == ApplesoftToken::StringVarTokenVal   ||
                tid == ApplesoftToken::StringAryVarTokenVal)
            {
                QString varname = token.getStringValue();
                QString fullname = varname;
                varname = shortenName(varname);
                if (varname.contains("(")) { varname.append(")"); }
                if (fullname.contains("(")) { fullname.append(")"); }
                vardata[varname].append(QString("%1").arg(linenum));
                vartypes[varname] = tid;
                if (fullname != varname)
                {
                    varalias[varname].append(fullname);
                }
            }
        }
    }

    QMap<quint16,QString> typemap;

    QStringList keys = vardata.keys();
    ui->m_varView->setRowCount(keys.count());
   keys.sort();

    int idx = 0;
    foreach (QString key, keys)
    {
        vardata[key].removeDuplicates();
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

        QString keywithalias = key;
        if (varalias.contains(key))
        {
             varalias[key].removeDuplicates();
             QString aliases = varalias[key].join(", ");
             keywithalias.append(", ");
             keywithalias.append(aliases);
        }
        twi = new QTableWidgetItem(keywithalias);
        twi->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        ui->m_varView->setItem(idx,1,twi);

        twi = new QTableWidgetItem("     ");
        if (m_notes.contains(key))
        {
            twi->setText(m_notes[key]);
        }
        twi->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
        ui->m_varView->setItem(idx,2,twi);


        twi = new QTableWidgetItem(linenums);
        twi->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        ui->m_varView->setItem(idx,3,twi);
        idx++;
    }

    ui->m_varView->resizeColumnToContents(0);
    ui->m_varView->resizeColumnToContents(1);
    ui->m_varView->resizeColumnToContents(2);

}

QString ApplesoftFileDetailViewer::shortenName(QString name)
{
    bool hasParen = name.contains('(');
    if (hasParen) { name.remove(name.indexOf('('),1); }

    bool hasDollar = name.contains('$');
    if (hasDollar) { name.remove(name.indexOf('$'),1); }

    bool hasPercent = name.contains('%');
    if (hasPercent) { name.remove(name.indexOf('%'),1); }

    name = name.left(2);
    if (hasDollar) { name.append("$"); }
    if (hasPercent) { name.append("%"); }
    if (hasParen) { name.append("("); }

    return name;
 }
