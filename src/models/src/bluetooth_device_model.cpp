#include "models/bluetooth_device_model.hpp"

#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

BluetoothDeviceModel::BluetoothDeviceModel(QObject* parent)
    : QAbstractListModel(parent) {}

int BluetoothDeviceModel::rowCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return devices_.size();
}

QVariant BluetoothDeviceModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.row() >= devices_.size())
        return {};

    const auto& dev = devices_[index.row()];
    switch (role) {
    case AddrRole: return dev.addr;
    case NameRole: return dev.name;
    default: return {};
    }
}

QHash<int, QByteArray> BluetoothDeviceModel::roleNames() const {
    return {
            {AddrRole, "addr"},
            {NameRole, "name"}
    };
}

void BluetoothDeviceModel::processJson(const QString& json) {
    try {
        // 🔹 원본 JSON 문자열 로깅
        spdlog::info("[BTModel] Raw JSON: {}", json.toStdString());

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

        spdlog::info("[BTModel] Updated {} devices", devices_.size());
    } catch (const std::exception& e) {
        spdlog::error("[BTModel] JSON parse error: {}", e.what());
    }
}
