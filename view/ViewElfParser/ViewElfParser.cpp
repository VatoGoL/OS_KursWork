#include "ViewElfParser.h"
#include "ui_ViewElfParser.h"

ViewElfParser::ViewElfParser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ViewElfParser)
{
    ui->setupUi(this);

    connect(ui->__b_load_elf_file,SIGNAL(clicked()), this, SLOT(__load_elf_file()));
}

void ViewElfParser::__load_elf_file(){
    QFileDialog *file_dialog = new QFileDialog(this,"Открыть elf файл","/","Elf-file (*.axf *.bin *.elf *.o *out *.prx *.puff *.ko *.mod *.so);; Exe files (*)");
    file_dialog->show();
    file_dialog->exec();
    file_dialog->close();
    __path_to_file = file_dialog->selectedFiles()[0];
    ui->__path_to_elf->setText(__path_to_file);

    delete file_dialog;

    if(__elf_controller != nullptr){
        delete __elf_controller;
        __elf_controller = nullptr;
    }
    __elf_controller = new ControllerElfParser(__path_to_file);

    __fill_elf_header();
    __fill_elf_sections_header();
    __fill_elf_program_header();
    __fill_symbols();
}

void ViewElfParser::__fill_elf_header(){
    QMap<QString,QString> __item_table_header = __elf_controller->getFileHeader();

    ui->__table_elf_header->setColumnCount(2);
    ui->__table_elf_header->setRowCount(__item_table_header.size());
    QTableWidgetItem *table_key = new QTableWidgetItem[__item_table_header.size()];
    QTableWidgetItem *table_value = new QTableWidgetItem[__item_table_header.size()];
    int i = 0;
    for(auto j = __item_table_header.constBegin(); j != __item_table_header.constEnd(); j++, i++){
        table_key[i].setText(j.key());
        table_value[i].setText(j.value());
        ui->__table_elf_header->setItem(i,0,&table_key[i]);
        ui->__table_elf_header->setItem(i,1,&table_value[i]);
    }
    ui->__table_elf_header->horizontalHeader()->setStretchLastSection(true);
    ui->__table_elf_header->setColumnWidth(0,400);
    ui->__table_elf_header->setWordWrap(true);
}
void ViewElfParser::__fill_elf_sections_header(){
    QVector<QMap<QString,QString>> *__item_table_header = __elf_controller->getSectionHeaders();
    QVector<QString> name = __elf_controller->getSectionTableName();
    int count_headers = __item_table_header->size();
    int count_cell = __item_table_header->at(0).size();

    ui->__table_section_header->setColumnCount(2);
    ui->__table_section_header->setRowCount((count_cell + 1)*count_headers + count_headers-1);
    QTableWidgetItem **table_key = new QTableWidgetItem *[count_headers];
    QTableWidgetItem **table_value = new QTableWidgetItem *[count_headers];
    int temp = 0;
    for(int i = 0; i < count_headers; ++i){
        int j = 0;
        table_key[i] = new QTableWidgetItem [count_cell];
        table_value[i] = new QTableWidgetItem [count_cell];
        QTableWidgetItem *name_one = new QTableWidgetItem;
        QTableWidgetItem *name_two = new QTableWidgetItem;
        name_one->setText("Название таблицы");
        name_two->setText(name[i]);
        ui->__table_section_header->setItem(temp,0,name_one);
        ui->__table_section_header->setItem(temp,1,name_two);
        temp++;
        for(auto k = __item_table_header->at(i).constBegin(); k != __item_table_header->at(i).constEnd(); k++, j++){
            table_key[i][j].setText(k.key());
            table_value[i][j].setText(k.value());

            ui->__table_section_header->setItem(temp,0,&table_key[i][j]);
            ui->__table_section_header->setItem(temp,1,&table_value[i][j]);
            temp++;
        }
        temp++;
    }
    ui->__table_section_header->horizontalHeader()->setStretchLastSection(true);
    ui->__table_section_header->setColumnWidth(0,400);
    ui->__table_section_header->setWordWrap(true);
}
void ViewElfParser::__fill_elf_program_header(){
    QVector<QMap<QString,QString>> *__item_table_header = __elf_controller->getProgramHeaders();
    int count_headers = __item_table_header->size();
    int count_cell = __item_table_header->at(0).size();

    ui->__table_program_header->setColumnCount(2);
    ui->__table_program_header->setRowCount(count_cell*count_headers + count_headers - 1);
    QTableWidgetItem **table_key = new QTableWidgetItem *[count_headers];
    QTableWidgetItem **table_value = new QTableWidgetItem *[count_headers];
    int temp = 0;
    for(int i = 0; i < count_headers; ++i){
        int j = 0;
        table_key[i] = new QTableWidgetItem [count_cell];
        table_value[i] = new QTableWidgetItem [count_cell];
        for(auto k = __item_table_header->at(i).constBegin(); k != __item_table_header->at(i).constEnd(); k++, j++){
            table_key[i][j].setText(k.key());
            table_value[i][j].setText(k.value());

            ui->__table_program_header->setItem(temp,0,&table_key[i][j]);
            ui->__table_program_header->setItem(temp,1,&table_value[i][j]);
            temp++;
        }
        temp++;
    }
    ui->__table_program_header->horizontalHeader()->setStretchLastSection(true);
    ui->__table_program_header->setColumnWidth(0,400);
    ui->__table_program_header->setWordWrap(true);
}
void ViewElfParser::__fill_symbols(){
    QVector<QString> dynsym = __elf_controller->getDynSym();
    QVector<QString> symtab = __elf_controller->getSymTab();

    ui->__table_dynsym->setColumnCount(1);
    ui->__table_dynsym->setRowCount(dynsym.size());
    QTableWidgetItem *table_dynsym = new QTableWidgetItem[dynsym.size()];

    for(int i = 0; i < dynsym.size(); i++){
        table_dynsym[i].setText(dynsym.at(i));
        ui->__table_dynsym->setItem(i, 0, &table_dynsym[i]);
    }

    ui->__table_sym_tab->setColumnCount(1);
    ui->__table_sym_tab->setRowCount(symtab.size());
    QTableWidgetItem *table_symtab = new QTableWidgetItem[symtab.size()];

    for(int i = 0; i < symtab.size(); i++){
        table_symtab[i].setText(symtab.at(i));
        ui->__table_sym_tab->setItem(i, 0, &table_symtab[i]);
    }
    ui->__table_dynsym->horizontalHeader()->setStretchLastSection(true);
    ui->__table_dynsym->setColumnWidth(0,400);
    ui->__table_dynsym->setWordWrap(true);
    ui->__table_sym_tab->horizontalHeader()->setStretchLastSection(true);
    ui->__table_sym_tab->setColumnWidth(0,400);
    ui->__table_sym_tab->setWordWrap(true);
}
ViewElfParser::~ViewElfParser()
{
    delete ui;
}
