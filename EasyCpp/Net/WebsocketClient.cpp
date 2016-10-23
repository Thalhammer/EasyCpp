#include "WebsocketClient.h"
#include "../Base64.h"
#include "../Hash/SHA1.h"
#include "../HexEncoding.h"
#include "../StringAlgorithm.h"
#include "Endian.h"
#include <random>

namespace EasyCpp
{
	namespace Net
	{
		struct WebsocketClient::frame {
			bool fin;
			uint8_t opcode;
			bool masked;
			uint64_t len;
			uint32_t mask;
			std::string data;
		};

		struct WebsocketClient::parser_state {
			std::string buf;
			steps step;
			std::string frame_continue;
			WebsocketClient::frame c_frame;

			bool process(const std::string& data) {
				buf += data;
				if (step == steps::READ_START && buf.size() >= 2) {
					uint8_t b1 = buf[0];
					uint8_t b2 = buf[1];
					buf = buf.substr(2);
					if (c_frame.fin) {
						// Clean up last frame
						c_frame.data = "";
					}
					if ((b1 & 0x0f) != 0x00)
						c_frame.opcode = b1 & 0x0f;
					c_frame.fin = (b1 & 0x80) != 0;
					c_frame.masked = (b2 & 0x80) != 0;
					c_frame.len = (b2 & 0x7f);
					if (c_frame.masked)
						throw std::runtime_error("Masked frame from server");
					if (c_frame.len > 125)
						step = steps::READ_LEN;
					else step = steps::READ_CONTENT;
				}
				if (step == steps::READ_LEN && ((c_frame.len == 127 && buf.size() >= 8) || (c_frame.len == 126 && buf.size() >= 2))) {
					if (c_frame.len == 127) {
						uint64_t len = Endian::ntoh(*((uint64_t*)buf.data()));
						c_frame.len = len;
						buf = buf.substr(8);
					}
					else {
						uint16_t len = Endian::ntoh(*((uint16_t*)buf.data()));
						c_frame.len = len;
						buf = buf.substr(2);
					}
					step = steps::READ_CONTENT;
				}
				if (step == steps::READ_CONTENT && buf.size() >= c_frame.len) {
					c_frame.data += buf.substr(0, c_frame.len);
					buf = buf.substr(c_frame.len);
					step = steps::READ_START;
					// Wait for other segments
					if (c_frame.fin) {
						return true;
					}
					return false;
				}
				return false;
			}
		};

		WebsocketClient::WebsocketClient()
			:_exit(false)
		{
		}

		WebsocketClient::~WebsocketClient()
		{
			if (!_curl)
				return;
			//this->onError([](auto) {});
			try {
				this->disconnect();
			}catch(const std::exception& e){}
		}

		void WebsocketClient::onOpen(const std::function<void()>& fn)
		{
			_on_open = fn;
		}

		void WebsocketClient::onClose(const std::function<void(uint16_t, const std::string&)>& fn)
		{
			_on_close = fn;
		}

		void WebsocketClient::onError(const std::function<void(std::exception_ptr)>& fn)
		{
			_on_error = fn;
		}

		void WebsocketClient::onMessage(const std::function<void(const std::string&, bool)>& fn)
		{
			_on_message = fn;
		}

		void WebsocketClient::send(const std::string & msg, bool bin)
		{
			frame frame;
			frame.data = msg;
			frame.fin = true;
			frame.len = msg.size();
			frame.masked = true;
			frame.opcode = bin ? 0x02 : 0x01;

			uint8_t* mask = (uint8_t*)&frame.mask;
			for (int i = 0; i < 4; i++) {
				mask[i] = rand() % 255;
			}
			this->sendFrame(frame);
		}

		void WebsocketClient::connect(const std::string & url)
		{
			std::string ws_magic_string = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

			URI uri(url);
			if (uri.getScheme() == "ws")
				uri.setScheme("http");
			else if (uri.getScheme() == "wss")
				uri.setScheme("https");
			else throw std::runtime_error("Invalid scheme");
			_curl = std::unique_ptr<Curl>(new Curl());
			_curl->setURL(uri);
			_curl->setConnectOnly(true);
			_curl->setSSLCABundle("curl-ca-bundle.crt");
			_curl->perform();

			std::string request = "GET " + uri.getPath() + uri.getQuery() + " HTTP/1.1\r\n";
			request += "Host: " + uri.getHostname();
			if(uri.getPort()!=-1)
				request += ":" + std::to_string(uri.getPort()) + "\r\n";
			else request += "\r\n";
			request += "Upgrade: websocket\r\n";
			request += "Connection: Upgrade\r\n";

			std::string nonce;
			nonce.resize(16);
			std::uniform_int_distribution<unsigned short> dist(0, 255);
			std::random_device rd;
			for (size_t c = 0; c < nonce.size(); c++)
				nonce[c] = static_cast<unsigned char>(dist(rd));

			nonce = Base64::toString(nonce);

			request += "Sec-Websocket-Key: " + nonce + "\r\n";
			request += "Sec-Websocket-Version: 13\r\n";
			request += "\r\n";


			this->sendAll(request);

			request = "";
			while (request.find("\r\n\r\n") == std::string::npos) {
				std::string buf(128, ' ');
				size_t read = 0;
				_curl->wait(true, 1000);
				_curl->receive((void*)buf.data(), buf.size(), read);
				buf.resize(read);
				request += buf;
			}

			// Remove first packet data (if any)
			std::string data_buf = request.substr(request.find("\r\n\r\n") + 4);
			request = request.substr(0, request.find("\r\n\r\n"));

			std::multimap<std::string, std::string> headers;
			std::string http_version;
			size_t status_code = -1;
			std::string http_message;
			{
				auto lines = stringSplit("\r\n", request);
				std::string status_line = lines[0];
				size_t pos = status_line.find(' ');
				if (pos == std::string::npos)
					throw std::runtime_error("Malformed response");
				http_version = status_line.substr(0, pos);
				status_line = status_line.substr(status_line.find_first_not_of(' ', pos));
				pos = status_line.find(' ');
				if (pos == std::string::npos)
					throw std::runtime_error("Malformed response");
				status_code = std::stoul(status_line.substr(0, pos));
				if (status_code < 100 || status_code > 600)
					throw std::runtime_error("Malformed response");
				status_line = status_line.substr(status_line.find_first_not_of(' ', pos));
				http_message = trim(status_line);

				lines.erase(lines.begin());
				for (auto&e : lines) {
					auto parts = stringSplit(":", e);
					if (parts.size() != 2) continue;
					headers.insert({ trim(parts[0]), trim(parts[1]) });
				}

				if (status_code != 101)
					throw std::runtime_error("Did not upgrade");
			}
			{
				std::string expected_sha = Base64::toString(HexEncoding::decode(Hash::SHA1::getString(nonce + ws_magic_string)));
				std::string actual_sha = headers.find("Sec-WebSocket-Accept")->second;

				if (expected_sha != actual_sha)
					std::runtime_error("Checksum mismatch");
			}
			if (_on_open)
				_on_open();



			parser_state state;
			state.buf = data_buf;
			state.step = steps::READ_START;
			state.c_frame.fin = true;
			state.c_frame.opcode = 0;
			state.c_frame.mask = false;

			_read_thread = std::thread([this, state]() mutable {
				try {
					while (!_exit.load() && _curl) {
						std::string buf(128, ' ');
						size_t read = 0;
						bool res = _curl->receive((void*)buf.data(), buf.size(), read);
						if (res && read > 0) {
							buf.resize(read);
							if (state.process(buf)) {
								if (this->_on_message)
									this->onFrame(state.c_frame);
							}
						}
						if(_curl)
							_curl->wait(true, 5000);
					}
					while (_curl) {
						std::string buf(128, ' ');
						size_t read = 0;
						bool res = _curl->receive((void*)buf.data(), buf.size(), read);
						if (res && read > 0) {
							buf.resize(read);
							if (state.process(buf)) {
								if (state.c_frame.opcode == 0x08)
								{
									uint16_t status_code = 1000;
									std::string status_msg = "";
									if (state.c_frame.data.size() >= 2) {
										status_code = (((uint16_t)state.c_frame.data.at(0)) << 8) | (uint8_t)state.c_frame.data.at(1);
										if (state.c_frame.data.size() > 2) {
											status_msg = state.c_frame.data.substr(2);
										}
									}
									if (_on_close)
										_on_close(status_code, status_msg);
									return;
								}
							}
						}
						else if (res && read == 0) {
							// Connection closed
							if (_on_close)
								_on_close(-1, "Connection dropped");
						}
						_curl->wait(true, 5000);
					}
				}
				catch (...) {
					if (_on_error)
						_on_error(std::current_exception());
				}
			});
		}

		void WebsocketClient::disconnect(uint16_t code, const std::string & cmsg)
		{
			if (!_curl)
				return;
			std::string msg(2, 0x00);
			msg[0] = code >> 8;
			msg[1] = code & 0xff;
			msg.append(cmsg);

			frame frame;
			frame.data = msg;
			frame.fin = true;
			frame.len = msg.size();
			frame.masked = true;
			frame.opcode = 0x08;
			uint8_t* mask = (uint8_t*)&frame.mask;
			for (int i = 0; i < 4; i++) {
				mask[i] = rand() % 255;
			}

			_exit.store(true);
			this->sendFrame(frame);
			if(_read_thread.joinable())
				_read_thread.join();
			_curl.reset();
		}

		void WebsocketClient::sendAll(const std::string & data)
		{
			if (!_curl)
				return;
			size_t bytes_sent = 0;
			while (bytes_sent != data.size()) {
				size_t sent = 0;
				_curl->send(((char*)data.data()) + bytes_sent, data.size() - bytes_sent, sent);
				bytes_sent += sent;
			}
		}

		void WebsocketClient::sendFrame(const frame & frame)
		{
			if (!_curl)
				return;
			std::string msg(2, 0x00);
			msg[0] = (frame.fin ? 0x80 : 0x00) | (frame.opcode & 0x0f);
			msg[1] = frame.masked ? 0x80 : 0x00;
			if (frame.len < 126) msg[1] |= frame.len;
			else if (frame.len < UINT16_MAX) {
				uint16_t len = Endian::hton((uint16_t)frame.len);
				msg.append(std::string((char*)&len, ((char*)&len) + 2));
				msg[1] |= 126;
			}
			else {
				uint64_t len = Endian::hton(frame.len);
				msg.append(std::string((char*)&len, ((char*)&len) + 8));
				msg[1] |= 127;
			}
			if (frame.masked) {
				msg.append(std::string((char*)&frame.mask, ((char*)&frame.mask) + 4));
			}
			size_t offset = msg.size();
			msg.append(frame.data);
			if (frame.masked) {
				uint8_t* mask = (uint8_t*)&frame.mask;
				for (size_t i = offset; i < msg.size(); i++) {
					msg[i] = msg[i] ^ mask[(i - offset) % 4];
				}
			}
			this->sendAll(msg);
		}

		void WebsocketClient::onFrame(const frame & cframe)
		{
			if (cframe.opcode == 0x08) {
				// Close
				uint16_t status_code = 1000;
				std::string status_msg = "";
				if (cframe.data.size() >= 2) {
					status_code = (((uint16_t)cframe.data.at(0)) << 8) | (uint8_t)cframe.data.at(1);
					if (cframe.data.size() > 2) {
						status_msg = cframe.data.substr(2);
					}
				}
				if (_on_close)
					_on_close(status_code, status_msg);

				frame frame;
				frame.data = cframe.data;
				frame.fin = true;
				frame.len = cframe.data.size();
				frame.masked = true;
				frame.opcode = 0x08;
				uint8_t* mask = (uint8_t*)&frame.mask;
				for (int i = 0; i < 4; i++) {
					mask[i] = rand() % 255;
				}
				_exit.store(true);
				this->sendFrame(frame);
				_curl.reset();
			}
			else if (cframe.opcode == 0x09) {
				// Ping
				frame pong;
				pong.data = cframe.data;
				pong.fin = true;
				pong.len = cframe.data.size();
				pong.masked = true;
				pong.opcode = 0x0A;
				uint8_t* mask = (uint8_t*)&pong.mask;
				for (int i = 0; i < 4; i++) {
					mask[i] = rand() % 255;
				}
				this->sendFrame(pong);
			}
			else if (cframe.opcode == 0x0A) {
				// Pong
			}
			else if (cframe.opcode == 0x01) {
				if (_on_message)
					_on_message(cframe.data, false);
			}
			else if (cframe.opcode == 0x02) {
				if (_on_message)
					_on_message(cframe.data, true);
			}
		}
	}
}