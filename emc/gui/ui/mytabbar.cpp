#include "mytabbar.h"

myTabBar::myTabBar(QWidget *parent/* = NULL*/):QTabBar(parent)
{
    //                   "QTabBar::tab:selected {background-color: yellow;}  "
            setFocusPolicy(Qt::NoFocus);

        setIconSize(QSize(80, 80));
       setStyleSheet(

                   "QTabBar::tab:selected {\
                    background-color: qlineargradient(spread:pad, x1:0.493, y1:1, x2:0.489, y2:0, stop:0 rgba(0, 170, 127, 125), stop:0.986667 rgba(85, 255, 0, 150));\
                    border: 1px solid rgb(190, 190, 190);\
                    border-top-left-radius: 4px;\
                    border-top-right-radius: 5px;\
                    max-height: 0.8em;\
                    min-width: 2em;\
                    padding: 8px;\
                    margin-left: -1px;\
                    margin-right: -1px;\
                   }"
                     );

}
myTabBar::~myTabBar()
{

}
QSize myTabBar::tabSizeHint(int) const
{
      return QSize(125, 48);
}

void myTabBar::mouseMoveEvent(QMouseEvent *e)
{
    int index = this->currentIndex();

    if (m_isDrag)
    {
        QPoint Point = e->pos();
        //拖动的垂直距离大于Bar的高度的话就新建一个窗口，这个应该比较好理解啊
          if (qAbs(m_mousePressPoint.y() - Point.y()) > this->height())
          {
              emit starDragTab(index);
          }
      }
      QTabBar::mouseMoveEvent(e);
  }

  void myTabBar::mousePressEvent(QMouseEvent *e)
  {
      if (e->button() == Qt::LeftButton)
      {
          m_isDrag = true;
          m_mousePressPoint = e->pos();
      }
      QTabBar::mousePressEvent(e);
  }

  void myTabBar::mouseReleaseEvent(QMouseEvent *e)
  {
      if (e->button() == Qt::LeftButton)
      {
          m_mouseReleasePoint = e->pos();
          if (qAbs(m_mousePressPoint.y() - m_mouseReleasePoint.y()) > this->height())
          {
              emit endDragTab();
          }
      }
      QTabBar::mouseReleaseEvent(e);
  }

  void myTabBar::paintEvent(QPaintEvent *)
  {
      int tabnum=count();
      for (int index = 0; index < tabnum; index++)
      {
          QStylePainter p(this);
          QStyleOptionTabV3 tab;
          initStyleOption(&tab, index);

          QIcon tempIcon = tab.icon;
          QString tempText = tab.text;

          tab.icon = QIcon();
          tab.text = QString();

          p.drawControl(QStyle::CE_TabBarTab, tab);

          QSize(80, 30);
          QPainter painter;
          painter.begin(this);

          QRect tabrect = tabRect(index);
          tabrect.adjust(0, 8, 0, -8);
          painter.drawText(tabrect, Qt::AlignBottom | Qt::AlignHCenter, tempText);
          tempIcon.paint(&painter, 0, tabrect.top(), tab.iconSize.width(), tab.iconSize.height(), Qt::AlignTop | Qt::AlignHCenter);
          painter.end();
      }

  }

//------------------------------------------

  CMyTabWidget::CMyTabWidget(QWidget *parent/* = NULL*/):QTabWidget(parent)
  {
      this->setAcceptDrops(true);
      setMouseTracking(true);
      m_pTabBar = new myTabBar(this);
      m_pTabBar->setMovable(true);
      setTabBar(m_pTabBar);               //设置自定义的Bar给TabWidget
      m_pDragWidget = NULL;
      connect(m_pTabBar,SIGNAL(starDragTab(int)),this,SLOT(starDrag(int)));
      connect(m_pTabBar,SIGNAL(endDragTab()),this,SLOT(endDrag()));
  }
  CMyTabWidget::~CMyTabWidget()
  {

  }
  void CMyTabWidget::starDrag(int index)
  {
      m_dragTabLabel = this->tabText(index);
      m_pDragWidget  = this->widget(index);
      m_dragIndex    = index;
      QPixmap pix;
      //pix = QPixmap::grabWidget(m_pDragWidget);
      pix = QPixmap::grabWindow(this->winId());
      if (pix.isNull())
      {
          int i;
      }
      QMimeData *mimeData = new QMimeData;
      //mimeData->setText("drag tab");
      QDrag *drag = new QDrag(this);
      drag->setMimeData(mimeData);
      drag->setPixmap(pix.scaled(QSize(200,200)));    //这里主要是想想QQ一样，可以拖动的过程显示被拖动窗口的图片。但是效果还不是很好
      drag->exec();
  }

  void CMyTabWidget::endDrag()
  {
      CMyTabWidget *pWidget = new CMyTabWidget(NULL);
      if (m_pDragWidget)
      {
          //新建一个独立的窗口
        removeTab(m_dragIndex);
        pWidget->addTab(m_pDragWidget,m_dragTabLabel);
        pWidget->show();
      }
}
