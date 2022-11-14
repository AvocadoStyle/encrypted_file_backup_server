#include "SocketHandler.h"

SocketHandler::SocketHandler() { 
	this->s = nullptr;
	this->file_handler = new FileHandler();
	this->file_utilities = new FileUtilities();
}

SocketHandler::~SocketHandler() {
	this->__reset_all();
}

/**
 * connect to a socket
 * throw exception if the port is not valid
 */
bool SocketHandler::connect(std::string address, std::string port) {
	if (!(this->__is_valid_port(port))) {
		throw std::invalid_argument("port value is invalid - out of scope 0 - 65535\n");
	}
	if (this->__is_socket_connected()) {
		throw std::exception("already connected and you can't connect again\n");
	}
	this->address = address;
	this->port = port;

	this->io_context = new boost::asio::io_context;
	this->resolver = new tcp::resolver(*this->io_context);
	this->s = new tcp::socket(*this->io_context);

	boost::asio::connect(*this->s, this->resolver->resolve(this->address.c_str(), this->port.c_str()));
	return true;
}

/**
 * disconnect and close and reset the socket
 * throw exception if the socket is not connected already
 */
bool SocketHandler::disconnect(){
	if (this->__is_socket_connected()) {
		this->__reset_all();
		return true;
	}
	throw std::exception("there is no socket connected so you can't disconnect any socket\n");
}
/**
 * send the desired message from a connected socket 
 */
bool SocketHandler::send_msg(uint8_t* buffer, size_t size) {
	if (!this->__is_socket_connected()) {
		throw std::exception("socket is not connected - you can't write to socket");
	}
	boost::asio::write(*this->s, boost::asio::buffer(buffer, 1024));
	return true;
}

bool SocketHandler::recv_msg(uint8_t* buffer, size_t size) {
	return true;
}

bool SocketHandler::__is_valid_port(std::string port) {
	int max_port = 65535;
	int min_port = 0;
	int portnum = std::stoi(port);
	return portnum > min_port && portnum <= max_port;
}

bool SocketHandler::__is_socket_connected() {
	return this->s != nullptr;
}

void SocketHandler::__reset_all() {
	this->s->close();
	delete this->s;
	delete this->io_context;
	delete this->resolver;
	this->s = nullptr;
	this->io_context = nullptr;
	this->resolver = nullptr;
}