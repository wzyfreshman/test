#include <QTabBar>
#include <QTabWidget>
#include <QPoint>
#include <QMimeData>
#include <QDrag>
#include <QMouseEvent>
#include <QStylePainter>
#include <QStyleOptionTabV3>

class myTabBar:public QTabBar
{
    Q_OBJECT
public:
    myTabBar(QWidget *parent = NULL);
    ~myTabBar();
protected:
    void mousePressEvent (QMouseEvent *e);
    void mouseMoveEvent (QMouseEvent *e);
    void mouseReleaseEvent (QMouseEvent *e);
    void paintEvent(QPaintEvent *);
     QSize tabSizeHint(int) const;

 signals:
    void starDragTab(int index);
    void endDragTab();
private:
    void drag();

    bool      m_isDrag;
    QPoint    m_mousePressPoint;
    QPoint    m_mouseReleasePoint;

};

class CMyTabWidget:public QTabWidget
{
    Q_OBJECT

public:
    CMyTabWidget(QWidget *parent = NULL);

    ~CMyTabWidget();



private slots:
    void starDrag(int index);
    void endDrag();


private:
    myTabBar      *m_pTabBar;
    QString        m_dragTabLabel;
    QWidget       *m_pDragWidget;
    int            m_dragIndex;


};
