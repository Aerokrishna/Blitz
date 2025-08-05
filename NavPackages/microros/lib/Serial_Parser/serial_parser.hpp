// lib/PacketUtils/include/packet_utils.hpp

#pragma once
#include <stdint.h>
#include <vector>

std::vector<int8_t> parse_int(uint8_t* payload, unsigned int length);