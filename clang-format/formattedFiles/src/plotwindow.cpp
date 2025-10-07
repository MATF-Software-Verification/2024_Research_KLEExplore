#include "plotwindow.h"
#include "./ui_plotwindow.h"
#include "dataframe.h"
#include "dynamicui.h"
#include "graphs.h"
#include "statistic.h"
#include "tcontainer.h"
#include "utils.h"

#include <QChart>
#include <QChartView>
#include <QColor>
#include <QColorDialog>
#include <QDebug>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPalette>
#include <QVBoxLayout>

enum { MAX_LENGTH = 15 };

QChart *returnPlot(QString &type, DataFrame &df, QString &xAxis, QString &yAxis, QString &stat, QColor &color) {
    // if(!color.isValid())
    //  color = Qt::blue;
    if (type == "None") {
        QChart *empty = new QChart();
        empty->setBackgroundBrush(Qt::white);
        return empty;
    }
    if (type == "Bar plot") {
        return Graphs::barPlot(df, xAxis, yAxis, stat, color);
    }
    if (type == "Line plot") {
        return Graphs::createLineChart(df, xAxis, yAxis, color);
    }
    if (type == "Scatter plot") {
        return Graphs::createScatterChart(df, xAxis, yAxis, color);
    }
    if (type == "Pie plot") {
        return Graphs::createPieChart(df, xAxis);
    }
    if (type == "Horizontal bar plot") {
        return Graphs::horizontalBarPlot(df, xAxis, yAxis, stat, color);
    }
    if (type == "Donut plot") {
        return Graphs::createDonutChart(df, xAxis);
    }

    return nullptr;
}

PlotWindow::PlotWindow(QWidget *parent, QLayout *parentLayout) : QMainWindow(parent), ui(new Ui::PlotWindow), _parrentLayout(parentLayout) {
    ui->setupUi(this);

    currentChartColor = new QColorDialog(QColor::fromRgb(1, 1, 1), this);
    currentChartColor->hide();
    _targetParent = qobject_cast<DynamicUI *>(parent)->getContentWidget();

    this->data = qobject_cast<DynamicUI *>(parent)->df();

    ui->cbX->clear();
    ui->cbY->clear();
    for (auto &s : data.header()) {
        ui->cbX->addItem(s);
        ui->cbY->addItem(s);
    }

    setupHideAll();

    _graph = new QChartView();
    ui->chartHolder->addWidget(_graph);
    _graph->setMaximumSize(300, 250);
    _graph->setMinimumSize(250, 190);

    connect(ui->cbGraph, &QComboBox::currentTextChanged, this, &PlotWindow::on_cbGraph_currentTextChanged);
    connect(ui->cbGraph, &QComboBox::currentTextChanged, this, &PlotWindow::drawDummie);

    connect(ui->cbX, &QComboBox::currentTextChanged, this, &PlotWindow::drawGraph);
    connect(ui->cbY, &QComboBox::currentTextChanged, this, &PlotWindow::drawDummie);
    connect(ui->cbStat, &QComboBox::currentTextChanged, this, &PlotWindow::drawDummie);
    connect(currentChartColor, &QColorDialog::currentColorChanged, this, &PlotWindow::refreshColor);
    // connect(currentChartColor, &QColorDialog::colorSelected,this, &PlotWindow::refreshColor);
}

PlotWindow::~PlotWindow() {
    delete ui;
}

void PlotWindow::setupHideAll() {
    ui->lblX->hide();
    ui->cbX->hide();
    ui->lblY->hide();
    ui->cbY->hide();
    ui->cbStat->hide();
    ui->lblStat->hide();
    ui->btnColor->hide();
}

// pomocne f za prikaz mogucnosti
void PlotWindow::setupOne() {
    ui->lblX->show();
    ui->cbX->show();
    ui->lblY->hide();
    ui->cbY->hide();
    ui->cbStat->hide();
    ui->lblStat->hide();
    ui->btnColor->hide();
}

void PlotWindow::setupTwo() {
    ui->lblX->show();
    ui->cbX->show();
    ui->lblY->show();
    ui->cbY->show();
    ui->cbStat->hide();
    ui->lblStat->hide();
    ui->btnColor->show();
}

void PlotWindow::setupNum() {

    ui->cbX->clear();
    ui->cbY->clear();
    QVector<QString> header = data.header();
    for (auto &x : header) {
        if (data.colType(x) == TYPE::NUMERIC) {
            ui->cbX->addItem(x);
            ui->cbY->addItem(x);
        }
    }
}
void PlotWindow::setupCat() {
    ui->cbX->clear();
    ui->cbY->clear();
    ui->cbX->addItems(data.header());
    ui->cbY->addItems(data.header());
}

void PlotWindow::on_cbGraph_currentTextChanged(const QString &arg1) {

    if (arg1 == "Pie plot" || arg1 == "Donut plot") {
        setupOne();
    } else if (arg1 == "Bar plot" || arg1 == "Horizontal bar plot") {
        setupTwo();
        ui->cbStat->show();
        ui->lblStat->show();
    } else if (arg1 == "Scatter plot") {
        setupTwo();
    } else if (arg1 == "Line plot") {
        setupTwo();
    } else {
        setupHideAll();
    }

    ui->btnMakePlot->show();
}

QChart *PlotWindow::getChartFromConfig() {
    QString x    = ui->cbX->currentText();
    QString y    = ui->cbY->currentText();
    QString type = ui->cbGraph->currentText();
    QString stat = ui->cbStat->currentText();
    QColor color = currentChartColor->currentColor();
    // qDebug()<<"BOJA JE: "<<color;
    QChart *chart = returnPlot(type, data, x, y, stat, color);

    // ui->lineEdit->setMaxLength(MAX_LENGTH);

    if (ui->lineEdit->text().length() > MAX_LENGTH) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle("Character Limit Exceeded");
        msgBox.setText("The maximum allowed length is 15 characters.");
        msgBox.setMinimumWidth(200);
        msgBox.exec();
        ui->lineEdit->setText(ui->lineEdit->text().left(MAX_LENGTH));
    }

    chart->setTitle(ui->lineEdit->text());

    QFont titleFont;
    titleFont.setFamily("Arial");
    titleFont.setPointSize(12);
    titleFont.setBold(true);
    chart->setTitleFont(titleFont);

    return chart;
}

void PlotWindow::refreshColor(const QColor &color) {
    /*if(currentChartColor == nullptr){
        return;
    }*/
    QChart *chart = getChartFromConfig();
    _graph->setChart(chart);
}

void PlotWindow::drawDummie(const QString &arg1) {
    disconnect(ui->cbX, &QComboBox::currentTextChanged, this, &PlotWindow::drawGraph);
    disconnect(ui->cbY, &QComboBox::currentTextChanged, this, &PlotWindow::drawDummie);
    disconnect(ui->cbStat, &QComboBox::currentTextChanged, this, &PlotWindow::drawDummie);
    if (arg1 == "Line plot" || arg1 == "Scatter plot") {
        setupNum();
    } else if (arg1 == "Pie plot" || arg1 == "Donut plot" || arg1 == "Bar plot" || arg1 == "Horizontal bar plot") {
        setupCat();
    }
    if (_graph->chart() == nullptr) {
        delete _graph->chart();
    }
    QChart *chart = getChartFromConfig();
    _graph->setChart(chart);
    connect(ui->cbX, &QComboBox::currentTextChanged, this, &PlotWindow::drawGraph);
    connect(ui->cbY, &QComboBox::currentTextChanged, this, &PlotWindow::drawDummie);
    connect(ui->cbStat, &QComboBox::currentTextChanged, this, &PlotWindow::drawDummie);
}

void PlotWindow::drawGraph(const QString &arg1) {
    if (_graph->chart() == nullptr) {
        delete _graph->chart();
    }
    QChart *chart = getChartFromConfig();
    _graph->setChart(chart);
}

void PlotWindow::on_btnMakePlot_clicked() {
    if (ui->cbGraph->currentText() == "None") {
        this->close();
        return;
    }
    QChart *chart = getChartFromConfig();

    auto *graph = new QChartView(chart);
    graph->setRenderHint(QPainter::Antialiasing);
    graph->resize(200, 200);

    auto *par = qobject_cast<DynamicUI *>(this->parentWidget());

    QPoint spawnPoint = par->widgetSpawn;
    auto *con1        = new TContainer(_targetParent, spawnPoint, graph, _parrentLayout);
    par->addObject(con1);

    this->close();
}

void PlotWindow::on_btnColor_clicked() {
    QColor color = QColorDialog::getColor(Qt::red, this, tr("Select Color"));
    currentChartColor->setCurrentColor(color);
}
