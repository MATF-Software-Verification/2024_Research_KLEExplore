#include "statistic.h"
#include "qdebug.h"
#include <algorithm>
#include <map>

Statistic::Statistic() = default;

int tmpSize = 0;

QString Statistic::mode(const QVector<T> &data) {
    std::map<QString, int> counter;
    for (const auto &i : data) {
        counter[i]++;
    }

    if (counter.empty()) {
        return {};
    }
    qDebug() << counter.begin()->first << counter.begin()->second;
    return counter.begin()->first;
}

QVector<double> toDoubleVector(const QVector<T> &data) {
    QVector<double> tmp;
    for (const auto &x : data) {
        if (x != "") {
            tmp.push_back(x.toDouble());
        }
    }
    return tmp;
}

double Statistic::sum(const QVector<T> &data) {
    auto tmp = toDoubleVector(data);
    tmpSize  = tmp.size();
    return std::accumulate(tmp.begin(), tmp.end(), 0.0);
}

double Statistic::mean(const QVector<T> &data) {
    if (data.isEmpty()) {
        return std::numeric_limits<double>::quiet_NaN();
    }
    auto sum = Statistic::sum(data);
    return sum / static_cast<double>(tmpSize);
}

double Statistic::min(const QVector<T> &data) {
    if (data.isEmpty()) {
        return std::numeric_limits<double>::quiet_NaN();
    }
    auto tmp = toDoubleVector(data);
    return *std::min_element(tmp.begin(), tmp.end());
}

double Statistic::max(const QVector<T> &data) {
    if (data.isEmpty()) {
        return std::numeric_limits<double>::quiet_NaN();
    }
    auto tmp = toDoubleVector(data);
    return *std::max_element(tmp.begin(), tmp.end());
}

double Statistic::median(const QVector<T> &data) {
    auto tmp = toDoubleVector(data);
    int n    = tmp.length();
    if (n == 0) {
        return std::numeric_limits<double>::quiet_NaN();
    }
    if (n % 2 == 0) {
        return (tmp[n / 2] + tmp[n / 2 - 1]) / 2.0;
    } else {
        return tmp[n / 2];
    }
}

double Statistic::sd(const QVector<T> &data) {
    return sqrt(variance(data));
}

double Statistic::variance(const QVector<T> &data) {
    auto sum = 0.0;
    auto tmp = toDoubleVector(data);
    auto m   = mean(data);
    for (auto &x : tmp) {
        sum += (x - m) * (x - m);
    }
    return (1.0 / tmp.size()) * sum;
}

std::map<T, double> Statistic::value_counts(const QVector<T> &data) {
    std::map<QString, double> counter;
    for (const auto &i : data) {
        counter[i]++;
    }

    return counter;
}

double Statistic::null_count(const QVector<T> &data) {
    int counter = 0;
    for (const auto &i : data) {
        if (i == "") {
            counter++;
        }
    }

    return counter;
}
