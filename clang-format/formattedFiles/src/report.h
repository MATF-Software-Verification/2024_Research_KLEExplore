#ifndef REPORT_H
#define REPORT_H

#include <QWidget>

class Report : public QWidget {
    Q_OBJECT

  public:
    Report(QWidget *parent = nullptr);

    void setTargetWidget(QWidget *widget);

    QWidget *getTargetWidget() const { return targetWidget; }

  public slots:
    void saveToPdf(const QString &fileName);

  private:
    QPixmap grabWidgetPixmap() const;
    void paintToPdf(QPainter &painter);

  private:
    QWidget *targetWidget;
};

#endif
