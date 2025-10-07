#ifndef STATISTIC_H
#define STATISTIC_H
#include <QString>
#include <QVector>
#include <map>

using T = QString;

class Statistic {
  public:
    Statistic();
    static double null_count(const QVector<T> &data);
    static std::map<T, double> value_counts(const QVector<T> &data);
    static QString mode(const QVector<T> &data);
    static double mean(const QVector<T> &data);
    static double sum(const QVector<T> &data);
    static double min(const QVector<T> &data);
    static double max(const QVector<T> &data);
    static double median(const QVector<T> &data);
    static double sd(const QVector<T> &data);
    static double variance(const QVector<T> &data);
};

#endif // STATISTIC_H
