#ifndef MQTTLOGGER_H_
#define MQTTLOGGER_H_

#include <array>

#include <Arduino.h>
#include <Print.h>
#include <PubSubClient.h>

constexpr unsigned int MQTT_LOGGER_MAX_ELEMENTS = 255;

enum MqttLoggerMode {
  MqttAndSerialFallback,
  SerialOnly,
  MqttOnly,
  MqttAndSerial
};

class MqttLogger : public Print {
public:
  MqttLogger(PubSubClient &client, String topic, const MqttLoggerMode mode = MqttLoggerMode::MqttAndSerialFallback);
  ~MqttLogger();

  void setTopic(const String &topic);

  virtual size_t write(uint8_t character);
  using Print::write;

private:
  const MqttLoggerMode _mode;
  String              &_topic;
  PubSubClient        &_client;

  std::array<uint8_t, MQTT_LOGGER_MAX_ELEMENTS>           _buffer;
  std::array<uint8_t, MQTT_LOGGER_MAX_ELEMENTS>::iterator _buffer_iter;

  void _sendBuffer();
};

#endif
