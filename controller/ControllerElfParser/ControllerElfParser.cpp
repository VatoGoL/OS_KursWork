#include "ControllerElfParser.h"
#include "string"
using namespace std;
ControllerElfParser::ControllerElfParser(QString path_to_file)
{
    startParse(path_to_file);
    parseAllMapElf();
}
void ControllerElfParser::startParse(QString path_to_file){
    if(__elf_parser != nullptr){
        delete __elf_parser;
        __elf_parser = nullptr;
    }
    __elf_parser = new ModelElfParser(path_to_file);

    try {
        __elf_parser->parse();
    }  catch (invalid_argument& message) {
        qDebug() << "||||" << message.what() << "||||";
        return;
    }
}

void ControllerElfParser::parseAllMapElf(){
    try {
        __parseMapElf(__path_elf_h_e_ident_class, &__map_elf_h_e_ident_class);
        __parseMapElf(__path_elf_h_e_indent_os_abi, &__map_elf_h_e_indent_os_abi);
        __parseMapElf(__path_elf_h_e_ident_data, &__map_elf_h_e_ident_data);
        __parseMapElf(__path_elf_h_e_version, &__map_elf_h_e_version);
        __parseMapElf(__path_elf_h_e_machine, &__map_elf_h_e_machine);
        __parseMapElf(__path_elf_h_e_type, &__map_elf_h_e_type);
        __parseMapElf(__path_elf_sh_type, &__map_elf_sh_type);
        __parseMapElf(__path_elf_sh_flags, &__map_elf_sh_flags);
        __parseMapElf(__path_elf_ph_p_type, &__map_elf_ph_p_type);
        __parseMapElf(__path_elf_ph_p_flags, &__map_elf_ph_p_flags);
    }  catch (invalid_argument &message) {
        qDebug() << message.what();
    }
}
void ControllerElfParser::__parseMapElf(QString path_to_file, QMap<int,QString> *map){
    QFile file(path_to_file);
    file.open(QFile::ReadOnly);
    if(!file.isOpen()){
        throw invalid_argument("File "+ path_to_file.toStdString() +" not open");
    }

    int n = 0;
    int key;
    QString value;
    QTextStream fin(&file);

    fin >> n;
    for(int i = 0; i < n; ++i){
        fin >> key;
        fin.read(1); //читаю пробел
        value = fin.readLine();
        map->insert(key,value);
    }
    file.close();
}

QMap<QString,QString> ControllerElfParser::getFileHeader(){
    Elf64_Ehdr *header = __elf_parser->getElfHeader();
    QMap<QString,QString> result;

    result.insert("Класс: ", __map_elf_h_e_ident_class[header->e_ident[EI_CLASS]]);
    result.insert("Метод кодирования данных: ", __map_elf_h_e_ident_data[header->e_ident[EI_DATA]]);
    result.insert("Версия ELF: ", QString("").setNum((int)header->e_ident[EI_VERSION]));
    result.insert("OS/ABI: ", __map_elf_h_e_indent_os_abi[header->e_ident[EI_OSABI]]);
    result.insert("Версия ABI: ", QString("").setNum((int)header->e_ident[EI_ABIVERSION]));
    result.insert("Тип файла: ", __map_elf_h_e_type[header->e_type]);
    result.insert("Архитектура: ", __map_elf_h_e_machine[header->e_machine]);
    result.insert("Номер версии формата: ",QString("").setNum(header->e_version));
    result.insert("Адресс точки входа: ","0x" + QString("").setNum(header->e_entry,16));
    result.insert("Смещение таблицы заголовков программы от начала файла в (байты): ",QString("").setNum(header->e_phoff));
    result.insert("Смещение таблицы заголовков разделов от начала файла (байты): ",QString("").setNum(header->e_shoff));
    result.insert("Флаги: ",QString("").setNum(header->e_flags,16));
    result.insert("Размер заголовка файла (байты): ",QString("").setNum(header->e_ehsize));
    result.insert("Размер одного заголовка программы: ",QString("").setNum(header->e_phentsize));
    result.insert("Число заголовков программы: ",QString("").setNum(header->e_phnum));
    result.insert("Размер одного заголовка секции: ",QString("").setNum(header->e_shentsize));
    result.insert("Число секций программы: ",QString("").setNum(header->e_shnum));
    result.insert("Индекс таблицы с названием секций: ",QString("").setNum(header->e_shstrndx));

    return result;
}

QVector<QMap<QString,QString>>* ControllerElfParser::getSectionHeaders(){

    QString temp;
    Elf64_Ehdr *header = __elf_parser->getElfHeader();
    unsigned short n = header->e_shnum;

    QVector<QMap<QString,QString>> *result = new QVector<QMap<QString,QString>>;

    for(unsigned short i = 0; i < n; i++){
        temp = "";
        QMap<QString,QString> map;
        if(__elf_parser->isTypeElf64()){
            Elf64_Shdr *section_header_64 = __elf_parser->getElf64SectionHeader();
            map.insert("Позиция имени в .shstrtab: ", QString("").setNum(section_header_64[i].sh_name));
            map.insert("Тип заголовка: ", __map_elf_sh_type[section_header_64[i].sh_type]);

            for(unsigned long k = section_header_64[i].sh_flags, j = 0; k > 0; k = k >> 1, j++){
                if((0b1 & k) == 1){
                    temp += __map_elf_sh_flags[j] + '\n';
                }
            }
            map.insert("Флаги: ", temp);

            map.insert("Адрес начала секции: ", "0x" + QString("").setNum(section_header_64[i].sh_addr,16));
            map.insert("Смещение секции от начала файла (байты): ", QString("").setNum(section_header_64[i].sh_offset));
            map.insert("Размер секции (байты): ", QString("").setNum(section_header_64[i].sh_size));
            map.insert("Индекс ассоциативной секции: ", QString("").setNum(section_header_64[i].sh_link));
            map.insert("Необходимое выравнивание секции: ", QString("").setNum(section_header_64[i].sh_addralign));
            map.insert("Размер каждой секции (байты): ", QString("").setNum(section_header_64[i].sh_entsize));
        }
        else{
            Elf32_Shdr *section_header_32 = __elf_parser->getElf32SectionHeader();
            map.insert("Позиция имени в .shstrtab: ", QString("").setNum(section_header_32[i].sh_name));
            map.insert("Тип заголовка: ", __map_elf_sh_type[section_header_32[i].sh_type]);
            for(unsigned long k = section_header_32[i].sh_flags, j = 0; k > 0; k = k >> 1, j++){
                if((0b1 & k) == 1){
                    temp += __map_elf_sh_flags[j] + "\n";
                }
            }
            map.insert("Флаги: ", temp);

            map.insert("Адрес начала секции: ", "0x" + QString("").setNum(section_header_32[i].sh_addr,16));
            map.insert("Смещение секции от начала файла (байты): ", QString("").setNum(section_header_32[i].sh_offset));
            map.insert("Размер секции (байты): ", QString("").setNum(section_header_32[i].sh_size));
            map.insert("Индекс ассоциативной секции: ", QString("").setNum(section_header_32[i].sh_link));
            map.insert("Необходимое выравнивание секции: ", QString("").setNum(section_header_32[i].sh_addralign));
            map.insert("Размер каждой секции (байты): ", QString("").setNum(section_header_32[i].sh_entsize));
        }
        result->push_back(map);
    }

    return result;
}

QVector<QMap<QString,QString>>* ControllerElfParser::getProgramHeaders(){
    QString temp;
    Elf64_Ehdr *header = __elf_parser->getElfHeader();
    unsigned short n = header->e_phnum;
    QVector<QMap<QString,QString>> *result = new QVector<QMap<QString,QString>>;
    for(unsigned short i = 0; i < n; ++i){
        temp = "";
        QMap<QString,QString> map;
        if(__elf_parser->isTypeElf64()){
            Elf64_Phdr *program_header_64 = __elf_parser->getElf64ProgramHeader();

            map.insert("Тип сегмента: ",__map_elf_ph_p_type[program_header_64[i].p_type]);
            for(unsigned long k = program_header_64[i].p_flags, j = 0; k > 0; k = k >> 1, j++){
                if((0b1 & k) == 1){
                    temp += __map_elf_ph_p_flags[j] + "\n";
                }
            }
            map.insert("Флаги: ", temp);
            map.insert("Смещение сегмента от начала файла (байты): ",QString("").setNum(program_header_64[i].p_offset));
            map.insert("Виртуальный адрес сегмента: ","0x" + QString("").setNum(program_header_64[i].p_vaddr,16));
            map.insert("Физический адрес сегмента: ","0x" + QString("").setNum(program_header_64[i].p_paddr,16));
            map.insert("Размер сегмента в файле: ",QString("").setNum(program_header_64[i].p_filesz));
            map.insert("Размер сегмента в памяти: ",QString("").setNum(program_header_64[i].p_memsz));
            map.insert("Выравнивание сегмента: ",QString("").setNum(program_header_64[i].p_align));
        }
        else{
            Elf32_Phdr *program_header_32 = __elf_parser->getElf32ProgramHeader();

            map.insert("Тип сегмента: ",__map_elf_ph_p_type[program_header_32[i].p_type]);
            for(unsigned long k = program_header_32[i].p_flags, j = 0; k > 0; k = k >> 1, j++){
                if((0b1 & k) == 1){
                    temp += __map_elf_ph_p_flags[j] + "\n";
                }
            }
            map.insert("Флаги: ", temp);
            map.insert("Смещение сегмента от начала файла (байты): ",QString("").setNum(program_header_32[i].p_offset));
            map.insert("Виртуальный адрес сегмента: ","0x" + QString("").setNum(program_header_32[i].p_vaddr,16));
            map.insert("Физический адрес сегмента: ","0x" + QString("").setNum(program_header_32[i].p_paddr,16));
            map.insert("Размер сегмента в файле: ",QString("").setNum(program_header_32[i].p_filesz));
            map.insert("Размер сегмента в памяти: ",QString("").setNum(program_header_32[i].p_memsz));
            map.insert("Выравнивание сегмента: ",QString("").setNum(program_header_32[i].p_align));
        }
        result->push_back(map);
    }
    return result;
}
QVector<QString> ControllerElfParser::getSectionTableName(){
    return __elf_parser->getSectionTableName();
}
QVector<QString> ControllerElfParser::getSymTab(){
    return __elf_parser->getSymTab();
}
QVector<QString> ControllerElfParser::getDynSym(){
    return __elf_parser->getDynSym();
}


ControllerElfParser::~ControllerElfParser(){
    if(__elf_parser != nullptr)
        delete __elf_parser;
}
