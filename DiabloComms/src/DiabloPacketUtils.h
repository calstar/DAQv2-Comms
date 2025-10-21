#pragma once

#include "DiabloEnums.h"   // For enums like PacketType
#include "DiabloPackets.h" // For all packet data structures
#include <Arduino.h>       // ESP32 Arduino core for standard library support
#include <stdint.h>        // For standard integer types
#include <vector>          // For std::vector

namespace Diablo {

//==============================================================================
// PACKET SERIALIZATION (Struct -> uint8_t* Buffer)
//
// These functions take high-level data structs, serialize them into a
// byte buffer, and return the final number of bytes to be sent.
// A return value of 0 indicates an error.
//==============================================================================

/**
 * @brief Creates a complete Board Heartbeat packet in the provided buffer.
 *
 * This is a fixed-size packet sent periodically by a board to the server to
 * indicate it is online and operational.
 *
 * @param data The heartbeat data to encode (board type, state, etc.).
 * @param buffer The output buffer to write the final packet into.
 * @param buffer_size The total size of the output buffer, used for safety
 * checks.
 * @return The number of bytes written to the buffer (always
 * sizeof(PacketHeader) + sizeof(BoardHeartbeatPacket)), or 0 on error.
 */
size_t create_board_heartbeat_packet(const BoardHeartbeatPacket &data,
                                     uint8_t *buffer, size_t buffer_size);

/**
 * @brief Creates a complete Sensor Data packet in the provided buffer.
 *
 * This is a variable-size packet containing readings from one or more sensor
 * data chunks. It consists of a header, a fixed-size body specifying the number
 * of chunks and sensors, followed by the actual sensor data chunks and their
 * datapoints.
 *
 * @param chunks A vector of SensorDataChunkCollection structs containing the
 * sensor data.
 * @param buffer The output buffer to write the final packet into.
 * @param buffer_size The total size of the output buffer.
 * @return The total number of bytes written to the buffer, or 0 on error.
 */
size_t
create_sensor_data_packet(const std::vector<SensorDataChunkCollection> &chunks,
                          uint8_t *buffer, size_t buffer_size);

/**
 * @brief Creates a simple Abort Done packet.
 *
 * This packet is sent from a board to the server to acknowledge that it has
 * successfully completed its abort sequence. It has no data payload.
 *
 * @param buffer The output buffer to write the final packet into.
 * @param buffer_size The total size of the output buffer.
 * @return The number of bytes written (always sizeof(PacketHeader)), or 0 on
 * error.
 */
size_t create_abort_done_packet(uint8_t *buffer, size_t buffer_size);

// Add declarations for other packets the boards will create...

} // namespace Diablo
