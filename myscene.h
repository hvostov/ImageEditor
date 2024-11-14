#ifndef MYSCENE_H
#define MYSCENE_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainterPath>
#include <QGraphicsPathItem>
#include <QPainter>
#include <QGraphicsPixmapItem>
#include <QtGlobal>
#include <mypathitem.h>
#include <QDebug>
#include <QThread>
#include <QVector>
#include <QLine>
#include <QMap>
#include <algorithm>

class MyScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit MyScene(QObject *parent = nullptr);
    void my_addItem(const QPixmap pixmap);
    ~MyScene();

public slots:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void attributestClear();
private slots:
    void my_selectionChanged();

private:
    void makeClip(MyPathItem* pathItem, QPainterPath painterPath);
    void drawPicInClip(const QPainterPath& painterPath, MyPathItem* parent);
    void clipFromPic(const QPainterPath &painterPath);
    void showHideRects(MyPathItem *item, bool showHide);
    void reDraw(MyPathItem* pathItem);
    bool clipRectSelected(MyPathItem* pathItem);
    bool clipRectContains(MyPathItem* pathItem, QPointF point);
    void drawLine(QLineF &line, QPointF p2);
private:
    QGraphicsPixmapItem *pic = nullptr;
    bool nDrawAllowed = false;
    QVector<QPointF> vertices{};
    QPointF prev{};
    QVector<MyPathItem*> clips{};
    QLineF prev_line{};
    QGraphicsLineItem* cur_line{};
    bool startDrawingLine = false;
};

#endif // MYSCENE_H
