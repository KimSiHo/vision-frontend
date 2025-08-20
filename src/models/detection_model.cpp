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
    spdlog::debug("DetectionModel: Processing new JSON data.");

    // JSON 원문 로그 (너무 길면 trace 레벨 권장)
    spdlog::trace("Raw JSON: {}", jsonData.toStdString());

    beginResetModel();
    m_detections.clear();

    QJsonParseError err;
    const QJsonDocument doc = QJsonDocument::fromJson(jsonData, &err);
    if (err.error != QJsonParseError::NoError) {
        spdlog::error("JSON parse error: {}", err.errorString().toStdString());
        endResetModel();
        return;
    }

    if (doc.isObject()) {
        const QJsonArray detectionsArray = doc.object()["detections"].toArray();
        spdlog::debug("Parsed detections count: {}", detectionsArray.size());

        for (const auto &val : detectionsArray) {
            QJsonObject obj = val.toObject();
            QJsonArray boxArray = obj["box"].toArray();

            QString label = obj["label"].toString();
            double conf   = obj["confidence"].toDouble();

            spdlog::debug("Detection: label={} conf={} box=[{:.2f}, {:.2f}, {:.2f}, {:.2f}]",
                          label.toStdString(),
                          conf,
                          boxArray[0].toDouble(),
                          boxArray[1].toDouble(),
                          boxArray[2].toDouble(),
                          boxArray[3].toDouble());

            m_detections.append({
                label,
                conf,
                QRectF(boxArray[0].toDouble(), boxArray[1].toDouble(),
                       boxArray[2].toDouble(), boxArray[3].toDouble())
            });
        }
    }

    endResetModel();
}

