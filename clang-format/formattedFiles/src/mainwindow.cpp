#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "dataframe.h"
#include "graphs.h"
#include "statistic.h"

#include <QDialog>
#include <QGraphicsView>
#include <QVector>
#include <QtCharts/QChartView>

QVector<QString> mapTypeToStat(TYPE t) {
    if (t == TYPE::NUMERIC) {
        return QVector<QString>({"mode", "mean", "sum", "min", "max", "median", "sd", "variance"});
    }
    return QVector<QString>({"mode"});
}

MainWindow::MainWindow(QWidget *parent, const QString &filePath) : QMainWindow(parent), ui(new Ui::MainWindow), _filePath(filePath) {
    ui->setupUi(this);

    qDebug() << "ucitao";
    _df = DataFrame(filePath);

    for (auto &x : _df.header()) {
        ui->cbX->addItem(x);
        ui->cbY->addItem(x);
        ui->cbStatAxis->addItem(x);
    }
    connect(ui->cbStatAxis, &QComboBox::currentIndexChanged, this, &MainWindow::fillStats);
    connect(ui->pbShowStat, &QPushButton::clicked, this, &MainWindow::showStat);

    connect(ui->pushButton_3, &QPushButton::clicked, this, &MainWindow::openChartWindow);
    lineChart = new Graphs();

    QGraphicsView *graphicsView = ui->graphicsView;

    auto *scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, graphicsView->width(), graphicsView->height());
    graphicsView->setScene(scene);

    scene->addWidget(lineChart);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::fillStats() {
    ui->cbStats->clear();
    ui->cbStats->addItem("Stat select");
    QString col = ui->cbStatAxis->currentText();

    TYPE t                        = _df[col].type();
    QVector<QString> allowedStats = mapTypeToStat(t);

    for (auto &x : allowedStats) {
        ui->cbStats->addItem(x);
    }
}

void MainWindow::showStat() {
    QString stat    = ui->cbStats->currentText();
    QString colName = ui->cbStatAxis->currentText();

    QString output = "Error!";

    if (stat == "mean") {
        output = QString::number(Statistic::mean(_df[colName].vars()));
    }
    if (stat == "sum") {
        output = QString::number(Statistic::sum(_df[colName].vars()));
    }
    if (stat == "mode") {
        output = Statistic::mode(_df[colName].vars());
    }
    if (stat == "min") {
        output = QString::number(Statistic::min(_df[colName].vars()));
    }
    if (stat == "max") {
        output = QString::number(Statistic::max(_df[colName].vars()));
    }
    if (stat == "median") {
        output = QString::number(Statistic::median(_df[colName].vars()));
    }
    if (stat == "sd") {
        output = QString::number(Statistic::sd(_df[colName].vars()));
    }
    if (stat == "variance") {
        output = QString::number(Statistic::variance(_df[colName].vars()));
    }

    ui->label->setText(output);
}

void MainWindow::openChartWindow() {
    QString xColumn = ui->cbX->currentText();
    QString yColumn = ui->cbY->currentText();

    QGraphicsView *graphicsView = ui->graphicsView;

    auto *lineChart = new Graphs(graphicsView);
    lineChart->setFixedSize(graphicsView->size());

    auto *scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, graphicsView->width(), graphicsView->height());
    // lineChart->setSize(scene->width(), scene->height());
    QColor color = QColor(0, 0, 0);
    Graphs::createLineChart(_df, xColumn, yColumn, color);
    scene->addWidget(lineChart);
    graphicsView->setScene(scene);

    lineChart->show();
}
