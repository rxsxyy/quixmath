#include "vars.h"

#include "!INFO.h"
#include "ctype.h"
#include "string.h"

typedef struct {
    char name[VAR_NAME_MAX];
    mpfr_t value;
    bool live;
} VarEntry;

static VarEntry g_vars[VAR_CAP];
static usize g_var_count = 0;

void vars_init(void) {
    g_var_count = 0;
    for (usize i = 0; i < VAR_CAP; i++) {
        g_vars[i].live = false;
    }
}

void vars_free(void) {
    for (usize i = 0; i < VAR_CAP; i++) {
        if (g_vars[i].live) {
            mpfr_clear(g_vars[i].value);
            g_vars[i].live = false;
        }
    }
    g_var_count = 0;
}

static VarEntry *find_var(const char *name) {
    for (usize i = 0; i < g_var_count; i++) {
        if (g_vars[i].live && strcmp(g_vars[i].name, name) == 0) {
            return &g_vars[i];
        }
    }
    return NULL;
}

bool var_set(const char *name, const char *val_str) {
    VarEntry *e = find_var(name);
    if (!e) {
        if (g_var_count >= VAR_CAP) {
            return false;
        }
        e = &g_vars[g_var_count++];
        strncpy(e->name, name, VAR_NAME_MAX - 1);
        e->name[VAR_NAME_MAX - 1] = '\0';
        mpfr_init2(e->value, QM_PRECISION);
        e->live = true;
    }
    char *end;
    mpfr_strtofr(e->value, val_str, &end, 10, MPFR_RNDN);
    return end != val_str;
}

bool var_get(const char *name, mpfr_t out) {
    VarEntry *e = find_var(name);
    if (!e) {
        return false;
    }
    mpfr_set(out, e->value, MPFR_RNDN);
    return true;
}

bool var_exists(const char *name) {
    return find_var(name) != NULL;
}

void vars_scan(const char *expr, void (*cb)(const char *name)) {
    // Tracks which names have already been reported so cb is called once per name.
    char seen[VAR_CAP][VAR_NAME_MAX];
    usize seen_count = 0;
    const char *p = expr;

    while (*p) {
        if (!isalpha((u8)*p)) {
            p++;
            continue;
        }

        char name[VAR_NAME_MAX];
        usize len = 0;
        while (isalnum((u8)*p) && len < VAR_NAME_MAX - 1) {
            name[len++] = *p++;
        }
        name[len] = '\0';

        // deduplicate
        bool already = false;
        for (usize i = 0; i < seen_count; i++) {
            if (strcmp(seen[i], name) == 0) {
                already = true;
                break;
            }
        }
        if (!already && seen_count < VAR_CAP) {
            memcpy(seen[seen_count++], name, len + 1);
            cb(name);
        }
    }
}
