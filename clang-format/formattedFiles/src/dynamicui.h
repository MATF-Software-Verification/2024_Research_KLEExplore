#ifndef DYNAMICUI_H
#define DYNAMICUI_H

#include "dataframe.h"
#include "plotwindow.h"
#include "statwindow.h"
#include "tcontainer.h"
#include <QComboBox>
#include <QHash>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QScrollArea>

namespace Ui {
class DynamicUI;
}

class DynamicUI : public QMainWindow {
    Q_OBJECT

  public:
    explicit DynamicUI(QWidget *parent = nullptr);
    DynamicUI(QWidget *parent = nullptr, const QString &filePath = "");
    ~DynamicUI() override;
    DataFrame df();
    QWidget *getContentWidget();
    QPoint widgetSpawn;
    void addObject(TContainer *tc);

  public slots:
    void openPlotWindow();
    void openStatWindow();
    void saveToPdf(const QString &fileName);
    void updateDateTime();
    void saveToPdfButtonClicked();
    void expandWorkSpace();

  private:
    QVector<TContainer *> objects;
    Ui::DynamicUI *ui;
    QString _filePath;
    DataFrame _df;
    QScrollArea *sa;
    QWidget *contentWidget;
    PlotWindow *p;
    StatWindow *s;
};

#endif
