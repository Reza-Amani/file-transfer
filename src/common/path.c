/*
 * path.c — Reject traversal; join a bare name under the serve root.
 *
 * Validation is intentionally strict and string-based. Once separators and
 * ".." are rejected, opening under --dir cannot escape the serve root.
 */

#include "path.h"

#include <stdio.h>
#include <string.h>

int path_is_bare_filename(const char *name)
{
    size_t i;
    size_t len;

    if (name == NULL) {
        return 0;
    }

    len = strlen(name);
    if (len == 0) {
        return 0;
    }

    /* Exact "." or ".." are not bare file names. */
    if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0) {
        return 0;
    }

    for (i = 0; i < len; i++) {
        char c = name[i];
        if (c == '/' || c == '\\') {
            return 0;
        }
        /* Reject ".." as a substring (covers "a..b" and embedded ".." ). */
        if (c == '.' && i + 1 < len && name[i + 1] == '.') {
            return 0;
        }
    }

    return 1;
}

int path_join_under_root(const char *serve_root, const char *filename,
                         char *out, size_t out_len)
{
    int n;
    size_t root_len;

    if (serve_root == NULL || filename == NULL || out == NULL || out_len == 0) {
        return -1;
    }

    root_len = strlen(serve_root);
    /* Avoid a double separator if --dir already ends with \ or /. */
    if (root_len > 0 &&
        (serve_root[root_len - 1] == '\\' || serve_root[root_len - 1] == '/')) {
        n = snprintf(out, out_len, "%s%s", serve_root, filename);
    } else {
        n = snprintf(out, out_len, "%s\\%s", serve_root, filename);
    }

    if (n < 0 || (size_t)n >= out_len) {
        return -1;
    }
    return 0;
}
