// takes raw byte array, matches the interface and parses the data
void store_data(std::vector<uint8_t> payload) {

    if (!payload.empty()) {

        // find id
        uint8_t id = payload[0];
    
        // parse the struct based on the id
        if (id == COUNTER) {
            
            // function to parse the struct
            count = parse_struct<Counter>(payload); // count.a, count.b, count.c, count.d, based on your interface

            // function to send data, data must be of the type of one of the interfaces 
            send_data(pack_data<Counter>(count));

        }
    }
}
