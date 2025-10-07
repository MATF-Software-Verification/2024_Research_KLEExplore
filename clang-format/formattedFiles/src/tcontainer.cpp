#include "tcontainer.h"
#include <QApplication>
#include <QLabel>
#include <QScrollArea>

TContainer::TContainer(QWidget *parent, QPoint p, QWidget *cWidget, QLayout *l) : QWidget(parent) {

    mode = NONE;
    setAttribute(Qt::WA_DeleteOnClose);
    setChildWidget(cWidget);
    this->setVisible(true);
    this->setAutoFillBackground(false);
    this->setMouseTracking(true);
    this->setFocusPolicy(Qt::ClickFocus);
    this->setFocus();
    this->move(p);

    if (cWidget != nullptr) {
        cWidget->setParent(this);
        cWidget->releaseMouse();
        cWidget->setAttribute(Qt::WA_TransparentForMouseEvents, true);
        l->addWidget(cWidget);
        l->setContentsMargins(0, 0, 0, 0);
    }
    this->setLayout(l);

    m_infocus   = true;
    m_showMenu  = false;
    m_isEditing = true;
    this->installEventFilter(parent);

    this->resize(cWidget->geometry().width(), cWidget->geometry().height());
}

TContainer::~TContainer() {
    delete childWidget;
    delete l;
}

void TContainer::setChildWidget(QWidget *cWidget) {
    if (cWidget != nullptr) {
        childWidget = cWidget;
    }
}

void TContainer::focusInEvent(QFocusEvent * /*event*/) {
    m_infocus = true;
    this->parentWidget()->installEventFilter(this);
    this->parentWidget()->repaint();
    emit inFocus(true);
}

void TContainer::focusOutEvent(QFocusEvent * /*event*/) {
    if (!m_isEditing) {
        return;
    }
    if (m_showMenu) {
        return;
    }
    mode = NONE;
    emit outFocus(false);
    m_infocus = false;
}

void TContainer::mousePressEvent(QMouseEvent *e) {
    position = QPoint(e->globalX() - geometry().x(), e->globalY() - geometry().y());
    if (!m_isEditing) {
        return;
    }
    if (!m_infocus) {
        return;
    }
    if (!e->buttons() && (Qt::LeftButton != 0u)) {
        setCursorShape(e->pos());
        return;
    }
}

void TContainer::setCursorShape(const QPoint &ePos) {
    const int diff = 15;
    if (
        // Left-Bottom
        ((ePos.y() > y() + height() - diff) && // Bottom
         (ePos.x() < x() + diff)) ||           // Left
        // Right-Bottom
        ((ePos.y() > y() + height() - diff) && // Bottom
         (ePos.x() > x() + width() - diff)) || // Right
        // Left-Top
        ((ePos.y() < y() + diff) &&  // Top
         (ePos.x() < x() + diff)) || // Left
        // Right-Top
        ((ePos.y() < y() + diff) &&         // Top
         (ePos.x() > x() + width() - diff)) // Right
    ) {
        // Left-Bottom
        if ((ePos.y() > y() + height() - diff) && // Bottom
            (ePos.x() < x() + diff))              // Left
        {
            mode = RESIZEBL;
            setCursor(QCursor(Qt::SizeBDiagCursor));
        }
        // Right-Bottom
        if ((ePos.y() > y() + height() - diff) && // Bottom
            (ePos.x() > x() + width() - diff))    // Right
        {
            mode = RESIZEBR;
            setCursor(QCursor(Qt::SizeFDiagCursor));
        }
        // Left-Top
        if ((ePos.y() < y() + diff) && // Top
            (ePos.x() < x() + diff))   // Left
        {
            mode = RESIZETL;
            setCursor(QCursor(Qt::SizeFDiagCursor));
        }
        // Right-Top
        if ((ePos.y() < y() + diff) &&         // Top
            (ePos.x() > x() + width() - diff)) // Right
        {
            mode = RESIZETR;
            setCursor(QCursor(Qt::SizeBDiagCursor));
        }
    }
    // check cursor horizontal position
    else if ((ePos.x() < x() + diff) ||           // Left
             ((ePos.x() > x() + width() - diff))) // Right
    {
        if (ePos.x() < x() + diff) { // Left
            setCursor(QCursor(Qt::SizeHorCursor));
            mode = RESIZEL;
        } else { // Right
            setCursor(QCursor(Qt::SizeHorCursor));
            mode = RESIZER;
        }
    }
    // check cursor vertical position
    else if (((ePos.y() > y() + height() - diff)) || // Bottom
             (ePos.y() < y() + diff))                // Top
    {
        if (ePos.y() < y() + diff) { // Top
            setCursor(QCursor(Qt::SizeVerCursor));
            mode = RESIZET;
        } else { // Bottom
            setCursor(QCursor(Qt::SizeVerCursor));
            mode = RESIZEB;
        }
    } else {
        setCursor(QCursor(Qt::OpenHandCursor));
        mode = MOVE;
    }
}

void TContainer::mouseReleaseEvent(QMouseEvent *e) {
    QWidget::mouseReleaseEvent(e);
}

void TContainer::mouseMoveEvent(QMouseEvent *e) {
    QWidget::mouseMoveEvent(e);
    if (!m_isEditing) {
        return;
    }
    if (!m_infocus) {
        return;
    }
    if (!e->buttons() && (Qt::LeftButton != 0u)) {
        QPoint p = QPoint(e->x() + geometry().x(), e->y() + geometry().y());
        setCursorShape(p);
        return;
    }

    if ((mode == MOVE || mode == NONE) && (e->buttons() != 0u) && (Qt::LeftButton != 0u)) {
        QPoint toMove = e->globalPos() - position;
        if (toMove.x() < 0 + 5) {
            return;
        }
        if (toMove.y() < 0 + 5) {
            return;
        }
        if (toMove.x() > this->parentWidget()->width() - this->width() - 5) {
            return;
        }
        if (toMove.y() > this->parentWidget()->height() - this->height() - 5) {
            return;
        }
        move(toMove);

        emit newGeometry(this->geometry());
        this->parentWidget()->repaint();
        return;
    }
    if ((mode != MOVE) && (e->buttons() != 0u) && (Qt::LeftButton != 0u)) {
        switch (mode) {
        case RESIZETL: { // Left-Top
            int newwidth  = e->globalX() - position.x() - geometry().x();
            int newheight = e->globalY() - position.y() - geometry().y();
            QPoint toMove = e->globalPos() - position;
            this->resize(this->geometry().width() - newwidth, this->geometry().height() - newheight);
            move(toMove.x(), toMove.y());
            break;
        }
        case RESIZETR: { // Right-Top
            int newheight = e->globalY() - position.y() - geometry().y();
            QPoint toMove = e->globalPos() - position;
            this->resize(e->x(), this->geometry().height() - newheight);
            move(this->x(), toMove.y());
            break;
        }
        case RESIZEBL: { // Left-Bottom
            int newwidth  = e->globalX() - position.x() - geometry().x();
            QPoint toMove = e->globalPos() - position;
            this->resize(this->geometry().width() - newwidth, e->y());
            move(toMove.x(), this->y());
            break;
        }
        case RESIZEB: { // Bottom
            this->resize(width(), e->y());
            break;
        }
        case RESIZEL: { // Left
            int newwidth  = e->globalX() - position.x() - geometry().x();
            QPoint toMove = e->globalPos() - position;
            this->resize(this->geometry().width() - newwidth, height());
            move(toMove.x(), this->y());
            break;
        }
        case RESIZET: { // Top
            int newheight = e->globalY() - position.y() - geometry().y();
            QPoint toMove = e->globalPos() - position;
            this->resize(width(), this->geometry().height() - newheight);
            move(this->x(), toMove.y());
            break;
        }
        case RESIZER: { // Right
            this->resize(e->x(), height());
            break;
        }
        case RESIZEBR: { // Right-Bottom
            this->resize(e->x(), e->y());
            break;
        }
        }
        this->parentWidget()->repaint();
    }
    emit newGeometry(this->geometry());
}
