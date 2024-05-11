#ifndef MSGLIB_H
#define MSGLIB_H

/**--------------
 * C-lang Headers
 * --------------**/
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
//#include <string.h>

/**--------------
 * C++ Headers
 * --------------**/
#include <string>
#include <iostream>

namespace netlib {

  /* --- [ const values used to configuration of socket ] --- */

  extern const std::string loopback_address;
  extern const int         non_blocking_flag;
  extern const int         blocing_flag;

  /**================================================
   * [ struct of datas of socket (needed on Linux) ]
   * ================================================
   * - socket ( file discripter )
   * - Internet address structure
   * - Length of Internet address structure
   * - String of IP-Address
   * - PortNumber ( 16Bit unsigned int: 0 ~ 65535 )  
   * ---------------------------------------------**/
  
  struct socket_data {
    int         sock_fd;
    sockaddr_in addr;
    socklen_t   addr_len;
    std::string ip_addr;
    uint16_t    port;
  };


  /**---------------------------------------------------------
   * [ BaseSocket class ]
   * - Hold datas related to socket and Internet-communication
   * - 
   * ---------------------------------------------------------**/

  class BaseSocket {
   private:
    socket_data sd_;

   public:
    BaseSocket(int socket_type, int protocol, std::string ip_address, uint16_t port_number);
    ~BaseSocket();

    void print_socket_info();
    socket_data get_sock_data();
  };

  /* --- [ UDPSender Classes ] --- */

  class UDPSender {
   private:
    BaseSocket  base_sock_;
    socket_data sd_;

   public:
    UDPSender(uint16_t port_number);
    UDPSender(std::string ip_address, uint16_t port_number);
    ~UDPSender();

    template <typename T>
    int send(T& msg) const{
      int result_size = sendto(sd_.sock_fd, &msg, sizeof(T), 0, (sockaddr *)&(sd_.addr), sd_.addr_len);
      if (result_size < 0) {
        std::cout << "fail to send data" << std::endl;
      } 
      return result_size;
    } 

  };

  /* --- [ UDPReceiver ] --- */

  class UDPReceiver {
   private:
    BaseSocket  base_sock_;
    socket_data sd_;
    sockaddr_in sender_addr_;

   public:
    UDPReceiver(uint16_t port_number);
    UDPReceiver(std::string ip_address, uint16_t port_number);
    ~UDPReceiver();

    template <typename T>
    int receive(T& buf) {
      socklen_t addr_len = static_cast<socklen_t>(sizeof(sender_addr_));
      return recvfrom(sd_.sock_fd, &buf, sizeof(T), 0, (sockaddr *)&sender_addr_, &addr_len);
    }

    std::string get_sender_addr();
    int get_sender_port();
  };
}

#endif // MSGLIB_H