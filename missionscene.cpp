#include "missionscene.h"

#include <QFontMetrics>
#include <QPainter>
#include <QDebug>
#include <QGraphicsView>
#include <QtMath>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>

int ActionItem::m_Center = 0;

ActionItem::ActionItem( QGraphicsItem *parent)
    : QGraphicsWidget(parent)
{
    setAcceptHoverEvents(true);
    setCacheMode(DeviceCoordinateCache);

    QFont font = this->font();
    font.setPointSize(20);
    this->setFont(font);

    m_Pos = "0.000,0.000";
    m_index = 0;

    m_values.insert("任务描述", "这是一个测试的任务描述");
    m_values.insert("key1", "valuevaluevaluevaluevaluevalue");
    m_values.insert("key2", "valuevaluevaluevaluevaluevalue");
   // m_values.insert("key3", "valuevaluevaluevaluevaluevalue");
}

QRectF ActionItem::boundingRect() const
{
    int width = this->scene()->width();
    if(scene() && scene()->views().size())
        width = scene()->views().first()->width();
    int offset = 5;

    QFontMetrics metrics( this->font() );
    int fontHeight = metrics.height();
    float height = 0.0f;
    float tmpHeight = 0.0f;

    //index
    float msgWidth = metrics.width("ID:"+QString::number(m_index));
    tmpHeight = qCeil(msgWidth/(width-offset*2)) * fontHeight + metrics.leading()*5;
    height+=tmpHeight;

    //pos
    msgWidth = metrics.width("坐标:"+m_Pos);
    tmpHeight = qCeil(msgWidth/(width-offset*2)) * fontHeight + metrics.leading()*5;
    height+=tmpHeight;

    //params
    if(m_index == m_Center){
        foreach (QString var, m_values.keys()) {
            QString showMsg = var+":"+m_values.value(var);
            msgWidth = metrics.width(showMsg);
            tmpHeight = qCeil(msgWidth/(width-offset*2)) * fontHeight + metrics.leading()*5;
            height+=tmpHeight;
        }
    }

    return QRectF(-width/2, -height/2, width, height).adjusted(offset,0,-offset,0);
}

QPainterPath ActionItem::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}

float ActionItem::getFontHeight(float width, QString  showMsg)
{
    QFontMetrics metrics( this->font() );
    int fontHeight = metrics.height();

    float msgWidth = metrics.width(showMsg);
    return qCeil(msgWidth/(width)) * fontHeight + metrics.leading()*5;
}

void ActionItem::setIndex(int index)
{
    this->m_index = index;
}

void ActionItem::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
    qDebug() << m_index;
}

void ActionItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
//    bool down = option->state & QStyle::State_Sunken;
    painter->setRenderHint(QPainter::Antialiasing);
    QRectF r = boundingRect();
    QLinearGradient grad(r.topLeft(), r.bottomRight());
    grad.setColorAt(0, /*Qt::lightGray*/QColor(73,158,216).darker(100));
    grad.setColorAt(1, /*Qt::darkGray*/QColor(163,216,236).darker(100));
    painter->setPen(Qt::NoPen);
    painter->setBrush(grad);
//    painter->drawRect(r);
    painter->drawRoundRect(r,3,3);

    int offset = 5;
    r.adjust(offset,0,-offset,0);
    int lastTop = r.top();
    if(m_index == m_Center)
        painter->setPen(Qt::red);
    else
        painter->setPen(Qt::darkRed);
    painter->setFont(this->font());


    //id
    QString showMsg = "ID:"+QString::number(m_index);

    QRectF textRect;
    textRect.setLeft(r.left());
    textRect.setTop(lastTop);
    textRect.setWidth(r.width());
    textRect.setHeight(  getFontHeight(r.width(), showMsg) );
    QTextOption opt;
    opt.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    opt.setAlignment(Qt::AlignVCenter);
    painter->drawText(textRect, showMsg, opt);
    lastTop=textRect.bottom();

    //pos
    showMsg = "坐标:"+m_Pos;
    textRect.setLeft(r.left());
    textRect.setTop(lastTop);
    textRect.setWidth(r.width());
    textRect.setHeight(getFontHeight(r.width(), showMsg));
    painter->drawText(textRect, showMsg, opt);
    lastTop=textRect.bottom();

    if(m_index == m_Center){
        //params
        foreach (QString var, m_values.keys()) {
            showMsg = var+":"+m_values.value(var);
            textRect.setLeft(r.left());
            textRect.setTop(lastTop);
            textRect.setWidth(r.width());
            textRect.setHeight(getFontHeight(r.width(), showMsg));
            painter->drawText(textRect, showMsg, opt);

            lastTop=textRect.bottom();
        }
    }
}

MissionScene::MissionScene(QObject *parent) : QGraphicsScene(parent)
{
    int sWidth = 100;
    int sHeight = 600;
    this->setSceneRect(-sWidth/2,-sHeight/2,sWidth,sHeight);
    this->setBackgroundBrush(QColor(0,50,80));

    int sceneHeight = 20;
    for(int i=0; i<20; ++i){
        ActionItem *item = new ActionItem;
        this->addItem(item);
        item->setIndex(i);
        m_items.append(item);
        sceneHeight+=item->boundingRect().height();
    }

    this->setSceneRect(-sWidth/2,-sceneHeight,sWidth,sceneHeight*2);
}

void MissionScene::setCurrentIndex(int index)
{

    int size = m_items.size();
    if(size <= index){
        return;
    }

    if(ActionItem::m_Center == index){
        return;
    }

    ActionItem::m_Center = index;
    float centerHeight = m_items.at(index)->boundingRect().height();

    QParallelAnimationGroup *ganimate = new QParallelAnimationGroup;

    for(int i=0; i<size; ++i){
        QPropertyAnimation *anim = new QPropertyAnimation(m_items.at(i), "pos");
        anim->setDuration(750 + i * 25);
        anim->setEasingCurve(QEasingCurve::InOutBack);
        ganimate->addAnimation(anim);
        anim->setStartValue(m_items.at(i)->pos());

        int offset = i - index;
        float itemHeight = 0;
        float itemWidth = 0;
        if(offset < 0){
            itemHeight = m_items.at(i)->boundingRect().height();
            itemWidth = ( offset+1 ) * itemHeight - itemHeight/2-centerHeight/2 - 10*qAbs(offset);
        }else if(offset > 0){
            itemHeight = m_items.at(i)->boundingRect().height();
            itemWidth = ( offset-1 ) * itemHeight + itemHeight/2 + centerHeight/2 + 10*qAbs(offset);
            //m_items.at(i)->setPos(0, itemWidth);
        }else{
            //m_items.at(index)->setPos(0,0);
        }
        anim->setEndValue(QPointF(itemHeight/2,itemWidth));
        m_items.at(i)->setOpacity( (size-qAbs(offset))*0.8f/size + 0.2 );
    }

    ganimate->start(QAbstractAnimation::DeleteWhenStopped);
    this->update();
}













