#include "tchpar.h"
#include "ui_tchpar.h"

#include "extern.h"
#include "emcstatus.hh"
#include "shcom.hh"
#include "mywidget.h"

#include<QDoubleSpinBox>
#include<QHBoxLayout>
#include <QTimer>
#include <QMap>
#include <QMessageBox>
#include <QtDebug>

TCHPar::TCHPar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TCHPar)
{
    ui->setupUi(this);

    tableType = _ini->readEntry("PAR_RS274_TOOLTABLE_TYPE/VALUE","0").toInt();
    initData();
    initTool();

    show_tnum_z1 = new MyLabel(0);
    show_tnum_z1->setAxis(AXIS_Z1);
    show_tnum_z2 = new MyLabel(0);
    show_tnum_z2->setAxis(AXIS_A);
    QHBoxLayout *hbox = new QHBoxLayout(ui->showtool1frame);
    hbox->addWidget(show_tnum_z1);
    hbox->setContentsMargins(0,0,0,0);
    QHBoxLayout *hbox2 = new QHBoxLayout(ui->showt2frame);
    hbox2->addWidget(show_tnum_z2);
    hbox2->setContentsMargins(0,0,0,0);
    connect(show_tnum_z1,SIGNAL(leftClick(int)),this,SLOT(setToolnow(int)));
    connect(show_tnum_z2,SIGNAL(leftClick(int)),this,SLOT(setToolnow(int)));

    show_toollen = new MyLabel(0);
    QHBoxLayout *lenbox = new QHBoxLayout(ui->wid_toollen);
    lenbox->addWidget(show_toollen);
    lenbox->setContentsMargins(0,0,0,0);
    show_toollen->setText("0.000");
    connect(show_toollen,SIGNAL(leftClick(int)),this,SLOT(setToolLen(int)));

    ui->chk_ignoretool->setChecked(par_get_var(4292) > 0);
    ui->chk_ignorespindle->setChecked(par_get_var(4293) > 0);
    ui->chk_ignorecover->setChecked(par_get_var(4291) > 0);
    ui->chk_usetbl->setChecked(par_get_var(4127) > 0);
    ui->chk_autoprobe->setChecked(par_get_var(4290)>0);
    ui->changedelay->setValue(par_get_var(4302));
    safeypos = par_get_var(4286);

    lab_safeypos = new MyLabel(0);
    QHBoxLayout *lay_safeypos = new QHBoxLayout(ui->safeYpos);
    lay_safeypos->addWidget(lab_safeypos);
    lay_safeypos->setContentsMargins(0,0,0,0);
    lab_safeypos->setText(QString::number(safeypos));
    connect(lab_safeypos,SIGNAL(clicked()),this,SLOT(set_safeYpos()));

    connect(ui->tbl_par,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(tableclick(int,int)));

    QTimer *timer= new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateui()));
    timer->start(300);


    ui->tbl_par->setEnabled(false);
    ui->tableToolLen->setEnabled(false);
    ui->groupBox->setEnabled(false);

    //刀长表初始化
    initToolLengthTbl();

//    if(!_doublez) {
        ui->label_7->hide();
        ui->pos_z2->hide();
        ui->widget_6->hide();
        ui->tbl_par->setColumnWidth(3,0);
        ui->showt2frame->hide();
//    }

    if(tableType == 1 || tableType == 3 || tableType == 4) {
        ui->tbl_par->setColumnWidth(4,0);
        ui->tbl_par->setColumnWidth(5,0);
        ui->pos_c->hide();
        ui->label_10->hide();
        if(tableType != 4) {
//            ui->safetblpos->hide();
//            ui->label_11->hide();
//            ui->allowdebug->hide();
        }
//        ui->showt2frame->hide();
//        ui->label_8->hide();
        ui->label_9->hide();
        ui->pos_b->hide();
        if(tableType == 3) {
            ui->chk_ignorecover->setText("忽略刀盘信号");
            ui->label_8->setText("当前刀盘号");
            ui->widget_6->show();
            ui->showt2frame->show();
            ui->label_2->setText("安全距离:");
        }
    } else if(tableType == 2) {
        ui->tbl_par->setColumnWidth(5,0);
        ui->pos_c->hide();
        ui->label_10->hide();
//        ui->label_11->hide();
//        ui->safetblpos->hide();
//        ui->showt2frame->hide();
//        ui->label_8->hide();
//        ui->allowdebug->hide();
    }

    sendMultiFunction(20,0,par_get_var(4287));

    ui->checkBox->hide();
}

TCHPar::~TCHPar()
{
    delete ui;
}


void TCHPar::initData()
{
    //设置换刀参数
   double changetoolspeedfast =  _ini->read("PAR_GUICHANGETOOL_FASTSPEED" ,"VALUE","5000").toDouble();
   sendSetParameter(4282,changetoolspeedfast);
   double changetoolspeedslow =  _ini->read("PAR_GUICHANGETOOL_SLOWSPEED" ,"VALUE","1500").toDouble();
   sendSetParameter(4297,changetoolspeedslow);
   double changetooldistance =  _ini->read("PAR_GUI_CHANGETOOL_DISTANCE" ,"VALUE","70").toDouble();
   sendSetParameter(4298,changetooldistance);
   double toolliftz =  _ini->read("PAR_GUI_CHANGETOOL_LIFTHEIGHT" ,"VALUE","0").toDouble();
   sendSetParameter(4280,toolliftz);

//设置二级限位
    sendSetParameter(4225,(par_get_var(4051)+par_get_var(4052))/2);
    sendSetParameter(4226,(par_get_var(4053)+par_get_var(4054))/2);
    sendSetParameter(4221,par_get_var(4051)-par_get_var(4052));
    sendSetParameter(4222,par_get_var(4053)-par_get_var(4054));
    sendSetParameter(4223,0);
    sendSetParameter(4224,0);

    ui->safetblpos->setValue(par_get_var(4285));

    setToolnum = _ini->read("PAR_RS274_TOOLNUMBER","VALUE","0").toInt();
    for(int i = 0;i<MAXTOOL;i++) {
        tooldata[i].x = par_get_var(4300+(i+1)*5);
        tooldata[i].y = par_get_var(4301+(i+1)*5);
        tooldata[i].z1 = par_get_var(4302+(i+1)*5);
        if(i < 19) {
            tooldata[i].z2 = par_get_var(4700+(i+1)*5);
            tooldata[i].b = par_get_var(4701+(i+1)*5);
            tooldata[i].c = par_get_var(4702+(i+1)*5);
        } else {
            tooldata[i].z2 = par_get_var(4700+(i+1)*5 + 20);
            tooldata[i].b = par_get_var(4701+(i+1)*5 + 20);
            tooldata[i].c = par_get_var(4702+(i+1)*5 + 20);
        }
    }

    pocket.clear();
    for(int i=0;i<setToolnum;i++) {
        pocket[i] = par_get_var(3201+i);
    }

    on_safetblpos_valueChanged(par_get_var(4285));
}

void TCHPar::initTool()
{
    ui->tbl_par->setRowCount(setToolnum);
    QStringList toolname;
    int settoolnum = setToolnum;
    if(tableType == 3){
        settoolnum = 1;
        ui->tbl_par->setRowCount(1);
    }
    for(int i=0;i<settoolnum;i++) {
        toolname.append(QString("刀具%1").arg(i+1));

        data_X[i] = new QTableWidgetItem();
        data_X[i]->setText(QString("%1").arg(tooldata[i].x,0,'f',3));
        data_X[i]->setFlags(Qt::ItemIsEnabled);

        data_Y[i] = new QTableWidgetItem();
        data_Y[i]->setText(QString("%1").arg(tooldata[i].y,0,'f',3));
        data_Y[i]->setFlags(Qt::ItemIsEnabled);

        data_Z1[i] = new QTableWidgetItem();
        data_Z1[i]->setText(QString("%1").arg(tooldata[i].z1,0,'f',3));
        data_Z1[i]->setFlags(Qt::ItemIsEnabled);

        data_Z2[i] = new QTableWidgetItem();
        data_Z2[i]->setText(QString("%1").arg(tooldata[i].z2,0,'f',3));
        data_Z2[i]->setFlags(Qt::ItemIsEnabled);

        data_B[i] = new QTableWidgetItem();
        data_B[i]->setText(QString("%1").arg(tooldata[i].b,0,'f',3));
        data_B[i]->setFlags(Qt::ItemIsEnabled);

        data_C[i] = new QTableWidgetItem();
        data_C[i]->setText(QString("%1").arg(tooldata[i].c,0,'f',3));
        data_C[i]->setFlags(Qt::ItemIsEnabled);

        ui->tbl_par->setItem(i,0,data_X[i]);
        ui->tbl_par->setItem(i,1,data_Y[i]);
        ui->tbl_par->setItem(i,2,data_Z1[i]);
        ui->tbl_par->setItem(i,3,data_Z2[i]);;
        ui->tbl_par->setItem(i,4,data_B[i]);
        ui->tbl_par->setItem(i,5,data_C[i]);
    }

    ui->tbl_par->setVerticalHeaderLabels(toolname);
    ui->tbl_par->setColumnWidth(0,80);
    ui->tbl_par->setColumnWidth(1,80);
    ui->tbl_par->setColumnWidth(2,80);
    ui->tbl_par->setColumnWidth(3,80);
    ui->tbl_par->setColumnWidth(4,80);
    ui->tbl_par->setColumnWidth(5,80);
}

void TCHPar::tableclick(int x, int y)
{
    QTableWidgetItem *temp = ui->tbl_par->item(x,y);
    double val = temp->text().toDouble();
    double oldval  =  val;
    SetValue getval(val,x,y);
    getval.exec();
    temp->setText(QString("%1").arg(getval.defvel,0,'f',3));
    if(fabs(oldval-getval.defvel) > 0.001) {
        if(y < 3) {
            sendSetParameter(4300+y+(x+1)*5,getval.defvel);
        } else {
            if(x < 19) {
                sendSetParameter(4700+(y-3)+(x+1)*5,getval.defvel);
            } else {
                sendSetParameter(4700+(y-3)+(x+1)*5+20,getval.defvel);
            }
        }
    }
}

void TCHPar::setToolnow(int num)
{
    if(emcStatus->task.runState != EMC_IDLE ){
        int ret=QMessageBox::information(this,tr("提示"),
                                         tr("请先退出运行!!"),
                                         QMessageBox::Ok);
        return;
    }

    if(!PassWord::getCodecnc(this,1)) {
        return;
    }
    int toolnum =1;
    if(num == AXIS_Z1)

        toolnum = QInputDialog::getInt(this,"设置刀具号","刀具号",par_get_var(4288),tableType == 3?0:1,100);

    if(num == AXIS_Z1) {
        sendSetParameter(4288,toolnum);
        sendSetParameter(4283,toolnum);
    }
    if(num == AXIS_A) {

        int pocketnum = QInputDialog::getInt(this,"设置当前刀槽号","刀槽号",par_get_var(3810),tableType == 3?0:1,100);
        sendSetParameter(3810,pocketnum);

    }
}

void TCHPar::setToolLen(int)
{
    if(!ui->tbl_par->isEnabled())
        return;
    double currentlen = par_get_var(4303+5*par_get_var(4283));
    double oldval  =  currentlen;
    SetValue getval(currentlen,0,2);
    getval.exec();
    if(fabs(oldval-getval.defvel) > 0.001) {
        double recordtl = par_get_var(4303+5*par_get_var(4283));
        sendSetParameter(4303+5*par_get_var(4283),getval.defvel);
        showMsg(1,QString("修改%1号刀刀长值为%2").arg(par_get_var(4283)).arg(getval.defvel));
        double diff = getval.defvel - recordtl;
        double changeZ = par_get_var(5203+par_get_var(5220)*20) + diff;
        QString cmd = "";
        cmd.append(QString("G10L2P%1Z%2").arg((int)par_get_var(5220)).arg(changeZ));
        ensure_mode(EMC_TASK_MODE_MDI);
        sendMdiCmd(cmd.toLocal8Bit().data(),0);
        ensure_mode(EMC_TASK_MODE_MANUAL);
    }
}




SetValue::SetValue (double defaultval, int x, int y, QDialog *parent)
{
    row = x;
    column = y;
    this->setWindowTitle("设置坐标值");

    defaulval = new QDoubleSpinBox(parent);
    defaulval->setMaximum(100000);
    defaulval->setMinimum(-100000);
    defaulval->setDecimals(3);
    defaulval->setButtonSymbols(QAbstractSpinBox::NoButtons);
    showdefault = new QLabel("设置坐标值:");
    showdefault->setMaximumWidth(90);
    defaultset = new QWidget(parent);
    QHBoxLayout *defaultlay = new QHBoxLayout(defaultset);
    defaultlay->addWidget(showdefault);
    defaultlay->addWidget(defaulval);
    defaultlay->setContentsMargins(0,0,0,0);


    btn_ensure = new QPushButton("确定");
    btn_cancel = new QPushButton("取消");
    btn_getpos = new QPushButton("获取坐标值");
    btnset = new QWidget(parent);
    QHBoxLayout *btnlay = new QHBoxLayout(btnset);
    btnlay->addWidget(btn_ensure);
    btnlay->addWidget(btn_getpos);
    btnlay->addWidget(btn_cancel);
    btnlay->setContentsMargins(0,0,0,0);

    QVBoxLayout *totallay = new QVBoxLayout(this);
    totallay->addWidget(defaultset);
    totallay->addWidget(btnset);
    totallay->setContentsMargins(5,5,5,5);

    defvel = defaultval;
    defaulval->setValue(defaultval);
    btn_cancel->setFocusPolicy(Qt::NoFocus);
    defaulval->setFocus();


    connect(btn_getpos,SIGNAL(clicked()),this,SLOT(getPos()));
    connect(btn_ensure,SIGNAL(clicked()),this,SLOT(ensureSetValue()));
    connect(btn_cancel,SIGNAL(clicked()),this,SLOT(close()));

}

void SetValue::ensureSetValue()
{
    defvel = defaulval->value();
    this->close();
}

void SetValue::getPos()
{    
    int axis = column;
    defaulval->setValue(get_abs_pos(axis));
}

void TCHPar::on_chk_ignoretool_toggled(bool checked)
{
    if(checked) {
        sendSetParameter(4292,1);
    } else {
        sendSetParameter(4292,0);
    }
}

void TCHPar::on_chk_ignorespindle_toggled(bool checked)
{
    if(checked) {
        sendSetParameter(4293,1);
    } else {
        sendSetParameter(4293,0);
    }
}

void TCHPar::on_chk_ignorecover_toggled(bool checked)
{
    if(checked) {
        sendSetParameter(4291,1);
    } else {
        sendSetParameter(4291,0);
    }
}

void TCHPar::on_chk_usetbl_toggled(bool checked)
{
    if(checked) {
        sendSetParameter(4127,1);
    } else {
        sendSetParameter(4127,0);
    }
}


void TCHPar::updateui()
{
    if(this->isVisible()) {

        ui->chk_runprobe->setChecked(par_get_var(4295) > 0);
        ui->safeYpos->setEnabled(par_get_var(4281) > 0);
        ui->safetblpos->setEnabled(par_get_var(4281) > 0);
        ui->changedelay->setEnabled(par_get_var(4281) > 0);

        if(lab_safeypos->text().toDouble() != par_get_var(4286)) {
            lab_safeypos->setText(QString::number(par_get_var(4286),'f',3));
        }
        ui->pos_x->setText(QString::number(get_abs_pos(AXIS_X),'f',3));
        ui->pos_y->setText(QString::number(get_abs_pos(AXIS_Y),'f',3));
        ui->pos_z1->setText(QString::number(get_abs_pos(AXIS_Z1),'f',3));
//        ui->pos_z2->setText(QString::number(get_abs_pos(AXIS_Z2),'f',3));
        ui->pos_b->setText(QString::number(get_abs_pos(AXIS_B),'f',3));
        ui->pos_c->setText(QString::number(get_abs_pos(AXIS_C),'f',3));
        show_tnum_z1->setText(QString::number(par_get_var(4283)));
        show_tnum_z2->setText(QString::number(par_get_var(4284)));
        if(tableType == 3)
            show_tnum_z2->setText(QString::number(par_get_var(3810)));
        ui->allowdebug->setChecked(par_get_var(4287) == 0);
        if(show_toollen->text().toDouble() != par_get_var(4303+par_get_var(4283)*5))
            show_toollen->setText(QString::number(par_get_var(4303+par_get_var(4283)*5),'f',3));


        for(int i=0;i<setToolnum;i++) {
            pocket[i] = (int)par_get_var(3201+i);

            if(toolno[i]->text().toInt() != pocket[i]) {
                toolno[i]->setText(QString::number(pocket[i],10));
                toolno[i]->setTextAlignment(Qt::AlignHCenter);
            }
            if(tool1len[i]->text().toDouble() != par_get_var(4303+5*pocket[i])) {
                tool1len[i]->setText(QString::number(par_get_var(4303+5*pocket[i]),'f',3));
            }

        }

    }
}

void TCHPar::on_chk_modify_toggled(bool checked)
{
    if(checked) {

        if(!PassWord::getCodecnc(this,-1)) {
            sendSetParameter(4281,0);
            return;
        }
        ui->tbl_par->setEnabled(true);
        ui->groupBox->setEnabled(true);
        ui->tableToolLen->setEnabled(true);
        sendSetParameter(4281,1);
    } else {
        ui->tbl_par->setEnabled(false);
        ui->groupBox->setEnabled(false);
        ui->tableToolLen->setEnabled(false);
        sendSetParameter(4281,0);
    }
}

void TCHPar::on_chk_autoprobe_toggled(bool checked)
{
    if(checked) {
        sendSetParameter(4290,1);
    } else {
        sendSetParameter(4290,0);
    }
}


void TCHPar::on_changedelay_valueChanged(double arg1)
{
    sendSetParameter(4302,arg1);
}

void TCHPar::on_safetblpos_valueChanged(double arg1)
{
    int seclimittype = _ini->read("PAR_TASK_SECLIMIT_TYPE","VALUE","0").toInt();
    sendSetParameter(4278,(double)seclimittype);
    sendSetParameter(4285,arg1);
    if(seclimittype == 0) { //Y正限位
        double offset = arg1-par_get_var(4054);
        sendSetParameter(4226,par_get_var(4054) + (offset/2));
        sendSetParameter(4222,offset);
        if(par_get_var(4287) == 1)
        on_allowdebug_toggled(0);
    } else if(seclimittype == 1) { //X正限位
        double offset = arg1-par_get_var(4052);
        sendSetParameter(4225,par_get_var(4052) + (offset/2));
        sendSetParameter(4221,offset);
        if(par_get_var(4287) == 1)
        on_allowdebug_toggled(0);
    } else if(seclimittype == 2) { //Y负限位
        double offset = par_get_var(4053) - arg1;
        sendSetParameter(4226,par_get_var(4053) - (offset/2));
        sendSetParameter(4222,offset);
        if(par_get_var(4287) == 1)
        on_allowdebug_toggled(0);
    } else if(seclimittype == 3) { //X负限位
        double offset = par_get_var(4051) - arg1;
        sendSetParameter(4225,par_get_var(4051) - (offset/2));
        sendSetParameter(4221,offset);
        if(par_get_var(4287) == 1)
        on_allowdebug_toggled(0);
    }
}

void TCHPar::on_allowdebug_toggled(bool checked)
{
    if(checked) {
        sendSetParameter(4287,0);
         sendSetParameter(4227,0);
    } else {
         sendSetParameter(4287,1);
        sendSetParameter(4227,1);
    }
    sendMultiFunction(20,0,par_get_var(4287));
}

void TCHPar::initToolLengthTbl() {

    ui->tableToolLen->verticalHeader()->setVisible(false);
    ui->tableToolLen->clear();
    for(int i=ui->tableToolLen->rowCount();i>0;i--) {
        ui->tableToolLen->removeRow(i-1);
    }
    ui->tableToolLen->setRowCount(30);
    ui->tableToolLen->setColumnCount(6);

    QTableWidgetItem *title0 = new QTableWidgetItem(tr("刀槽号"));
    ui->tableToolLen->setHorizontalHeaderItem(0,title0);
    QTableWidgetItem *title1 = new QTableWidgetItem(tr("刀具号"));
    ui->tableToolLen->setHorizontalHeaderItem(1,title1);
    QTableWidgetItem *title2 = new QTableWidgetItem(tr("刀长"));
    ui->tableToolLen->setHorizontalHeaderItem(2,title2);

    QTableWidgetItem *title3 = new QTableWidgetItem(tr("刀槽号"));
    ui->tableToolLen->setHorizontalHeaderItem(3,title3);
    QTableWidgetItem *title4 = new QTableWidgetItem(tr("刀具号"));
    ui->tableToolLen->setHorizontalHeaderItem(4,title4);
    QTableWidgetItem *title5 = new QTableWidgetItem(tr("刀长"));
    ui->tableToolLen->setHorizontalHeaderItem(5,title5);

    for(int j=0;j<setToolnum;j++) {
        toolpocket[j] = new QTableWidgetItem();
        toolpocket[j]->setText(QString::number(j+1));
        const QColor color = QColor(201, 235, 255);
        toolpocket[j]->setBackgroundColor(color);

        toolno[j] = new QTableWidgetItem();
        toolno[j]->setText(QString::number(pocket[j]));
        const QColor color2 = QColor(255, 247, 155);
        toolno[j]->setBackgroundColor(color2);

        tool1len[j] = new QTableWidgetItem();
        tool1len[j]->setText(QString::number(par_get_var(4303+5*(pocket[j]+1)),'f',3));

        if(j%2 == 0) {
            ui->tableToolLen->setItem(j/2,0,toolpocket[j]);
            ui->tableToolLen->setItem(j/2,1,toolno[j]);
            ui->tableToolLen->setItem(j/2,2,tool1len[j]);
        } else {
            ui->tableToolLen->setItem(j/2,3,toolpocket[j]);
            ui->tableToolLen->setItem(j/2,4,toolno[j]);
            ui->tableToolLen->setItem(j/2,5,tool1len[j]);
        }

        toolpocket[j]->setFlags(Qt::NoItemFlags);
        toolno[j]->setFlags(Qt::ItemIsEnabled);
        tool1len[j] ->setFlags(Qt::ItemIsEnabled);
        toolno[j]->setTextAlignment(Qt::AlignHCenter);
    }


    ui->tableToolLen->setColumnWidth(0,65);
    ui->tableToolLen->setColumnWidth(1,65);
    ui->tableToolLen->setColumnWidth(2,120);
    ui->tableToolLen->setColumnWidth(3,65);
    ui->tableToolLen->setColumnWidth(4,65);
    ui->tableToolLen->setColumnWidth(5,120);
}


void TCHPar::on_checkBox_toggled(bool checked)
{
//    initToolLengthTbl();
}

void TCHPar::set_safeYpos()
{
    int val = 0;
    MyInput input(0,(QVariant)safeypos,-10000,10000,tr("退刀安全距离"),tr("退刀安全距离"),0);
    input.exec();
    val = input.val.toInt();
    if(val >= -10000 && val <= 10000){
        safeypos = val;
        lab_safeypos->setText(QString::number(safeypos));
        sendSetParameter(4286,safeypos);
    }
}

void TCHPar::on_tableToolLen_cellDoubleClicked(int row, int column)
{
    int x = row;
    int y = column;
    QTableWidgetItem *temp = ui->tableToolLen->item(x,y);
    if(y == 2 || y == 5) {
        double defval = temp->text().toDouble();
        MyInput input(1,(QVariant)defval,(QVariant)-1000,(QVariant)1000,"设置刀具长度","设置刀具长度",0);
        input.exec();
        double val = input.val.toDouble();
        if(val >= -1000 && val <= 1000 && fabs(val - defval) > 0.0001){
            int pocketnum = 0;
            if(y == 2) {
                pocketnum = row*2;
            }else if(y == 5){
                pocketnum = row*2+1;
            }
            int toolno = pocket[pocketnum];
            temp->setText(QString("%1").arg(val,0,'f',3));
            QString cmd = "";
            double recordtl = par_get_var(4303+5*toolno);
            sendSetParameter(4303+5*toolno,val);
            showMsg(1,QString("修改%1号刀刀长值为%2").arg(toolno).arg(val));

            if(toolno == (int)par_get_var(4283)) {
                double diff = val - recordtl;
                double changeZ = par_get_var(5203+par_get_var(5220)*20) + diff;
                cmd.append(QString("G10L2P%1Z%2").arg((int)par_get_var(5220)).arg(changeZ));
                ensure_mode(EMC_TASK_MODE_MDI);
                sendMdiCmd(cmd.toLocal8Bit().data(),0);
                ensure_mode(EMC_TASK_MODE_MANUAL);
            }
        }
    } else if( y == 1 || y == 4) {
        int defval = temp->text().toInt();
        MyInput input(0,(QVariant)defval,(QVariant)0,(QVariant)setToolnum,"设置刀具","设置刀具",0);
        input.exec();
        double val = input.val.toInt();
        if(val >= 0 && val <= setToolnum && fabs(val - defval) > 0.0001){
            int pocketnum = 0;
            if(y == 1) {
                pocketnum = row*2;
            }else if(y == 4){
                pocketnum = row*2+1;
            }
            for(int i = 0;i<setToolnum;i++) {
                if(pocket[i] == val && val != 0 && i != pocketnum) {
                    QMessageBox::warning(this,"警告","刀号重复，不能设置",QMessageBox::Ok);
                    return;
                }
            }
            pocket[pocketnum] = val;
            temp->setText(QString("%1").arg(val,10));
            temp->setTextAlignment(Qt::AlignHCenter);
            tool1len[pocketnum]->setText(QString::number(par_get_var(4303+5*(val)),'f',3));
            sendSetParameter(3201+pocketnum,val);
            showMsg(1,QString("修改%1号刀槽的刀号为%2").arg(pocketnum+1).arg(val));

            }

        }

}

void TCHPar::on_chk_runprobe_clicked()
{
    if(par_get_var(4295) > 0)
        sendSetParameter(4295,0);
    else
        sendSetParameter(4295,1);
}
