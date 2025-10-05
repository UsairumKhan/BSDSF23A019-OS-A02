# lsv1.2.0

This version adds the `-l` flag for detailed file listing.

## New Features
- `-l` → Show file type, permissions, size, and name.
- Uses `lstat()` to identify symbolic links.
- Displays permissions using bitwise checks on `st_mode`.

## Example Usage
make
./bin/ls
./bin/ls -l
./bin/ls /etc -l

## Example Output
-rw-r--r--  1024  file1.txt
drwxr-xr-x  4096  myfolder
lrwxrwxrwx    11  shortcut
# BSDSF23A019-OS-A02
