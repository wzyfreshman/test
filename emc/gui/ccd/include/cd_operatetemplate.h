#ifndef CD_OPERATETEMPLATE_H
#define CD_OPERATETEMPLATE_H

#include "cd_interestrect.h"
#include "cd_imagescreen.h"
#include "cd_template.h"

class CCD_API CD_OperateTemplate
{
public:

    CD_OperateTemplate(CD_ImageScreen *screen);

    CD_Template* getTemplateImage();
    void copy(CD_OperateTemplate &);

    void showUp();
    void hideDown();
    bool doSelectTemplate();

private:

    CD_ImageScreen *screen;
    CD_InterestRect rect;
    CD_Template templateimage;

};

#endif // CD_OPERATETEMPLATE_H
