#ifndef CD_FETCHPIECE_H
#define CD_FETCHPIECE_H

#include "cd_interestrect.h"

class CCD_API CD_FetchPiece : public CD_InterestRect
{
public:
    CD_FetchPiece(CD_ImageScreen *parent);
    void virtual paintEvent(QPaintEvent *);
    void virtual calculatespecPoint();


};

#endif // CD_FETCHPIECE_H
