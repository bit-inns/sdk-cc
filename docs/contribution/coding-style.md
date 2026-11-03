# Coding style


## Formatting

- **Line length:** Set the editor ruler to 120 columns. Wrap lines longer than 120 characters.

  ```json
  "editor.rulers": [120]
  ```

- **Pointers and references:** Attach `*`, `&`, and `&&` to the name, with a space after the type. Put `const` before the type.

  ```cpp
  poly *output;
  const poly *input;
  const poly &value = *input;
  T &&value;
  ```

- **Indentation:** Use two spaces, no tabs. Indent nested namespace declarations and their braces by two spaces per nesting level. Keep their contents at the same indentation level as the nested namespace declaration; do not add another indentation level for the contents. Top-level namespace contents are not indented.

  ```cpp
  namespace crypto::mlkem
  {


  void process(poly *p) {
    poly_reduce(p);
  }


  } // namespace crypto::mlkem
  ```

  ```cpp
  namespace crypto::mlkem
  {


    namespace impl
    {


    void process(poly *p) {
      poly_reduce(p);
    }


    } // namespace impl


  } // namespace crypto::mlkem
  ```

- **Braces:** Namespace and type braces start on a new line. Function and control-flow braces stay on the same line. End types with `};`.

  ```cpp
  struct poly
  {
    int16_t coeffs[256];
  };

  void process(poly *p) {
    if (ready) {
      poly_reduce(p);
    } else {
      poly_ntt(p);
    }
  }
  ```

- **Control flow:** Put a space after keywords. Braces may be omitted for an `if`, `else if`, or `else` branch containing a single statement on one line. Use braces for other bodies and whenever omission would make an `else` bind to the wrong `if`.

  ```cpp
  for (int i = 0; i < count; ++i) {
    process(i);
  }

  if (ready) process(value);
  else if (retry) process_again(value);
  else return;
  ```


## Spacing

- **Header guards and body:** No empty line between `#ifndef` and `#define`. Leave one empty line after `#define` before includes. Surround body content (declarations, definitions, or namespaces) with two empty lines: before its start and after its end. If there are no includes, the body starts two empty lines after `#define`. If there is no body content, leave only one empty line before the closing `#endif`. Do not indent the body for the guard.

  ```cpp
  #ifndef CRYPTO_MLKEM_POLY_H
  #define CRYPTO_MLKEM_POLY_H

  #include <cstdint>


  // Declarations.


  #endif // #ifndef CRYPTO_MLKEM_POLY_H
  ```

  An include-only header has no body content:

  ```cpp
  #ifndef CRYPTO_H
  #define CRYPTO_H

  #include "sdk/crypto/mlkem/mlkem.h"

  #endif // #ifndef CRYPTO_H
  ```

- **Binary operators:** Use one space around operators outside array brackets and function arguments.

  ```cpp
  int value = (a + b) * c;
  int shifted = value << 4;
  ```

- **Array brackets:** No padding or spaces around arithmetic and shift operators inside brackets, including array bounds.

  ```cpp
  uint8_t buf[mlkem_max_eta1*mlkem_n/4];
  p->coeffs[8*i+j] = a - b;
  value = input[(i+1)%n];
  value = table[index>>2];
  ```

- **Function calls:** No spaces inside arguments except after separating commas. No space before `(`.

  ```cpp
  foo(value, (a+b)*c);
  ```

- **Unary operators:** No space between operator and operand.

  ```cpp
  ++i;
  bool valid = !fail;
  poly_reduce(&result);
  ```

- **Member access:** No spaces around `->` or `.`.

  ```cpp
  value = p->coeffs[0];
  value = result.coeffs[0];
  ```


## Blank Lines

- **Namespaces:** Treat a namespace as a label around its body content. Leave two empty lines before the namespace declaration and surround nonempty body content with two empty lines: after the opening brace and before the closing brace. Shared boundaries need only two empty lines, not an extra pair for each nested namespace. Do not add empty namespace blocks. Keep namespace comments accurate.

  ```cpp
  #include "sdk/crypto/mlkem/mlkem.h"


  namespace crypto::mlkem
  {


  void process(poly *p) {
    poly_reduce(p);
  }


  } // namespace crypto::mlkem
  ```

- **Anonymous namespaces:** Do not add anonymous (`namespace { ... }`) wrappers unless internal linkage is needed. Test helpers can be declared directly at file scope.

- **Function groups:** Leave one empty line between functions with the same or similar purpose, and two empty lines between unrelated function groups.

  ```cpp
  void reduce(poly *p) {
    poly_reduce(p);
  }

  void reduce_vector(const mlkem_ctx *ctx, poly_vec *pv) {
    poly_vec_reduce(ctx, pv);
  }


  void transform(poly *p) {
    poly_ntt(p);
  }
  ```

- **Trailing whitespace:** No spaces or tabs at the end of a line, including empty lines.

  ```cpp
  int value = 0;

  process(value);
  ```

- **End of file:** No empty lines or newline character after the last content line.

  ```text
  } // namespace crypto::mlkem
  ```


## Includes

- **Project headers:** Use double quotes. Use `sdk/` paths for public headers and relative paths for nearby private helpers.

  ```cpp
  #include "sdk/crypto/mlkem/mlkem.h"
  #include "impl/core.h"
  ```

- **Standard and platform headers:** Use angle brackets.

  ```cpp
  #include <cstdint>
  #include <cstring>
  ```

- **Order:** In `.cc` files, include the matching header first, then standard and platform headers, then other project headers. Separate groups with one empty line. Sort includes alphabetically within each group.

  ```cpp
  #include "sdk/crypto/mlkem/poly.h"

  #include <cstdint>
  #include <cstring>

  #include "sdk/crypto/mlkem/mlkem.h"
  ```

- **Dependencies:** Include headers directly for the types and functions used. Headers must compile on their own. Use forward declarations for pointer and reference declarations to avoid circular includes.

  ```cpp
  namespace crypto::mlkem
  {


  struct mlkem_ctx;
  struct poly_vec;

  void poly_vec_reduce(const mlkem_ctx *ctx, poly_vec *pv);


  } // namespace crypto::mlkem
  ```

- **Header guards:** Use `#ifndef` and `#define` in headers. Name the guard after the path relative to `include/sdk/`, using uppercase letters and underscores. Close with `#endif // #ifndef GUARD_NAME`.

  ```cpp
  #ifndef CRYPTO_MLKEM_POLY_H
  #define CRYPTO_MLKEM_POLY_H

  #include <cstdint>


  // Declarations.


  #endif // #ifndef CRYPTO_MLKEM_POLY_H
  ```
