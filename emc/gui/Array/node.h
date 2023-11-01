#ifndef NODE_H
#define NODE_H

#include <QObject>
#include <QGraphicsItem>

extern int nodesig;

class Node : public QGraphicsItem
{
public:
    explicit Node(QGraphicsItem *parent = 0, QPointF *pos = new QPointF(0,0), double width = 10, double height = 10,int num = 0);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setItemText(QString text = QString("yes")) {
        itemtext = text;
        this->update(boundingRect());
    }

    int nodenum;

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

private:
    double nodewidth,nodeheight;
    QPointF newPos;
    QString itemtext;
};

#endif // NODE_H
