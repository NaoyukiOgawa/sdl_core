/**
 * \file Connection.cpp
 * \brief Connection class implementation.
 *
 * Copyright (c) 2013, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ford Motor Company nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <algorithm>

#include "connection_handler/connection.h"
#include "connection_handler/connection_handler.h"
#include "utils/macro.h"

/**
 * \namespace connection_handler
 * \brief SmartDeviceLink ConnectionHandler namespace.
 */
namespace connection_handler {

log4cxx::LoggerPtr Connection::logger_ = log4cxx::LoggerPtr(
    log4cxx::Logger::getLogger("ConnectionHandler"));

Connection::Connection(ConnectionHandle connection_handle,
                       DeviceHandle connection_device_handle,
                       ConnectionHandler* connection_handler,
                       int32_t heartbeat_timeout)
    : connection_handler_(connection_handler),
      connection_handle_(connection_handle),
      connection_device_handle_(connection_device_handle),
      session_id_counter_(1),
      heartbeat_monitor_(heartbeat_timeout, this) {
  DCHECK(connection_handler_);
}

Connection::~Connection() {
  session_list_.clear();
  session_map_.clear();
}

int32_t Connection::AddNewSession() {
  int32_t result = -1;

  if (session_list_.empty()) {
    heartbeat_monitor_.BeginMonitoring();
  }

  const uint8_t max_connections = 255;
  if (max_connections > session_id_counter_) {
    session_list_.push_back(session_id_counter_);

    /* whenever new session created RPC and Bulk services are
    established automatically */
    session_map_[session_id_counter_].push_back(protocol_handler::kRpc);
    session_map_[session_id_counter_].push_back(protocol_handler::kBulk);

    result = session_id_counter_++;
  }

  return result;
}

int32_t Connection::RemoveSession(uint8_t session) {
  int32_t result = -1;
  SessionListIterator it = std::find(session_list_.begin(), session_list_.end(),
                                     session);
  if (session_list_.end() == it) {
    LOG4CXX_ERROR(logger_, "Session not found in this connection!");
  } else {
    session_list_.erase(it);
    session_map_.erase(session);
    result = session;
  }

  return result;
}

bool Connection::AddNewService(uint8_t session, uint8_t service) {
  bool result = false;

  SessionMapIterator session_it = session_map_.find(session);
  if (session_it == session_map_.end()) {
    LOG4CXX_ERROR(logger_, "Session not found in this connection!");
    return result;
  }

  ServiceListIterator service_it = find(session_it->second.begin(),
                                        session_it->second.end(), service);
  if (service_it != session_it->second.end()) {
    LOG4CXX_ERROR(logger_, "Session " << session << " already established"
                  " service " << service);
  } else {
    session_it->second.push_back(service);
    result = true;
  }

  return result;
}

bool Connection::RemoveService(uint8_t session, uint8_t service) {
  bool result = false;

  SessionMapIterator session_it = session_map_.find(session);
  if (session_it == session_map_.end()) {
    LOG4CXX_ERROR(logger_, "Session not found in this connection!");
    return result;
  }

  ServiceListIterator service_it = find(session_it->second.begin(),
                                        session_it->second.end(), service);
  if (service_it != session_it->second.end()) {
    session_it->second.erase(service_it);
    result = true;
  } else {
    LOG4CXX_ERROR(logger_, "Session " << session << " didn't established"
                  " service " << service);
  }

  return result;
}

ConnectionHandle Connection::connection_handle() const {
  return connection_handle_;
}

DeviceHandle Connection::connection_device_handle() {
  return connection_device_handle_;
}

void Connection::GetSessionList(SessionList& session_list) {
  session_list = session_list_;
}

void Connection::Close() {
  connection_handler_->CloseConnection(connection_handle_);
}

void Connection::KeepAlive() {
  heartbeat_monitor_.KeepAlive();
}

}/* namespace connection_handler */
