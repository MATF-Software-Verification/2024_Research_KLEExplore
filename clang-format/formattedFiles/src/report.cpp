#include "report.h"
#include <QDebug>
#include <QPageSize>
#include <QPainter>
#include <QPdfWriter>
#include <QScreen>

Report::Report(QWidget *parent) : QWidget(parent), targetWidget(nullptr) {}

void Report::setTargetWidget(QWidget *widget) {
    targetWidget = widget;
}

/*QPixmap Report::grabWidgetPixmap() const{
    if (targetWidget != nullptr) {
        QScreen *screen = targetWidget->screen();
        if (screen != nullptr) {
            static QPixmap pixmap;
            pixmap = screen->grabWindow(targetWidget->winId());
            return pixmap;
        }             qDebug() << "Nije moguće dobiti QScreen za ciljni widget!";

    } else {
        qDebug() << "Ciljni widget nije postavljen!";
    }

    static QPixmap emptyPixmap;
    return emptyPixmap;
}*/

void Report::saveToPdf(const QString &fileName) {

    QImage fullImage(targetWidget->size(), QImage::Format_ARGB32);
    fullImage.fill(Qt::darkGray); // Set the background color to white

    QPainter fullPainter(&fullImage);
    targetWidget->render(&fullPainter);

    QPdfWriter pdfWriter(fileName);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));
    pdfWriter.setPageOrientation(QPageLayout::Landscape);

    QPainter painter(&pdfWriter);
    paintToPdf(painter);

    double scaleFactor = 9.0;
    int targetWidth    = static_cast<int>(scaleFactor * fullImage.width());
    int targetHeight   = static_cast<int>(scaleFactor * fullImage.height());

    painter.drawImage(0, 0, fullImage.scaled(targetWidth, targetHeight));
}
void Report::paintToPdf(QPainter &painter) {
    if (!painter.begin(this)) {
        qDebug() << "Greška pri početku crtanja na uređaju!";
    }
}
