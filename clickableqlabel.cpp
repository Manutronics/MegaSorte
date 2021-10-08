#include "clickableqlabel.h"

ClickableQLabel :: ClickableQLabel(QWidget* parent) : QLabel(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    selected = true;
}

ClickableQLabel::~ClickableQLabel()
{

}

bool ClickableQLabel::event(QEvent *myEvent)
{
    switch(myEvent->type())
    {
        case(QEvent::MouseButtonRelease):   // Identify Mouse press Event
        {
            QString style = styleSheet();
            emit sendLabelClicked();
            if(selected)
            {
                style.replace("_on", "_off");
                setStyleSheet(style);
            }
            else
            {
                style.replace("_off", "_on");
                setStyleSheet(style);
            }
            selected = !selected;
            break;
        }
    }
    return QWidget::event(myEvent);
}

