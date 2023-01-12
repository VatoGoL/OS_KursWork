#include "dispetcher.h"
#include "ui_dispetcher.h"

Dispetcher::Dispetcher(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dispetcher)
{
    ui->setupUi(this);
}

Dispetcher::~Dispetcher()
{
    delete ui;
}
