#include "graphs.h"

#include "utils.h"
#include <algorithm>
#include <iostream>
#include <memory>
#include <random>
#include <unordered_map>

#include <QApplication>
#include <QPalette>
#include <QPropertyAnimation>
#include <QRandomGenerator>
#include <QtCharts/QChartView>
#include <QtCharts/QLegend>
#include <QtCharts/QLegendMarker>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>

#define PEN_WIDTH 1.75

struct DataPoint {
    double xValue;
    double yValue;
};

bool compareDataPoints(const DataPoint &dp1, const DataPoint &dp2) {
    if (dp1.xValue == dp2.xValue) {
        return dp1.yValue < dp2.yValue;
    }
    return dp1.xValue < dp2.xValue;
}

Graphs::Graphs(QGraphicsView *parent) : QGraphicsView(parent) {}

QVector<DataPoint> Graphs::generateDataPoints(DataFrame &data, const QString &xColumn, const QString &yColumn) {
    QList<QString> xValues = data[xColumn].vars();
    QList<QString> yValues = data[yColumn].vars();

    QVector<DataPoint> dataPoints;
    for (int i = 0; i < xValues.size(); ++i) {
        DataPoint dp;
        dp.xValue = xValues[i].toDouble();
        dp.yValue = yValues[i].toDouble();
        dataPoints.push_back(dp);
    }
    std::sort(dataPoints.begin(), dataPoints.end(), compareDataPoints);

    return dataPoints;
}

QColor Graphs::generateRandomColor() {
    int red   = QRandomGenerator::global()->bounded(256);
    int green = QRandomGenerator::global()->bounded(256);
    int blue  = QRandomGenerator::global()->bounded(256);

    return {red, green, blue};
}

QPen Graphs::createPen(int width, QColor &color) {
    // QColor color = generateRandomColor();
    QPen pen;
    pen.setColor(color);
    pen.setStyle(Qt::SolidLine);
    pen.setWidthF(width);

    return pen;
}

QPropertyAnimation *Graphs::createSeriesAnimation(QAbstractSeries *series) {
    auto *seriesAnimation = new QPropertyAnimation(series, "opacity");
    seriesAnimation->setDuration(2000);
    seriesAnimation->setStartValue(0.0);
    seriesAnimation->setEndValue(4.0);
    return seriesAnimation;
}

void Graphs::setChartProperties(QChart *chart, QAbstractSeries *series, const QString &xColumn, const QString &yColumn) {
    auto *font = new QFont();
    font->setStyleStrategy(QFont::PreferAntialias);
    font->setWeight(QFont::Normal);
    font->setPointSize(10);

    auto *axisX = new QValueAxis();
    axisX->setTitleText(xColumn);
    axisX->setLinePenColor(QColor(0, 0, 0));
    axisX->setLabelsFont(*font);
    chart->setAxisX(axisX, series);

    auto *axisY = new QValueAxis();
    axisY->setTitleText(yColumn);
    axisY->setLabelsFont(*font);
    axisY->setLinePenColor(QColor(0, 0, 0));
    chart->setAxisY(axisY, series);

    QLegend *legend = chart->legend();
    legend->setVisible(false);

    QPropertyAnimation *seriesAnimation = createSeriesAnimation(series);
    Q_UNUSED(seriesAnimation);
    chart->setAnimationOptions(QChart::AllAnimations);
    chart->setBackgroundBrush(QBrush(QColor(255, 255, 255)));
    chart->setMargins(QMargins(15, 15, 15, 15));
}

QChart *Graphs::createLineChart(DataFrame &data, const QString &xColumn, const QString &yColumn, QColor &color) {
    auto *chart = new QChart();
    chart->removeAllSeries();

    auto *lineSeries              = new QLineSeries();
    QVector<DataPoint> dataPoints = generateDataPoints(data, xColumn, yColumn);

    for (const DataPoint &dp : dataPoints) {
        QPen pen = createPen(PEN_WIDTH, color);

        lineSeries->append(dp.xValue, dp.yValue);
        lineSeries->setPen(pen);
    }

    chart->addSeries(lineSeries);
    Graphs::setChartProperties(chart, lineSeries, xColumn, yColumn);

    return chart;
}

QChart *Graphs::createScatterChart(DataFrame &data, const QString &xColumn, const QString &yColumn, QColor &color) {
    auto *chart = new QChart();
    chart->removeAllSeries();

    auto *scatterSeries           = new QScatterSeries();
    QVector<DataPoint> dataPoints = generateDataPoints(data, xColumn, yColumn);

    for (const DataPoint &dp : dataPoints) {
        QPen pen = createPen(PEN_WIDTH, color);

        scatterSeries->append(dp.xValue, dp.yValue);
        scatterSeries->setPen(pen);
    }

    chart->addSeries(scatterSeries);
    scatterSeries->setMarkerSize(7);

    // QColor color = generateRandomColor();
    QBrush brush(color);
    scatterSeries->setBrush(brush);

    Graphs::setChartProperties(chart, scatterSeries, xColumn, yColumn);

    return chart;
}

QChart *Graphs::createPieOrDonutChart(DataFrame &data, const QString &xColumn, bool isDonutChart) {
    auto *chart = new QChart();

    std::unordered_map<QString, int> valueCounts = data[xColumn].valueCounts();
    std::vector<std::pair<QString, int>> sortedValues(valueCounts.begin(), valueCounts.end());
    std::sort(sortedValues.begin(), sortedValues.end(), [](const auto &a, const auto &b) { return a.second > b.second; });

    auto *pieSeries = new QPieSeries();

    if (isDonutChart) {
        pieSeries->setHoleSize(0.35);
    }

    bool firstSlice = true;
    for (const auto &pair : valueCounts) {
        QString label = pair.first;
        int count     = pair.second;

        QColor sliceColor = generateRandomColor();
        ;

        QPieSlice *slice = pieSeries->append(label, count);
        if (firstSlice) {
            slice->setExploded();
            slice->setExplodeDistanceFactor(0.2);
            firstSlice = false;
        }

        slice->setColor(sliceColor);
        chart->legend()->setVisible(true);
        chart->legend()->setAlignment(Qt::AlignRight);
        chart->legend()->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

        chart->legend()->setMaximumHeight(100);
        chart->legend()->setMaximumWidth(400);
    }

    QPropertyAnimation *seriesAnimation = createSeriesAnimation(pieSeries);
    Q_UNUSED(seriesAnimation);
    chart->setAnimationOptions(QChart::AllAnimations);
    chart->addSeries(pieSeries);
    chart->setMargins(QMargins(5, 5, 5, 5));

    return chart;
}

QChart *Graphs::createPieChart(DataFrame &data, const QString &xColumn) {
    return createPieOrDonutChart(data, xColumn, false);
}

QChart *Graphs::createDonutChart(DataFrame &data, const QString &xColumn) {
    return createPieOrDonutChart(data, xColumn, true);
}

QVector<double> returnStat(DataFrame &df, QString &xAxis, QString &col, QString &stat) {
    QVector<QString> categories = df.unique(xAxis).vars();
    std::unordered_map<QString, QVector<QString>> res;

    for (auto &x : categories) {
        DataFrame tmpDf = df.filter(xAxis, x);
        res[x]          = tmpDf[col].vars();
    }

    QVector<double> returnValue;
    for (auto &category : categories) {
        returnValue.push_back(Utils::calculateStat(stat, res[category]));
    }
    return returnValue;
}

QChart *Graphs::barPlot(DataFrame &df, QString &xAxis, QString &yAxis, QString &stat, QColor &color) {
    QVector<QString> x = df.unique(xAxis).vars();
    QVector<double> y  = returnStat(df, xAxis, yAxis, stat);

    auto *chart  = new QChart();
    auto *series = new QBarSeries();
    auto *qbSet  = new QBarSet(xAxis);

    for (size_t i = 0; i < x.size(); i++) {
        *qbSet << y[i];
    }

    qbSet->setColor(color);

    series->append(qbSet);
    chart->addSeries(series);
    auto *font = new QFont();
    font->setStyleStrategy(QFont::PreferAntialias);
    font->setWeight(QFont::DemiBold);
    font->setPointSize(7);

    auto *axis = new QBarCategoryAxis();
    axis->append(x);
    axis->setLabelsFont(*font);
    axis->setLabelsAngle(12);

    chart->createDefaultAxes();
    chart->setAxisX(axis, series);
    QLegend *legend = chart->legend();
    legend->setVisible(true);
    legend->setAlignment(Qt::AlignBottom);
    legend->setMarkerShape(QLegend::MarkerShapeCircle);
    legend->setContentsMargins(5, 5, 5, 5);
    QFont legendFont = legend->font();
    legendFont.setWeight(QFont::DemiBold);
    legendFont.setStyleStrategy(QFont::PreferAntialias);
    legend->setFont(legendFont);

    QPropertyAnimation *seriesAnimation = createSeriesAnimation(series);
    Q_UNUSED(seriesAnimation);
    chart->setAnimationOptions(QChart::AllAnimations);

    chart->setMargins(QMargins(5, 5, 5, 5));

    return chart;
}

QChart *Graphs::horizontalBarPlot(DataFrame &df, QString &xAxis, QString &yAxis, QString &stat, QColor &color) {
    QVector<QString> x = df.unique(xAxis).vars();
    QVector<double> y  = returnStat(df, xAxis, yAxis, stat);

    auto *chart  = new QChart();
    auto *series = new QHorizontalStackedBarSeries();
    auto *qbSet  = new QBarSet(xAxis);

    for (size_t i = 0; i < x.size(); i++) {
        *qbSet << y[i];
    }

    qbSet->setColor(color);

    series->append(qbSet);
    chart->addSeries(series);
    auto *font = new QFont();
    font->setStyleStrategy(QFont::PreferAntialias);
    font->setWeight(QFont::DemiBold);
    font->setPointSize(10);

    auto *axis = new QBarCategoryAxis();
    axis->append(x);
    axis->setLabelsFont(*font);

    chart->createDefaultAxes();
    chart->setAxisY(axis, series);
    QLegend *legend = chart->legend();
    legend->setVisible(true);
    legend->setAlignment(Qt::AlignBottom);
    legend->setContentsMargins(2, 2, 2, 2);
    QFont legendFont = legend->font();
    legendFont.setWeight(QFont::DemiBold);
    legendFont.setStyleStrategy(QFont::PreferAntialias);
    legend->setFont(legendFont);

    QPropertyAnimation *seriesAnimation = createSeriesAnimation(series);
    Q_UNUSED(seriesAnimation);
    chart->setAnimationOptions(QChart::AllAnimations);

    chart->setMargins(QMargins(5, 5, 5, 5));

    return chart;
}
