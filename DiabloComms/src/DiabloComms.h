#pragma once

#include <Arduino.h>

// Version information
#define DIABLO_COMMS_VERSION "0" // TODO: should this be decimal? our version in header is one byte

// Maximum values
#define MAX_SENSORS_PER_BOARD 10
#define MAX_ACTUATORS_PER_BOARD 10
#define MAX_CHUNKS_PER_PACKET 10
#define MAX_PACKET_SIZE 512

// Include all other headers
#include "DiabloDefs.h"


