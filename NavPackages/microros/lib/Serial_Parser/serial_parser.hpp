// lib/PacketUtils/include/packet_utils.hpp

#pragma once
#include <stdint.h>
#include <vector>
#include <Arduino.h>

std::vector<int8_t> parse_int(uint8_t* payload, unsigned int length);
std::vector<uint8_t> pack_data(void* data, size_t element_size, size_t element_count, uint8_t id);
void send_data(const std::vector<uint8_t>& buffer);
std::vector<uint8_t> receive_data();