#ifndef MISSIONSCENE_H
#define MISSIONSCENE_H

#include <QGraphicsScene>
#include <QGraphicsWidget>

#include <QParallelAnimationGroup>

class ActionItem : public QGraphicsWidget
{
    Q_OBJECT

public:
    ActionItem(QGraphicsItem *parent = 0);
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent*);
    void setIndex(int);
    static int m_Center;


private:
    QMap<QString, QString> m_values;
    QVector<float> m_textHeight;
    QString m_Pos;
    int m_index;

    float getFontHeight(float, QString);

};

class MissionScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit MissionScene(QObject *parent = Q_NULLPTR);
    void setCurrentIndex(int);

private:
    QList<ActionItem*> m_items;

//    QParallelAnimationGroup *m_animate;

signals:

public slots:
};

#endif // MISSIONSCENE_H
