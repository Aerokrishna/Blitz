// lib/PacketUtils/include/packet_utils.hpp

#pragma once
#include <stdint.h>
#include <vector>
#include <Arduino.h>

std::vector<int16_t> parse_int(uint8_t* payload, unsigned int length);
std::vector<uint8_t> pack_data(int16_t* data, size_t count, uint8_t id);
void send_data(const std::vector<uint8_t>& buffer);
std::vector<uint8_t> receive_data();