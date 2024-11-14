#include "mypathitem.h"

MyPathItem::MyPathItem(QVector<QPointF> v) : QGraphicsPathItem(), vertices{v}
{
    this->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    pixMapItem = new QGraphicsPixmapItem(this);
}

void MyPathItem::setNewPoint(QPointF posPoint, QPointF newPoint, QGraphicsRectItem* rect)
{
        prepareGeometryChange();
        for (int i = 0; i < vertices.size(); ++i) {
            if(vertices[i] == posPoint) {
                vertices[i] = newPoint - delta;
                vertMap[rect] = {rect, newPoint - delta};
            }
        }

        QPainterPath path_{};
        QPolygonF poly{};
        foreach(QPointF p, vertices) {
            poly << p;
        }
        path_.addPolygon(poly);
        this->setPath(path_);
}

void MyPathItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
        if(event->button() == Qt::LeftButton) {
            this->setCursor(QCursor(Qt::ClosedHandCursor));
        }
        QGraphicsItem::mousePressEvent(event);
}

void MyPathItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton) {
        this->setCursor(QCursor(Qt::ArrowCursor));
         delta = this->scenePos() - oldPos;
        foreach(QPointF p, vertices) {
            p += delta;
        }
        foreach(QPair rectItemAndPos, vertMap) {
            rectItemAndPos.second += delta;
        }
        oldPos = this->scenePos() - delta;
    }
    QGraphicsItem::mouseReleaseEvent(event);
}

void MyPathItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
       Q_UNUSED(widget);
        painter->setRenderHint(QPainter::Antialiasing);
       painter->setPen(QPen(option->palette.windowText(), 2));
       painter->drawPath(path());


    if (option->state & QStyle::State_Selected){
        painter->setRenderHint(QPainter::SmoothPixmapTransform);
        QPen pen = QPen(option->palette.windowText(), 2, Qt::SolidLine);
        pen.setColor(Qt::white);
        painter->setPen(pen);
        painter->setBrush(Qt::NoBrush);
        painter->drawPath(path());

        pen.setStyle(Qt::DashLine);
        pen.setColor(Qt::black);
        painter->setPen(pen);
        painter->setBrush(Qt::NoBrush);
        painter->drawPath(path());
    }
}
