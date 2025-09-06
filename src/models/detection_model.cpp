#include "detection_model.hpp"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include "spdlog/spdlog.h"

DetectionModel::DetectionModel(QObject *parent)
    : QAbstractListModel(parent) {}

int DetectionModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid())
        return 0;

    return m_detections.count();
}

QVariant DetectionModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= m_detections.count())
        return QVariant();

    const DetectionItem &item = m_detections.at(index.row());
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
    m_detections.clear();
    spdlog::debug("Raw JSON: {}", jsonData.toStdString());

    QJsonParseError err;
    const QJsonDocument doc = QJsonDocument::fromJson(jsonData, &err);
    if (err.error != QJsonParseError::NoError) {
        spdlog::error("JSON parse error: {}", err.errorString().toStdString());
        endResetModel();
        return;
    }

    if (doc.isObject()) {
        const QJsonArray detectionsArray = doc.object()["objects"].toArray();
        spdlog::debug("Parsed detections count: {}", detectionsArray.size());

        for (const auto &val : detectionsArray) {
    		QJsonObject obj = val.toObject();

    		QString label = obj["label"].toString();
    		double conf   = obj["confidence"].toDouble();

            QJsonObject boxObject = obj["box"].toObject();
    		double x = boxObject["x"].toDouble();
    		double y = boxObject["y"].toDouble();
    		double w = boxObject["w"].toDouble();
    		double h = boxObject["h"].toDouble();

    		spdlog::debug("Detection: label={} conf={} box=[x:{:.2f}, y:{:.2f}, w:{:.2f}, h:{:.2f}]",
		                  label.toStdString(), conf, x, y, w, h);

    		m_detections.append({
        			label,
        			conf,
        			QRectF(x, y, w, h)
    		});
		}
    }

    endResetModel();
}

