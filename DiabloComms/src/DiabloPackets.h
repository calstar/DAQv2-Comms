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

//-----------------------------------------------------------------------------
// High-Level Data Collection Structures
//-----------------------------------------------------------------------------
// This structure is used for collecting and managing sensor data before
// serializing into network packets. It provides a higher-level interface
// for data collection compared to the packed network structures above.

/**
 * @brief Represents a single data chunk with timestamp and sensor datapoints.
 *
 * This struct represents one data chunk containing a timestamp and an array
 * of sensor readings. It's used for collecting sensor data before serializing
 * into network packets.
 *
 * @note This is NOT a packed struct as it's used for data collection,
 *       not network transmission. Use the packed SensorDataChunk for network
 * packets.
 */
struct SensorDataChunkCollection {
  static const uint8_t MAX_DATAPOINTS_PER_CHUNK =
      16; // Maximum number of datapoints per chunk

  uint32_t timestamp; // Timestamp for this data chunk
  SensorDatapoint
      datapoints[MAX_DATAPOINTS_PER_CHUNK]; // Array of sensor readings
  uint8_t num_datapoints;                   // Current number of datapoints

  /**
   * @brief Default constructor
   */
  SensorDataChunkCollection() : timestamp(0), num_datapoints(0) {}

  /**
   * @brief Constructor with timestamp
   * @param ts The timestamp for this data chunk
   */
  SensorDataChunkCollection(uint32_t ts) : timestamp(ts), num_datapoints(0) {}

  /**
   * @brief Add a sensor datapoint to this chunk
   * @param sensor_id The ID of the sensor
   * @param data The sensor reading value
   * @return True if successfully added, false if array is full
   */
  bool add_datapoint(uint8_t sensor_id, uint32_t data) {
    if (num_datapoints >= MAX_DATAPOINTS_PER_CHUNK) {
      return false; // Array is full
    }
    datapoints[num_datapoints] = {sensor_id, data};
    num_datapoints++;
    return true;
  }

  /**
   * @brief Get the number of datapoints in this chunk
   * @return The number of datapoints
   */
  uint8_t size() const { return num_datapoints; }

  /**
   * @brief Check if the chunk is empty
   * @return True if no datapoints, false otherwise
   */
  bool empty() const { return num_datapoints == 0; }

  /**
   * @brief Check if the chunk is full
   * @return True if at maximum capacity, false otherwise
   */
  bool full() const { return num_datapoints >= MAX_DATAPOINTS_PER_CHUNK; }

  /**
   * @brief Clear all datapoints from this chunk
   */
  void clear() { num_datapoints = 0; }
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