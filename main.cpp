#include "mainwindow.h"

#include <QApplication>
#include "controller/ControllerElfParser/ControllerElfParser.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();

    ///ControllerElfParser c_elf_parser("/home/vato/Desktop/ppp/build/main");
    //QVector<QMap<QString,QString>> *t = c_elf_parser.getSectionHeaders();

    /*for(int j = 0; j < t->size(); ++j){
        qDebug() << "J = " << j;
        for(QMap<QString,QString>::const_iterator i = t->at(j).constBegin();i != t->at(j).constEnd(); ++i){
            qDebug() << i.key() << " " << i.value();
        }
    }*/

}
