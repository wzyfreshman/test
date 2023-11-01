#include "actionfactory.h"

#include <QDebug>

ActionFactory* ActionFactory::uniqueInstance = NULL;


My_Action::My_Action(QObject *parent)
    :QAction(parent){
    setAutoRepeat(false);
    connect(this,SIGNAL(triggered()),this,SLOT(slotTrigle()));
    connect(this,SIGNAL(toggled(bool)),this,SLOT(slotTriglebool(bool)));
}

My_Action::My_Action(const QString &text, QObject *parent)
    :QAction(text,parent) {
    setAutoRepeat(false);
    connect(this,SIGNAL(triggered()),this,SLOT(slotTrigle()));
    connect(this,SIGNAL(hovered()),this,SLOT(slotTriglebooln()));
}

My_Action::My_Action(const QIcon &icon,const QString &text, QObject *parent)
    :QAction(icon,text,parent) {
    setAutoRepeat(false);
    connect(this,SIGNAL(triggered()),this,SLOT(slotTrigle()));
    connect(this,SIGNAL(toggled(bool)),this,SLOT(slotTriglebool(bool)));
}

void My_Action::setTrigleValue(int _set) {

    Feedvalue = _set;

}
void My_Action::slotTriglebool(bool ) {

}
void My_Action::slotTriglebooln() {

}
void My_Action::slotTrigle() {

    emit trigleValue(Feedvalue);

}

ActionFactory* ActionFactory::instance() {

    if (uniqueInstance==NULL) {
        uniqueInstance = new ActionFactory();
    }
    return uniqueInstance;

}

ActionFactory::ActionFactory()
    :QObject(NULL) {

    QAction *action = NULL;
    action = new QAction(tr("TrigleManual"),this);
    addToCatch(CNC::ActionTrigleManual,action);
    action = new QAction(tr("TrigleMdi"),this);
    addToCatch(CNC::ActionTrigleMdi,action);
    action = new QAction(tr("TrigleProbe"),this);
    addToCatch(CNC::ActionTrigleProbe,action);

    action = new QAction(tr("FocusOnFeedSlider"),this);
    addToCatch(CNC::ActionFocusOnFeedSlider,action);
    action = new QAction(tr("FocusOnSpindleSlider"),this);
    addToCatch(CNC::ActionFocusOnSpindleSlider,action);

    action = new QAction(tr("ActionSpindleDec"),this);
    action->setShortcut(QKeySequence("-"));
    addToCatch(CNC::ActionSpindleDec,action);

    action = new QAction(tr("ActionSpindlePlus"),this);
    QList<QKeySequence> keys;
    keys.append(QKeySequence("+"));
    keys.append(QKeySequence("="));
    action->setShortcuts(keys);
    addToCatch(CNC::ActionSpindlePlus,action);




    My_Action *myact;
    myact = new My_Action(tr("Feed10"),this);
    myact->setTrigleValue(10);
    myact->setShortcut(QKeySequence("1"));
    addToCatch(CNC::ActionFeed10,myact);

    myact = new My_Action(tr("Feed20"),this);
    myact->setTrigleValue(20);
    myact->setShortcut(QKeySequence("2"));
    addToCatch(CNC::ActionFeed20,myact);

    myact = new My_Action(tr("Feed30"),this);
    myact->setTrigleValue(30);
    myact->setShortcut(QKeySequence("3"));
    addToCatch(CNC::ActionFeed30,myact);

    myact = new My_Action(tr("Feed40"),this);
    myact->setTrigleValue(40);
    myact->setShortcut(QKeySequence("4"));
    addToCatch(CNC::ActionFeed40,myact);

    myact = new My_Action(tr("Feed50"),this);
    myact->setTrigleValue(50);
    myact->setShortcut(QKeySequence("5"));
    addToCatch(CNC::ActionFeed50,myact);

    myact = new My_Action(tr("Feed60"),this);
    myact->setTrigleValue(60);
    myact->setShortcut(QKeySequence("6"));
    addToCatch(CNC::ActionFeed60,myact);

    myact = new My_Action(tr("Feed70"),this);
    myact->setTrigleValue(70);
    myact->setShortcut(QKeySequence("7"));
    addToCatch(CNC::ActionFeed70,myact);

    myact = new My_Action(tr("Feed80"),this);
    myact->setTrigleValue(80);
    myact->setShortcut(QKeySequence("8"));
    addToCatch(CNC::ActionFeed80,myact);

    myact = new My_Action(tr("Feed90"),this);
    myact->setTrigleValue(90);
    myact->setShortcut(QKeySequence("9"));
    addToCatch(CNC::ActionFeed90,myact);

    myact = new My_Action(tr("Feed100"),this);
    myact->setTrigleValue(100);
    myact->setShortcut(QKeySequence("0"));
    addToCatch(CNC::ActionFeed100,myact);


    myact = new My_Action(tr("TrigleCoor"),this);
    myact->setTrigleValue(0);
    addToCatch(CNC::ActionTrigleCoor,myact);
    myact = new My_Action(tr("TrigleModel"),this);
    myact->setTrigleValue(1);
    addToCatch(CNC::ActionTrigleModel,myact);
    myact = new My_Action(tr("TrigleInfo"),this);
    myact->setTrigleValue(2);
    addToCatch(CNC::ActionTrigleInfo,myact);
    myact = new My_Action(tr("TrigleIoIn"),this);
    myact->setTrigleValue(3);
    addToCatch(CNC::ActionTrigleIoIn,myact);
    myact = new My_Action(tr("TrigleIoOut"),this);
    myact->setTrigleValue(4);
    addToCatch(CNC::ActionTrigleIoOut,myact);
    myact = new My_Action(tr("TrigleArray"),this);
    myact->setTrigleValue(5);
    addToCatch(CNC::ActionTrigleArray,myact);

}

void ActionFactory::addToCatch(CNC::ActionType id,QAction *add) {

    QAction *action;
    action = actions.value(id);

    if(action == NULL) {
        actions.insert(id,add);
    } else {
        delete action;
        actions.insert(id,add);
    }

}

QAction *ActionFactory::getAction(CNC::ActionType id) {

    return actions.value(id);

}

void ActionFactory::createAction(CNC::ActionType id, QObject *obj, QObject *) {


    QAction *action = NULL;
    QIcon icon;
    switch(id) {

    case CNC::ActionViewX:

        icon.addFile(":/opengl/tool_axis_x.gif");
        action = new QAction(icon,tr("ViewX"),this);
        connect(action,SIGNAL(triggered()),obj,SLOT(viewX()));
        addToCatch(id,action);
        break;

    case CNC::ActionViewY:

        icon.addFile(":/opengl/tool_axis_y.gif");
        action = new QAction(icon,tr("ViewY"),this);
        connect(action,SIGNAL(triggered()),obj,SLOT(viewY()));
        addToCatch(id,action);
        break;

    case CNC::ActionViewZ:

        icon.addFile(":/opengl/tool_axis_z.gif");
        action = new QAction(icon,tr("ViewZ"),this);
        connect(action,SIGNAL(triggered()),obj,SLOT(viewZ()));
        addToCatch(id,action);
        break;

    case CNC::ActionViewZ2:

        icon.addFile(":/opengl/tool_axis_z2.gif");
        action = new QAction(icon,tr("ViewZ2"),this);
        connect(action,SIGNAL(triggered()),obj,SLOT(viewZ2()));
        addToCatch(id,action);
        break;

    case CNC::ActionViewP:

        icon.addFile(":/opengl/tool_axis_p.gif");
        action = new QAction(icon,tr("ViewP"),this);
        connect(action,SIGNAL(triggered()),obj,SLOT(viewP()));
        addToCatch(id,action);
        break;

    case CNC::ActionPathClear:

        icon.addFile(":/opengl/tool_clear.gif");
        action = new QAction(icon,tr("ViewClear"),this);
        connect(action,SIGNAL(triggered()),obj,SLOT(clear()));
        addToCatch(id,action);
        break;

    case CNC::ActionModelReload:

        icon.addFile(":/opengl/tool_reload.gif");
        action = new QAction(icon,tr("ViewReload"),this);
        connect(action,SIGNAL(triggered()),obj,SLOT(reload()));
        addToCatch(id,action);
        break;

    case CNC::ActionZoomIn:

        icon.addFile(":/opengl/tool_zoomin.gif");
        action = new QAction(icon,tr("ViewZoomIn"),this);
        connect(action,SIGNAL(triggered()),obj,SLOT(zoomIn()));
        addToCatch(id,action);
        break;

    case CNC::ActionZoomOut:

        icon.addFile(":/opengl/tool_zoomout.gif");
        action = new QAction(icon,tr("ViewZoomOut"),this);
        connect(action,SIGNAL(triggered()),obj,SLOT(zoomOut()));
        addToCatch(id,action);
        break;

    case CNC::ActionLoadAbort:
        icon.addFile(":/opengl/tool_zoomout.gif");
        action = new QAction(icon,tr("ViewZoomOut"),this);
        connect(action,SIGNAL(triggered()),obj,SLOT(abortLoad()));
        addToCatch(id,action);
        break;

    case CNC::ActionSaveCoor1:

        action = new QAction(this);
        addToCatch(id,action);
        break;

    case CNC::ActionSaveCoor2:

        action = new QAction(this);
        addToCatch(id,action);
        break;

    case CNC::ActionSaveCoor3:

        action = new QAction(this);
        addToCatch(id,action);
        break;

    case CNC::ActionSaveCoor4:

        action = new QAction(this);
        addToCatch(id,action);
        break;

    case CNC::ActionSaveCoor5:

        action = new QAction(this);
        addToCatch(id,action);
        break;

    case CNC::ActionSaveCoor6:

        action = new QAction(this);
        addToCatch(id,action);
        break;

    case CNC::ActionSaveCoor7:

        action = new QAction(this);
        addToCatch(id,action);
        break;

    case CNC::ActionSaveCoor8:

        action = new QAction(this);
        addToCatch(id,action);
        break;

    case CNC::ActionSaveCoor9:

        action = new QAction(this);
        addToCatch(id,action);
        break;

    case CNC::ActionSaveCoor10:

        action = new QAction(this);
        addToCatch(id,action);
        break;

    case CNC::ActionLoadCoor1:

        action = new QAction(this);
        addToCatch(id,action);
        break;

    case CNC::ActionLoadCoor2:

        action = new QAction(this);
        addToCatch(id,action);
        break;

    case CNC::ActionLoadCoor3:

        action = new QAction(this);
        addToCatch(id,action);
        break;

    case CNC::ActionLoadCoor4:

        action = new QAction(this);
        addToCatch(id,action);
        break;

    case CNC::ActionLoadCoor5:

        action = new QAction(this);
        addToCatch(id,action);
        break;

    case CNC::ActionLoadCoor6:

        action = new QAction(this);
        addToCatch(id,action);
        break;

    case CNC::ActionLoadCoor7:

        action = new QAction(this);
        addToCatch(id,action);
        break;

    case CNC::ActionLoadCoor8:

        action = new QAction(this);
        addToCatch(id,action);
        break;

    case CNC::ActionLoadCoor9:

        action = new QAction(this);
        addToCatch(id,action);
        break;

    case CNC::ActionLoadCoor10:

        action = new QAction(this);
        addToCatch(id,action);
        break;


    default:
        break;

    }


}
