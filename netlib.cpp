#include "netlib.hpp"

namespace netlib {
  /* --- [ const values ] --- */

  const std::string loopback_address  = "127.0.0.1";
  const int         non_blocking_flag = 1;
  const int         blocing_flag      = 0;

  /* --- [ BaseSocket Class ] --- */

  void BaseSocket::print_socket_info() {
    std::cout << "[ IP: " << sd_.ip_addr << " PORT: " << sd_.port << " ]" << std::endl;
  }

  BaseSocket::BaseSocket(int socket_type, int protocol, std::string ip_address, uint16_t port_number) {
    sd_.ip_addr = ip_address;
    sd_.port = port_number;

    sd_.sock_fd = socket(AF_INET, socket_type, protocol);
    sd_.addr.sin_family = AF_INET;
    sd_.addr.sin_addr.s_addr = inet_addr(ip_address.c_str());
    sd_.addr.sin_port = htons(port_number);
    sd_.addr_len = sizeof(sd_.addr);

    print_socket_info();
  }

  BaseSocket::~BaseSocket() {
    close(sd_.sock_fd);
  }

  socket_data BaseSocket::get_sock_data() {
    return this->sd_;
  }

  /* --- [ UDPSender Class ] --- */

  UDPSender::UDPSender(uint16_t port_number):
    base_sock_(SOCK_DGRAM, IPPROTO_UDP, loopback_address, port_number)
  {
    sd_ = base_sock_.get_sock_data();
    std::cout << "=== Create UDPSender ===" << std::endl;
  }

  UDPSender::UDPSender(std::string ip_address, uint16_t port_number):
    base_sock_(SOCK_DGRAM, IPPROTO_UDP, ip_address, port_number)
  {
    sd_ = base_sock_.get_sock_data();
    std::cout << "=== Create UDPSender ===" << std::endl;
  }

  UDPSender::~UDPSender() {

  }

  /* --- [ UDPReceiver class ] --- */

  UDPReceiver::UDPReceiver(uint16_t port_number):
    base_sock_(SOCK_DGRAM, IPPROTO_UDP, loopback_address, port_number)
  {
    sd_ = base_sock_.get_sock_data();
    bind(sd_.sock_fd, (const sockaddr *)&(sd_.addr), sd_.addr_len);
    std::cout << "=== Create UDPReceiver ===" << std::endl;
  }

  UDPReceiver::UDPReceiver(std::string ip_address, uint16_t port_number):
    base_sock_(SOCK_DGRAM, IPPROTO_UDP, ip_address, port_number)
  {
    sd_ = base_sock_.get_sock_data();
    bind(sd_.sock_fd, (const sockaddr *)&(sd_.addr), sd_.addr_len);
    std::cout << "=== Create UDPReceiver ===" << std::endl;
  }

  UDPReceiver::~UDPReceiver() {

  }

  std::string UDPReceiver::get_sender_addr() {
    return std::string(inet_ntoa(sender_addr_.sin_addr));
  }

  int UDPReceiver::get_sender_port() {
    return static_cast<int>(ntohs(sender_addr_.sin_port));
  }
}