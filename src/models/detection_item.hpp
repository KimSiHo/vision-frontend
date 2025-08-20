#pragma once

#include <QString>
#include <QRectF>

struct DetectionItem {
    QString label;
    double confidence;
    QRectF box;
};
