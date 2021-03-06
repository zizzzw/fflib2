/***********************************************
The MIT License (MIT)

Copyright (c) 2012 Athrun Arthur <athrunarthur@gmail.com>

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

#include <assert.h>
#include <cstdint>
#include <cstring>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

namespace ff {
namespace net {
template <uint32_t PackageID, typename... ARGS> class ntpackage;

template <typename T> class archive_helper {};

template <typename T> struct is_ntpackage { const static bool value = false; };
template <uint32_t PackageID, typename... ARGS>
struct is_ntpackage<ntpackage<PackageID, ARGS...>> {
  const static bool value = true;
};

//! Note, we can not use a base class and sub-classes to handle this, because we
// have to use template method archive for different types. Remind that a
// template  method can't be a virtual method.  Also, we can't make marshaler as
// a template class, because the calling point of  Archive as parameter is a
// virtual method. So we can't involve any template parameter  for Archive.
// Finally, our only choice is to use arch_type and switch-case to gain
// polymorphism.

class marshaler {
public:
  enum marshaler_type { serializer, deserializer, length_retriver };

  marshaler(const char *buf, size_t len, marshaler_type at);

  marshaler(char *buf, size_t len, marshaler_type at);

  marshaler(marshaler_type at);

#include "ff/net/common/archive_data.h"

  size_t get_length() { return m_iBase; }

  marshaler_type get_marshaler_type() { return m_iAT; }

  bool is_serializer() const { return m_iAT == serializer; }

  bool is_deserializer() const { return m_iAT == deserializer; }

  bool is_lengther() const { return m_iAT == length_retriver; }

  size_t &internal_m_iBase() { return m_iBase; }

  char *internal_m_pWriteBuf() { return m_pWriteBuf; }

  const char *internal_m_pReadBuf() { return m_pReadBuf; }

protected:
  marshaler_type m_iAT;
  size_t m_iBase;
  char *m_pWriteBuf;
  const char *m_pReadBuf;
  size_t m_iBufLen;
}; // end  class marshaler

template <class Ty_>
typename std::enable_if<std::is_arithmetic<Ty_>::value, size_t>::type
serialize(const Ty_ &val, char *pBuf) {
  std::memcpy(pBuf, (const char *)&val, sizeof(Ty_));
  return sizeof(Ty_);
}

template <class Ty_>
typename std::enable_if<std::is_arithmetic<Ty_>::value, size_t>::type
deserialize(const char *pBuf, Ty_ &val) {
  std::memcpy((char *)&val, pBuf, sizeof(Ty_));
  return sizeof(Ty_);
}

template <class Ty_>
typename std::enable_if<std::is_arithmetic<Ty_>::value, size_t>::type
length(const Ty_ &) {
  return sizeof(Ty_);
}
} // namespace net
} // namespace ff

