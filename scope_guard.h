/* Basic usage:
 *
 *   SCOPE_GUARD [=] {
 *     std::fclose(file);
 *   };
 *
 *   SCOPE_GUARD [] {
 *     std::puts("at scope exit");
 *   };
 *
 * Without macros:
 *
 *   auto guard = scope_guard::make([]{std::puts("~guard");});
 *
 * Passing ownership:
 *
 *   auto guard = scope_guard::make(f);
 *   // for lambdas you can use a template instead of naming the type explicitly
 *   void new_owner(scope_guard::guard_object<void()>);
 *   new_owner(std::move(guard));
 *
 * More usage:
 *
 *   void f(void);
 *   SCOPE_GUARD(f);  // declaration of a scope_guard::guard_object
 *
 *   auto guard = scope_guard::empty{} + x; // synonymous to scope_guard::make(x);
 *
 *   static SCOPE_GUARD [] {
 *     std::puts("at exit");
 *   };
 *
 */

#ifndef SCOPE_GUARD_H
#define SCOPE_GUARD_H

namespace scope_guard {

template<typename T>
class guard_object {
public:
  guard_object(T func) : func(std::move(func)) {}
  ~guard_object() {
    if(active) func();
  }

  guard_object(const guard_object&) = delete;
  guard_object& operator=(const guard_object&) = delete;

  // move constructor
  guard_object(guard_object&& other) :
    func(std::move(other.func)),
    active(other.active) {
    other.active = false;
  }
  guard_object& operator=(guard_object&&) = delete;

  void cancel() { active = false; }
  void destroy() {
    if(active) func();
    active = false;
  }

private:
  // std::decay needed for plain functions to become pointers to functions
  typename std::decay<T>::type func;
  bool active = true;
};

// Used to infer template parameter for scope_guard::guard_object
// std::remove_reference for plain functions, e.g: void(&)() to become void()
template<typename T>
guard_object<typename std::remove_reference<T>::type> make(T&& func) {
  return {std::forward<T>(func)};
}

// operator+ used to infer template parameter for scope_guard::guard_object
struct empty {
  template<typename T>
  guard_object<typename std::remove_reference<T>::type> operator+(T&& func) {
    return {std::forward<T>(func)};
  }
};

}  //namespace _scope_guard

// Macro magic
#define SCOPE_GUARD_CONCAT2(X,Y) X##Y
#define SCOPE_GUARD_CONCAT(X,Y) SCOPE_GUARD_CONCAT2(X,Y)

#define SCOPE_GUARD auto SCOPE_GUARD_CONCAT(_scope_guard_, __LINE__) = scope_guard::empty{} +

#endif
