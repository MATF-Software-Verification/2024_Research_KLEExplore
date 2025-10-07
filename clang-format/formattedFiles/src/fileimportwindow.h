#ifndef FILEIMPORTWINDOW_H
#define FILEIMPORTWINDOW_H

#include <QMainWindow>

#include <QMainWindow>
#include <QMenuBar>

namespace Ui {
class FileImportWindow;
}

class FileImportWindow : public QMainWindow {
    Q_OBJECT

  public:
    explicit FileImportWindow(QWidget *parent = nullptr);
    ~FileImportWindow() override;
    void dragEnterEvent(QDragEnterEvent *e) override;
    void dropEvent(QDropEvent *e) override;
    QString fileName() const;

  private slots:
    void openFile();

  private:
    Ui::FileImportWindow *ui;
    QString _fileName;

    QMenuBar *menuBar;
};

#endif // FILEIMPORTWINDOW_H
