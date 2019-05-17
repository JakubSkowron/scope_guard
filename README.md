# scope_guard
C++ scope guard, a way to run code at exit of current scope. The code is in public domain.

Example:

```c++
  FILE* file = std::fopen( argv[1], "r");
  if(!file) {
    std::puts("file error");
    return 1;
  }

  scope_guard [=] {
    std::fclose(file);
  };
```

In example above `std::fclose` will be called in any case when execution leaves current function, code block, etc.

If you don't need to capture any variables use empty capture:

```c++
 scope_guard [] {
  std::puts("at scope exit");
 };
```
