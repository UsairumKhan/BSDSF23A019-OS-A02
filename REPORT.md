### Feature 3: Detailed Listing (-l)

**Objective:**
To extend the ls utility by adding the `-l` option that displays detailed file information such as type, permissions, size, and name.

**Implementation Details:**
- Introduced the `-l` flag in command-line arguments.
- Used `lstat()` instead of `stat()` to handle symbolic links properly.
- Used the `st_mode` field of `struct stat` with bitwise operators and predefined macros (`S_ISDIR`, `S_ISLNK`, `S_IRUSR`, etc.) to extract file type and permissions.
- Printed details in the format similar to `ls -l`:  
  `<type><permissions> <size> <name>`
