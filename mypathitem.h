#ifndef MYPATHITEM_H
#define MYPATHITEM_H

#include <QGraphicsPathItem>
#include <QPointF>
#include <QGraphicsSceneMouseEvent>
#include <QCursor>
#include <QDebug>
#include <QPainter>
#include <QStyleOption>
#include <QVector>
#include <QGraphicsRectItem>
#include <QMap>
#include <QPair>
class MyPathItem : public QGraphicsPathItem
{
public:
    MyPathItem(QVector<QPointF> v);
    void setNewPoint(QPointF posPoint, QPointF newPoint, QGraphicsRectItem* rect);

private:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *) override;

public slots:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QVector<QPointF> vertices{};

public:
    QVector<QGraphicsRectItem*> rects{};
    QMap<QGraphicsRectItem*, QPair<QGraphicsRectItem*, QPointF>> vertMap;
    QPointF oldPos{};
    QPointF delta{};
    QGraphicsPixmapItem* pixMapItem{};
};

#endif // MYPATHITEM_H
