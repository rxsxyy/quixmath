#include "quixmath.h"

typedef struct {
    char data[256];
    usize len;
    usize cur;
} Line;

static struct termios s_old, s_raw, s_raw_nb;

static void term_save(void) {
    tcgetattr(STDIN_FILENO, &s_old);

    s_raw = s_old;
    s_raw.c_lflag &= (tcflag_t) ~(ECHO | ICANON | ISIG);
    s_raw.c_cc[VMIN] = 1;
    s_raw.c_cc[VTIME] = 0;

    s_raw_nb = s_raw;
    s_raw_nb.c_cc[VMIN] = 0;
    s_raw_nb.c_cc[VTIME] = 1; // 100ms timeout
}

static void term_raw(void) {
    tcsetattr(STDIN_FILENO, TCSANOW, &s_raw);
}

static void term_raw_nb(void) {
    tcsetattr(STDIN_FILENO, TCSANOW, &s_raw_nb);
}

static void term_restore(void) {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &s_old);
}

static i32 read_byte(void) {
    u8 c;
    return (read(STDIN_FILENO, &c, 1) == 1) ? (i32)c : -1;
}

// Returns -1 on abort (bare ESC), 0 on handled sequence.
static i32 handle_escape(Line *ln) {
    term_raw_nb();
    i32 b1 = read_byte();
    term_raw();

    if (b1 != '[') {
        write(STDOUT_FILENO, "\n", 1);
        return -1;
    }

    i32 code = read_byte();

    if (code == 'C' && ln->cur < ln->len) {
        ln->cur++;
        write(STDOUT_FILENO, "\x1b[C", 3);
    } else if (code == 'D' && ln->cur > 0) {
        ln->cur--;
        write(STDOUT_FILENO, "\x1b[D", 3);
    }

    return 0;
}

static void handle_backspace(Line *ln) {
    if (ln->cur == 0) {
        return;
    }

    memmove(ln->data + ln->cur - 1, ln->data + ln->cur, ln->len - ln->cur);
    ln->len--;
    ln->cur--;

    write(STDOUT_FILENO, "\b", 1);
    write(STDOUT_FILENO, ln->data + ln->cur, ln->len - ln->cur);
    write(STDOUT_FILENO, " ", 1);

    for (usize i = ln->cur; i <= ln->len; i++) {
        write(STDOUT_FILENO, "\b", 1);
    }
}

static void handle_char(Line *ln, u8 c, usize cap) {
    if (c < 0x20 || c >= 0x7f || ln->len >= cap - 1) {
        return;
    }

    memmove(ln->data + ln->cur + 1, ln->data + ln->cur, ln->len - ln->cur);
    ln->data[ln->cur] = (char)c;
    ln->len++;

    write(STDOUT_FILENO, ln->data + ln->cur, ln->len - ln->cur);
    ln->cur++;

    for (usize i = ln->cur; i < ln->len; i++) {
        write(STDOUT_FILENO, "\b", 1);
    }
}

i32 prompt_read(char *buf, usize cap) {
    term_save();
    term_raw();

    Line ln = {{0}, 0, 0};
    i32 ret = 0;

    while (1) {
        i32 c = read_byte();
        if (c == -1) {
            break;
        }

        if (c == 0x1b) {
            if (handle_escape(&ln) == -1) {
                ret = -1;
                break;
            }
            continue;
        }

        if (c == '\r' || c == '\n') {
            write(STDOUT_FILENO, "\n", 1);
            break;
        }
        if (c == 127 || c == 8) {
            handle_backspace(&ln);
            continue;
        }

        handle_char(&ln, (u8)c, cap);
    }

    term_restore();

    if (ret == 0) {
        ln.data[ln.len] = '\0';
        memcpy(buf, ln.data, ln.len + 1);
    }

    return ret;
}
