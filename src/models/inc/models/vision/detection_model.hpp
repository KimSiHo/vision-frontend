#pragma once

#include <QAbstractListModel>
#include <QList>

#include "models/vision/detection_item.hpp"

class DetectionModel : public QAbstractListModel {
  Q_OBJECT

public:
  explicit DetectionModel(QObject* parent = nullptr);

  enum DetectionRoles { LabelRole = Qt::UserRole + 1, ConfidenceRole, BoxRole };

  int rowCount(const QModelIndex& parent = QModelIndex()) const override;
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
  QHash<int, QByteArray> roleNames() const override;

public slots:
  void processInferenceJson(const QByteArray& json_data);

private:
  QList<DetectionItem> detection_items_;
};
