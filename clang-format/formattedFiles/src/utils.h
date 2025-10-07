#ifndef UTILS_H
#define UTILS_H

#include "dataframe.h"

class Utils {
  public:
    Utils();
    static QVector<QString> filterStatsByType(TYPE t);
    static double calculateStat(QString &stat, QVector<QString> data);
};

#endif
