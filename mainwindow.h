#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QPushButton>
#include <QFileDialog>
#include <QStandardPaths>
#include <QLayout>
#include <QDebug>
#include <QMouseEvent>
#include "myscene.h"
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    MyScene *scene;
    QGraphicsView *view;
    QPushButton *loadImageButton;

private slots:
    void resizeEvent(QResizeEvent *event) override;
    void loadImageButtonClicked();
    void setImageOnScene(QString pathToImage);
    void resizePix();

signals:
    void havePath(QString pathToImage);
    void sizeChanged();
    void sceneAttributesClear();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QPixmap *pixmap = nullptr;
};
#endif // MAINWINDOW_H
