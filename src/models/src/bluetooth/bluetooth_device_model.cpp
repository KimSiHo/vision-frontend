#include "models/bluetooth/bluetooth_device_model.hpp"

#include <nlohmann/json.hpp>

#include "common/utils/logging.hpp"

BluetoothDeviceModel::BluetoothDeviceModel(QObject* parent) : QAbstractListModel(parent) {}

int BluetoothDeviceModel::rowCount(const QModelIndex& parent) const {
  Q_UNUSED(parent);
  return devices_.size();
}

QVariant BluetoothDeviceModel::data(const QModelIndex& index, int role) const {
  if (!index.isValid() || index.row() >= devices_.size()) return {};

  const auto& dev = devices_[index.row()];
  switch (role) {
    case AddrRole:
      return dev.addr;
    case NameRole:
      return dev.name;
    default:
      return {};
  }
}

QHash<int, QByteArray> BluetoothDeviceModel::roleNames() const { return {{AddrRole, "addr"}, {NameRole, "name"}}; }

void BluetoothDeviceModel::clear() {
  beginResetModel();
  devices_.clear();
  endResetModel();
}

void BluetoothDeviceModel::processJson(const QString& json) {
  try {
    SPDLOG_SERVICE_DEBUG("Raw JSON: {}", json.toStdString());

    auto arr = nlohmann::json::parse(json.toStdString());

    beginResetModel();
    devices_.clear();
    for (auto& item : arr) {
      BluetoothDevice dev;
      dev.addr = QString::fromStdString(item.value("addr", ""));
      dev.name = QString::fromStdString(item.value("name", ""));
      devices_.append(dev);
    }
    endResetModel();

    SPDLOG_SERVICE_INFO("[BluetoothDeviceModel] Updated {} devices", devices_.size());
  } catch (const std::exception& e) {
    SPDLOG_SERVICE_ERROR("[BluetoothDeviceModel] JSON parse error: {}", e.what());
  }
}
