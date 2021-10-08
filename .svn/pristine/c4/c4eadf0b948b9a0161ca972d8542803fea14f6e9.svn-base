#ifndef MYQLABEL_H
#define MYQLABEL_H

#include <QObject>
#include <QWidget>

#include <QLabel>
#include <QEvent>
#include <QDebug>

class ClickableQLabel : public QLabel
{
    Q_OBJECT
public:
    ClickableQLabel(QWidget* parent = nullptr);
    virtual ~ClickableQLabel();

    bool isChecked() {return selected;}

private:
    bool selected;

protected:
    virtual bool event(QEvent *myEvent); // This method will give all kind of events on Label Widget

signals:
    void sendLabelClicked();       // Signal to emit

};

#endif // MYQLABEL_H
