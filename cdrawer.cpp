#include "cdrawer.h"

CDrawer::CDrawer(QWidget *parent) : QWidget(parent)
{

}


void CDrawer::paintEvent(QPaintEvent *event)
{
    static QImage *static_image = 0;
    QPainter painter;
}
