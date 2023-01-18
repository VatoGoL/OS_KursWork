#ifndef MODELELFPARSER_H
#define MODELELFPARSER_H

#include <elf.h>
#include <QString>
#include <QFile>
#include <QDataStream>
#include <stdio.h>
#include <QDebug>
class ModelElfParser
{
private:
    QString __path_to_elf_file = "";
    FILE *__elf_file = NULL;

    Elf64_Ehdr __elf_header;
    Elf32_Shdr *__elf32_section_header = nullptr;
    Elf32_Phdr *__elf32_program_header = nullptr;
    Elf64_Shdr *__elf64_section_header = nullptr;
    Elf64_Phdr *__elf64_program_header = nullptr;
    QVector<QString> __sections_table_name;
    QVector<QString> __sym_tab;
    QVector<QString> __dyn_sym;

    bool __type_elf_64 = false;

    void __parseHeader();
    void __parseProgramHeader();
    void __parseSectionHeader();
    void __parseSectionTableName();
    void __parseSymTable();
public:
    ModelElfParser(QString path_to_elf_file);

    void parse();
    Elf64_Ehdr* getElfHeader();

    Elf32_Phdr* getElf32ProgramHeader();
    Elf32_Shdr* getElf32SectionHeader();

    Elf64_Phdr* getElf64ProgramHeader();
    Elf64_Shdr* getElf64SectionHeader();

    QVector<QString> getSectionTableName();
    QVector<QString> getSymTab();
    QVector<QString> getDynSym();

    bool isTypeElf64();

    virtual ~ModelElfParser();
};

#endif // MODELELFPARSER_H
