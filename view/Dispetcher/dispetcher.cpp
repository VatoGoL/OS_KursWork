#include "dispetcher.h"
#include "ui_dispetcher.h"

/// ---------------------------------------------------------------
///        Constructor
/// ---------------------------------------------------------------

Dispetcher::Dispetcher(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dispetcher)
{
    ui->setupUi(this);
    taskmgr_func=new DispetcherModel();
    features<<"Name"<<"PID"<<"PPID"<<"Start time"<<"Priory"<<"Status"<<"Memory"<<"Path";
    status_puncts<<"Uptime"<<"Total RAM"<<"Free RAM"<<"CPU";
    //qDebug() << features;
    //qDebug() << status_puncts;

    active_features=features;
    // model for proc table
    model = new QStandardItemModel;

    // fill proc table
    //qDebug() << 0;
    Dispetcher::fill_proc_table();
    //qDebug() << 1;
    Dispetcher::fill_status_bar();
    //qDebug() << 2;

    SelectRow=0;

    connect(&timer,SIGNAL(timeout()),this,SLOT(timer_slot()));
    timer.start(10000);

}

/// ---------------------------------------------------------------
///        Refresh timer
/// ---------------------------------------------------------------
void Dispetcher::timer_slot()
{
    //qDebug() << 20;
    model->clear();
    fill_proc_table();
    //refresh_proc_table();
    //qDebug() << 21;
    refresh_status_bar();
    ui->tableView->scrollToBottom();
    //qDebug() << 23;
}

///----------------------------------------------------------------
///        Fill proc table
/// ---------------------------------------------------------------
void Dispetcher::fill_proc_table()
{
    QStandardItem *item;

    //Заголовки столбцов
    QStringList horizontalHeader;
    proc_info.clear();

    qDebug() << 10;
    proc_info=taskmgr_func->get_proc_info();
    qDebug() << 11;


    // fill header
    foreach(QString active_punct,active_features)
    {
        horizontalHeader.append(active_punct);
    }
    //qDebug() << 12;

    model->setHorizontalHeaderLabels(horizontalHeader);
    qDebug() << 13;

    // fill columns
    int i=0,j;
    foreach(QString active_punct,active_features)
    {
        j=0;
        foreach(QString str,proc_info[active_punct])
        {
            //qDebug() << proc_info[active_punct];
            item = new QStandardItem(str);
            item->setEditable(0); // lock items
            model->setItem(j, i, item);
            j++;
        }
        i++;
    }
    qDebug() << 14;
    RowCount=j;
    ui->tableView->setModel(model);
    qDebug() << 15;

//    ui->ProcTable->resizeRowsToContents();
    ui->tableView->resizeColumnsToContents();
    qDebug() << 16;



}



///----------------------------------------------------------------
///        Fill status bar
/// ---------------------------------------------------------------
void Dispetcher::fill_status_bar()
{
    QMap<QString,QString> status_bar_info=taskmgr_func->status_info();




    qDebug() << 30;
    QString Status_string="Process count: "+QString::number(proc_info["PID"].length())+"   |   ";
    foreach(QString status_punct,status_puncts)
    {
        Status_string+=status_punct+":"+status_bar_info[status_punct]+"   |   ";
    }


    ui->label->setText(Status_string);
    qDebug() << 31;


}

///----------------------------------------------------------------
///        Refresh proc table
/// ---------------------------------------------------------------
void Dispetcher::refresh_proc_table()
{
    qDebug() << 40;
    QStandardItem *item;
    //proc_info["Name"].clear();
    proc_info.clear();

    proc_info=taskmgr_func->get_proc_info();
    qDebug() << 41;
    int i=0,j;
    RowCount=proc_info["PID"].length();
    qDebug()<<"RowCount"<<RowCount;
    for (int t = 0; t < active_features.size(); t++)
    {
        QString active_punct = active_features[t];
        j=0;
        for(int l = 0; l < proc_info[active_punct].size(); l++)
        {
            QString str = proc_info[active_punct][l];
            // Добавление новое ячейки, если создался процесс
            if(j<=RowCount -1)//if(j>=RowCount-1)
            {
                qDebug()<<j<<i;
                model->item(j, i)->setData(str,Qt::DisplayRole);
            }
            // Обновление значения старой ячейки
            else
            {

                item = new QStandardItem(str);
                item->setEditable(0); // lock items
                model->setItem(j,i, item);
                ui->tableView->setModel(model);
                if(RowCount<j) RowCount=j;//j+1;
                qDebug() << 42;
            }
            //qDebug() << 43;

            // счетчик строк
            j++;
        }
        // считчик столбцов
        i++;
    }
    qDebug() << 44;

   qDebug()<<"j"<<j;
   qDebug()<<"RowCount"<<RowCount;
    // очистка ячеек при завершении одного из процессов
    for(int i=j;i<RowCount;i++)
    {
//        qDebug()<<"empty row"<<i;
        for(int j1=0;j1<active_features.length();j1++)
        {
            model->item(i, j1)->setData("",Qt::DisplayRole);
        }
    }
    qDebug() << 45;

    ui->tableView->setModel(model);
    qDebug() << 46;

}

///----------------------------------------------------------------
///        Destructor
/// ---------------------------------------------------------------


void Dispetcher::on_tableView_clicked(const QModelIndex &index)
{
    SelectRow=index.row();
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
}

///----------------------------------------------------------------
///        Refresh status bar
/// ---------------------------------------------------------------
void Dispetcher::refresh_status_bar()
{
    QMap<QString,QString> status_bar_info=taskmgr_func->status_info();

//    QString Status_string="";
    QString Status_string="Process count: "+QString::number(proc_info["PID"].length())+"   |   ";
    foreach(QString status_punct,status_puncts)
    {
        Status_string+=status_punct+": "+status_bar_info[status_punct]+"   |   ";
    }
//    qDebug()<<Status_string;
    ui->label->clear();
    ui->label->setText(Status_string);

}

///----------------------------------------------------------------
///        Kill process
/// ---------------------------------------------------------------
void Dispetcher::on_Delete_clicked()
{
    if(!SelectRow) return;

    QString PID=proc_info["PID"][SelectRow];
    taskmgr_func->kill_process(PID);
}


Dispetcher::~Dispetcher()
{
    timer.stop();
    disconnect(this,SLOT(timer_slot()));
    delete model;
    delete taskmgr_func;
    delete ui;
}
