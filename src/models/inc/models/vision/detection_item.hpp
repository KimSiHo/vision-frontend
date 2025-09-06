#pragma once

#include <QRectF>
#include <QString>

struct DetectionItem {
  QString label;
  double confidence;
  QRectF box;
};
