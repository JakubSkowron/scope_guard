/* Usage:
 *
 *  FILE* file = std::fopen(argv[1], "r");
 *  if(!file) {
 *    std::puts("file error");
 *    return 1;
 *  }
 *
 *  scope_guard [=] {
 *    std::fclose(file);
 *  };
 *
 *  scope_guard [] {
 *    std::puts("at scope exit");
 *  }
 */

#ifndef SCOPE_GUARD_H
#define SCOPE_GUARD_H

#include <utility>

namespace _scope_guard {

template<typename T>
struct impl {
  impl(T func) : func(func) {}
  ~impl() {
    if(active)
      func();
  }
  impl(impl&& other) : func(std::move(other.func)), active(other.active) {
    other.active = false;
  }
  impl(const impl&) = delete;
  impl& operator=(const impl&) = delete;
  impl& operator=(impl&&) = delete;

  T func;
  bool active = true;
};

struct empty{};

}  //namespace _scope_guard


template<typename T>
_scope_guard::impl<T> operator+(_scope_guard::empty, T&& func) {
  return _scope_guard::impl<T>{std::forward<T>(func)};
}

#define _SCOPE_GUARD_JOIN2(X,Y) X##Y
#define _SCOPE_GUARD_JOIN(X,Y) _SCOPE_GUARD_JOIN2(X,Y)
#define scope_guard auto _SCOPE_GUARD_JOIN(_scope_guard_, __LINE__) = _scope_guard::empty{} +

#endif
