#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(Disp_Open()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Disp_Open()
{
    Dispetcher *dr = new Dispetcher(this);
    dr->show();
}
