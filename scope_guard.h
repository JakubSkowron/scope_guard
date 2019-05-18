/* Usage:
 *
 *  SCOPE_GUARD [=] {
 *    std::fclose(file);
 *  };
 *
 *  SCOPE_GUARD [] {
 *    std::puts("at scope exit");
 *  };
 *
 *  auto guard1 = scope_guard::make( []{std::puts("Goodbye");} );
 *
 *  void f(void);
 *  SCOPE_GUARD f;
 *  auto guard2 = scope_guard::make(f);
 */

#ifndef SCOPE_GUARD_H
#define SCOPE_GUARD_H

#include <utility>

namespace scope_guard {

template<typename T>
class _impl {
public:
  _impl(T func) : func(std::move(func)) {}
  ~_impl() {
    if(active)
      func();
  }

  _impl(const _impl&) = delete;
  _impl& operator=(const _impl&) = delete;

  _impl(_impl&& other) :
    func(std::move(other.func)),
    active(other.active) {
    other.active = false;
  }
  _impl& operator=(_impl&&) = delete;

private:
  T func;
  bool active = true;
};

// Used to infer template parameter for scope_guard::_impl
template<typename T>
_impl<T> make(T&& func) {
  return _impl<T>{std::forward<T>(func)};
}

// Used to infer template parameter for scope_guard::_impl using operator+
struct _empty {
  template<typename T>
  _impl<T> operator+(T&& func) {
    return _impl<T>{std::forward<T>(func)};
  }
};

}  //namespace _scope_guard

#define SCOPE_GUARD_CONCAT2(X,Y) X##Y
#define SCOPE_GUARD_CONCAT(X,Y) SCOPE_GUARD_CONCAT2(X,Y)

#define SCOPE_GUARD auto SCOPE_GUARD_CONCAT(_scope_guard_, __LINE__) = scope_guard::_empty{} +

#endif
