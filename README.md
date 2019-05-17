# scope_guard
C++ scope guard, a way to run arbitrary code at scope exit.

Example:
```c++
#include "scope_guard.h"

int func() {
  FILE* file = std::fopen( argv[1], "r");
  if(!file) {
    // handle error
    return 1;
  }
  scope_guard {
    std::fclose(file);
  }
  ...
}
```
In example above `std::fclose` will be called in any case when execution leaves the function.
