#include "MqttLogger.h"

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
  } else {
    *_buffer_iter = character;
    _buffer_iter++;
  }
  return 1;
}

void MqttLogger::_sendBuffer() {
  if (_buffer_iter == _buffer.begin()) {
    return;
  }
  bool doSerial = _mode == MqttLoggerMode::SerialOnly || _mode == MqttLoggerMode::MqttAndSerial;
  if (_mode != MqttLoggerMode::SerialOnly && _client.connected()) {
    _client.publish(_topic.c_str(), reinterpret_cast<const uint8_t *>(_buffer.data()), _buffer_iter - _buffer.begin());
  } else if (_mode == MqttLoggerMode::MqttAndSerialFallback) {
    doSerial = true;
  }
  if (doSerial) {
    Serial.write(reinterpret_cast<const uint8_t *>(_buffer.data()), _buffer_iter - _buffer.begin());
    Serial.println();
  }
  _buffer_iter = _buffer.begin();
}
