#ifndef LAYOUTOVERLAY_H
#define LAYOUTOVERLAY_H

#include <QWidget>
#include <QtMath>

#define MAX(a, b) a>b?a:b

class LayoutOverlay : public QWidget
{
public:
    explicit LayoutOverlay(QWidget * parent = 0) : QWidget(parent)
    {
        if (parent)
        {
            parent->installEventFilter(this);
            raise();
        }
    }
protected:
    //! Catches resize and child events from the parent widget
    bool eventFilter(QObject * obj, QEvent * ev)
    {
        if (obj == parent())
        {
            if (ev->type() == QEvent::Resize)
            {
                QResizeEvent * rev = static_cast<QResizeEvent*>(ev);
                resize(rev->size());
            }
            else if (ev->type() == QEvent::ChildAdded)
            {
                raise();
            }
        }
        return QWidget::eventFilter(obj, ev);
    }
    //! Tracks parent widget changes
    bool event(QEvent* ev) {
        if (ev->type() == QEvent::ParentAboutToChange)
        {
            if (parent()) parent()->removeEventFilter(this);
        }
        else if (ev->type() == QEvent::ParentChange)
        {
            if (parent()) {
                parent()->installEventFilter(this);
                raise();
            }
        }
        return QWidget::event(ev);
    }
};

class LoadingOverlay : public LayoutOverlay
{
public:
    QPainterPath pint;
    QColor color;
    int w, h, lastw, lasth;
    LoadingOverlay(QWidget * parent = 0, QPainterPath * pp = 0, QColor *color = 0) : LayoutOverlay(parent)
    {
        setAttribute(Qt::WA_TranslucentBackground);
        pint.addPath(*pp);
        w = lastw = parent->width();
        h = lasth = parent->height();
        this->color.setRgb(color->rgb());
    }

protected:

    void paintEvent(QPaintEvent *)
    {
        QPainter p(this);
        p.setPen(QPen(color, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        p.setRenderHint(QPainter::Antialiasing);
        QTransform transform;
        w = this->width();
        h = this->height();
        transform.scale(lastw-w<0?((float)1+(qFabs(lastw-w))/lastw):((float)1-(qFabs(lastw-w))/lastw),
                        lasth-h<0?((float)1+(qFabs(lasth-h))/lasth):((float)1-(qFabs(lasth-h))/lasth));
        p.setTransform(transform);
        p.drawPath(pint);
    }
};

#endif // LAYOUTOVERLAY_H
