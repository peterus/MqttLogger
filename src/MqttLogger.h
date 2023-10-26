#ifndef MQTTLOGGER_H_
#define MQTTLOGGER_H_

#include <Arduino.h>
#include <Print.h>
#include <PubSubClient.h>
#include <vector>

enum MqttLoggerMode {
  MqttAndSerialFallback,
  SerialOnly,
  MqttOnly,
  MqttAndSerial
};

class MqttLogger : public Print {
public:
  MqttLogger(PubSubClient &client, const char *topic, const MqttLoggerMode mode = MqttLoggerMode::MqttAndSerialFallback);
  ~MqttLogger();

  virtual size_t write(uint8_t character);
  using Print::write;

private:
  const MqttLoggerMode _mode;
  const char          *_topic;

  PubSubClient &_client;

  std::vector<uint8_t> _buffer;

  void _sendBuffer();
};

#endif
