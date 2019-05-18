# scope_guard
C++ scope guard, a way to run code at exit of current scope. The code is in public domain.

Example:

```c++
  FILE* file = std::fopen( argv[1], "r");
  if(!file) {
    std::puts("file error");
    return 1;
  }

  SCOPE_GUARD [=] {
    std::fclose(file);
  };
```

In example above `std::fclose` will be called in any case when execution leaves current function, code block, etc.

Another example:

```c++
 auto guard1 = scope_guard::make([]{std::puts("Test1");});
 void f(void);
 auto guard2 = scope_guard::make(f);
```

Or using macro:

```c++
 void f(void);
 SCOPE_GUARD f;
```

If you don't need to capture any variables use empty capture:

```c++
 SCOPE_GUARD [] {
  std::puts("at scope exit");
 };
```
