#include "array.h"
#include "ui_array.h"
#include "node.h"
#include "compensation.h"
#include "shcom.hh"
#include "ini_settings.h"

#include <unistd.h>

#include <QtDebug>
#include <QGraphicsScene>
#include <QFile>
#include <QMouseEvent>
#include <QTimer>
#include <QScrollBar>
#include <QMessageBox>



Array::Array(QWidget *parent, double prowidth, double proheight) :
    QWidget(parent),
    ui(new Ui::Array)
{
    ui->setupUi(this);

    itemwidth = prowidth;
    itemheigth= proheight;
    startverify = false;
    for(int i=0;i<MAXARRAYNUM;i++)
        node[i] = NULL;

    getfile(rownum,columnum);
    scene = new QGraphicsScene();
    ui->graphicsView->setScene(scene);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(200);

    ui->totalcompx->setValue(emcStatus->task.parameters[4043]);
    ui->totalcompy->setValue(emcStatus->task.parameters[4044]);

    connect(ui->btn_buildarray,SIGNAL(clicked()),this,SLOT(buildarray()));
    connect(ui->btn_rowcomp,SIGNAL(clicked()),this,SLOT(setTotalRowComp()));
    connect(ui->btn_columncomp,SIGNAL(clicked()),this,SLOT(setTotalColumnComp()));
    connect(ui->btn_totalcompx,SIGNAL(clicked()),this,SLOT(setTotalCompx()));
    connect(ui->btn_totalcompy,SIGNAL(clicked()),this,SLOT(setTotalCompy()));

    _preferce = new Ini_Settings(PREFER_FILE);

    ui->sprownum->setValue(_preferce->read("ARRAY","rownum","1",true).toInt());
    ui->spcolnum->setValue(_preferce->read("ARRAY","columnnum","1",true).toInt());
    ui->spdisx->setValue(_preferce->read("ARRAY","rowdistance","1",true).toDouble());
    ui->spdisy->setValue(_preferce->read("ARRAY","columndistance","1",true).toDouble());
    itemwidth = _preferce->read("ARRAY","width","1",true).toDouble();
    itemheigth= _preferce->read("ARRAY","height","1",true).toDouble();
    rownum = ui->sprownum->value();
    columnum = ui->spcolnum->value();
    distancex = ui->spdisx->value();
    distancey = ui->spdisy->value();
    reflushtext();
}

void Array::caculate() {
    scene->clear();
    double totalwidth = this->width()-ui->functrionframe->width()-10;
    double totalheight = this->height()-10;
    double tempx = fabs(distancex);
    double tempy = fabs(distancey);
    if(tempx == 0 || tempy == 0) {
        tempx = 10;
        tempy = 10;
    }
    double distancescale = tempy/tempx;

    double scalewidth = totalwidth/((itemwidth+10)*columnum+10);
    double scaleheight = totalheight/((itemheigth+10*distancescale)*rownum
                                      +10*distancescale);
    double scale = qMin(scalewidth,scaleheight);
    for(int j=0;j<rownum;j++) {
        for(int i=0;i<columnum;i++) {
            QPointF *po = new QPointF(0,0);
            node[i]   = new Node(0,po,itemwidth*scale,itemheigth*scale,i+j*columnum);
            node[i]->setItemText(iteminfo[i+j*columnum]);
            node[i]->setFlags(QGraphicsItem::ItemIsSelectable |
                              QGraphicsItem::ItemSendsGeometryChanges  );
            scene->addItem(node[i]);
            node[i]->setPos((totalwidth-(10+itemwidth)*scale*columnum)/2
                            +i*(10+itemwidth)*scale,
                            (totalheight-(10*distancescale+itemheigth)*scale*rownum)/2
                            +j*(10*distancescale+itemheigth)*scale);
        }
    }
    scene->setSceneRect(0,0,ui->graphicsView->width(),ui->graphicsView->height());
}

void Array::resizeEvent(QResizeEvent *) {
    caculate();
}

void Array::dragEnterEvent(QDragEnterEvent *)  {

}

void Array::update() {
    static int olditemflag = -1;
    if(olditemflag != nodesig) {
        compensation comp(0,nodesig,rownum,columnum);
        comp.exec();
        reflushtext();
        nodesig = -1;
        olditemflag = -1;
    }

    if(startverify) {
        if(1 != emcStatus->task.only_reading) {
            startverify = false;
            this->setItemSize(emcStatus->task.promax.tran.x - emcStatus->task.promin.tran.x,
                               emcStatus->task.promax.tran.y - emcStatus->task.promin.tran.y);
        }
    }
}


void Array::reflushtext() {
    getfile(rownum,columnum);
    caculate();
}

void Array::cleartext() {
    ui->sprownum->setValue(1);
    ui->spcolnum->setValue(1);
    ui->spdisx->setValue(0);
    ui->spdisy->setValue(0);
    ui->totalcompx->setValue(0);
    ui->totalcompy->setValue(0);
    ui->sprowcomp->setValue(0);
    ui->spcolumncomp->setValue(0);
    rownum = 1;
    columnum = 1;
    sendSetParameter(4043,0);
    sendSetParameter(4044,0);
    _preferce->writeEntry("ARRAY/rownum",QString("%1").arg(ui->sprownum->value()));
    _preferce->writeEntry("ARRAY/columnnum",QString("%1").arg(ui->spcolnum->value()));
    _preferce->writeEntry("ARRAY/rowdistance",QString("%1").arg(ui->spdisx->value()));
    _preferce->writeEntry("ARRAY/columndistance",QString("%1").arg(ui->spdisy->value()));
    _preferce->writeEntry("ARRAY/width",QString("%1").arg(itemwidth));
    _preferce->writeEntry("ARRAY/height",QString("%1").arg(itemheigth));
    getfile(rownum,columnum);
    caculate();
}

int Array::getfile(int row, int column) {
    int maxnum = row*column;
    for(int i=0;i<MAXARRAYNUM;i++) {
        iteminfo[i] = "";
    }
    QFile file(ARRAY_COMPENSATION_FILE);
    if(!file.open(QIODevice::ReadOnly))
        return -1;
    int oldmaxnum = 0;
    int toolnum = 0;
    int toolinfo[MAXTOOL];
    for(int i=0;i<MAXTOOL;i++)
        toolinfo[i] = 0;
    while(!file.atEnd()) {
        QString line = file.readLine();
        if(line.contains('[') && line.contains(']')) {
            iteminfo[oldmaxnum].append(QString("ARRAY:%1\n").arg(oldmaxnum));
            oldmaxnum++;
            toolnum = 0;
        } else {
            QStringList linelist = line.split(" ",QString::SkipEmptyParts);
            toolinfo[toolnum] = linelist.at(0).toInt();
            iteminfo[oldmaxnum-1].append(QString("Tool:%1 X:%2 Y%3")
                                         .arg(linelist.at(0)).arg(linelist.at(1)).arg(linelist.at(2)));
            toolnum++;
        }
    }
    file.close();

    if(oldmaxnum < maxnum) {
        file.open(QIODevice::WriteOnly | QIODevice::Append);
        QTextStream out(&file);
        for(int j=oldmaxnum;j<maxnum;j++) {
            out<<QString("[%1]\n").arg(j);
            iteminfo[j].append(QString("ARRAY:%1\n").arg(j));
            for(int i=0;i<toolnum;i++) {
                out<<toolinfo[i]<<" 0.000 0.000\n";
                iteminfo[j].append(QString("Tool:%1 X:0.000 Y0.000\n").arg(i));
            }
        }
        file.close();
    }

}

void Array::buildarray() {
    rownum = ui->sprownum->value();
    columnum = ui->spcolnum->value();
    distancex = ui->spdisx->value();
    distancey = ui->spdisy->value();
    reflushtext();
    if(rownum > 1 || columnum > 1)
        arraySlotBuild();
    _preferce->writeEntry("ARRAY/rownum",QString("%1").arg(ui->sprownum->value()));
    _preferce->writeEntry("ARRAY/columnnum",QString("%1").arg(ui->spcolnum->value()));
    _preferce->writeEntry("ARRAY/rowdistance",QString("%1").arg(ui->spdisx->value()));
    _preferce->writeEntry("ARRAY/columndistance",QString("%1").arg(ui->spdisy->value()));
    _preferce->writeEntry("ARRAY/width",QString("%1").arg(itemwidth));
    _preferce->writeEntry("ARRAY/height",QString("%1").arg(itemheigth));
}

void Array::ItemGetInfo(QString text) {
    startverify = true;
    filepath = text;
}

void Array::setItemSize(double width,double heigth) {
    itemwidth = width;
    itemheigth = heigth;
    reflushtext();
}

void Array::setTotalColumnComp() {
    compensation comp(0,ui->spcolumncomp->value()
                      ,rownum,columnum,-1,ui->spcolumncomp->value());
    comp.exec();
    reflushtext();
}


void Array::setTotalRowComp() {
    compensation comp(0,ui->sprowcomp->value()*columnum
                      ,rownum,columnum,ui->sprowcomp->value(),-1);
    comp.exec();
    reflushtext();
}

void Array::setTotalCompx() {
    sendSetParameter(4043,ui->totalcompx->value());
}

void Array::setTotalCompy() {
    sendSetParameter(4044,ui->totalcompy->value());
}

bool Array::eventFilter(QObject *, QEvent *) {

}

void Array::arraySlotBuild()
{
    int m99flag = 0;
    QString text = "";
    QFile fileget(filepath);
    if(!fileget.open(QIODevice::ReadOnly)) {
        qDebug()<<"array.ngc cannot open";
        return;
    }
    QString textline = "";

    while(!fileget.atEnd())
    {
        textline = fileget.readLine();

        if(!textline.contains(QRegExp("/")) &&
                (textline.contains(QRegExp("[Mm]30")) ||
                textline.contains(QRegExp("[Mm]2")) ||
                textline.contains(QRegExp("[Mm]99")))) {
            if(textline.contains(QRegExp("[Mm]99")))
                m99flag = 1;
            break;
        }
        text.append(textline);
    }
    fileget.close();

    int arrayBuildmode = 1;

    QString getfile = filepath;
    QString arrayfile = getfile;
    arrayfile.append(tr("(array)%1X%2").arg(columnum)
                     .arg(rownum));

    QFile file(arrayfile);
    if(file.exists()) {
        file.open(QIODevice::WriteOnly);
        file.close();
    }

    if(!file.open(QIODevice::ReadWrite)) {
        qDebug()<<"array.ngc cannot open";
        return;
    }


    QTextStream arraytext(&file);
    if(1 == arrayBuildmode) {
        for(int i=0;i<rownum;i++) {
            for(int j=0;j<columnum;j++) {
                arraytext<<"g11l"<<columnum*i+j<<"x"
                        <<j*distancex<<"y"<<i*distancey<<"\n";
                arraytext<<text;
                arraytext<<"g53z0g0\n";
                arraytext<<"g11l"<<columnum*i+j<<"x0y0\n";
            }
        }
    } else {
        arraytext<<"o101 while [#<YCOUNT> LT #<ROWS>]\n";
        arraytext<<"o102 while [#<XCOUNT> LT #<COLUMNS>]\n";
        arraytext<<"#4041=-[#<XCOUNT>*#<DISTANCEX>]\n#4042=-[#<YCOUNT>*#<DISTANCEY>]\n";
        arraytext<<"#4043=#<COUNTS>\n#4044=#<COUNTS>\n\n";
        arraytext<<"g92x[#<DISTANCEX>*#<XCOUNT>+#5420]y[#<DISTANCEY>*#<YCOUNT>+#5421]\n";
        arraytext<<text;
        arraytext<<"\nG92.1\n#<COUNTS> = [#<COUNTS>+1]\n#<XCOUNT> = [#<XCOUNT>+1]\ng53g90g0z0\n";
        arraytext<<"O102 endwhile\ng92.1\n#<XCOUNT> = 0\n#<YCOUNT> = [#<YCOUNT>+1]\n";
        arraytext<<"o101 endwhile\ng92.1\n\n";
    }

    if(1 == m99flag)
        arraytext<<"m99\n\n";
    else
        arraytext<<"m30\n\n";

    file.close();


    int ret =  QMessageBox::information(this,tr("tips"),
                    tr("Array file is build in \n%1,ensure load the array file?").
                                        arg(arrayfile),QMessageBox::Ok,QMessageBox::Cancel);
    if(ret == QMessageBox::Ok) {
        emit(loadArrayfile(arrayfile));
    }
}

Array::~Array()
{
    delete ui;
}
