#ifndef DATAFRAME_H
#define DATAFRAME_H

#include <QString>
#include <QVector>
#include <unordered_map>

enum TYPE { NUMERIC, STRING };

class VarVector {
  public:
    VarVector();
    VarVector(TYPE t, QVector<QString> vars);

    QString operator[](int x);
    TYPE type();
    QVector<QString> vars();
    void push_back(const QString &x);
    void set_type(TYPE t);
    std::unordered_map<QString, int> valueCounts();

  private:
    TYPE _t;
    QVector<QString> _vars;
};

class DataFrame {
  public:
    std::unordered_map<QString, VarVector> _df;

    DataFrame();
    DataFrame(const QString &path);

    VarVector operator[](const QString &s);
    QVector<QString> header() const;
    void push_back(QString &col, const QString &value);
    VarVector unique(const QString &column);
    void setColType(QString &col, TYPE t);
    TYPE colType(const QString &column);
    int count(QString &column, QString &item);
    DataFrame filter(QString &column, QString &item);
    // VALUE COUNTS
};

#endif // DATAFRAME_H
