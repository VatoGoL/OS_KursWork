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
    features<<"Name"<<"PID"<<"PPID"<<"User"<<"Start time"<<"Priory"<<"Status"<<"Memory"<<"Path";
    status_puncts<<"Uptime"<<"Total RAM"<<"Free RAM"<<"CPU";
    qDebug() << features;
    qDebug() << status_puncts;

    active_features=features;
    // model for proc table
    model = new QStandardItemModel;

    // fill proc table
    qDebug() << 0;
    Dispetcher::fill_proc_table();
    qDebug() << 1;
    Dispetcher::fill_status_bar();
    qDebug() << 2;

    SelectRow=0;

    connect(&timer,SIGNAL(timeout()),this,SLOT(timer_slot()));
    timer.start(10000);

}

/// ---------------------------------------------------------------
///        Refresh timer
/// ---------------------------------------------------------------
void Dispetcher::timer_slot()
{
    qDebug() << 20;
    refresh_proc_table();
    qDebug() << 21;
    refresh_status_bar();
    qDebug() << 23;
}

///----------------------------------------------------------------
///        Fill proc table
/// ---------------------------------------------------------------
void Dispetcher::fill_proc_table()
{
    QStandardItem *item;

    //Заголовки столбцов
    QStringList horizontalHeader;

    qDebug() << 10;
    proc_info=taskmgr_func->get_proc_info();
    qDebug() << 11;


    // fill header
    foreach(QString active_punct,active_features)
    {
        horizontalHeader.append(active_punct);
    }
    qDebug() << 12;

    model->setHorizontalHeaderLabels(horizontalHeader);
    qDebug() << 13;

    // fill columns
    int i=0,j;
    foreach(QString active_punct,active_features)
    {
        j=0;
        foreach(QString str,proc_info[active_punct])
        {
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
    QString Status_string="Process count: "+QString::number(proc_info["PID"].length()+1)+"   |   ";
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
    proc_info["Name"].clear();
    proc_info.clear();

    proc_info=taskmgr_func->get_proc_info();
    qDebug() << 41;
    int i=0,j;
    RowCount=proc_info["PID"].length();
    qDebug()<<"RowCount"<<RowCount;
    foreach(QString active_punct,active_features)
    {
        j=0;
        foreach(QString str,proc_info[active_punct])
        {
            // Добавление новое ячейки, если создался процесс
            if(j>RowCount)//if(j>=RowCount-1)
            {
                item = new QStandardItem(str);
                item->setEditable(0); // lock items
                model->setItem(j,i, item);
                ui->tableView->setModel(model);
                if(RowCount<j) RowCount=j;//j+1;
                qDebug() << 42;
            }
            // Обновление значения старой ячейки
            else
//                qDebug()<<j<<i;
                model->item(j, i)->setData(str,Qt::DisplayRole);
            qDebug() << 43;

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
    QString Status_string="Process count: "+QString::number(proc_info["PID"].length()+1)+"   |   ";
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
///----------------------------------------------------------------
///        Create proc click
/// ---------------------------------------------------------------
void Dispetcher::on_Add_clicked()
{
    bool bOk;
    QString proc_path = QInputDialog::getText( 0,"Input","Name:",QLineEdit::Normal,"",&bOk);

    if (bOk)
    {
        taskmgr_func->create_process(proc_path);
    }

    else qDebug()<<"Create: not found exe";
}
///----------------------------------------------------------------
///        Kill proc group click
/// ---------------------------------------------------------------
void Dispetcher::on_Delete_something_clicked()
{
    if(!SelectRow) return;

    QString PID=proc_info["PID"][SelectRow];
    taskmgr_func->kill_proc_tree(PID);
}

Dispetcher::~Dispetcher()
{
    timer.stop();
    disconnect(this,SLOT(timer_slot()));
    delete model;
    delete taskmgr_func;
    delete ui;
}
