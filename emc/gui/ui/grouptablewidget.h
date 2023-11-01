#ifndef GROUPTABLEWIDGET_H
#define GROUPTABLEWIDGET_H

#include <QTableWidget>
#include <QEvent>
#include <QLabel>
#include <QStyledItemDelegate>
#include <QKeyEvent>

class GroupTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit GroupTableWidget(QWidget *parent = 0);

public:
    void addnewgroup(QString gpname,int nrow);

protected:
    void setRowColor(int row, QColor color);
    QLabel *addgroup(QString gpname,int grouptype);

signals:

    void rowEntered(int row, int column);
    void rowDbClicked(int row, int column);

public slots:
    void mycellEntered(int row, int column);
private:
    QTableWidget *tableWidget;
    QColor defaultBkColor;
    int previousColorRow;
};

class NoFocusDelegate:public QStyledItemDelegate
{
public:
    explicit NoFocusDelegate(QWidget* parent=0) : QStyledItemDelegate(parent)
    {
    }
protected:
    void paint(QPainter*painter, const QStyleOptionViewItem & option, const QModelIndex&index) const
     {
       QStyleOptionViewItem itemOption(option);
       if (itemOption.state & QStyle::State_HasFocus)
       {
          itemOption.state = itemOption.state ^ QStyle::State_HasFocus;
       }

       QStyledItemDelegate::paint(painter, itemOption, index);
     }
};

#endif // GROUPTABLEWIDGET_H
