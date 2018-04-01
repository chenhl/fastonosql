/*  Copyright (C) 2014-2018 FastoGT. All right reserved.

    This file is part of FastoNoSQL.

    FastoNoSQL is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    FastoNoSQL is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FastoNoSQL.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "core/db_key.h"

#include <common/convert2string.h>
#include <common/string_util.h>  // for JoinString, Tokenize

#include "core/value.h"

namespace fastonosql {
namespace core {

bool IsBinaryData(const command_buffer_t& data) {
  for (size_t i = 0; i < data.size(); ++i) {
    unsigned char c = static_cast<unsigned char>(data[i]);
    if (c < ' ') {  // should be hexed symbol
      return true;
    }
  }
  return false;
}

KeyString::KeyString() : key_(), type_(TEXT_KEY) {}

KeyString::KeyString(const string_key_t& key_data) : key_(), type_(TEXT_KEY) {
  SetKeyData(key_data);
}

KeyString::KeyType KeyString::GetType() const {
  return type_;
}

std::string KeyString::GetKeyData() const {
  return key_;
}

std::string KeyString::GetHumanReadable() const {
  if (type_ == BINARY_KEY) {
    return detail::hex_string(key_);
  }

  return key_;
}

string_key_t KeyString::GetKeyForCommandLine() const {
  if (type_ == BINARY_KEY) {
    command_buffer_writer_t wr;
    wr << "\"" << detail::hex_string(key_) << "\"";
    return wr.str();
  }

  if (detail::have_space(key_)) {
    return "\"" + key_ + "\"";
  }

  return key_;
}

void KeyString::SetKeyData(const string_key_t& key_data) {
  key_ = key_data;
  type_ = IsBinaryData(key_data) ? BINARY_KEY : TEXT_KEY;
}

bool KeyString::Equals(const KeyString& other) const {
  return key_ == other.key_;
}

NKey::NKey() : key_(), ttl_(NO_TTL) {}

NKey::NKey(key_t key, ttl_t ttl_sec) : key_(key), ttl_(ttl_sec) {}

key_t NKey::GetKey() const {
  return key_;
}

void NKey::SetKey(key_t key) {
  key_ = key;
}

ttl_t NKey::GetTTL() const {
  return ttl_;
}

void NKey::SetTTL(ttl_t ttl) {
  ttl_ = ttl;
}

bool NKey::EqualsKey(const NKey& key) const {
  return key_ == key.key_;
}

bool NKey::Equals(const NKey& other) const {
  if (key_ != other.key_) {
    return false;
  }

  return ttl_ == other.ttl_;
}

NDbKValue::NDbKValue() : key_(), value_() {}

NDbKValue::NDbKValue(const NKey& key, NValue value) : key_(key), value_(value) {}

NKey NDbKValue::GetKey() const {
  return key_;
}

NValue NDbKValue::GetValue() const {
  return value_;
}

common::Value::Type NDbKValue::GetType() const {
  if (!value_) {
    return common::Value::TYPE_NULL;
  }

  return value_->GetType();
}

void NDbKValue::SetKey(const NKey& key) {
  key_ = key;
}

void NDbKValue::SetValue(NValue value) {
  value_ = value;
}

std::string NDbKValue::GetValueString() const {
  return ConvertToHumanReadable(value_.get(), " ");
}

std::string NDbKValue::GetValueForCommandLine() const {
  return ConvertValue(value_.get(), " ", true);
}

std::string NDbKValue::GetHumanReadable() const {
  std::string data = ConvertToHumanReadable(value_.get(), " ");
  return IsBinaryData(data) ? detail::hex_string(data) : data;
}

bool NDbKValue::EqualsKey(const NKey& key) const {
  return key_.EqualsKey(key);
}

bool NDbKValue::Equals(const NDbKValue& other) const {
  if (!key_.Equals(other.key_)) {
    return false;
  }

  if (!value_) {
    return !other.value_;
  }

  return value_->Equals(other.value_.get());
}

}  // namespace core
}  // namespace fastonosql
