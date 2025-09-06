#include "models/vision/detection_model.hpp"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

#include "common/utils/logging.hpp"

DetectionModel::DetectionModel(QObject *parent)
    : QAbstractListModel(parent) { }

int DetectionModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid())
        return 0;

    return detectionItems.count();
}

QVariant DetectionModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= detectionItems.count())
        return QVariant();

    const DetectionItem &item = detectionItems.at(index.row());
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

void DetectionModel::processInferenceJson(const QByteArray &jsonData) {
    beginResetModel();
    detectionItems.clear();
    SPDLOG_SERVICE_DEBUG("Raw JSON: {}", jsonData.toStdString());

    QJsonParseError err;
    const QJsonDocument doc = QJsonDocument::fromJson(jsonData, &err);
    if (err.error != QJsonParseError::NoError) {
        SPDLOG_SERVICE_ERROR("JSON parse error: {}", err.errorString().toStdString());
        endResetModel();
        return;
    }

    if (doc.isObject()) {
        const QJsonArray detectionsArray = doc.object()["objects"].toArray();
        SPDLOG_SERVICE_DEBUG("Parsed detections count: {}", detectionsArray.size());

        for (const auto &val : detectionsArray) {
    		QJsonObject obj = val.toObject();

    		QString label = obj["label"].toString();
    		double conf   = obj["confidence"].toDouble();

            QJsonObject boxObject = obj["box"].toObject();
    		double x = boxObject["x"].toDouble();
    		double y = boxObject["y"].toDouble();
    		double w = boxObject["w"].toDouble();
    		double h = boxObject["h"].toDouble();

    		SPDLOG_SERVICE_DEBUG("Detection: label={} conf={} box=[x:{:.2f}, y:{:.2f}, w:{:.2f}, h:{:.2f}]",
		                  label.toStdString(), conf, x, y, w, h);

    		detectionItems.append({
        			label,
        			conf,
        			QRectF(x, y, w, h)
    		});
		}
    }

    endResetModel();
}
