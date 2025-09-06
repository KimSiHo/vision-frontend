#pragma once

#include <QDebug>

class QtConfig {
public:
  static void init();

private:
  static void messageHandler(QtMsgType type, const QMessageLogContext& ctx, const QString& msg);
};
