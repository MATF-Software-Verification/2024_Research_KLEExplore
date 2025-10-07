#ifndef STATWINDOW_H
#define STATWINDOW_H

#include <QComboBox>
#include <QLayout>
#include <QMainWindow>
#include <QScrollArea>

#include "dataframe.h"

namespace Ui {
class StatWindow;
}

class StatWindow : public QMainWindow {
    Q_OBJECT

  public:
    explicit StatWindow(QWidget *parent = nullptr, QLayout *parentLayout = nullptr);
    ~StatWindow() override;
    DataFrame data;

  public slots:
    void on_cbColumn_currentTextChanged(const QString &arg1);
    void on_btnMake_clicked();

  private:
    Ui::StatWindow *ui;
    QComboBox *_cbSender;
    QLayout *_parrentLayout;
    QWidget *_targetParent;
};

#endif // STATWINDOW_H
