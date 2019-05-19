# scope_guard
C++ scope guard, a way to run code at exit of current scope. The code is in public domain.
Creates objcect which will call a function in its destructor. Can be moved or cancelled.

Basic usage:

```c++
SCOPE_GUARD [=] {
  std::fclose(file);
};

SCOPE_GUARD [] {
  std::puts("at scope exit");
};
```

The same without using macros:

```c++
auto guard = scope_guard::make([]{std::puts("at scope exit");});
```

Passing ownership:

```c++
auto guard = scope_guard::make(f);
void new_owner(scope_guard::guard_object<void()>);
new_owner(std::move(guard));
```

For lambdas you can use std::function:

```c++
auto guard = scope_guard::make<std::function<void()>>([]{});
void new_owner(scope_guard::guard_object<std::function<void()>>);
new_owner(std::move(guard));
```

Works also on plain functions:

```c++
void f(void);
SCOPE_GUARD(f);  // initialization of a scope_guard::guard_object
```

You can declare it static:

```c++
static SCOPE_GUARD [] {
  std::puts("at exit");
};
```

Example:

```c++
  FILE* file = std::fopen(argv[1], "r");
  if(!file) {
    std::puts("file error");
    return 1;
  }

  SCOPE_GUARD [=] {
    std::fclose(file);
  };
```

In example above `std::fclose` will be called in any case when execution leaves current function or code block.

Works with C++11.

