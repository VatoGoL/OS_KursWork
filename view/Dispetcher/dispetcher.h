#ifndef DISPETCHER_H
#define DISPETCHER_H

#include "./controller/DispetcherControl/dispetchercontrol.h"
#include "./model/DispetcherModel/dispetchermodel.h"

#include <QDialog>
#include <QDebug>
#include<QMessageBox>


#include "QStandardItemModel"
#include "QStandardItem"
#include <QMenu>
#include <QTimer>
#include <QCheckBox>
#include <QGridLayout>
#include <QInputDialog>

namespace Ui {
class Dispetcher;
}

class Dispetcher : public QDialog
{
    Q_OBJECT

public:
    explicit Dispetcher(QWidget *parent = nullptr);
    ~Dispetcher();

private slots:
    void on_tableView_clicked(const QModelIndex &index);
    void on_Delete_clicked();
    void timer_slot();

private:
    Ui::Dispetcher *ui;
    // список свойств диспечера задач
    QStringList features;

    // чекбоксы для выбора отображения столбцов
    QCheckBox *checkBoxes;

    // указатель на абстрактый класс
    DispetcherModel *taskmgr_func;

    // модель для таблицы
    QStandardItemModel *model;

    // номер текущей выборанной строки
    int SelectRow;

    // ассоциативный массив инфы о процессах
    QMap<QString,QStringList>  proc_info;

    // таймер для обновления инфы
    QTimer timer;

    int RowCount;

    void fill_proc_table();
    void refresh_proc_table();
    void fill_status_bar();
    void refresh_status_bar();

    QStringList status_puncts;
    QStringList active_features;

};

#endif // DISPETCHER_H
