#include "dynamicui.h"
#include "graphs.h"
#include "plotwindow.h"
#include "report.h"
#include "src/ui_dynamicui.h"
#include "statwindow.h"
#include "tcontainer.h"
#include <QDateTime>
#include <QFileDialog>
#include <QPageSize>
#include <QScrollArea>
#include <QTimer>
#include <QtCharts/QChartView>

DynamicUI::DynamicUI(QWidget *parent) : QMainWindow(parent), ui(new Ui::DynamicUI) {}

DynamicUI::DynamicUI(QWidget *parent, const QString &filePath) : QMainWindow(parent), ui(new Ui::DynamicUI), _filePath(filePath) {
    ui->setupUi(this);
    ui->scrollArea->setWidgetResizable(true);
    // ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    contentWidget = new QWidget(ui->scrollArea);
    ui->scrollArea->setWidget(contentWidget);
    // contentWidget->setMinimumSize(1080,10000);

    auto *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &DynamicUI::updateDateTime);
    timer->start(1000);

    _df         = DataFrame(filePath);
    widgetSpawn = QPoint(ui->scrollArea->rect().topLeft().x() + 5, ui->scrollArea->rect().topLeft().y() + 5);

    connect(ui->btnAddGraph, &QPushButton::clicked, this, &DynamicUI::openPlotWindow);
    connect(ui->btnAddStatistic, &QPushButton::clicked, this, &DynamicUI::openStatWindow);
    connect(ui->btnSave, &QPushButton::clicked, this, &DynamicUI::saveToPdfButtonClicked);
    connect(ui->btnAddWorkSpace, &QPushButton::clicked, this, &DynamicUI::expandWorkSpace);
}

DynamicUI::~DynamicUI() {
    if (p)
        delete p;
    if (s)
        delete s;

    for (auto *o : objects) {
        delete (o);
    }
    delete ui;
}

void DynamicUI::addObject(TContainer *tc) {
    objects.push_back(tc);
}

DataFrame DynamicUI::df() {
    return this->_df;
}

QWidget *DynamicUI::getContentWidget() {
    return contentWidget;
}

void DynamicUI::openPlotWindow() {
    if (p)
        delete p;
    QLayout *layout = new QVBoxLayout(ui->scrollArea);
    p               = new PlotWindow(this, layout);
    p->show();
}

void DynamicUI::openStatWindow() {
    if (s)
        delete s;
    QLayout *layout = new QVBoxLayout(ui->scrollArea);
    s               = new StatWindow(this, layout);
    s->show();
}

void DynamicUI::saveToPdf(const QString &fileName) {

    Report report;
    report.setTargetWidget(this);
    report.saveToPdf(fileName);

    setFixedSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
}
void DynamicUI::updateDateTime() {
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString dateTimeString    = currentDateTime.toString("yyyy-MM-dd hh:mm:ss");

    auto *lblDateTime = ui->centralwidget->findChild<QLabel *>("lblDateTime");
    if (lblDateTime != nullptr) {
        lblDateTime->setText(dateTimeString);
    }
}

void DynamicUI::expandWorkSpace() {
    qDebug() << "KLIK";
    contentWidget->setMinimumSize(contentWidget->minimumWidth(), contentWidget->minimumHeight() + 300);
}

void DynamicUI::saveToPdfButtonClicked() {
    QString fileName = QFileDialog::getSaveFileName(this, "Save PDF", "", "PDF Files (*.pdf)");

    if (!fileName.isEmpty()) {
        saveToPdf(fileName);
    }
}
