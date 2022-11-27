#ifndef __SOCKETHANDLER_H__
#define __SOCKETHANDLER_H__

#include <string>
#include <boost/asio.hpp>

#include "FileHandler.h"

const size_t MAX_LEN_PACK = 1024;

using boost::asio::ip::tcp;


class SocketHandler {
public:
	std::string address;
	std::string port;

	tcp::socket* s;
	tcp::resolver* resolver;
	boost::asio::io_context* io_context;


	FileHandler *file_handler;
	FileUtilities* file_utilities;
	SocketHandler();
	~SocketHandler();

	/* connection handler */
	bool connect(std::string address, std::string port);
	bool disconnect();

	/* negotiation with the server */
	bool send_msg(uint8_t* buffer, size_t size);
	bool recv_msg(uint8_t* buffer, size_t size);

private:
	bool __is_valid_port(std::string port);
	bool __is_socket_connected();
	void __reset_all();

};



#endif
