#include "readcsv.h"
#include <QDebug>
#include <QDir>
#include <QFile>
#include <unordered_map>

ReadCSV::ReadCSV() = default;

std::unordered_map<int, QString> ReadCSV::mapCols(const QVector<QString> &headers) {
    int cnt = 0;
    std::unordered_map<int, QString> mappedCols;
    for (const auto &x : headers) {
        mappedCols[cnt++] = x;
    }
    return mappedCols;
}

/*QTextStream& qStdOut()
{
    static QTextStream ts( stdout );
    return ts;
}*/

std::unordered_map<QString, QVector<QString>> ReadCSV::readCsv(const QString &path) {

    QString filePath = QDir::cleanPath(path);

    if (!QFile::exists(filePath)) {
        qDebug() << "File does not exist";
        throw "FILE DOES NOT EXIST";
    }

    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Could not open file for reading" << file.errorString();
        throw "COULD NOT OPEN FILE";
    }

    QTextStream in(&file);
    QString headers = in.readLine();
    std::unordered_map<QString, QVector<QString>> df;

    QVector<QString> colNames = headers.split(',');
    for (auto &x : colNames) {
        qDebug() << x << " ";
    }

    std::unordered_map<int, QString> mapped = mapCols(colNames);

    while (!in.atEnd()) {
        QString line           = in.readLine();
        QVector<QString> lines = line.split(',');
        int cnt                = 0;
        for (auto &str : lines) {
            df[mapped[cnt]].push_back(str.trimmed());
            cnt++;
        }
    }

    file.close();
    return df;
}
