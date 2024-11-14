#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    QWidget::setGeometry(500,300,800,600);
    QGridLayout *mainLayout = new QGridLayout(centralWidget());

    scene = new MyScene(this);
    scene->setSceneRect(25,25,450,250);
    view = new QGraphicsView(this);
    view->setGeometry(0,0,500,300);
    view->setScene(scene);
    view->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    mainLayout->addWidget(view,0,0);

    loadImageButton = new QPushButton("Загрузить изображение", this);
    mainLayout->addWidget(loadImageButton,1,0);

    connect(loadImageButton, &QPushButton::clicked, this, &MainWindow::loadImageButtonClicked);
    connect(this, &MainWindow::havePath, this, &MainWindow::setImageOnScene);
    connect(this, &MainWindow::sizeChanged, this, &MainWindow::resizePix);
    connect(this, &MainWindow::sceneAttributesClear, scene, &MyScene::attributestClear);
    view->setMouseTracking(true);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete pixmap;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    emit sizeChanged();
    Q_UNUSED(event);
}

void MainWindow::loadImageButtonClicked()
{
    QStringList list = QStandardPaths::standardLocations(QStandardPaths::DesktopLocation);
    QString pathToImage = list.first();
    pathToImage = QFileDialog::getOpenFileName(this, "Выбрать файл",
                                               pathToImage,
                                               "Images(*.png *.jpg)");
    if(!pathToImage.isEmpty()) {
        emit havePath(pathToImage);
    }
}

void MainWindow::setImageOnScene(QString pathToImage)
{

    pixmap = new QPixmap(pathToImage);
    QPixmap pixmap_tmp = pixmap->scaledToWidth(view->width());
    scene->setSceneRect(0,0,view->width()-25, pixmap_tmp.height());
    scene->my_addItem(pixmap_tmp);
}

void MainWindow::resizePix()
{
    if(!pixmap) return;

    scene->clear();
    emit sceneAttributesClear();
    int height = 0;
    QPixmap pixmap_tmp;
    if(pixmap->width()>view->width()) {
        pixmap_tmp = pixmap->scaledToWidth(view->width());
        height = pixmap_tmp.height() > view->height() ? pixmap_tmp.height() : view->height();
    }
    else {
        pixmap_tmp = *pixmap;
        height = pixmap->height() > view->height() ? pixmap->height() : view->height();
    }
    scene->setSceneRect(0,0,view->width()-25, height-25);
    scene->my_addItem(pixmap_tmp);
}
