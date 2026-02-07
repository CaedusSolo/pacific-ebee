#include "connection.h"
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>     // For read, write, close
#include <sys/socket.h> // For socket definitions
#include <arpa/inet.h>  // For htonl, ntohl
#include <stdio.h>
#include <string.h>


void send_message(const int fd, const char* outgoing_message, int len) {
    // 4 bytes
    uint32_t message_length = len;

    // convert to big endian
    uint32_t network_order_length = htonl(message_length);

    // 2. Send the Header (First 4 bytes)
    ssize_t bytes_written_header = write(fd, &network_order_length, sizeof(network_order_length));

    if (bytes_written_header != sizeof(network_order_length)) {
        perror("Failed to send length of message");
        return;
    }

    ssize_t total_bytes_written = 0;
    while (total_bytes_written < message_length) {
        const char* remaining_data_pointer = outgoing_message + total_bytes_written;
        size_t bytes_remaining = message_length - total_bytes_written;

        ssize_t bytes_written_current_chunk = write(fd, remaining_data_pointer, bytes_remaining);

        if (bytes_written_current_chunk < 0) {
            perror("Failed to send message body");
            return;
        }
        total_bytes_written += bytes_written_current_chunk;
    }
}

size_t listen_for_message(const int fd, char* msg_buffer) {
    // 4 bytes
    uint32_t network_order_length = 0;

    char* header_buffer_pointer = (char*)&network_order_length;

    ssize_t total_header_bytes_read = 0;
    size_t header_size = sizeof(network_order_length);

    // Loop until received exactly 4 bytes
    while (total_header_bytes_read < header_size) {
        ssize_t bytes_read_current_chunk = read(fd, header_buffer_pointer + total_header_bytes_read, header_size - total_header_bytes_read);

        if (bytes_read_current_chunk == 0) return -1; // client closed connection
        if (bytes_read_current_chunk < 0) {
            perror("Read header failed. Aborting now.");
            return -1;
        }
        total_header_bytes_read += bytes_read_current_chunk;
    }

    // convert to big endian
    uint32_t message_length = ntohl(network_order_length);

    // read message body
    ssize_t total_message_bytes_read = 0;

    while (total_message_bytes_read < message_length) {
        char* write_location = &msg_buffer[total_message_bytes_read];
        size_t bytes_to_read = message_length - total_message_bytes_read;

        ssize_t bytes_read_current_chunk = read(fd, write_location, bytes_to_read);

        if (bytes_read_current_chunk <= 0) {
            perror("Read body failed or connection closed mid-message");
            return -1;
        }
        total_message_bytes_read += bytes_read_current_chunk;
    }

    return message_length;
}
