#ifndef CD_DBSCANCAL_H
#define CD_DBSCANCAL_H

#include <math.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

#include <QList>
#include <QPointF>

#include "ccd_setting.h"

namespace namepace_dbscan {
    struct Point{
        double x;
        double y;
    };
}

class CCD_API CD_DbscanCal {

public:
    CD_DbscanCal();

    int dbscan(const QList<QPointF>&input,
               QList<int> &labels,
               double eps, int min);
    int dbscan(const std::vector<namepace_dbscan::Point> &input,
                             std::vector<int> &labels,
                             double eps, int min);
private:
    bool expand_cluster(const std::vector<namepace_dbscan::Point> &input,
                        int p, std::vector<int> &output,
                        int cluster, double eps, int min);

};

#endif // CD_DBSCANCAL_H
