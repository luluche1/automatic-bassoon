#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPropertyAnimation>

#include "missionscene.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent*);
    void resizeEvent(QResizeEvent*);

private slots:


private:
    Ui::MainWindow *ui;
    QPropertyAnimation* animation_show;
    QPropertyAnimation* animation_hide;

    MissionScene *m_scene;
    QTimer *m_timer;
};

#endif // MAINWINDOW_H
