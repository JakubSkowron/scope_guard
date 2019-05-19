#include "scope_guard.h"

#include <cstdio>
#include <string>
#include <sstream>
#include <functional>

std::ostringstream result;
template<typename T>
void log(T t) {
  result << t << "|";
}

void f() {
  log("~4f");
}

template<typename T> void owner_template(T t) {
  log("moved 4");
}

void owner( scope_guard::guard_object<void()> t ) {
  log("moved 4");
  owner_template(std::move(t));
}

void owner8(std::function<void()> f) {
  SCOPE_GUARD(f);
}

const char* expected = "1|2|5|3|4f|4f|moved 4|moved 4|~4f|6|~6|7 (cancel)|8|~8|~5|~4f|~3|~2|~1 OK|";
void test() {
  const char* ok = "OK";
  log("1");
  SCOPE_GUARD [=] {
    log(std::string("~1 ") + ok);
  };
  ok = "not OK";

  log("2");
  SCOPE_GUARD [] {
    log("~2");
  };

  log("5");
  auto lambda = []{log("~5");};
  auto guard5 = scope_guard::guard_object<decltype(lambda)>{lambda};

  log("3");
  auto guard3 = scope_guard::make([]{log("~3");});

  void f(void);
  log("4f");
  SCOPE_GUARD f;
  log("4f");
  auto guard4 = scope_guard::make(f);

  owner(std::move(guard4));

  auto guard5_moved = std::move(guard5);

  log("6");
  auto guard6 = scope_guard::make([]{log("~6");});
  guard6.destroy();

  log("7 (cancel)");
  auto guard7 = scope_guard::make([]{log("~7");});
  guard7.cancel();

  log("8");
  auto f8 =[] {
    log("~8");
  };
  owner8(f8);
}

void new_owner(scope_guard::guard_object<void()>) {}
void new_owner(scope_guard::guard_object<std::function<void()>>) {}


void test2() {
 // Without macro:

   auto guard1 = scope_guard::make([]{});

 // Passing ownership:

   auto guard = scope_guard::make(f);
   // for lambdas you can use a template instead of naming the type explicitly
   void new_owner(scope_guard::guard_object<void()>);
   new_owner(std::move(guard));

 // More usage:

   void f(void);
   SCOPE_GUARD(f);
   auto guard3 = scope_guard::empty{} + f; // synonymous to scope_guard::make(f);
   auto guard4 = scope_guard::empty{} + []{}; // synonymous to scope_guard::make([]{});

   auto guard5 = scope_guard::guard_object<std::function<void()>>([]{});

   auto guard6 = scope_guard::make<std::function<void()>>([]{});
   void new_owner(scope_guard::guard_object<std::function<void()>>);
   new_owner(std::move(guard6));
}

int main() {
  test();

  if( result.str() != expected ) {
    std::puts("Failed!");
    std::puts("expected:");
    std::puts(expected);
    std::puts("result:");
    std::puts(result.str().c_str());
    return 1;
  }

  std::puts("Passed!");

  test2();
  return 0;
}

// "at exit"
static SCOPE_GUARD []{};
