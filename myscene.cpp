#include "myscene.h"

MyScene::MyScene(QObject *parent)
    : QGraphicsScene{parent}
{
}

void MyScene::my_addItem(const QPixmap pixmap)
{
    pic = new QGraphicsPixmapItem(pixmap);
    this->addItem(pic);
    cur_line = new QGraphicsLineItem();
}

MyScene::~MyScene()
{
    delete cur_line;
}

void MyScene::makeClip(MyPathItem *pathItem, QPainterPath painterPath)
{
    pathItem->setPath(painterPath);
        this->addItem(pathItem);
        pathItem->setPos(0,0);
        pathItem->oldPos = pathItem->scenePos();
        drawPicInClip(painterPath, pathItem);
        clips.push_back(pathItem);
        this->update(this->itemsBoundingRect());
        QPointF lastPoint = vertices.back();
        auto iter = std::remove_if(vertices.begin()+1, vertices.end(), [lastPoint](auto it){return it == lastPoint;});
        vertices.erase(iter, vertices.end());
        for(int i = 0; i < vertices.size(); ++i) {
                QGraphicsRectItem *rect = new QGraphicsRectItem(vertices[i].x()-5,vertices[i].y()-5, 10,10, pathItem);
                rect->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
                rect->setPen(QPen(Qt::red));
                rect->setBrush(QBrush(Qt::red));
                rect->setZValue(1);
                rect->hide();
                pathItem->rects.push_back(rect);
                pathItem->vertMap[rect] = {rect, QPointF(vertices[i].x(), vertices[i].y())};
            }
        vertices.clear();
        clipFromPic(painterPath);
}

void MyScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << this->items().size() <<"ITEMS";
    foreach(MyPathItem *clip, clips) {
            nDrawAllowed = false;
        if((clip->isSelected() || clip->mapToScene(clip->shape()).contains(event->scenePos()))  || clipRectSelected(clip) || clipRectContains(clip, event->scenePos()) ){
            nDrawAllowed = true;
            break;
        }
    }
    if(!nDrawAllowed) {
        QPen pen;
        pen.setWidth(1);
        pen.setColor(Qt::black);
        if(event->button() == Qt::LeftButton) {
            if(vertices.size() > 0) {
                prev_line.setPoints(prev, event->scenePos());
                cur_line->setLine(prev_line);
                cur_line->setPen(pen);
                this->addLine(prev.x(), prev.y(), event->scenePos().x(), event->scenePos().y(), pen);
            }
            startDrawingLine = true;
            prev = event->scenePos();
            vertices.push_back(prev);
        }

        if(event->button() == Qt::RightButton) {
            startDrawingLine = false;
            if(vertices.size() > 2) {
                this->addLine(prev.x(), prev.y(), vertices[0].x(), vertices[0].y(), pen);
                vertices.push_back(vertices[0]);

                QPolygonF poly;
                foreach(QPointF p, vertices) {
                    poly << p;
                }

                QPainterPath painterPath{};
                painterPath.addPolygon(poly);
                QList<QGraphicsItem*> itemList = this->items();
                foreach(QGraphicsItem* item, itemList) {
                    QGraphicsLineItem *line_at = qgraphicsitem_cast<QGraphicsLineItem *>(item);
                    if (line_at == nullptr)
                            continue;
                        this->removeItem(item);
                }
            MyPathItem *pathItem = new MyPathItem(vertices);
            makeClip(pathItem, painterPath);
            }
            else {
                vertices.clear();
                QList<QGraphicsItem*> itemList = this->items();
                foreach(QGraphicsItem* item, itemList) {
                    QGraphicsLineItem *line_at = qgraphicsitem_cast<QGraphicsLineItem *>(item);
                    if (line_at == nullptr)
                            continue;
                        this->removeItem(item);
                }
            }
        }

    }
    QGraphicsScene::mousePressEvent(event);
}

void MyScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(startDrawingLine) {
        drawLine(prev_line, event->scenePos());
    }
    QGraphicsScene::mouseMoveEvent(event);
}

void MyScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton) {
        foreach(MyPathItem *clip, clips) {
            foreach(QGraphicsRectItem *rect, clip->rects) {
                if (rect->isSelected()) {
                    clip->setNewPoint(clip->vertMap[rect].second,
                                      rect->mapToScene(rect->boundingRect()).boundingRect().center(), rect);
                    reDraw(clip);
                    break;
                }
                else if (clip->isSelected() || clipRectSelected(clip)) {
                    showHideRects(clip, true);
                }
                else {
                    showHideRects(clip,false);
                }
            }
        }
    }
    QGraphicsScene::mouseReleaseEvent(event);
}

void MyScene::my_selectionChanged()
{
    foreach(MyPathItem *clip, clips) {
        if(clip->isSelected()) {
            showHideRects(qgraphicsitem_cast<MyPathItem*>(clip), true);
            vertices.clear();
        }
    }
}

void MyScene::attributestClear()
{
    vertices.clear();
    clips.clear();
}


void MyScene::showHideRects(MyPathItem *item, bool showHide)
{
    if(showHide) {
        foreach(QGraphicsRectItem *rect, item->rects) {
            rect->show();
        }
    }
    else {
        foreach(QGraphicsRectItem *rect, item->rects) {
            QGraphicsRectItem* rect_ = qgraphicsitem_cast<QGraphicsRectItem*>(rect);
            if(rect_) {
               rect->hide();
            }

        }
    }
}

void MyScene::reDraw(MyPathItem *pathItem)
{
    drawPicInClip(pathItem->path(), pathItem);
    clipFromPic(pathItem->path());
}

bool MyScene::clipRectSelected(MyPathItem *pathItem)
{
    if(pathItem && pathItem->rects.size() > 0) {
    foreach(QGraphicsRectItem* rect, pathItem->rects) {
        if(rect->isSelected())
        return true;
    }
    }
    return false;
}

bool MyScene::clipRectContains(MyPathItem *pathItem, QPointF point)
{
    if(pathItem && pathItem->rects.size() > 0) {
    foreach(QGraphicsRectItem* rect, pathItem->rects) {
        if(rect->contains(point))
        return true;
    }
    }
    return false;
}

void MyScene::drawLine(QLineF &line, QPointF p2)
{
    line.setPoints(prev, p2);
    this->removeItem(cur_line);
    cur_line->setLine(line);
    this->addItem(cur_line);
}

void MyScene::drawPicInClip(const QPainterPath &painterPath, MyPathItem *parent)
{
    QPixmap pixmap = pic->pixmap();
    QPixmap pixmap1 = parent->pixMapItem->pixmap();
    QPixmap pixmapForDrawing = pic->pixmap();
    pixmapForDrawing.fill(Qt::transparent);
    QPainter painter(&pixmapForDrawing);
    QPainterPath painterPathTmp{parent->pixMapItem->shape()};
    QPainterPath substracted = painterPath.subtracted(painterPathTmp);
    painter.setClipPath(substracted);
    painter.drawPixmap(0,0,pixmap);
    painter.end();

    painter.begin(&pixmapForDrawing);
    painter.drawPixmap(0,0,pixmap1);
    painter.end();

    QPixmap pixmapForDrawing1 = pixmapForDrawing;
    pixmapForDrawing1.fill(Qt::transparent);
    painter.begin(&pixmapForDrawing1);
    painter.setClipPath(painterPath);
    painter.drawPixmap(0,0,pixmapForDrawing);
    painter.end();

    parent->pixMapItem->setPixmap(pixmapForDrawing1);
}

void MyScene::clipFromPic(const QPainterPath &painterPath)
{
    QPainterPath painterPathWhole{};
    painterPathWhole.addRect(pic->boundingRect());
    QPainterPath substracted = painterPathWhole.subtracted(painterPath);

    QPixmap pixmapForDrawing = pic->pixmap();
    pixmapForDrawing.fill(Qt::transparent);
    QPainter painter(&pixmapForDrawing);

    painter.setClipPath(substracted);
    painter.drawPixmap(0,0,pic->pixmap());
    painter.end();

    pic->setPixmap(pixmapForDrawing);
    this->update(this->itemsBoundingRect());
}
