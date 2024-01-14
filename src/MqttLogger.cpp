#include "MqttLogger.h"

#include <ArduinoJson.h>

MqttLogger::MqttLogger(PubSubClient &client, String topic, const MqttLoggerMode mode) : _mode(mode), _topic(topic), _client(client), _buffer({0}), _buffer_iter(_buffer.begin()) {
}

MqttLogger::~MqttLogger() {
}

void MqttLogger::setTopic(const String &topic) {
  _topic = topic;
}

size_t MqttLogger::write(uint8_t character) {
  if (character == '\n') {
    _sendBuffer();
  } else if (_buffer_iter == _buffer.end()) {
    _sendBuffer();
    *_buffer_iter = character;
    _buffer_iter++;
    *_buffer_iter = 0;
  } else if (character == '\r') {
    return 1;
  } else {
    *_buffer_iter = character;
    _buffer_iter++;
    *_buffer_iter = 0;
  }
  return 1;
}

void MqttLogger::_sendBuffer() {
  if (_buffer_iter == _buffer.begin()) {
    return;
  }
  bool doSerial = _mode == MqttLoggerMode::SerialOnly || _mode == MqttLoggerMode::MqttAndSerial;

  StaticJsonDocument<MQTT_LOGGER_MAX_ELEMENTS + 50> doc;
  doc["time"] = millis();
  doc["msg"]  = reinterpret_cast<const uint8_t *>(_buffer.data());

  char buffer[MQTT_LOGGER_MAX_ELEMENTS + 50 + 50];
  serializeJson(doc, buffer);

  if (_mode != MqttLoggerMode::SerialOnly && _client.connected()) {
    _client.publish(_topic.c_str(), buffer);
  } else if (_mode == MqttLoggerMode::MqttAndSerialFallback) {
    doSerial = true;
  }
  if (doSerial) {
    Serial.write(buffer);
    Serial.println();
  }
  _buffer_iter  = _buffer.begin();
  *_buffer_iter = 0;
}
