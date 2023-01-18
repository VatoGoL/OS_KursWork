#ifndef CONTROLLERELFPARSER_H
#define CONTROLLERELFPARSER_H

#include <QDebug>
#include <QMap>
#include <QTextStream>
#include <QVector>
#include <stdexcept>
#include "../../model/ModelElfParser/ModelElfParser.h"

class ControllerElfParser
{
private:

    const QString __path_elf_h_e_ident_class = ":/controller/ControllerElfParser/elf_header_e_ident_class.txt";
    const QString __path_elf_h_e_ident_data = ":/controller/ControllerElfParser/elf_header_e_ident_data.txt";
    const QString __path_elf_h_e_indent_os_abi = ":/controller/ControllerElfParser/elf_header_e_ident_os_abi.txt";
    const QString __path_elf_h_e_type = ":/controller/ControllerElfParser/elf_header_e_type.txt";
    const QString __path_elf_h_e_machine = ":/controller/ControllerElfParser/elf_header_e_machine.txt";
    const QString __path_elf_h_e_version = ":/controller/ControllerElfParser/elf_header_e_version.txt";
    const QString __path_elf_ph_p_flags = ":/controller/ControllerElfParser/elf_ph_p_flags.txt";
    const QString __path_elf_ph_p_type = ":/controller/ControllerElfParser/elf_ph_p_type.txt";
    const QString __path_elf_sh_flags = ":/controller/ControllerElfParser/elf_sh_flags.txt";
    const QString __path_elf_sh_type = ":/controller/ControllerElfParser/elf_sh_type.txt";

    QMap<int, QString> __map_elf_h_e_ident_class;
    QMap<int, QString> __map_elf_h_e_ident_data;
    QMap<int, QString> __map_elf_h_e_indent_os_abi;
    QMap<int, QString> __map_elf_h_e_type;
    QMap<int, QString> __map_elf_h_e_machine;
    QMap<int, QString> __map_elf_h_e_version;
    QMap<int, QString> __map_elf_ph_p_flags;
    QMap<int, QString> __map_elf_ph_p_type;
    QMap<int, QString> __map_elf_sh_flags;
    QMap<int, QString> __map_elf_sh_type;

    ModelElfParser *__elf_parser = nullptr;

    void __parseMapElf(QString path_to_file, QMap<int,QString> *map);
public:
    ControllerElfParser(QString path_to_file);

    void startParse(QString path_to_file);
    void parseAllMapElf();
    QMap<QString,QString> getFileHeader();
    QVector<QMap<QString,QString>>* getSectionHeaders();
    QVector<QMap<QString,QString>>* getProgramHeaders();
    QVector<QString> getSectionTableName();
    QVector<QString> getSymTab();
    QVector<QString> getDynSym();
    unsigned short getSectionSize();


    virtual ~ControllerElfParser();
};

#endif // CONTROLLERELFPARSER_H
