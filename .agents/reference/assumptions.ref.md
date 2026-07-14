# Assumptions

Simple notes on things this implementation does **not** fully handle.
Written for reviewers of the coding challenge.

## Serve directory

The server only accepts a plain filename (no folders in the request).
It joins that name with `--dir` and opens the file.

**Assumption:** Everything under `--dir` is a normal file. There are no
symlinks or junctions that point outside the serve folder. If someone puts
such a link in the folder, the server could open a file outside `--dir`.
This was not hardened further to keep the code simple.

## Long file paths

The server builds paths up to about 4 KB, but it opens files with the
standard C `fopen` call.

**Assumption:** File paths stay within Windows’s usual short path limit
(about 260 characters). Longer paths may fail with “file not found” even
when the file exists.

## Special Windows names

Names like `CON`, `PRN`, `AUX`, `NUL`, and `COM1` are allowed by the
filename check.

**Assumption:** Nobody will use those names as real files in the serve
folder. On Windows they are treated as special device names, so behavior
for those requests is not reliable.
