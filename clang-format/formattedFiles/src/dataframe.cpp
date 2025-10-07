#include "dataframe.h"
#include "readcsv.h"

#include <QDebug>
#include <QString>
#include <QVector>
#include <regex>
#include <utility>

QString TYPEtoString(TYPE t) {
    if (t == TYPE::STRING) {
        return "String";
    }
    // if(t == TYPE::DATE) return "Date";
    if (t == TYPE::NUMERIC) {
        return "Numeric";
    }
    return "";
}

bool isDateType(const std::string &s) {
    QVector<std::string> dateFormats = {
        R"(\d{4}-\d{2}-\d{2})",          // YYYY-MM-DD
        R"(\d{4}/\d{2}/\d{2})",          // YYYY/MM/DD
        R"(\d{4}\.\d{2}\.\d{2})",        // YYYY.MM.DD
        R"(\d{2}/\d{2}/\d{4})",          // DD/MM/YYYY
        R"(\d{2}-\d{2}-\d{4})",          // DD-MM-YYYY
        R"(\d{2}\.\d{2}\.\d{4})",        // DD.MM.YYYY
        R"(\d{2}\s[A-Za-z]{3}\s\d{4})",  // DD Mon YYYY
        R"([A-Za-z]{3}\s\d{2},\s\d{4})", // Mon DD, YYYY
    };
    for (const auto &format : dateFormats) {
        std::regex pattern(format);
        if (std::regex_match(s, pattern)) {
            return true;
        }
    }

    return false;
}

VarVector::VarVector() {
    _vars = QVector<QString>();
    _t    = TYPE::STRING;
};
VarVector::VarVector(TYPE t, QVector<QString> vars) : _t(t), _vars(std::move(vars)) {}

QString VarVector::operator[](int x) {
    return _vars[x];
}
TYPE VarVector::type() {
    return _t;
}

QVector<QString> VarVector::vars() {
    return _vars;
}

void VarVector::push_back(const QString &x) {
    _vars.push_back(x);
}

void VarVector::set_type(TYPE t) {
    _t = t;
}

std::unordered_map<QString, int> VarVector::valueCounts() {
    std::unordered_map<QString, int> cnts;
    for (auto &x : _vars) {
        cnts[x]++;
    }
    return cnts;
}

TYPE deductType(QVector<QString> &s) {
    // check if it is NUMERIC
    bool numeric = false;
    for (auto &x : s) {
        if (x == "") {
            continue;
        }
        x.toDouble(&numeric);
        if (!numeric) {
            break;
        }
    }
    if (numeric) {
        return TYPE::NUMERIC;
    }

    return TYPE::STRING;
}

DataFrame::DataFrame() = default;

DataFrame::DataFrame(const QString &path) {
    std::unordered_map<QString, QVector<QString>> df = ReadCSV::readCsv(path);
    for (auto &col : df) {
        TYPE t = deductType(col.second);
        // qDebug()<<"kolona: "<<col.first<<" "<<t;
        _df[col.first] = VarVector(t, col.second);
    }
}
VarVector DataFrame::operator[](const QString &s) {
    return _df[s];
}
QVector<QString> DataFrame::header() const {
    QVector<QString> res;
    for (const auto &x : _df) {
        if (x.first != "") {
            res.append(x.first);
        }
    }
    return res;
}

TYPE DataFrame::colType(const QString &column) {
    return _df[column].type();
}

VarVector DataFrame::unique(const QString &column) {
    std::unordered_map<QString, int> appear;
    QVector<QString> res;
    for (auto &s : _df[column].vars()) {
        if (appear.find(s) == appear.end()) {
            res.append(s);
        }
        appear[s]++;
    }

    return {this->colType(column), res};
}

int DataFrame::count(QString &column, QString &item) {
    int cnt = 0;
    for (auto &x : _df[column].vars()) {
        if (x == item) {
            cnt++;
        }
    }
    return cnt;
}

void DataFrame::push_back(QString &col, const QString &value) {
    _df[col].push_back(value);
}

void DataFrame::setColType(QString &col, TYPE t) {
    _df[col].set_type(t);
}

DataFrame DataFrame::filter(QString &column, QString &item) {
    DataFrame df              = DataFrame();
    QVector<QString> colNames = this->header();
    size_t n                  = _df[column].vars().size();
    QVector<QString> tmp      = _df[column].vars();
    for (size_t i = 0; i < n; i++) {
        if (tmp[i] == item) {
            for (auto &x : colNames) {
                df.push_back(x, _df[x][i]);
            }
        }
    }

    for (auto &col : df.header()) {
        df.setColType(col, _df[col].type());
    }
    return df;
}
