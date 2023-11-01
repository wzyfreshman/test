/****************************************************************************
**
** This file is part of the LibreCAD project, a 2D CAD program
**
** Copyright (C) 2010 R. van Twisk (librecad@rvt.dds.nl)
** Copyright (C) 2001-2003 RibbonSoft. All rights reserved.
**
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software 
** Foundation and appearing in the file gpl-2.0.txt included in the
** packaging of this file.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
** 
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**
** This copyright notice MUST APPEAR in all copies of the script!  
**
**********************************************************************/

// RVT_PORT changed QSettings s(QSettings::Ini) to QSettings s("./qcad.ini", QSettings::IniFormat);

#include "ini_settings.h"

#include <QTextCodec>

Ini_Settings::Ini_Settings(const QString &_inifilepath) {

    inifilepath = _inifilepath;
    initialized = true;
    code = QTextCodec::codecForName("utf-8");
}

/**
 * Destructor
 */
Ini_Settings::~Ini_Settings() {
    while (!cache.isEmpty()) {
        QVariant *value = *cache.begin();
        cache.erase(cache.begin());
        delete value;
    }
}


bool Ini_Settings::writeEntry(const QString& groupkey, int value,bool tocach ) {
    return writeEntry(groupkey, QVariant(value),tocach);
}

bool Ini_Settings::writeEntry(const QString& groupkey,const QString& value,bool tocach ) {
    return writeEntry(groupkey, QVariant(value),tocach);
}

bool Ini_Settings::writeEntry(const QString& groupkey, double value,bool tocach ) {
    return writeEntry(groupkey, QVariant(value),tocach);
}
bool Ini_Settings::writeEntry(const QString &groupkey, const QVariant& value,bool tocach ) {

    QStringList list = groupkey.split("/",QString::SkipEmptyParts);
    if(list.size() < 2)
        return false;

    QString sect = list.at(0);
    QString key = list.at(1);
    QString v = value.toString();
    if(!writeToDisk(inifilepath,sect,key,v))
        return false;

    QString ckey = QString("%1:%2").arg(sect).arg(key);

    if(tocach) {
        //qDebug("add to catch:%s--%s",ckey.toUtf8().data(),value.toString().toUtf8().data());
        addToCache(ckey, value);
    }

    return true;

}

QString Ini_Settings::read(const QString &sect, const QString &key, const QString &def, bool creat) {

    QString tt = QString("%1/%2").arg(sect).arg(key);
    return readEntry(tt,def,creat);

}


QString Ini_Settings::readEntry(const QString &groupkey,
                                         const QString &def,bool creat) {

    QStringList list = groupkey.split("/",QString::SkipEmptyParts);
    if(list.size() < 2)
        return def;

    QString _sect = list.at(0);
    QString _key = list.at(1);

    QString ckey = QString("%1:%2").arg(_sect).arg(_key);
    QVariant ret = readEntryCache(ckey);

    if (!ret.isValid()) {

        QSettings s(inifilepath, QSettings::IniFormat);
        ret = s.value(groupkey, QVariant("__NOT__OK"));

        if(ret.toString() == "__NOT__OK") {
            if(creat) {
                //qDebug("creatting......");
                writeEntry(groupkey,def);
                ret = QVariant(def);
            } else {
                ret = QVariant(def);
                addToCache(ckey, ret);
            }

        } else {

            addToCache(ckey, ret);
            //qDebug("readfromfile_in_readcreatfun......");
        }

    } else {
        //qDebug("catching_in_readcreatfun......");
    }

    return ret.toString().toLatin1();

}


QString Ini_Settings::getIniFilePath() {

    return inifilepath;

}


bool Ini_Settings::writeToDisk(QString &ini,QString &_section,QString &_key,QString &_value) {

    QFile file(ini);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        return false;
    }

    QTextStream instream(&file);
    QString allstr,read;
    QString section,key,value,tmp;

    if(_section.isEmpty() || _key.isEmpty())
        return false;

    section = QString("[%1]").arg(_section);
    key = _key;
    value = _value;
    allstr = instream.readAll();

    QTextDocument doc(allstr);
    QTextCursor modify(&doc);
    QStringList readstrlist;

    bool sectionexist = false;
    bool keyexist = false;
    int sectionline = 0;
    int linecount = 0;
    for(modify.movePosition(QTextCursor::Start);
        !modify.atEnd();
        modify.movePosition(QTextCursor::Down)) {
        modify.select(QTextCursor::LineUnderCursor);
        read=modify.selectedText();
        linecount ++;
        deletePreBlank(read);

        readstrlist = read.split("=",QString::SkipEmptyParts);
        if(readstrlist.size())
            read = readstrlist.at(0);
        else
            read = "";
        deleteTailBlank(read);

        if( read.contains(section)) {
            sectionline = linecount;
            sectionexist = true;
            //qDebug("continue ----%d",sectionline);
            continue;
        }

        if(sectionexist == true ) {
          if(read == key) {

            keyexist  = true;
            modify.removeSelectedText();
            tmp = QString("%1=%2").arg(key).arg(value);
            //qDebug("key exist ----%s",tmp.toUtf8().data());
            modify.insertText(tmp);
            break;
          }
          if(read.startsWith("[")) break;
        }

    }


    if(!sectionexist) {
        //qDebug("no section ----");
        modify.movePosition(QTextCursor::End);
        modify.insertText("\n\n");
        section.append('\n');
        modify.insertText(section);
        tmp = QString("%1=%2").arg(key).arg(value);
        modify.insertText(tmp);

    } else if(sectionexist && !keyexist) {

        //qDebug("has section but no key %d",sectionline);
        modify.movePosition(QTextCursor::Start);
        modify.movePosition(QTextCursor::Down,QTextCursor::MoveAnchor,sectionline);
        modify.movePosition(QTextCursor::Down);
        tmp = QString("%1=%2\n").arg(key).arg(value);
        modify.insertText(tmp);
    }

    file.close();
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    file.write(doc.toPlainText().toUtf8().data());
    file.close();
    return true;

}

void Ini_Settings::deletePreBlank(QString &str) {

    while(!str.isEmpty()) {
        if(str.at(0).isSpace())
            str.remove(0,1);
        else
            break;
    }

}

void Ini_Settings::deleteTailBlank(QString &str) {

    while(!str.isEmpty()) {

        if(str.endsWith(" "))
            str.chop(1);
        else
            break;
    }

}

QVariant Ini_Settings::readEntryCache(const QString& key) {
       QVariant *s = cache.value(key);
        if (s == NULL)
            return QVariant();
        else
            return *s;
}

void Ini_Settings::addToCache(const QString& key, const QVariant& value) {
        delete cache.take(key);
        cache.insert(key, new QVariant(value));
}



CNC_Settings *CNC_Settings::unique = NULL;

CNC_Settings::CNC_Settings() {

}


CNC_Settings::~CNC_Settings() {

    while(!listsettings.isEmpty()) {
       Ini_Settings *tmp = listsettings.takeFirst();
       delete tmp;
    }

}

void CNC_Settings::addIniFile(const QString &file) {


    listsettings.append(new Ini_Settings(file));
    listfiles.append(file);

}

Ini_Settings * CNC_Settings::getAccess(QString &name) {

    for(int i=0;i<listfiles.size();i++) {

        if(listfiles.at(i) == name)
            return listsettings.at(i);

    }
    return NULL;

}
