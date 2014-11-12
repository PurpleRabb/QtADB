#ifndef SCREENFORM_H
#define SCREENFORM_H

#include <QWidget>
#include <QMouseEvent>
#include <QGesture>
#include <QGestureEvent>

namespace Ui {
class ScreenForm;
}

class ScreenForm : public QWidget
{
    Q_OBJECT

public:
    explicit ScreenForm(QWidget *parent = 0);
    ~ScreenForm();
    void paintEvent(QPaintEvent *e);
protected:
    void mousePressEvent(QMouseEvent *e=0);

private:
    Ui::ScreenForm *ui;
private slots:
    void back();
    void left();
    void right();
};

#endif // SCREENFORM_H
