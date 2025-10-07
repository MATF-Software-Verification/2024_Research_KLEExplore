#ifndef READCSV_H
#define READCSV_H

#include <QVector>
#include <qstring.h>
#include <unordered_map>
class ReadCSV {
  public:
    ReadCSV();
    static std::unordered_map<QString, QVector<QString>> readCsv(const QString &path);
    static std::unordered_map<int, QString> mapCols(const QVector<QString> &headers);

    // CHECK
};

#endif // READCSV_H
