/*
 * path.h — Bare-filename validation and serve-root path join.
 *
 * A bare filename is a single path segment: no '/', '\\', or '..'.
 * The server resolves it only under the serve root (--dir).
 */

#ifndef FT_PATH_H
#define FT_PATH_H

#include <stddef.h>

/*
 * Return 1 if name is a valid bare filename, 0 otherwise.
 * Rejects empty, ".", "..", path separators, and any ".." substring.
 */
int path_is_bare_filename(const char *name);

/*
 * Build "<serve_root>\<filename>" into out (nul-terminated).
 * Returns 0 on success, -1 if the result would not fit in out_len.
 * Does not validate the filename; call path_is_bare_filename first.
 */
int path_join_under_root(const char *serve_root, const char *filename,
                         char *out, size_t out_len);

#endif /* FT_PATH_H */
