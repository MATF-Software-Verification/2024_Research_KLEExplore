#include "fileimportwindow.h"
#include "dynamicui.h"
#include "ui_fileimportwindow.h"

#include <QDebug>
#include <QDragEnterEvent>
#include <QFileDialog>
#include <QMimeData>

FileImportWindow::FileImportWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::FileImportWindow) {
    ui->setupUi(this);
    setAcceptDrops(true);

    menuBar = new QMenuBar(this);
    setMenuBar(menuBar);

    QMenu *fileMenu = menuBar->addMenu("File");

    auto *openAction = new QAction("Open", this);
    connect(openAction, &QAction::triggered, this, &FileImportWindow::openFile);
    fileMenu->addAction(openAction);
}

QString FileImportWindow::fileName() const {
    return _fileName;
}

void FileImportWindow::dragEnterEvent(QDragEnterEvent *e) {
    if (e->mimeData()->hasUrls()) {
        e->acceptProposedAction();
    }
}

void FileImportWindow::dropEvent(QDropEvent *e) {
    foreach (const QUrl &url, e->mimeData()->urls()) {
        QString fileName = url.toLocalFile();
        if (!fileName.endsWith(".txt") && !fileName.endsWith(".csv")) {
            ui->lblError->setText("Wrong format, try another!");
        } else {
            auto *m = new DynamicUI(nullptr, fileName);
            // MainWindow *m = new MainWindow(nullptr, fileName);
            _fileName = fileName;
            m->show();
            this->close();
        }
        // qDebug() << "Dropped file:" << fileName;
    }
}

void FileImportWindow::openFile() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Text Files (*.txt);;CSV Files (*.csv);;All Files (*)"));

    if (!fileName.isEmpty()) {
        auto *m   = new DynamicUI(nullptr, fileName);
        _fileName = fileName;
        m->show();
        this->close();
    }
}

FileImportWindow::~FileImportWindow() {
    delete ui;
}
