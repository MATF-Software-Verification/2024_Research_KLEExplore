#include "dynamicui.h"

#include "fileimportwindow.h"
#include <QApplication>
#include <QDebug>
#include <QString>
#include <QTextStream>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    FileImportWindow w;
    // DynamicUI w;
    w.show();

    // FileImportWindow file;
    // file.show();
    // a.exec();

    // debug
    // std::unordered_map<QString, QVector<QString>> df =
    // ReadCSV::readCsv("C:/Users/zeljk/OneDrive/Desktop/RS/alat-za-analitiku/alatZaAnalitiku/test.txt");

    return QApplication::exec();
}
