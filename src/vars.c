#include "quixmath.h"

typedef struct {
    char name[VARIABLE_NAME_MAX];
    mpfr_t value;
    bool live;
} Variable;

static Variable var_group[VARIABLE_MAX];
static usize var_count = 0;

void vars_init(void) {
    var_count = 0;
    for (usize i = 0; i < VARIABLE_MAX; i++) {
        var_group[i].live = false;
    }
}

void vars_free(void) {
    for (usize i = 0; i < VARIABLE_MAX; i++) {
        if (var_group[i].live) {
            mpfr_clear(var_group[i].value);
            var_group[i].live = false;
        }
    }
    var_count = 0;
}

static Variable *var_find(const char *name) {
    for (usize i = 0; i < var_count; i++) {
        if (var_group[i].live && strcmp(var_group[i].name, name) == 0) {
            return &var_group[i];
        }
    }

    return NULL;
}

bool var_set(const char *name, const char *val_str) {
    Variable *n = var_find(name);
    if (!n) {
        if (var_count >= VARIABLE_MAX) {
            return false;
        }

        n = &var_group[var_count++];
        strncpy(n->name, name, VARIABLE_NAME_MAX - 1);
        n->name[VARIABLE_NAME_MAX - 1] = '\0';
        mpfr_init2(n->value, PRECISION);
        n->live = true;
    }

    char *end;
    mpfr_strtofr(n->value, val_str, &end, 10, MPFR_RNDN);
    return end != val_str;
}

bool var_get(const char *name, mpfr_t out) {
    Variable *n = var_find(name);
    if (!n) {
        return false;
    }

    mpfr_set(out, n->value, MPFR_RNDN);
    return true;
}

bool var_exists(const char *name) {
    return var_find(name) != NULL;
}

void vars_scan(const char *expr, void (*cb)(const char *name)) {
    // tracks which names have already been reported so cb is called once per
    // name
    char seen[VARIABLE_MAX][VARIABLE_NAME_MAX];
    usize seen_count = 0;
    const char *p = expr;

    while (*p) {
        if (!isalpha((u8)*p)) {
            p++;
            continue;
        }

        char name[VARIABLE_NAME_MAX];
        usize len = 0;
        while (isalnum((u8)*p) && len < VARIABLE_NAME_MAX - 1) {
            name[len++] = *p++;
        }
        name[len] = '\0';

        // deduplicate, just so it doesnt read the same variable twice
        bool already = false;
        for (usize i = 0; i < seen_count; i++) {
            if (strcmp(seen[i], name) == 0) {
                already = true;
                break;
            }
        }

        if (!already && seen_count < VARIABLE_MAX) {
            memcpy(seen[seen_count++], name, len + 1);
            cb(name);
        }
    }
}
