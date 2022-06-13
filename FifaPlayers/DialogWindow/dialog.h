#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QGraphicsScene>
#include <QTextStream>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

private:
    Ui::Dialog *ui;
    QGraphicsScene *scene;
    QPolygon letterF1(int w, int h);
    QPolygon letterF2(int w, int h);
    QPolygon letterA(int w, int h);
    QPolygon letterBridge(int w, int h);
    void drawLogo();
    void resizeEvent(QResizeEvent *) override;
};

#endif // DIALOG_H
