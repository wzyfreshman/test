#include <QVBoxLayout>

#include "ccd_marksface.h"
#include "cd_mainscreen.h"


CCD_MarksFace::CCD_MarksFace() {

    mainscreen = new CD_MainScreen(0);

}

void CCD_MarksFace::activeEditeScreen() {

}

void CCD_MarksFace::activeSelectScreen( ) {

}

void CCD_MarksFace::activeMainScreen() {

}

void CCD_MarksFace::showInMainScreen(cv::Mat &mat) {
    mainscreen->showMatImage(mat);
}

void CCD_MarksFace::showInMainScreen(cv::Mat *mat) {
    mainscreen->showMatImage(*mat);
}

void CCD_MarksFace::showInMainScreen(QString &filename) {
    mainscreen->showMatImage(filename);
}

void CCD_MarksFace::activeScreen(QWidget *scplace,CD_ImageScreen *sc) {

    if(scplace != NULL) {

        for(int i = 0;i<scplace->children().size();i++) {
            if(scplace->children().at(i)->isWidgetType()) {

                QWidget *wdgt;
                wdgt = static_cast<QWidget *>(scplace->children().at(i));
                wdgt->hide();
            }
        }
        sc->show();
    }

}

void CCD_MarksFace::putImageScreenToWidget(CD_ImageScreen *s, QWidget *_place) {

    if(_place->layout() == NULL) {
        QVBoxLayout *bovly = new QVBoxLayout(_place);
        _place->setLayout(bovly);
    }

    s ->setParent(_place);
    _place->layout()->setContentsMargins(0,0,0,0);
    _place->layout()->addWidget(s);

}
