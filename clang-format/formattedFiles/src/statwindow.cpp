#include "statwindow.h"
#include "dataframe.h"
#include "dynamicui.h"
#include "statistic.h"
#include "tcontainer.h"
#include "ui_statwindow.h"
#include "utils.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

StatWindow::StatWindow(QWidget *parent, QLayout *parentLayout) : QMainWindow(parent), ui(new Ui::StatWindow), _parrentLayout(parentLayout) {
    ui->setupUi(this);

    _targetParent = qobject_cast<DynamicUI *>(parent)->getContentWidget();

    this->data = qobject_cast<DynamicUI *>(parent)->df();

    ui->cbColumn->clear();
    ui->cbStat->clear();

    QList<QString> columnContents = data.header();
    columnContents.push_front("Choose");
    ui->cbColumn->addItems(columnContents);

    this->setStyleSheet("background-color: #2b2e3d;"
                        "QComboBox {"
                        "background-color: #354056;"
                        "color: white;"
                        "border: 1px solid #cccccc;"
                        "padding: 5px;"
                        "border-radius: 10px;"
                        "}"
                        "QComboBox::down-arrow {"
                        "image: url(':/icons/strelica.png');"
                        "border-radius: 5px 5px 5px 5px"
                        "}"
                        "QComboBox::drop-down {"
                        "subcontrol-origin: padding;"
                        "subcontrol-position: top right;"
                        "}");
}

StatWindow::~StatWindow() {
    delete ui;
}

void StatWindow::on_cbColumn_currentTextChanged(const QString &arg1) {
    if (arg1 != "Choose") {
        ui->cbStat->clear();
        TYPE t                      = this->data.colType(arg1);
        QList<QString> statContents = Utils::filterStatsByType(t);
        ui->cbStat->addItems(statContents);
    } else {
        ui->cbStat->clear();
    }
}

void StatWindow::on_btnMake_clicked() {

    QString stat = ui->cbStat->currentText();
    QString col  = ui->cbColumn->currentText();

    auto *par          = qobject_cast<DynamicUI *>(this->parentWidget());
    QPoint spawnPoint  = par->widgetSpawn;
    QString buttonText = "";
    if (stat != "mode") {
        buttonText = QString::number(Utils::calculateStat(stat, data[col].vars()));
    } else {
        buttonText = Statistic::mode(data[col].vars());
    }
    QString txt  = ui->leTitle->text() + ":\n" + buttonText;
    auto *button = new QPushButton(_targetParent);
    button->setText(txt);
    button->setMinimumSize(200, 80);

    QSizePolicy buttonSizePolicy = button->sizePolicy();
    buttonSizePolicy.setHorizontalPolicy(QSizePolicy::Fixed);
    buttonSizePolicy.setVerticalPolicy(QSizePolicy::Fixed);
    button->setSizePolicy(buttonSizePolicy);

    QColor startColors[] = {
        QColor(241, 158, 143), // Svetlo roze
        QColor(36, 182, 206),  // Svetlo zelena
        QColor(43, 199, 244),  // Svetlo plava
        QColor(144, 128, 251), // Svetlo ljubicasta
    };

    QColor endColors[] = {
        QColor(241, 114, 140), // Tamnije roze
        QColor(36, 182, 206),  // Tamnije zelena
        QColor(112, 132, 245), // Tamnije plava
        QColor(225, 131, 236), // Tamnije ljubicasta
    };

    int randomIndex = QRandomGenerator::global()->bounded(4);

    QColor startColor = startColors[randomIndex];
    QColor endColor   = endColors[randomIndex];

    button->setStyleSheet(QString("QPushButton {"
                                  "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
                                  "stop:0 %1, stop:1 %2);"
                                  "color: white;"
                                  "font-size: 30px;"
                                  "font-weight: bold;"
                                  "}")
                              .arg(startColor.name(), endColor.name()));
    auto *con = new TContainer(_targetParent, spawnPoint, button, _parrentLayout);
    par->addObject(con);

    this->close();
}
