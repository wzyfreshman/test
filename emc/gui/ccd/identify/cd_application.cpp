#include <QWidget>
#include <QDebug>
#include <QTime>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QApplication>
//#include <unistd.h>
#include <float.h>

#include "rs_spline.h"
#include "rs_debug.h"
#include "cd_inisettings.h"
#include "cd_application.h"
#include "cd_identify.h"
#include "cd_matchtemplate.h"
#include "ccd_mainwidget.h"
#include "ccd_markeditor.h"
#include "ccd_syspareditor.h"
#include "ccd_sysparameter.h"
#include "ccd_cncmanager.h"
#include "ccd_mark.h"
#include "ccd_sysparcommoneditor.h"
#include "cd_mainscreen.h"
#include "rs_doubleline.h"
#include "rs_polyline.h"
#include "camera.h"


CD_Application::CD_Application(int marknum,const QString &confi) {

    local_conf_file = confi;

    for( int i=0; i<marknum; i++ ) {
        for(  int j=0;  j<CCD_CAMERA_COUNT; j++ ) {
            CD_ImageScreen *edit = new CD_ImageScreen(0);
            CD_ImageScreen *select = new CD_ImageScreen(0);
            CCD_Mark *mark = new CCD_Mark(edit,select,i);
            mark->setCameraId(0);
            mark->setEnableCameraPar(true);
            marks[j].append(mark);

            auxscreens[j].append(new CD_ImageScreen(0));
            auxscreensplace[j].append(NULL);
        }
    }

    markeditor = new CCD_MarkEditor(0);

     for(  int i=0;  i<CCD_CAMERA_COUNT; i++ ) {
         mainscreens[i] = new  CD_MainScreen(0);
         sysparameter[i] = new CCD_SysParameter(QString("CCD_Parameter_%1").arg(i+1),i);
         sysparametereditor[i] = new CCD_SysParEditor(0);
         mainscreensplace[i] = NULL;
     }

    syscommonpareditor = new CCD_SysParCommonEditor(0);


    appstate.state = CD_AppState::StateIdle;
    appstate.index = 0;


    cncmanager = new CCD_CncManager();
    run_without_camera = false;

    clearMainScreenCoordinate();
    edit_camera_par_index = 0;
    which_y = 0;
    which_y_set = 0;
    localReadFromDisk();
}

CD_Application::~CD_Application() {

    for(  int j=0;  j<CCD_CAMERA_COUNT; j++ ) {
        CCD_Mark *m;
        while(!marks[j].isEmpty()) {
            m = marks[j].takeFirst();
            if(m != NULL) delete m;
        }
    }
    localSaveToDisk();

}

void CD_Application::setMainScreen(int camera,QWidget *_place) {

    if(camera<0 || camera>= CCD_CAMERA_COUNT) return;

    putImageScreenToWidget(mainscreens[camera],_place);
    mainscreensplace[camera] = _place;
    activeScreen(mainscreensplace[camera],mainscreens[camera]);

}

void CD_Application::setAuxScreen(int camera, QList<QWidget *> &list) {

    if(camera<0 || camera>= CCD_CAMERA_COUNT) return;
    int c;

    if( list.size() < auxscreensplace[camera].size() )
        c = list.size();
    else
        c = auxscreensplace[camera].size();

    for(int i = 0; i < c; i++) {
        auxscreensplace[camera].replace(i,list[i]);
        putImageScreenToWidget(auxscreens[camera][i],auxscreensplace[camera][i]);
    }

}

void CD_Application::slotClearScreen() {

    appstate.state = CD_AppState::StateIdle;
    mainscreen->resetLines();
    mainscreen->update();

}


void CD_Application::syncCommonParameter(int main) {

    CCD_SysParameter *sys = sysparameter[main];
    for(int i=0;i<CCD_CAMERA_COUNT;i++) {
        if(i == main) continue;
        sys->copyCommonParameterTo(sysparameter[i]);
    }
}


void CD_Application::initAllMarkProfile() {

    for(int i=0;i<CCD_CAMERA_COUNT;i++) {
        for(int j=0;j<marks[i].size();j++) {
            marks[i].at(j)->initProfile();
        }
    }

}

void CD_Application::actionEditeCommonPar(int index) {


    updateArrayParameter();

    int old_contrast_v = CD_Parameter::Instance()->getContrastValidValue();

    syscommonpareditor->setWindowTitle(QString("CommonParEditor-%1").arg(index+1));
    syscommonpareditor->init(sysparameter[index],cncmanager);
    syscommonpareditor->exec();
    syncCommonParameter(index);
    appstate.state = CD_AppState::StateIdle;
    mainwidget->slotSaveSetting();
    run_without_camera = syscommonpareditor->offline_mode;

    if( old_contrast_v != CD_Parameter::Instance()->getContrastValidValue() ) {

            RS_MathAcc::instance()->initAtan2Pool();
            initAllMarkProfile();

    }


}

void CD_Application::actionEditSysParameter(int index) {


    if(index<0 || index >= CCD_CAMERA_COUNT) return;

    cncmanager->changeM64IO(5,1);

    edit_camera_par_index = index;
    sysparameter[index]->camergain = camergain[index];
    sysparameter[index]->shutter = shutter[index];
    sysparameter[index]->blacklevel = blacklevel[index];
    sysparameter[index]->mirrorV = mirrorV[index];
    sysparameter[index]->mirrorH = mirrorH[index];
    sysparameter[index]->img_resolution = imgresolution[index];

    appstate.state = CD_AppState::StateEditeSysParameter;
    sysparametereditor[1]->showInMainScreen(mainscreen->getCurrentImageMat());
    sysparametereditor[1]->init(sysparameter[index],capture);
    sysparametereditor[1]->showInMainScreen(*mainscreens[0]->getCurrentImageMat());
    sysparametereditor[1]->exec();
    syncCommonParameter(index);
    appstate.state = CD_AppState::StateIdle;

    camergain[index] = sysparameter[index]->camergain;
    shutter[index] = sysparameter[index]->shutter;
    blacklevel[index] = sysparameter[index]->blacklevel;
    mirrorV[index] = sysparameter[index]->mirrorV;
    mirrorH[index] = sysparameter[index]->mirrorH;
    imgresolution[index] = sysparameter[index]->img_resolution;

    mainwidget->slotSaveSetting();

}

void CD_Application::actionUpdateDisplayTemp() {

    updateWhichY();
    if( which_y ==1) { //y2


        showInAuxScreen(0,0,marks[1].at(0)->getTemplateImage()->returnAsMat());
        showInAuxScreen(0,1,marks[1].at(1)->getTemplateImage()->returnAsMat());
        showInAuxScreen(0,2,marks[1].at(2)->getTemplateImage()->returnAsMat());
        showInAuxScreen(0,3,marks[1].at(3)->getTemplateImage()->returnAsMat());
    } else {

        showInAuxScreen(0,0,marks[0].at(0)->getTemplateImage()->returnAsMat());
        showInAuxScreen(0,1,marks[0].at(1)->getTemplateImage()->returnAsMat());
        showInAuxScreen(0,2,marks[0].at(2)->getTemplateImage()->returnAsMat());
        showInAuxScreen(0,3,marks[0].at(3)->getTemplateImage()->returnAsMat());
    }

}

void CD_Application::actionEditTemplate(int camera,int index) {

    if(camera<0 || camera >= CCD_CAMERA_COUNT) return;
    if(index >= marks[camera].size()) return;

    updateWhichY();
    if( which_y ==1) { //y2
        marks[1].at(index)->getScreenTemplate()->catchOneImage((CD_ImageScreen*)mainscreens[0]);
        actionEditTemplate(marks[1].at(index));
        showInAuxScreen(0,index,marks[1].at(index)->getTemplateImage()->returnAsMat());
    } else {
        marks[0].at(index)->getScreenTemplate()->catchOneImage((CD_ImageScreen*)mainscreens[0]);
        actionEditTemplate(marks[0].at(index));
        showInAuxScreen(0,index,marks[0].at(index)->getTemplateImage()->returnAsMat());
    }


}

void CD_Application::actionEditTemplate(CCD_Mark *mark) {

    bool initornot = mark->isFirstRead();
    if(!mark->getTemplateImage()->isValid())
        QMessageBox::warning(mainwidget->getTopWidget(),QObject::tr("Warnning"),QObject::tr("The Templeate is invilid"));

    mark->actionEditFeature(initornot);
    mark->push();

    appstate.state = CD_AppState::StateEditFeature;

    markeditor->init(mark);
    markeditor->exec();
    int rr = markeditor->exe_ret;
    if( rr == -1 )
        mark->pop();
    else
        mainwidget->slotSaveSetting("",true);

    back();
    mark->resettFirstRead();
    //showInAuxScreen(pos,mark->getTemplateImage()->returnAsMat());
    appstate.state = CD_AppState::StateIdle;

}

void CD_Application::back() {

    //if(appstate.state != CD_AppState::StateEditFeature ) return;
    //CD_OneMark *mark = determineActiveMark();
    //if(mark->exitEditeFeature()) {
    //    activeMainScreen();
    //    appstate.state = CD_AppState::StateIdle;
    //}

}


void CD_Application::doImgCompensate(cv::Mat &mat,cv::Mat &To) {


    To = mat;
    return;
    /*
    if( sysparameter->compensate_img_enable ) {
            double local[3][3];
            for(int i=0;i<3;i++) {
                for(int j=0;j<3;j++) {
                    local[i][j] = sysparameter->compensate_mat[i][j];
                }
            }
            cv::Mat C = (cv::Mat_<double>(3,3) << local[0][0], local[0][1], local[0][2],
                    local[1][0], local[1][1], local[1][2],
                    local[2][0], local[2][1], local[2][2]);

            qDebug()<<"start cal warpPerspective"<<QTime::currentTime().second()<<QTime::currentTime().msec();
            cv::warpPerspective(mat, To, C, mat.size(), cv::INTER_LINEAR,
                                cv::BORDER_CONSTANT,
                                cv::Scalar(255));
            qDebug()<<"end cal warpPerspective"<<QTime::currentTime().second()<<QTime::currentTime().msec();

    } else {
        To = mat;
    }
*/
/*
    if( sysparameter->compensate_img_enable ) {
            double local[2][3];
            for(int i=0;i<2;i++) {
                for(int j=0;j<3;j++) {
                    local[i][j] = sysparameter->compensate_mat[i][j];
                }
            }
            cv::Mat C = (cv::Mat_<double>(2,3) << local[0][0], local[0][1], local[0][2],
                    local[1][0], local[1][1], local[1][2] );

            qDebug()<<"start cal warpAffine"<<QTime::currentTime().second()<<QTime::currentTime().msec();
            cv::warpAffine(mat, To, C, mat.size(), cv::INTER_LINEAR,
                                cv::BORDER_CONSTANT,
                                cv::Scalar(255));
            qDebug()<<"end cal warpAffine"<<QTime::currentTime().second()<<QTime::currentTime().msec();

    } else {
        To = mat;
    }
*/

}


RS_Vector CD_Application::actionIdentify(int camera,int index) {

    CCD_Mark *mark1 = marks[which_y].at(index);

    RS_Vector resultP(false);
    if(!mark1->getTemplateImage()->isValid()) {
        return resultP;
    }

    CD_Template tmp;

    qDebug()<<"start"<<QTime::currentTime().msec();
    cv::Mat camera_mat;
    if( !capture->getCaptureDataMatDirect(&camera_mat,camera) ) {
        if(run_without_camera)
            camera_mat = *mainscreens[camera]->getCurrentImageMat();
        else {
            mainwidget->doLog("获取相机图像失败");
            return resultP;
        }
    }
    //RS_DEBUG->printImage(camera_mat,"get image");
    if(mark1->isShapeMode() && !mark1->getShapeBaseMatchFlag()) {

        double angle;
        cv::RotatedRect rect;
        RS_Vector PPP;
        resultP = mark1->actionIdentifyShapes( camera_mat,mainscreens[camera],\
                                              &PPP,&angle,&rect,&tmp,false );

    } else {
        resultP = mark1->actionIdentify( camera_mat,mainscreens[camera],&tmp);
    }

    qDebug()<<"end"<<QTime::currentTime().msec();

    mainscreens[camera]->update();
    return resultP;

}


void CD_Application::clearMainScreenCoordinate() {

    QStringList sl;
    QList<QColor> cl;
    sl.append(("0.000 0.000"));
    cl.append(QColor(255,0,0));
    sl.append(("0.000 0.000"));
    cl.append(QColor(255,0,0));
    sl.append(("0.000 0.000"));
    cl.append(QColor(255,0,0));
    sl.append((""));
    cl.append(QColor(255,0,0));
    for(int i=0;i<CCD_CAMERA_COUNT;i++) {
        mainscreens[i]->drawString(sl,cl);
        mainscreens[i]->update();
    }

}


void CD_Application::setMainWidget(CCD_MainWidget *wdgt) {

    mainwidget = wdgt;
    Qt::WindowFlags flags ;
    for(int i=0;i<CCD_CAMERA_COUNT;i++) {
        flags = sysparametereditor[i]->windowFlags();
        sysparametereditor[i]->setParent(mainwidget->getTopWidget());
        sysparametereditor[i]->setWindowFlags(flags);
        sysparametereditor[i]->hide();
    }

    flags = markeditor->windowFlags();
    markeditor ->setParent(mainwidget->getTopWidget());
    markeditor->setWindowFlags(flags);
    markeditor->hide();

    flags = syscommonpareditor->windowFlags();
    syscommonpareditor->setParent(mainwidget->getTopWidget());
    syscommonpareditor->setWindowFlags(flags);
    syscommonpareditor->hide();


}

void CD_Application::saveMainScreenImage(int i,const QString &file) {
    mainscreens[i]->saveMatImage(file);
}

void CD_Application::showInAuxScreen(int camera,int index,cv::Mat *mat) {

    if(  camera<0 ||  camera>= CCD_CAMERA_COUNT ) return;
    if( index>=0 && index<auxscreens[camera].size()) {
        auxscreens[camera].at(index)->showMatImage(*mat);
    }

}

void CD_Application::showInMainScreen(int i,  QString &filename) {
    if(i<0 ||  i>= CCD_CAMERA_COUNT) return;
    //qDebug()<<"CD_Application::showInMainScreen"<<i;
    mainscreens[i]->showMatImage(filename);
}

void CD_Application::showInMainScreen(int i, cv::Mat &mat) {
    if(i<0 ||  i>= CCD_CAMERA_COUNT) return;
    //qDebug()<<"CD_Application::showInMainScreen mat"<<i;
    mainscreens[i]->showMatImage(mat);
}

void CD_Application::showInRealTime(int camera,cv::Mat &mat,bool status) {

    int i = camera;
    if(i<0 ||  i>= CCD_CAMERA_COUNT) return;

    if(!status) {
        mainscreens[i]->drawString(3,QColor(255,0,0),"相机通讯出错");
        mainscreens[i]->update();
        return;
    } else mainscreens[i]->drawString(3,QColor(255,0,0),"");

    if(appstate.state == CD_AppState::StateIdle ) {
        mainscreens[i]->showMatImage(mat);
    } else if(appstate.state == CD_AppState::StateEditFeature) {
        markeditor->showInRealTime(i,mat);
    } else if(appstate.state == CD_AppState::StateEditeSysParameter) {

//        if(edit_camera_par_index == i ) {
            sysparametereditor[1]->showInRealTime(mat);
//        }

    }

}

void CD_Application::initScreen() {

    showInAuxScreen(0,0,marks[0].at(0)->getTemplateImage()->returnAsMat());
    showInAuxScreen(0,1,marks[0].at(1)->getTemplateImage()->returnAsMat());
    showInAuxScreen(0,2,marks[0].at(2)->getTemplateImage()->returnAsMat());
    showInAuxScreen(0,3,marks[0].at(3)->getTemplateImage()->returnAsMat());

}

void CD_Application::localSaveToDisk() {

    CD_IniSettings set(local_conf_file);
    for(int i=0;i<CCD_CAMERA_COUNT;i++) {
        sysparametereditor[i]->saveToDisk(set,QString("SYSPAR_MARK_%1").arg(i));
    }

}

void CD_Application::localReadFromDisk() {

    CD_IniSettings set(local_conf_file);
    for(int i=0;i<CCD_CAMERA_COUNT;i++) {
        sysparametereditor[i]->readFromDisk(set,QString("SYSPAR_MARK_%1").arg(i));
    }

}


void CD_Application::saveToDisk( CD_IniSettings &local_set,CD_IniSettings &mark_set,bool local,bool mark) {


    for(int i=0;i<CCD_CAMERA_COUNT;i++) {

        if(local)
            sysparameter[i]->saveToDisk(local_set);

        if( mark ) {
            for(int j=0;j<marks[i].size();j++)
                marks[i].at(j)->saveToDisk(mark_set,QString("CCD%1_MARKA%2").arg(i).arg(j),true);
        }

    }

    if(local) {
        for(int i=0;i<CCD_CAMERA_COUNT;i++) {
            QString str = QString("GLOABEL_%1").arg(i);
            local_set.beginGroup(str);
            local_set.setValue("CameraGain",camergain[i]);
            local_set.setValue("CameraShutter", shutter[i]);
            local_set.setValue("CameraBLackLevel",blacklevel[i]);
            local_set.setValue("CameraMirrorH",mirrorH[i]);
            local_set.setValue("CameraMirrorV",mirrorV[i]);
            local_set.setValue("CameraResolution",imgresolution[i]);
            CD_Parameter::commonSettingSaveToDisk(local_set);
            local_set.endGroup();
        }
    }

}

void CD_Application::readFromDisk( CD_IniSettings &local_set,CD_IniSettings &mark_set,bool local) {

    for(int i=0;i<CCD_CAMERA_COUNT;i++) {

        if(local)
            sysparameter[i]->readFromDisk(local_set);

        for(int j=0;j<marks[i].size();j++) {
            marks[i].at(j)->readFromDisk(mark_set,QString("CCD%1_MARKA%2").arg(i).arg(j),true);
            showInAuxScreen(i,j,marks[i].at(j)->getTemplateImage()->returnAsMat());
        }

    }

    if(local) {

        for(int i=0;i<CCD_CAMERA_COUNT;i++) {
            QString str = QString("GLOABEL_%1").arg(i);
            local_set.beginGroup(str);
            camergain[i] = local_set.value("CameraGain",10).toInt();
            shutter[i] = local_set.value("CameraShutter", 20).toInt();
            blacklevel[i] = local_set.value("CameraBLackLevel",128).toInt();
            mirrorH[i] = local_set.value("CameraMirrorH",false).toBool();
            mirrorV[i] = local_set.value("CameraMirrorV",false).toBool();
            imgresolution[i] = local_set.value("CameraResolution",0).toInt();

            CD_Parameter::commonSettingReadFromDisk(local_set);
            local_set.endGroup();

            capture->setCameraGain(camergain[i],i);
            capture->setCameraShutter(shutter[i],i);
            capture->setCameraContrast(blacklevel[i] ,i);
            capture->setCameraMirror(0,mirrorH[i],i);
            capture->setCameraMirror(1,mirrorV[i],i);
            capture->setCameraResolution(imgresolution[i],i);

        }

    }



}

void CD_Application::setExcuteFiles(const QString &sim, const QString &locate) {

    excute_file_simulation = sim;
    excute_file_location = locate;

}

void CD_Application::setCapture(CD_CaptureImage *c) {

    capture = c;
    markeditor->setCapture(c);
    for(  int i=0;  i<CCD_CAMERA_COUNT; i++ ) {
        sysparametereditor[i] ->setCapture(c);
    }

}

void CD_Application::setCncManager(CCD_CncManager *m) {

    cncmanager = m;
    cncmanager->addListener("AP",this);
    for(int i=0;i<CCD_CAMERA_COUNT;i++)
        sysparametereditor[i]->setCncManager(m,i);
    markeditor->setCncManager(m);

}


void CD_Application::setPosCondition(int camera,int index) {

    if(index<0 || index>=marks[camera].size()) return;

    CCD_Mark *mark1;

    updateWhichY();
    mark1 = marks[which_y].at(index);

    capture->setCameraGain(mark1->getCameraGain(),0);
    capture->setCameraShutter(mark1->getCameraExposeTime(),0);
    capture->setCameraContrast(mark1->getCameraContrast(),0);
    capture->setCameraGamma(mark1->getCameraGamma(),0);

}

void CD_Application::setWhichTemplate(int i) {
    which_y_set = i;
}

void CD_Application::updateWhichY() {


    which_y = which_y_set;

//    if( cncmanager->getParameter(3195) >0) { //y2
//        which_y = 1;
//    } else {
//        which_y = 0;
//    }

}

void CD_Application::updateArrayParameter() {

    qDebug()<<"getParameter()"<<cncmanager->getParameter(4069)
              <<cncmanager->getParameter(4073)
                <<cncmanager->getParameter(4074)
                  <<cncmanager->getParameter(4075)
                    <<cncmanager->getParameter(4076);

    sysparameter[0]->enable_array_identify = cncmanager->getParameter(4069);
    sysparameter[0]->array_with =  cncmanager->getParameter(4073);
    sysparameter[0]->array_height =  cncmanager->getParameter(4074);
    sysparameter[0]->array_distance_hor =  cncmanager->getParameter(4075);
    sysparameter[0]->array_distance_ver =  cncmanager->getParameter(4076);
    syncCommonParameter(0);

}

void CD_Application::m91Notify(QString n) {


    if(n.startsWith("AP-GETCOUNT")) {

        //4077 4078
        updateArrayParameter();
        updateWhichY();

        if( cncmanager->getZaxisNum() == 1)
            cncmanager->setParameter(2000,1);
        else
            cncmanager->setParameter(2000,2);

        int xx = cncmanager->getParameter(4077);
        int yy = cncmanager->getParameter(4078);

        if( xx>0 && yy>0 && sysparameter[which_y]->enable_array_identify) {
            one_piece_mode = true;
            one_piece_num = (yy-1)*(sysparameter[which_y]->array_with) + xx-1;

            qDebug()<<"one_piece_num"<<one_piece_num;

        } else {
            one_piece_mode = false;
        }

        if( sysparameter[which_y]->enable_array_identify  ) {
            cncmanager->setParameter(2007, \
                                     sysparameter[which_y]->array_with * sysparameter[which_y]->array_height);
        }  else {
            cncmanager->setParameter(2007,1);
        }

        cncmanager->setParameter(2005,sysparameter[which_y]->marknum);

        if( one_piece_mode ) {
            cncmanager->setParameter(2011,one_piece_num);
            cncmanager->m91Respond(-1);

        } else {
            cncmanager->m91Respond(0);
        }

        fail_count = 0;
        fail_piece.clear();
        fail_index.clear();
        cncmanager->setParameter(2013,cncmanager->getAbsPos(0));
        cncmanager->setParameter(2014,cncmanager->getAbsPos(1));


    } else  if(n.startsWith("AP-ARRAY")) {

        bool ok;
        int array = n.mid(8).toDouble(&ok);
        if( !ok )  {
            cncmanager->m91Respond(-1);
            return;
        }
        array_count = array;

        cncmanager->m91Respond(0);

    }

    if(n.startsWith("AP-GOTO")) {

        if( cncmanager->getZaxisNum() == 1)
            cncmanager->setParameter(2005,1);
        else
            cncmanager->setParameter(2005,2);

        int num = location_num;
        if(num>=0 && num< marks[which_y].size()) {

           CCD_Mark *mark = marks[which_y].at(num);
           double x = mark->getLocateX();
           double y = mark->getLocateY();
           double z = sysparameter[which_y]->camera_height;
           cncmanager->setParameter(2000,x);
           cncmanager->setParameter(2001,y);
           cncmanager->setParameter(2002,z);
           cncmanager->setParameter(2003,cncmanager->getAbsPos(0));
           cncmanager->setParameter(2004,cncmanager->getAbsPos(1));

           cncmanager->m91Respond(0);
           setPosCondition(0,num);
           mainwidget->displayIdentifyMark(num);

        } else {
            cncmanager->m91Respond(-1);
        }

    }  if(n.startsWith("AP-L")) {

        bool ok;
        int num = n.mid(4).toDouble(&ok);
        if( !ok )  {
            cncmanager->m91Respond(-1);
            return;
        }

        if(num>=0 && num< marks[which_y].size()) {

           double x,y,z;

           CCD_Mark *mark = marks[which_y].at(num);
           x = mark->getLocateX();
           y = mark->getLocateY();
           z = sysparameter[which_y]->camera_height;

           qDebug()<<"array-----count"<<array_count;
           if(sysparameter[which_y]->enable_array_identify && \
                 array_count > 0  ) {

               int index = array_count - 1;
               if( index<0 || index>= array_locations[num].size()) {
                   mainwidget->doLog(QString("阵列 %1 坐标出错!!! 没有记录的阵列标").arg(array_count));
                   cncmanager->m91Respond(-1);
                   return;
               }
               x = array_locations[num].at(index).x();
               y = array_locations[num].at(index).y();

           }

           cncmanager->setParameter(2000,x);
           cncmanager->setParameter(2001,y);
           cncmanager->setParameter(2002,z);

           if( one_piece_mode && \
                   sysparameter[which_y]->enable_array_identify) {

               if(one_piece_num == array_count) {
                    setPosCondition(0,num);
                   cncmanager->m91Respond(0);
                   mainwidget->displayIdentifyMark(num);
               } else {
                   cncmanager->m91Respond(-1);
               }

           } else {
                setPosCondition(0,num);
                cncmanager->m91Respond(0);
                mainwidget->displayIdentifyMark(num);
           }


        } else {
            cncmanager->m91Respond(-1);
        }

    } if(n.startsWith("AP-I")) {

        bool ok;
        int num = n.mid(4).toDouble(&ok);
        if( !ok )  {
            cncmanager->m91Respond(-1);
            return;
        }
        if( num>=0 && num < 4 ) {


            if( one_piece_mode && \
                    sysparameter[which_y]->enable_array_identify) {
                if(one_piece_num == array_count) {
                } else {
                    cncmanager->m91Respond(-2);
                    return;
                }
            }

                    id_img_pos[num] = actionIdentify(0,num);
                    if( ! id_img_pos[num].valid ) {

                        bool exist = false;
                        for(int k=0;k<fail_piece.size();k++) {
                            if( fail_piece[k] == array_count) {
                                exist = true;
                            }
                        }
                        fail_piece.append(array_count);
                        fail_index.append(num);

                        mainwidget->doLog(QString("识别 %1 位置失败").arg(num+1));

                        if(sysparameter[which_y]->sample_num == 0) {
                            cncmanager->m91Respond(-1);
                            return;
                        }
                        if(!sysparameter[which_y]->enable_array_identify) {
                            cncmanager->m91Respond(-1);
                            return;
                        }

                        //   -2 continue   -1 stop
                        if(!exist) {

                            fail_count ++;
                            if( fail_count >= sysparameter[which_y]->sample_num) {

                                int ret = QMessageBox::warning(mainwidget->getTopWidget(),QObject::tr("Warnning"),QObject::tr("识别失败次数超过设定值，是否继续加工?"),
                                                     QMessageBox::Yes,QMessageBox::No);

                                if( ret == QMessageBox::No ) {
                                    cncmanager->m91Respond(-1);
                                } else {
                                    cncmanager->m91Respond(-2);
                                }
                            } else {
                                cncmanager->m91Respond(-2);
                            }
                        } else {
                            cncmanager->m91Respond(-2);
                        }




            } else {


                RS_Vector realp;
                realp.x = marks[which_y][num]->getLocateX();
                realp.y = marks[which_y][num]->getLocateY();

                if(sysparameter[which_y]->enable_array_identify \
                        && array_count > 0) {

                    int index = array_count - 1;
                    if( index<0 || index>= array_locations[num].size()) {
                        mainwidget->doLog(QString("阵列 %1 坐标出错3!!! ").arg(array_count));
                        cncmanager->m91Respond(-1);
                        return ;
                    }
                    realp.x = array_locations[num].at(index).x();
                    realp.y = array_locations[num].at(index).y();
                }
                RS_Vector P1 = transFormPoints(id_img_pos[num],realp,sysparameter[which_y]);
                mainscreens[0]->drawString(num,QColor(0,255,0),QString("%1 %2").arg(P1.x).arg(P1.y));
                cncmanager->m91Respond(0);


            }
        } else {
            cncmanager->m91Respond(-1);
        }

    } else if(n.startsWith("AP-GetDelayTime")) {
        cncmanager->setParameter(2006,sysparameter[which_y]->getimg_delaytime/1000.0);
        cncmanager->m91Respond(0);
    } else if(n.startsWith("AP-Calculate")) {


        if( one_piece_mode && \
                sysparameter[which_y]->enable_array_identify) {
            if(one_piece_num == array_count) {
            } else {
                cncmanager->m91Respond(0);
                return;
            }
        }


        if(!calculate())
            cncmanager->m91Respond(-1);
        else if(one_piece_mode)
            cncmanager->m91Respond(-2);
        else
            cncmanager->m91Respond(0);

    } else if(n.startsWith("AP-SetCameraParDown")) {

        cncmanager->m91Respond(0);

    } else if(n.startsWith("AP-SetCameraParUp")) {

        cncmanager->m91Respond(0);

    } else if(n.startsWith("AP-SetFirstTemp")) {

        mainwidget->setWhitchTempalte(0);
        cncmanager->m91Respond(0);


    } else if(n.startsWith("AP-SetSecondTemp")) {

        mainwidget->setWhitchTempalte(1);

        cncmanager->m91Respond(0);


    }

}


void CD_Application::simulateIdentify() {

    updateWhichY();

//#ifdef __USING_LINUX__
    QFile file(":runfile/simulateidentify.ngc");
    QFile fileold("/tmp/simulateidentify.ngc");
    fileold.remove();
    file.copy("/tmp/simulateidentify.ngc");
//    cncmanager->excuteFile("/tmp/simulateidentify.ngc",-1);
//#else
    cncmanager->excuteFile(excute_file_simulation,-1);
//#endif


}

void CD_Application::gotoLocation(int index) {

    if(index >= sysparameter[0]->marknum) return;

    location_num = index;

//#ifdef __USING_LINUX__
    QFile file(":runfile/locate.ngc");
    QFile fileold("/tmp/locate.ngc");
    fileold.remove();
    file.copy("/tmp/locate.ngc");
//    cncmanager->excuteFile("/tmp/locate.ngc",-1);
//#else
    cncmanager->excuteFile(excute_file_location,-1);
//#endif


}

RS_Vector CD_Application::transFormPoints(RS_Vector &v,RS_Vector &real,
                                          CCD_SysParameter *par) {


    double x_t;
    double y_t;
    double newp_x;
    double newp_y;

    x_t = v.x;
    y_t = v.y;

    double orgw = mainscreens[0]->getCurrentImageMat()->cols;
    double orgh = mainscreens[0]->getCurrentImageMat()->rows;
    orgw /=2.0;
    orgh /=2.0;
    x_t -= orgw;
    y_t -= orgh;
    y_t = -y_t;

    newp_x = x_t/par->camera_magx;
    newp_y = y_t/par->camera_magy;

    double t = par->offset_angle;
    double X = - newp_x*cos(t) -newp_y*sin(t);
    double Y = newp_x*sin(t) - newp_y*cos(t);


    double curX = real.x;
    double curY = real.y;

    curX -= X;
    curY -= Y;

    curX += par->global_offset_h;
    curY += par->global_offset_v;

    return RS_Vector(curX,curY);

}

bool CD_Application::calculate() {


     if(sysparameter[which_y]->camera_magx == 0 || \
             sysparameter[which_y]->camera_magy == 0) {
         mainwidget->doLog("错误：放大倍率为零!");
         return false;
     }


    //QString str;
    RS_Vector realp[4];

    realp[0].x = marks[which_y][0]->getLocateX();
    realp[0].y = marks[which_y][0]->getLocateY();
    realp[1].x = marks[which_y][1]->getLocateX();
    realp[1].y = marks[which_y][1]->getLocateY();
    realp[2].x = marks[which_y][2]->getLocateX();
    realp[2].y = marks[which_y][2]->getLocateY();
    realp[3].x = marks[which_y][3]->getLocateX();
    realp[3].y = marks[which_y][3]->getLocateY();

    if(sysparameter[which_y]->enable_array_identify \
            && array_count > 0) {

        for(int num=0;num<4;num++) {
            int index = array_count - 1;
            if( index<0 || index>= array_locations[num].size()) {
                mainwidget->doLog(QString("阵列 %1 坐标出错2!!! ").arg(array_count));
                cncmanager->m91Respond(-1);
                return false;
            }
            realp[num].x = array_locations[num].at(index).x();
            realp[num].y = array_locations[num].at(index).y();
        }

    }

    RS_Vector P1 = transFormPoints(id_img_pos[0],realp[0],sysparameter[which_y]);
    RS_Vector P2 = transFormPoints(id_img_pos[1],realp[1],sysparameter[which_y]);
    RS_Vector P3 = transFormPoints(id_img_pos[2],realp[2],sysparameter[which_y]);
    RS_Vector P4 = transFormPoints(id_img_pos[3],realp[3],sysparameter[which_y]);

    //str.sprintf("P1 %.3f %.3f  P2 %.3f %.3f",P1.x,P1.y,P2.x,P2.y);
    //mainwidget->doLog(str);
    //str.sprintf("P3 %.3f %.3f  P4 %.3f %.3f",P3.x,P3.y,P4.x,P4.y);
    //mainwidget->doLog(str);

    bool ret = false;
    double first_angle = 0;

    if(sysparameter[which_y]->marknum == 1) {


        double angle_avg = (sysparameter[which_y]->compute_rotate_offset * M_PI)/180.0;

        mainwidget->displayResultAngle(sysparameter[which_y]->compute_rotate_offset);
        mainwidget->displayResultOffset(P1.x,P1.y);

        double x = sysparameter[which_y]->correct_offset_x;
        double y = sysparameter[which_y]->correct_offset_y;
        cncmanager->setParameter(2000,P1.x + x);
        cncmanager->setParameter(2001,P1.y + y);

        cncmanager->setParameter(2003,sysparameter[which_y]->compute_rotate_offset);

        first_angle = 0;

        ret = true;

    } else if(sysparameter[which_y]->marknum == 2) {

        double offset_angle;
        if( sysparameter[which_y]->two_points_w == 0.0 ) {
            offset_angle = M_PI_2 *3;
        } else if(sysparameter[which_y]->two_points_h == 0.0) {
            offset_angle = 0.0;
        } else {
            offset_angle = RS_Math::correctAngle( atan2(-sysparameter[which_y]->two_points_h,\
                    sysparameter[which_y]->two_points_w) );
        }

        qDebug()<<"offset_angle"<<offset_angle;
        double angle_act = P1.angleTo(P2);
        double angle_avg = RS_Math::correctAngle (RS_Math::angleDiff(angle_act,offset_angle) );
        double t = angle_avg;

        angle_avg = RS_Math::correctAngle(angle_avg + \
                                          (sysparameter[which_y]->compute_rotate_offset * M_PI)/180.0 );

        RS_Vector mid_point;
        RS_Line mid_Line(P1,P2);
        mid_point = mid_Line.getMiddlePoint();

        double x = sysparameter[which_y]->correct_offset_x;
        double y = sysparameter[which_y]->correct_offset_y;
        double x0;
        double y0;

        if(sysparameter[which_y]->origin_pos == 0) {
            x0 = mid_point.x;
            y0 = mid_point.y;
        } else {
            x0 = P1.x;
            y0 = P1.y;
        }
        double finalx = x*cos(t)-y*sin(t)+x0;
        double finaly = x*sin(t)+y*cos(t)+y0;


        if(  fabs( RS_Math::angleDiff(0,angle_avg) ) > sysparameter[which_y]->correct_max_a  ) {
            mainwidget->doLog(QString("角度摆放超过最大值 %1").arg(angle_avg*180/M_PI));
            ret =  false;
        } else {
            ret = true;
        }

        mainwidget->displayResultAngle(RS_Math::angleDiff(angle_avg,0));
        mainwidget->displayResultOffset(finalx,finaly);

        cncmanager->setParameter(2000,finalx);
        cncmanager->setParameter(2001,finaly);
        cncmanager->setParameter(2003,(angle_avg*180.0)/M_PI);

        first_angle = angle_avg;

    } else if(sysparameter[which_y]->marknum == 3) {

        ret =  calculateFromThreePoints(P1,P2,P3);

    } else if(sysparameter[which_y]->marknum == 4) {
        ///todo FIXME
        mainwidget->doLog("DO NOT SPPORT");
        return false;
    }

    //calculate the location of array
    if(sysparameter[which_y]->enable_array_identify \
            && array_count == 0 && !one_piece_mode) {

        int w = sysparameter[which_y]->array_with;
        int h = sysparameter[which_y]->array_height;
        double disw = sysparameter[which_y]->array_distance_hor;
        double dish = sysparameter[which_y]->array_distance_ver;

        double origin_x[4];
        double origin_y[4];

        origin_x[0] = P1.x -  sysparameter[which_y]->global_offset_h;
        origin_y[0] = P1.y -  sysparameter[which_y]->global_offset_v;
        origin_x[1] = P2.x -  sysparameter[which_y]->global_offset_h;
        origin_y[1] = P2.y -  sysparameter[which_y]->global_offset_v;
        origin_x[2] = P3.x -  sysparameter[which_y]->global_offset_h;
        origin_y[2] = P3.y -  sysparameter[which_y]->global_offset_v;
        origin_x[3] = P4.x -  sysparameter[which_y]->global_offset_h;
        origin_y[3] = P4.y -  sysparameter[which_y]->global_offset_v;

       array_locations[0].clear();
       array_locations[1].clear();
       array_locations[2].clear();
       array_locations[3].clear();

        for(int i = 0; i < h; i++) {
            for(int j = 0; j < w; j++) {

                if( i==0 && j==0) continue;

                for( int kk = 0; kk < 4; kk++ ) {

                    double x,y,t = first_angle;

                    QPointF offset(disw*j,dish*i);
                    QPointF tmp;

                    tmp = offset;
                    x = tmp.x() * cos(t) - tmp.y() * sin(t) + origin_x[kk];
                    y = tmp.x() * sin(t) + tmp.y() * cos(t) + origin_y[kk];

                    tmp.setX(x);
                    tmp.setY(y);
                    array_locations[kk].append(tmp);

                }

            }
        }


    }

    return ret;

}

bool CD_Application::calculateFromThreePoints(RS_Vector &P1, RS_Vector &P2, RS_Vector &P3) {

/*
    P1.x = 161.936;
    P1.y = -5.023;

    P2.x = 474.717;
    P2.y = -20.495;

    P3.x = 455.652;
    P3.y = -406.083;
*/
    double angle1 = P1.angleTo(P2);
    double angle2 = P3.angleTo(P2);
    angle2 = RS_Math::correctAngle(angle2 - M_PI_2);

    //mainwidget->doLog(QString("angle 1 2 = %1 %2").arg(angle1).arg(angle2));

    double angle[2];
    angle[0] = angle1;
    angle[1] = angle2;

    double angle_avg = RS_Math::angleAvg(angle,2);
    RS_Vector mid_point;

    RS_Line mid_Line(P1,P3);
    mid_point = mid_Line.getMiddlePoint();

    //mainwidget->doLog(QString("Angle = %1").arg(angle_avg));
    //mainwidget->doLog(QString("Points = %1 %2").arg(mid_point.x).arg(mid_point.y));


    ///need transfer to actual angle and offset
    /// x=x'cost-y'sint+x0,  y=x'sint+y'cost+y0.
    /// depend of the position of set
    double t = angle_avg;
    double x = sysparameter[which_y]->correct_offset_x;
    double y = sysparameter[which_y]->correct_offset_y;

    //double x0 = mid_point.x;
    //double y0 = mid_point.y;
    double x0;
    double y0;

    if(sysparameter[which_y]->origin_pos == 0) {
        x0 = mid_point.x;
        y0 = mid_point.y;
    } else {
        x0 = P1.x;
        y0 = P1.y;
    }

    double finalx = x*cos(t)-y*sin(t)+x0;
    double finaly = x*sin(t)+y*cos(t)+y0;

    //mainwidget->doLog(QString(" %1 %2").arg(finalx).arg(finaly));


    if(  fabs( RS_Math::angleDiff(0,angle_avg) ) > sysparameter[which_y]->correct_max_a  ) {
        mainwidget->doLog(QString("角度摆放超过最大值 %1").arg(angle_avg*180/M_PI));
        return false;
    }

    ///test fun RS_Math::angleDiff

    angle_avg = RS_Math::correctAngle(angle_avg + \
                                      (sysparameter[which_y]->compute_rotate_offset * M_PI)/180.0 );


    mainwidget->displayResultAngle(RS_Math::angleDiff(angle_avg,0));
    mainwidget->displayResultOffset(finalx,finaly);

    cncmanager->setParameter(2000,finalx);
    cncmanager->setParameter(2001,finaly);
    cncmanager->setParameter(2003,(angle_avg*180.0)/M_PI);

    return true;

}

void CD_Application::calculateOffsetMove( RS_Vector &,RS_Vector &,
                                         RS_Vector &,RS_Vector & )  {

}
