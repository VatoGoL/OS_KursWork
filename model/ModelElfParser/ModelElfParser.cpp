#include "ModelElfParser.h"
#include "string"
using namespace std;
ModelElfParser::ModelElfParser(QString path_to_elf_file)
{
    __path_to_elf_file = path_to_elf_file;

    __elf_file = fopen(__path_to_elf_file.toStdString().c_str(),"rb");
    if(__elf_file == NULL){
        string a = "Elf file not open. file_path: " + __path_to_elf_file.toStdString();
        throw invalid_argument(a);
    }
}

/*Parse function*/
void ModelElfParser::parse(){
    __parseHeader();
    __parseProgramHeader();
    __parseSectionHeader();
    __parseSectionTableName();
    __parseSymTable();
}

void ModelElfParser::__parseHeader(){
    fseek(__elf_file,0,SEEK_SET);
    fread(&__elf_header,sizeof (__elf_header),1,__elf_file);
    if(__elf_header.e_ident[EI_CLASS] == ELFCLASS64){
        __type_elf_64 = true;
    }
}

void ModelElfParser::__parseProgramHeader(){
    if(__elf_header.e_phnum == 0){
        return;
    }
    if(__type_elf_64){

        __elf64_program_header = new Elf64_Phdr [__elf_header.e_phnum];
        int size_section = __elf_header.e_phentsize;

        for(int i = 0; i < __elf_header.e_phnum; ++i){
            fseek(__elf_file,__elf_header.e_phoff + i*size_section,SEEK_SET);
            fread(&__elf64_program_header[i], size_section,1,__elf_file);
        }
    }
    else{

        __elf32_program_header = new Elf32_Phdr [__elf_header.e_phnum];
        int size_section = __elf_header.e_phentsize;

        for(int i = 0; i < __elf_header.e_phnum; ++i){
            fseek(__elf_file,__elf_header.e_phoff + i*size_section,SEEK_SET);
            fread(&__elf32_program_header[i], size_section,1,__elf_file);
        }
    }
}

void ModelElfParser::__parseSectionHeader(){
    if(__elf_header.e_shnum == 0){
        return;
    }
    if(__type_elf_64){

        __elf64_section_header = new Elf64_Shdr [__elf_header.e_shnum];
        int size_section = __elf_header.e_shentsize;

        for(int i = 0; i < __elf_header.e_shnum; ++i){
            fseek(__elf_file,__elf_header.e_shoff+i*size_section,SEEK_SET);
            fread(&__elf64_section_header[i], size_section,1,__elf_file);

        }
    }
    else{

        __elf32_section_header = new Elf32_Shdr [__elf_header.e_shnum];
        int size_section = __elf_header.e_shentsize;

        for(int i = 0; i < __elf_header.e_shnum; ++i){
            fseek(__elf_file,__elf_header.e_shoff+i*size_section,SEEK_SET);
            fread(&__elf32_section_header[i], size_section,1,__elf_file);
        }
    }
}
void ModelElfParser::__parseSectionTableName(){
    char s[32];

    if(__type_elf_64){
        for(unsigned short i = 0; i < __elf_header.e_shnum; ++i){
            fseek(__elf_file,__elf64_section_header[__elf_header.e_shstrndx].sh_offset + __elf64_section_header[i].sh_name,SEEK_SET);
            fread(s,1,32,__elf_file);
            __sections_table_name.push_back(s);
        }
    }
    else{
        for(unsigned short i = 0; i < __elf_header.e_shnum; ++i){
            fseek(__elf_file,__elf32_section_header[__elf_header.e_shstrndx].sh_offset + __elf32_section_header[i].sh_name,SEEK_SET);
            fread(s,1,32,__elf_file);
            __sections_table_name.push_back(s);
        }
    }
}
void ModelElfParser::__parseSymTable(){
    char s[32];

    if(__type_elf_64){
        Elf64_Sym sym;
        for(int i  = 0; i < __sections_table_name.size(); i++){
            if(__sections_table_name.at(i) == ".dynsym"){
                qDebug() << ".dynsym";
                for(unsigned long j = 0; j < __elf64_section_header[i].sh_size / __elf64_section_header[i].sh_entsize; j++){
                    fseek(__elf_file,__elf64_section_header[i].sh_offset + __elf64_section_header[i].sh_entsize * j, SEEK_SET);
                    fread(&sym,sizeof(Elf64_Sym),1,__elf_file);
                    fseek(__elf_file,__elf64_section_header[i+1].sh_offset + sym.st_name, SEEK_SET);
                    fread(s,1,32,__elf_file);
                    __dyn_sym.push_back(s);
                    qDebug() << __dyn_sym.at(j);
                }
            }
            else if(__sections_table_name.at(i) == ".symtab"){
                qDebug() << ".symtab";
                for(unsigned long j = 0; j<__elf64_section_header[i].sh_size / __elf64_section_header[i].sh_entsize; j++){
                    fseek(__elf_file,__elf64_section_header[i].sh_offset + __elf64_section_header[i].sh_entsize * j, SEEK_SET);
                    fread(&sym,sizeof(Elf64_Sym),1,__elf_file);
                    fseek(__elf_file,__elf64_section_header[i+1].sh_offset + sym.st_name, SEEK_SET);
                    fread(s,1,32,__elf_file);
                    __sym_tab.push_back(s);
                    qDebug() << __sym_tab.at(j);
                }
            }
        }
    }

}

/*Get function*/
Elf64_Ehdr* ModelElfParser::getElfHeader(){
    return &__elf_header;
}
Elf32_Phdr* ModelElfParser::getElf32ProgramHeader(){
    return __elf32_program_header;
}
Elf32_Shdr* ModelElfParser::getElf32SectionHeader(){
    return __elf32_section_header;
}
Elf64_Phdr* ModelElfParser::getElf64ProgramHeader(){
    return __elf64_program_header;
}
Elf64_Shdr* ModelElfParser::getElf64SectionHeader(){
    return __elf64_section_header;
}
QVector<QString> ModelElfParser::getSectionTableName(){
    return __sections_table_name;
}
QVector<QString> ModelElfParser::getSymTab(){
    return __sym_tab;
}
QVector<QString> ModelElfParser::getDynSym(){
    return __dyn_sym;
}
bool ModelElfParser::isTypeElf64(){
    return __type_elf_64;
}


ModelElfParser::~ModelElfParser(){
    fclose(__elf_file);
    if(__elf64_program_header != nullptr)
        delete __elf64_program_header;
    if(__elf64_section_header != nullptr)
        delete __elf64_section_header;
    if(__elf32_program_header != nullptr)
        delete __elf64_program_header;
    if(__elf32_section_header != nullptr)
        delete __elf64_section_header;
}
