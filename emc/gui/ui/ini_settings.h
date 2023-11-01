/*********************************************************************
**
** author mhc
**
**********************************************************************/

#ifndef INI_SETTINGS_H
#define INI_SETTINGS_H

#include <QHash>
#include <QSettings>
#include <QFile>
#include <QTextStream>
#include <QTextDocument>
#include <QTextCursor>

#define CNC_SET CNC_Settings::instance()

/**
 * This class can store and reload settings from a 
 * configuration file or the windoze registry.
 * Please note that the Qt default implementation doesn't
 * work as one would expect. That's why this class overwrites
 * most of the default behaviour.
 * 
 */
class Ini_Settings {

public:
    Ini_Settings(const QString&);
    bool writeEntry(const QString& key, int value,bool tocach = true);
    bool writeEntry(const QString& key, double value,bool tocach = true);
    bool writeEntry(const QString& key, const QString& value,bool tocach = true);
    bool writeEntry(const QString& groupkey, const QVariant& value,bool tocach = true);

    QString readEntry(const QString& key,
                        const QString& def = QString::null,bool creat = false);
    QString read(const QString& sect,const QString& key,
                        const QString& def = QString::null,bool creat = false);

    QString getIniFilePath();

public:
    ~Ini_Settings();

private:


    void deletePreBlank(QString &str);
    void deleteTailBlank(QString &str);
    QVariant readEntryCache(const QString& key);
    void addToCache(const QString& key, const QVariant& value);
    bool writeToDisk(QString &ini,QString &_section,QString &_key,QString &_value);

protected:

    QHash<QString, QVariant*> cache;
    QString inifilepath;
    bool initialized;
    QTextCodec *code;
    friend class CNC_Settings;
};

class CNC_Settings {

public:
    static CNC_Settings* instance() {
        if(unique == NULL) {
            unique = new CNC_Settings();
        }
         return unique;
    }
    ~CNC_Settings();
    void addIniFile(const QString &file);
    Ini_Settings *getAccess(QString &name);

private:
    CNC_Settings();
    static CNC_Settings* unique;
    QList<Ini_Settings *> listsettings;
    QList<QString> listfiles;

};


#endif

