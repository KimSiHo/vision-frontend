#include "models/vision/detection_model.hpp"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "common/utils/logging.hpp"

DetectionModel::DetectionModel(QObject* parent) : QAbstractListModel(parent) {}

int DetectionModel::rowCount(const QModelIndex& parent) const {
  if (parent.isValid()) return 0;
  return detection_items_.count();
}

QVariant DetectionModel::data(const QModelIndex& index, int role) const {
  if (!index.isValid() || index.row() >= detection_items_.count()) return QVariant();

  const DetectionItem& item = detection_items_.at(index.row());
  switch (role) {
    case LabelRole:
      return item.label;
    case ConfidenceRole:
      return item.confidence;
    case BoxRole:
      return item.box;
  }
  return QVariant();
}

QHash<int, QByteArray> DetectionModel::roleNames() const {
  QHash<int, QByteArray> roles;
  roles[LabelRole] = "label";
  roles[ConfidenceRole] = "confidence";
  roles[BoxRole] = "box";
  return roles;
}

void DetectionModel::processInferenceJson(const QByteArray& json_data) {
  beginResetModel();
  detection_items_.clear();
  SPDLOG_SERVICE_DEBUG("Raw JSON: {}", json_data.toStdString());

  QJsonParseError err;
  const QJsonDocument doc = QJsonDocument::fromJson(json_data, &err);
  if (err.error != QJsonParseError::NoError) {
    SPDLOG_SERVICE_ERROR("JSON parse error: {}", err.errorString().toStdString());
    endResetModel();
    return;
  }

  if (doc.isObject()) {
    const QJsonArray detections_array = doc.object()["objects"].toArray();
    SPDLOG_SERVICE_DEBUG("Parsed detections count: {}", detections_array.size());

    for (const auto& val : detections_array) {
      QJsonObject obj = val.toObject();

      QString label = obj["label"].toString();
      double confidence = obj["confidence"].toDouble();

      QJsonObject box_obj = obj["box"].toObject();
      double x = box_obj["x"].toDouble();
      double y = box_obj["y"].toDouble();
      double w = box_obj["w"].toDouble();
      double h = box_obj["h"].toDouble();

      SPDLOG_SERVICE_DEBUG("Detection: label={} confidence={} box=[x:{:.2f}, y:{:.2f}, w:{:.2f}, h:{:.2f}]",
                           label.toStdString(), confidence, x, y, w, h);

      detection_items_.append({label, confidence, QRectF(x, y, w, h)});
    }
  }

  endResetModel();
}
