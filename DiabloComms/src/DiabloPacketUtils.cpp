#include "DiabloPacketUtils.h"
#include <cstring> // For memcpy
#include <cstddef> // For size_t

namespace Diablo {

//==============================================================================
// PACKET SERIALIZATION IMPLEMENTATIONS
//==============================================================================

size_t create_board_heartbeat_packet(const BoardHeartbeatPacket &data,
                                     uint8_t *buffer, size_t buffer_size) {
  // Calculate the total packet size
  const size_t header_size = sizeof(PacketHeader);
  const size_t body_size = sizeof(BoardHeartbeatPacket);
  const size_t total_size = header_size + body_size;
  
  // Check if buffer is large enough
  if (buffer_size < total_size) {
    return 0; // Error: buffer too small
  }
  
  // Create the packet header
  PacketHeader header;
  header.packet_type = PacketType::BOARD_HEARTBEAT;
  header.version = 1; // Current protocol version
  header.timestamp = millis(); // Use Arduino millis() for timestamp (ESP32 compatible) (ESP32 compatible)
  
  // Copy header to buffer
  memcpy(buffer, &header, header_size);
  
  // Copy body data to buffer
  memcpy(buffer + header_size, &data, body_size);
  
  return total_size;
}

size_t create_sensor_data_packet(const std::vector<SensorDataChunkCollection> &chunks,
                                uint8_t *buffer, size_t buffer_size) {
  // TODO: Implement sensor data packet creation
  // This will be implemented later
  return 0;
}

size_t create_abort_done_packet(uint8_t *buffer, size_t buffer_size) {
  // Calculate the total packet size (header only, no body)
  const size_t header_size = sizeof(PacketHeader);
  
  // Check if buffer is large enough
  if (buffer_size < header_size) {
    return 0; // Error: buffer too small
  }
  
  // Create the packet header
  PacketHeader header;
  header.packet_type = PacketType::ABORT_DONE;
  header.version = 1; // Current protocol version
  header.timestamp = millis(); // Use Arduino millis() for timestamp (ESP32 compatible)
  
  // Copy header to buffer
  memcpy(buffer, &header, header_size);
  
  return header_size;
}

} // namespace Diablo
