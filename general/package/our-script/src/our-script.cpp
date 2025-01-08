#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <termios.h>
#include <unistd.h>

#define UDP_PORT 6666
#define BUFFER_SIZE 1024
#define UART_SPEED B115200

int main() {
  int socket_fd;
  struct sockaddr_in server_addr, client_addr;
  char buffer[BUFFER_SIZE];

  // Create UDP socket
  if ((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    std::cerr << "Socket creation failed: " << strerror(errno) << std::endl;
    return -1;
  }

  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(UDP_PORT);

  // Bind socket
  if (bind(socket_fd, (const struct sockaddr *)&server_addr,
           sizeof(server_addr)) < 0) {
    std::cerr << "Bind failed: " << strerror(errno) << std::endl;
    close(socket_fd);
    return -1;
  }

  // Open UART
  int uart_fd = open("/dev/ttyAMA0", O_RDWR | O_NOCTTY | O_NDELAY);
  if (uart_fd == -1) {
    std::cerr << "Error opening UART: " << strerror(errno) << std::endl;
    close(socket_fd);
    return -1;
  }

  // Configure UART
  struct termios uart_config;
  tcgetattr(uart_fd, &uart_config);
  uart_config.c_cflag = UART_SPEED | CS8 | CLOCAL | CREAD;
  uart_config.c_iflag = 0;
  uart_config.c_oflag = 0;
  uart_config.c_lflag = 0;
  tcsetattr(uart_fd, TCSANOW, &uart_config);

  std::cout << "Server started on UDP port " << UDP_PORT << std::endl;

  // Main loop
  while (true) {
    socklen_t len = sizeof(client_addr);
    int recv_len = recvfrom(socket_fd, buffer, BUFFER_SIZE, 0,
                            (struct sockaddr *)&client_addr, &len);

    if (recv_len > 0) {
      buffer[recv_len] = '\0';

      // Write received data to UART
      write(uart_fd, buffer, recv_len);

      std::cout << "Message from " << inet_ntoa(client_addr.sin_addr) << ":"
                << ntohs(client_addr.sin_port) << std::endl;
    }

    memset(buffer, 0, BUFFER_SIZE);
  }

  close(uart_fd);
  close(socket_fd);
  return 0;
}