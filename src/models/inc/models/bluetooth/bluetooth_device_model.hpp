#pragma once

#include <QAbstractListModel>
#include <QVector>

struct BluetoothDevice {
  QString addr;
  QString name;
};

class BluetoothDeviceModel : public QAbstractListModel {
  Q_OBJECT

public:
  enum Roles { AddrRole = Qt::UserRole + 1, NameRole };

  explicit BluetoothDeviceModel(QObject* parent = nullptr);

  int rowCount(const QModelIndex& parent = QModelIndex()) const override;
  QVariant data(const QModelIndex& index, int role) const override;
  QHash<int, QByteArray> roleNames() const override;
  Q_INVOKABLE void clear();

public slots:
  void processJson(const QString& json);

private:
  QVector<BluetoothDevice> devices_;
};
