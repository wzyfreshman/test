#include "hexvalidator.h"

#include <QDebug>

HexValidator::HexValidator()
{
}


QValidator::State HexValidator::validate(QString &input, int &pos) const
{
    // remove trailing comma


    if(input.isEmpty()) return  QValidator::Acceptable;


    bool ok = false;
    for(int i = 0; i<input.size(); i++) {

        QChar ch = input.at(i);

        if( (ch >= 'a' && ch <= 'f') ||
                (ch >= 'A' && ch <= 'F') || (ch >= '0' && ch <= '9') ) {
            ok = true;
        }
        else {
            ok = false;
            break;
        }


    }

      if (ok) {
            input = input.toUpper();
            return QValidator::Acceptable;
        } else
        return QValidator::Invalid;
}


Base36Validator::Base36Validator()
{
}


QValidator::State Base36Validator::validate(QString &input, int &pos) const
{
    // remove trailing comma


    if(input.isEmpty()) return  QValidator::Acceptable;


    bool ok = false;
    for(int i = 0; i<input.size(); i++) {
        QChar ch = input.at(i);

        if( (ch >= 'a' && ch <= 'z') ) {

            ok = true;

        } else if( (ch >= 'A' && ch <= 'Z') ||
                   (ch >= '0' && ch <= '9') ) {

             ok = true;

         } else {
            ok = false;
            break;
        }


    }


      if (ok) {
             input = input.toUpper();
            return QValidator::Acceptable;
        } else
        return QValidator::Invalid;
}
