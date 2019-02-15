#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QKeyEvent>
#include <QFileDialog>
#include <QDesktopServices>
#include <QDebug>
#include <QTimer>
#include <QScrollBar>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    animation_show=new QPropertyAnimation(ui->widget_2,"maximumHeight");
//    animation_show->setDuration(1000);
//    animation_show->setEasingCurve(QEasingCurve::InBounce);
//    animation_show->setStartValue(0);
//    animation_show->setEndValue(480);

//    animation_hide=new QPropertyAnimation(ui->widget_2,"maximumHeight");
//    animation_hide->setDuration(1000);
//    animation_hide->setEasingCurve(QEasingCurve::OutBounce);
//    animation_hide->setStartValue(480);
//    animation_hide->setEndValue(0);

    m_scene = new MissionScene();
    ui->graphicsView->setScene(m_scene);
    ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, [=](){
        m_scene->setCurrentIndex(qrand()%9);
    });
    m_timer->start(1500);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *)
{
    m_scene->setCurrentIndex(qrand()%9);
    //ui->graphicsView->fitInView(m_scene->sceneRect());
}

void MainWindow::keyPressEvent(QKeyEvent *key)
{
    static int index = 0;
    if(key->key() == Qt::Key_W){
//        animation_show->start();
        m_scene->setCurrentIndex(0);
        animation_show->setDirection(QAbstractAnimation::Forward);
    }else if(key->key() == Qt::Key_S){
        animation_show->setDirection(QAbstractAnimation::Backward);
        m_scene->setCurrentIndex(qrand()%20);
//        animation_hide->start();
    }else if(key->key() == Qt::Key_X){
        if(index == 9){
            index=0;
        }
        m_scene->setCurrentIndex(index++);

//        animation_hide->start();
    }
   // animation_show->start();
}

//void MainWindow::on_pushButton_clicked()
//{
//    QString filepath = QFileDialog::getOpenFileName();
//    if(!filepath.isEmpty()){
//        QDesktopServices::openUrl(QUrl::fromLocalFile(filepath));
//    }
//}
