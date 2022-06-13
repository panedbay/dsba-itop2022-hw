#include "dialog.h"
#include "ui_dialog.h"
#include <iostream>

#include <QGraphicsScene>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    drawLogo();
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::drawLogo()
{
    scene = new QGraphicsScene();

    // Dynamic sizes
    int heightF = ui->graphicsView->height() * 8 / 10;
    int widthF = ui->graphicsView->width() * 2 / 10;

    QPolygon polyF1 = letterF1(widthF, heightF);
    QPolygon polyF2 = letterF2(widthF, heightF);
    QPolygon polyA = letterA(widthF, heightF);
    QPolygon polyBridge = letterBridge(widthF, heightF);

    QPen pen(Qt::GlobalColor::blue);
    QPen bridgePen(Qt::GlobalColor::red);
    QBrush brush(Qt::GlobalColor::red);

    scene->addPolygon(polyF1, pen, brush); // first F
    scene->addRect(21 * widthF / 16, 0, widthF / 3, -heightF, pen, brush); // I
    scene->addPolygon(polyF2, pen, brush); // second F
    scene->addPolygon(polyA, pen, brush); // A
    scene->addPolygon(polyBridge, bridgePen, brush); // A letter bridge

    ui->graphicsView->setScene(scene);
}

QPolygon Dialog::letterF1(int w, int h)
{
    QPolygon poly;
    poly << QPoint(0, 0)
           << QPoint(0, -h) << QPoint(w, -h)
           << QPoint(w, -h * 5 / 6) << QPoint(w / 2, -h * 5 / 6)
           << QPoint(w / 2, -h / 2) << QPoint(w, -h / 2)
           << QPoint(w, -h / 3) << QPoint(w / 2, -h / 3)
           << QPoint(w / 2, 0);
    return poly;
}

QPolygon Dialog::letterBridge(int w, int h)
{
    QPolygon poly;
    poly << QPoint(15.06 * w / 4, -h / 3)
               << QPoint(16.95 * w / 4, -h / 3)
               << QPoint(17.3 * w / 4, -h / 4)
               << QPoint(14.8 * w / 4, -h / 4);
    return poly;
}

QPolygon Dialog::letterF2(int w, int h)
{
    QPolygon poly;
    int pad = w * 2;
    poly << QPoint(0 + pad, 0)
           << QPoint(0 + pad, -h) << QPoint(w + pad, -h)
           << QPoint(w + pad, -h * 5 / 6) << QPoint(pad + w / 2, -h * 5 / 6)
           << QPoint(pad + w/ 2, -h / 2) << QPoint(w + pad, -h / 2)
           << QPoint(w + pad, -h / 3) << QPoint(pad + w / 2, -h / 3)
           << QPoint(pad + w / 2, 0);
    return poly;
}

QPolygon Dialog::letterA(int w, int h)
{
    QPolygon poly;
    poly << QPoint(13 * w / 4, 0) << QPoint(4 * w, -h)
          << QPoint(19 * w / 4, 0) << QPoint(18 * w / 4, 0)
          << QPoint(16 * w / 4, -h * 10 / 16) << QPoint(14 * w / 4, 0);
    return poly;
}

void Dialog::resizeEvent(QResizeEvent *)
{
    drawLogo();
}
