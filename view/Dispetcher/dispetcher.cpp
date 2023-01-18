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
    Dispetcher::fill_proc_table();
    Dispetcher::fill_status_bar();

    SelectRow=0;

    connect(&timer,SIGNAL(timeout()),this,SLOT(timer_slot()));
    timer.start(10000);

}

/// ---------------------------------------------------------------
///        Refresh timer
/// ---------------------------------------------------------------
void Dispetcher::timer_slot()
{
    model->clear();
    fill_proc_table();
    fill_status_bar();
    ui->tableView->scrollToBottom();
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

    proc_info=taskmgr_func->get_proc_info();

    // fill header
    foreach(QString active_punct,active_features)
    {
        horizontalHeader.append(active_punct);
    }

    model->setHorizontalHeaderLabels(horizontalHeader);

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

    ui->tableView->resizeColumnsToContents();
    qDebug() << 16;



}



///----------------------------------------------------------------
///        Fill status bar
/// ---------------------------------------------------------------
void Dispetcher::fill_status_bar()
{
    QMap<QString,QString> status_bar_info=taskmgr_func->status_info();


    QString Status_string="Process count: "+QString::number(proc_info["PID"].length())+"   |   ";
    foreach(QString status_punct,status_puncts)
    {
        Status_string+=status_punct+":"+status_bar_info[status_punct]+"   |   ";
    }


    ui->label->setText(Status_string);


}


void Dispetcher::on_tableView_clicked(const QModelIndex &index)
{
    SelectRow=index.row();
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
}



///----------------------------------------------------------------
///        Kill process
/// ---------------------------------------------------------------
void Dispetcher::on_Delete_clicked()
{
    if(!SelectRow) return;

    QString PID=proc_info["PID"][SelectRow];
    taskmgr_func->kill_process(PID);
    fill_proc_table();
    fill_status_bar();
}

///----------------------------------------------------------------
///        Destructor
/// ---------------------------------------------------------------


Dispetcher::~Dispetcher()
{
    timer.stop();
    disconnect(this,SLOT(timer_slot()));
    delete model;
    delete taskmgr_func;
    delete ui;
}
