#ifndef ACTIONFACTORY_H
#define ACTIONFACTORY_H

#include <QAction>
#include <QList>

#include "actiontype.h"

class My_Action :public QAction
{

    Q_OBJECT

public:
    My_Action( QObject * parent );
    My_Action(const QString & text, QObject * parent );
    My_Action(const QIcon &icon, const QString &text, QObject* parent);
    ~My_Action(){}

    void setTrigleValue(int);

public slots:
    void slotTrigle();
    void slotTriglebool(bool);
    void slotTriglebooln();
signals:
    void trigleValue(int);

private:
    int Feedvalue;

};

class ActionFactory : public QObject
{
    Q_OBJECT
public:
    ActionFactory();
    static ActionFactory* instance();

    void createAction(CNC::ActionType id,QObject* obj,QObject* obj2);
    QAction *getAction(CNC::ActionType id);
private:

    void addToCatch(CNC::ActionType,QAction*);
    QHash<CNC::ActionType, QAction*> actions;

protected:
    static ActionFactory* uniqueInstance;

};

#endif // ACTIONFACTORY_H
