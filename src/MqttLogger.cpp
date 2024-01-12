#include "MqttLogger.h"

MqttLogger::MqttLogger(PubSubClient &client, const char *topic, const MqttLoggerMode mode) : _mode(mode), _topic(topic), _client(client) {
}

MqttLogger::~MqttLogger() {
}

size_t MqttLogger::write(uint8_t character) {
  if (character == '\n') {
    _sendBuffer();
  } else {
    _buffer.push_back(character);
  }
  return 1;
}

void MqttLogger::_sendBuffer() {
  if (_buffer.empty()) {
    return;
  }
  bool doSerial = _mode == MqttLoggerMode::SerialOnly || _mode == MqttLoggerMode::MqttAndSerial;
  if (_mode != MqttLoggerMode::SerialOnly && _client.connected()) {
    _client.publish(_topic, reinterpret_cast<const uint8_t *>(_buffer.data()), _buffer.size());
  } else if (_mode == MqttLoggerMode::MqttAndSerialFallback) {
    doSerial = true;
  }
  if (doSerial) {
    Serial.write(reinterpret_cast<const uint8_t *>(_buffer.data()), _buffer.size());
    Serial.println();
  }
}
