#pragma once

#include "DiabloEnums.h"
#include <stdint.h>

namespace Diablo {

/**
 * @brief The standard 6-byte header preceding every packet.
 */
struct __attribute__((packed)) PacketHeader {
  PacketType packet_type; // 1 byte
  uint8_t version;        // 1 byte
  uint32_t timestamp;     // 4 bytes
};

//==============================================================================
// Board Heartbeat
//==============================================================================

/**
 * @brief Body of a Board Heartbeat packet. Sent from a board to the server.
 */
struct __attribute__((packed)) BoardHeartbeatPacket {
  BoardType board_type;
  uint8_t engine_state;
  BoardState board_state;
};

//==============================================================================
// Server Heartbeat
//==============================================================================

/**
 * @brief Body of a Server Heartbeat packet. Sent from the server to all boards.
 */
struct __attribute__((packed)) ServerHeartbeatPacket {
  EngineState engine_state;
};

//==============================================================================
// Sensor Data
//
// Variable length, but having structs separate is so we can read them from
// buffer in parts
//
//==============================================================================

/**
 * @brief Body of a Sensor Data packet. Contains the fixed-size fields.
 * @note The actual packet sent over Ethernet will have this struct followed
 * by a variable number of Chunks and SensorDatapoints.
 */
struct __attribute__((packed)) SensorDataPacket {
  uint8_t num_chunks;
  uint8_t num_sensors;
  // Followed by N Chunks in the data payload
};

/**
 * @brief Chunk of a Sensor Data packet.
 * @note In the actual packet sent over Ethernet, each chunk will be followed by
 * num_sensors defined in the data packet
 */

struct __attribute__((packed)) SensorDataChunk {
  uint32_t timestamp;
};

/**
 * @brief Represents a single sensor reading. Used in the SensorDataPacket.
 */
struct __attribute__((packed)) SensorDatapoint {
  uint8_t sensor_id; // Which sensor on the board this data is from
  uint32_t data;     // The sensor value
};

//==============================================================================
// Actuator Command
//==============================================================================

/**
 * @brief Represents a command for a single actuator.
 */
struct __attribute__((packed)) ActuatorCommand {
  uint8_t actuator_id;
  uint8_t actuator_state;
};

/**
 * @brief Body of an Actuator Command packet.
 * @note The actual packet will have this struct followed by N ActuatorCommands.
 */
struct __attribute__((packed)) ActuatorCommandPacket {
  uint8_t num_commands;
  // Followed by 'num_commands' instances of ActuatorCommand
};

/**
 * @brief Defines the location of an actuator needed for an abort sequence.
 */
struct __attribute__((packed)) AbortActuatorLocation {
  uint32_t ip_address;
  uint8_t actuator_id;
  uint8_t purpose_id; // "What is it for?"
};

/**
 * @brief Defines the location of a pressure transducer needed for an abort
 * sequence.
 */
struct __attribute__((packed)) AbortPTLocation {
  uint32_t ip_address;
  uint8_t sensor_id;
  uint8_t purpose_id; // "What is it for?"
};
} // namespace Diablo