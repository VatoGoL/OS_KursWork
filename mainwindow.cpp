#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(openElfParser()));
}

void MainWindow::openElfParser(){
    __elf_parser = new ViewElfParser(this);
    __elf_parser->show();
}

MainWindow::~MainWindow()
{
    if(__elf_parser != nullptr)
        delete __elf_parser;
    delete ui;
}

