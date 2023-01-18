#ifndef VIEWELFPARSER_H
#define VIEWELFPARSER_H

#include <QWidget>
#include "../../controller/ControllerElfParser/ControllerElfParser.h"
#include <QTableWidgetItem>
#include <QFileDialog>

namespace Ui {
class ViewElfParser;
}

class ViewElfParser : public QWidget
{
    Q_OBJECT

private:
    Ui::ViewElfParser *ui;
    ControllerElfParser *__elf_controller = nullptr;
    QString __path_to_file = "";

    void __fill_elf_header();
    void __fill_elf_sections_header();
    void __fill_elf_program_header();
    void __fill_symbols();
private slots:
    void __load_elf_file();
public:
    explicit ViewElfParser(QWidget *parent = nullptr);

    ~ViewElfParser();

};

#endif // VIEWELFPARSER_H
