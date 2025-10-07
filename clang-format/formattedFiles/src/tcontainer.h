#ifndef TCONTAINER_H
#define TCONTAINER_H

#include <QMenu>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QWidget>
#include <QtGui>

enum modes { NONE = 0, MOVE = 1, RESIZETL = 2, RESIZET = 3, RESIZETR = 4, RESIZER = 5, RESIZEBR = 6, RESIZEB = 7, RESIZEBL = 8, RESIZEL = 9 };

class TContainer : public QWidget {
    Q_OBJECT

  public:
    TContainer(QWidget *parent, QPoint p, QWidget *cWidget = nullptr, QLayout *l = nullptr);
    ~TContainer() override;
    QWidget *childWidget;
    QMenu *menu;
    QPoint position;
    void setChildWidget(QWidget *cWidget);

  protected:
    int mode;
    QLayout *l;
    void setCursorShape(const QPoint &ePos);
    void focusInEvent(QFocusEvent *) override;
    void focusOutEvent(QFocusEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    bool m_infocus;
    bool m_showMenu;
    bool m_isEditing;
    QWidget *clone();

  signals:
    void inFocus(bool mode);
    void outFocus(bool mode);
    void newGeometry(QRect rect);
};

#endif // TCONTAINER_H
