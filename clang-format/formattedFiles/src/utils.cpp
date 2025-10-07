#include "utils.h"
#include "statistic.h"

Utils::Utils() = default;

QVector<QString> Utils::filterStatsByType(TYPE t) {
    if (t == TYPE::NUMERIC) {
        return QVector<QString>({"null_count", "mean", "sum", "min", "max", "median", "std. deviation", "variance"});
    }
    return QVector<QString>({"null_count", "mode", "value_counts"});
}

double Utils::calculateStat(QString &stat, QVector<QString> data) {
    if (stat == "mean") {
        return Statistic::mean(data);
    }
    if (stat == "sum") {
        return Statistic::sum(data);
    }
    if (stat == "min") {
        return Statistic::min(data);
    }
    if (stat == "max") {
        return Statistic::max(data);
    }
    if (stat == "median") {
        return Statistic::median(data);
    }
    if (stat == "sd. deviation") {
        return Statistic::sd(data);
    }
    if (stat == "variance") {
        return Statistic::variance(data);
    }
    if (stat == "count") {
        return (double)data.size();
    }
    if (stat == "null_count") {
        return Statistic::null_count(data);
    }
    return 0.0;
}
