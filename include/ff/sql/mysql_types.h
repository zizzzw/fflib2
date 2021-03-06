/***********************************************
  The MIT License (MIT)

  Copyright (c) 2022 Athrun Arthur <athrunarthur@gmail.com>

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
 *************************************************/
#pragma once
#include "ff/sql/table_create.h"
#include <cppconn/sqlstring.h>

namespace ff {
namespace mysql {
template <uint8_t Len> struct char_m {
public:
  char_m() : m_data(){};
  char_m(const char *s) : m_data(s){};
  char_m(const ::sql::SQLString &s) : m_data(s.c_str()) {}
  char_m(const char_m<Len> &s) : m_data(s.data()) {}
  char_m(char_m<Len> &&s) : m_data(std::move(s.m_data)) {}
  char_m<Len> &operator=(const char_m<Len> &s) {
    if (&s == this) {
      return *this;
    }
    m_data = s.m_data;
    return *this;
  };

  const std::string &data() const { return m_data; }
  std::string &data() { return m_data; }

protected:
  std::string m_data;
};

namespace internal {
template <class T> struct dump_col_type_creation;
template <uint8_t Len> struct dump_col_type_creation<char_m<Len>> {
  static void dump(std::stringstream &ss) { ss << " CHAR(" << Len << ") "; }
};
} // namespace internal
} // namespace mysql
namespace sql {
template <class STMT, class T> struct mysql_bind_setter;

template <class STMT, uint8_t Len>
struct mysql_bind_setter<STMT, ::ff::mysql::char_m<Len>> {
  static void bind(STMT stmt, int index,
                   const ::ff::mysql::char_m<Len> &value) {
    stmt->setString(index, value.data());
  }
};

template <class T> struct mysql_rs_getter;
template <uint8_t Len> struct mysql_rs_getter<::ff::mysql::char_m<Len>> {
  template <typename RST>
  static ::ff::mysql::char_m<Len> get(RST r, const std::string &name) {
    return ::ff::mysql::char_m<Len>(r->getString(name));
  }
};

} // namespace sql
} // namespace ff

