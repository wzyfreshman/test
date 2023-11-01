#include "node.h"
#include "compensation.h"

#include <QtDebug>
#include <QPainter>
#include <QStyleOption>

int nodesig = -1;

Node::Node(QGraphicsItem *parent, QPointF *pos, double width, double height, int num) :
    QGraphicsItem(parent)
{
    newPos.setX(pos->x());
    newPos.setY(pos->y());
    nodewidth = width;
    nodeheight = height;
    nodenum = num;
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    painter->setPen(Qt::DashLine);
    painter->setBrush(Qt::yellow);
    painter->drawRect(newPos.x(),newPos.y(), nodewidth, nodeheight);
    painter->setFont(QFont("SansSerif", 0.07*nodewidth));
    painter->drawText(boundingRect(),itemtext,QTextOption(Qt::AlignHCenter|Qt::AlignVCenter));
}

QRectF Node::boundingRect() const
{
    return QRectF( newPos.x(),newPos.y() , nodewidth, nodeheight);
}

void Node::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
    nodesig = this->nodenum;
}
