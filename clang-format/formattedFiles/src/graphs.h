#ifndef GRAPHS_H
#define GRAPHS_H

#include "dataframe.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPropertyAnimation>
#include <QString>
#include <QVector>
#include <QWidget>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QHorizontalStackedBarSeries>
#include <QtCharts/QLineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QValueAxis>

struct DataPoint;

class Graphs : public QGraphicsView {
    Q_OBJECT
  public:
    explicit Graphs(QGraphicsView *parent = nullptr);
    static QChart *createLineChart(DataFrame &data, const QString &xColumn, const QString &yColumn, QColor &color);
    static QChart *createScatterChart(DataFrame &data, const QString &xColumn, const QString &yColumn, QColor &color);
    static QChart *createPieOrDonutChart(DataFrame &data, const QString &xColumn, bool isDonutChart);
    static QChart *createPieChart(DataFrame &data, const QString &xColumn);
    static QChart *createDonutChart(DataFrame &data, const QString &xColumn);
    static QChart *barPlot(DataFrame &df, QString &xAxis, QString &yAxis, QString &stat, QColor &color);
    static QChart *horizontalBarPlot(DataFrame &df, QString &xAxis, QString &yAxis, QString &stat, QColor &color);

    static QVector<DataPoint> generateDataPoints(DataFrame &data, const QString &xColumn, const QString &yColumn);
    static QPen createPen(int width, QColor &color);
    static QColor generateRandomColor();
    static QPropertyAnimation *createSeriesAnimation(QAbstractSeries *series);
    static void setChartProperties(QChart *chart, QAbstractSeries *series, const QString &xColumn, const QString &yColumn);

  private:
};

#endif // GRAPHS_H
