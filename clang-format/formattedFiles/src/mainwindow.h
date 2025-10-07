#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "dataframe.h"
#include "graphs.h"

#include <QGraphicsView>
#include <QMainWindow>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    MainWindow(QWidget *parent = nullptr, const QString &filePath = "");
    ~MainWindow() override;

  public slots:
    void fillStats();
    void showStat();
    void openChartWindow();

  private:
    Ui::MainWindow *ui;
    QString _filePath;
    DataFrame _df;
    Graphs *lineChart;
};
#endif // MAINWINDOW_H
