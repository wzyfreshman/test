#ifndef RS_DEBUG_H
#define RS_DEBUG_H

#include <QString>

#include "cd_imagescreen.h"

/** print out a debug header*/
#define DEBUG_HEADER()  std::cout<<__FILE__<<" : "<<__FUNCTION__<<" : line "<<__LINE__<<std::endl
#define RS_DEBUG RS_Debug::instance()


class CCD_API RS_Debug {

public:
    /**
     * Enum for debug levels. Only messages of the current
     * or a higher level are printed.
     * <ul>
     *  <li>D_NOTHING:  nothing
     *  <li>D_CRITICAL: critical messages
     *  <li>D_ERROR:    errors
     *  <li>D_WARNING:  warnings
     *  <li>D_NOTICE:   notes
     *  <li>D_INFORMATIONAL: infos
     *  <li>D_DEBUGGING: very verbose
     * </ul>
     */
    enum RS_DebugLevel { D_NOTHING,
                         D_CRITICAL,
                         D_ERROR,
                         D_WARNING,
                         D_NOTICE,
                         D_INFORMATIONAL,
                         D_DEBUGGING };

private:
    RS_Debug();

public:
    static RS_Debug* instance();

    static void deleteInstance();
    void setLevel(RS_DebugLevel level);
    RS_DebugLevel getLevel();
    void print(RS_DebugLevel level, const char* format ...);
    void print(const char* format ...);
    void printUnicode(const QString& text);
    void timestamp();
    void setStream(FILE* s) {
        stream = s;
    }

    CD_ImageScreen * printImage(cv::Mat &mat,const QString &str = "");
    CD_ImageScreen * printImage(cv::Mat *mat,const QString &str = "");

private:
    static RS_Debug* uniqueInstance;

    QList<CD_ImageScreen *> images;
    RS_DebugLevel debugLevel;
    FILE* stream;
};

#endif

// EOF
