ReadableRegex
=============

ReadableRegex makes using regular expression tolerable. It is based C string literal concatenation (`"123456"` is the same as `"123" "456"`), and wraps regex syntax in macros, to make it resemble a more structured language.

Example: HTTP Request Line Parsing
----------------------------------

HTTP Request lines have the following form:

```
GET /path/to/resource HTTP/1.1
```

They have a method, a URI path, and an http version, with single spaces separating and a CR-LF terminating. The may optinally have a domain in the URI path:

```
GET http://www.example.com/path/to/resource HTTP/1.1
```

The set of valid characters that can be in a URI is:

```
a-z
0-9
._~:/?#[@!$&'()*+,;=]-
```

Everything else must be percent encoded, which is a `%` followed by 2 hexadecimal digits.

We'd like a regular expression to match lines that look like this. We want it to group the method, domain, path, and http version separatly. Normally, it'd look something like this:

```c
const char* http_request_regex =
    "([A-Z]+) (http://(?:[a-z0-9._~:/?#[@!$&'()*+,;=\\]\\-]|%[0-9a-f]{2})*?)?/((?:[a-z0-9._~:/?#[@!$&'()*+,;=\\]\\-]|%[0-9a-f]{2})*) HTTP/[0-9]+\\.[0-9]+\r\n";
```

What horror! There's a reason regular expressions have a reputation as being a "write-only language." Instead, let's try a more structured approach:

```c
#include "readable_regex.h"

#define SPACE " "
#define CR_LF "\r\n"

/* A single URI character- either 1 character or a %HH encoded value */
#define URI_CHARACTER \
    GROUP( EITHER( \
        CLASS( \
            "a-z0-9"               /* ALPHANUMERICS */ \
            "._~:/?#[@!$&'()*+,;=" /* PUNCTUATION */ \
            "\\]\\-"               /* ESCAPED PUNCTUATION */ \
        ), \
        "%" EXACTLY(2, "[0-9a-f]"))) /* PERCENT ENCODED CHARACTER */

/* The complete regex expression. Make sure to compile case insensitive! */
const char* http_request_regex =
    SUBMATCH(AT_LEAST_ONE("[A-Z]")) /* METHOD */
    SPACE
    OPTIONAL(SUBMATCH("http://" LAZY(MANY(URI_CHARACTER))) /* DOMAIN */
    "/" SUBMATCH(MANY(URI_CHARACTER)) /* PATH */
    SPACE
    "HTTP/" SUBMATCH(
        AT_LEAST_ONE("[0-9]")
        "\\."
        AT_LEAST_ONE("[0-9]")) /* HTTP VERSION */
    CR_LF;
```

It may be significantly more verbose, but the function-style structured syntax makes it much easier to both write and read, and the string literal comes out exactly the same when your code is compiled.

Some notes:

- Because this is based on C string literal concatenation, there are nearly no delimiting or separating characters, like commas or semicolons. The exceptions are the macros that take multiple arguments, like `EITHER(EXPR1, EXPR2)` and `EXACTLY(AMOUNT, EXPR)`.

- This syntax implies a level of safety and grouping that isn't nessesarily there. The macros all emit the basic regular expression characters, without any logic or analysis. However, if you add `#define SAFE_REGEX` before you `#include "readable_regex.h"`, all of the potentially ambiguous regex commands are wrapped in non-submatching groups, to remove any ambiguity. Depending on your regex engine, this may cause regex compliation or regex matching to be slower.
