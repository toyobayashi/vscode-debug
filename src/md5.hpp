#ifndef __MD5_HPP__
#define __MD5_HPP__

#ifndef MD5_CPP11
#	ifdef _MSVC_LANG
#		if _MSVC_LANG > 199711L
#			define MD5_CPP11	1
#		endif /* _MSVC_LANG > 199711L */
#	else /* _MSVC_LANG */
#		if __cplusplus > 199711L
#			define MD5_CPP11	1
#		endif /* __cplusplus > 199711L */
#	endif /* _MSVC_LANG */
#endif /* MD5_CPP11 */

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#endif

#include <string>
#include <vector>
#include <stdexcept>
#include <fstream>
#include "md5.h"

namespace toyo {

class md5 {
 public:
  ~md5() {
    if (hash_) {
      md5_free(hash_);
      hash_ = nullptr;
    }
  }

  md5(): hash_(md5_init()) {
    if (hash_ == nullptr) {
      throw std::runtime_error(md5_get_error_message(md5_get_last_error()));
    }
  }

  md5(const md5& other): md5() { md5_copy(hash_, other.hash_); }

#ifdef MD5_CPP11
  md5(md5&& other) {
    hash_ = other.hash_;
    other.hash_ = nullptr;
  }
  md5& operator=(md5&& other) {
    hash_ = other.hash_;
    other.hash_ = nullptr;
    return *this;
  }
#endif

  md5& operator=(const md5& other) {
    if (this == &other) {
      return *this;
    }
    md5_copy(hash_, other.hash_);
    return *this;
  };

  bool operator==(const md5& other) const {
    if (this == &other) {
      return true;
    }
    int result = 0;
    int r = md5_cmp(hash_, other.hash_, &result);
    if (r != 0) {
      throw std::runtime_error(md5_get_error_message(r));
    }
    return result == 0;
  }

  bool operator!=(const md5& other) const {
    return !(*this == other);
  }

  bool operator<(const md5& other) const {
    if (this == &other) {
      return false;
    }
    int result = 0;
    int r = md5_cmp(hash_, other.hash_, &result);
    if (r != 0) {
      throw std::runtime_error(md5_get_error_message(r));
    }
    return result == -1;
  }

  bool operator>(const md5& other) const {
    if (this == &other) {
      return false;
    }
    int result = 0;
    int r = md5_cmp(hash_, other.hash_, &result);
    if (r != 0) {
      throw std::runtime_error(md5_get_error_message(r));
    }
    return result == 1;
  }

  bool operator<=(const md5& other) const {
    if (this == &other) {
      return true;
    }
    int result = 0;
    int r = md5_cmp(hash_, other.hash_, &result);
    if (r != 0) {
      throw std::runtime_error(md5_get_error_message(r));
    }
    return result == -1 || result == 0;
  }

  bool operator>=(const md5& other) const {
    if (this == &other) {
      return true;
    }
    int result = 0;
    int r = md5_cmp(hash_, other.hash_, &result);
    if (r != 0) {
      throw std::runtime_error(md5_get_error_message(r));
    }
    return result == 1 || result == 0;
  }

  void update(const uint8_t* data, int length) {
    int r = md5_update(hash_, data, length);
    if (r != 0) {
      throw std::runtime_error(md5_get_error_message(r));
    }
  }

  void update(const std::string& data) {
    int r = md5_update(hash_, (const unsigned char*)data.c_str(), (int)data.length());
    if (r != 0) {
      throw std::runtime_error(md5_get_error_message(r));
    }
  }

  void update(const std::vector<uint8_t>& data) {
    int r = md5_update(hash_, (const unsigned char*)data.data(), (int)data.size());
    if (r != 0) {
      throw std::runtime_error(md5_get_error_message(r));
    }
  }

  std::string digest() {
    char hex[33];
    int r = md5_digest(hash_, hex);
    if (r != 0) {
      throw std::runtime_error(md5_get_error_message(r));
    }
    return hex;
  }

  md5& swap(md5& other) {
    md5_hash* tmp = other.hash_;
    other.hash_ = hash_;
    hash_ = tmp;
    return *this;
  }

  const md5_hash* data() const { return hash_; }

  static std::string calc_str(const std::string& msg) {
    md5 hash;
    hash.update(msg);
    return hash.digest();
  }

  static std::string calc_file(const std::string& path) {
#ifdef _WIN32
    int r = MultiByteToWideChar(CP_UTF8, 0, path.c_str(), -1, nullptr, 0);
    if (r == 0) {
      throw std::runtime_error("Convert failed.");
    }
    wchar_t* wpath = new wchar_t[r];
    r = MultiByteToWideChar(CP_UTF8, 0, path.c_str(), -1, wpath, r);
    if (r == 0) {
      throw std::runtime_error("Convert failed.");
    }
    std::ifstream f;
    f.open(wpath, std::ios::binary | std::ios::in);
    delete[] wpath;
#else
    std::ifstream f;
    f.open(path.c_str(), std::ios::binary | std::ios::in);
#endif
    if (!f.is_open()) {
      throw std::runtime_error("Open failed.");
    }
    const int buf_size = 128 * 1024;
    uint8_t* buf = new uint8_t[buf_size];

    md5 hash;
    while (!f.eof()) {
      f.read(reinterpret_cast<char*>(buf), buf_size);
      std::streamsize read_size = f.gcount();
      if (read_size > 0) hash.update(buf, static_cast<int>(read_size));
    }
    delete[] buf;
    f.close();

    return hash.digest();
  }
 private:
  md5_hash* hash_;
};

}

#endif
