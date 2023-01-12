#ifndef DISPETCHER_H
#define DISPETCHER_H

#include <QDialog>

namespace Ui {
class Dispetcher;
}

class Dispetcher : public QDialog
{
    Q_OBJECT

public:
    explicit Dispetcher(QWidget *parent = nullptr);
    ~Dispetcher();

private:
    Ui::Dispetcher *ui;
};

#endif // DISPETCHER_H
