#ifndef PLOTWINDOW_H
#define PLOTWINDOW_H

#include "dataframe.h"
#include <QColorDialog>
#include <QComboBox>
#include <QMainWindow>
#include <QtCharts/QChartView>

QT_BEGIN_NAMESPACE
namespace Ui {
class PlotWindow;
}
QT_END_NAMESPACE

class PlotWindow : public QMainWindow {
    Q_OBJECT

  public:
    PlotWindow(QWidget *parent = nullptr, QLayout *parentLayout = nullptr);
    ~PlotWindow() override;
    DataFrame data;

  public slots:
    void on_cbGraph_currentTextChanged(const QString &arg1);
    void drawDummie(const QString &arg1);
    void drawGraph(const QString &arg1);
    void on_btnMakePlot_clicked();
    void refreshColor(const QColor &color);

  private slots:
    void on_btnColor_clicked();

  private:
    Ui::PlotWindow *ui;
    QComboBox *_cbSender;
    QLayout *_parrentLayout;
    QChartView *_graph;
    QColorDialog *currentChartColor;
    QWidget *_targetParent;

    void setupNum();
    void setupCat();
    void setupOne();
    void setupTwo();
    void setupHideAll();
    QChart *getChartFromConfig();
};
#endif // PLOTWINDOW_H
