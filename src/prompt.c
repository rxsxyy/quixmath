#include "prompt.h"

#include "string.h"
#include "termios.h"
#include "unistd.h"

// Reads exactly one byte without blocking; returns -1 on timeout/error.
static i32 read_byte_timed(struct termios *raw_nb) {
    tcsetattr(STDIN_FILENO, TCSANOW, raw_nb);
    u8 c;
    i32 n = (i32)read(STDIN_FILENO, &c, 1);
    return (n == 1) ? (i32)c : -1;
}

i32 prompt_read(char *buf, usize cap) {
    struct termios old, raw, raw_nb;

    tcgetattr(STDIN_FILENO, &old);

    // Canonical raw mode (blocking, 1 byte at a time, no echo).
    raw = old;
    raw.c_lflag &= (tcflag_t) ~(ECHO | ICANON | ISIG);
    raw.c_cc[VMIN] = 1;
    raw.c_cc[VTIME] = 0;

    // Non-blocking variant used only for peeking after ESC.
    raw_nb = raw;
    raw_nb.c_cc[VMIN] = 0;
    raw_nb.c_cc[VTIME] = 1; // 100 ms timeout

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);

    char data[256];
    usize len = 0;
    usize cur = 0;
    i32 ret = 0;

    while (1) {
        u8 c;
        if (read(STDIN_FILENO, &c, 1) != 1) {
            break;
        }

        // ── Escape / arrow-key sequence ──────────────────────────────────
        if (c == 0x1b) {
            i32 b1 = read_byte_timed(&raw_nb);
            tcsetattr(STDIN_FILENO, TCSANOW, &raw); // restore blocking mode

            if (b1 == '[') {
                // Arrow-key escape sequence: ESC [ <code>
                u8 code;
                if (read(STDIN_FILENO, &code, 1) != 1) {
                    break;
                }
                switch (code) {
                case 'C': // right arrow
                    if (cur < len) {
                        cur++;
                        write(STDOUT_FILENO, "\x1b[C", 3);
                    }
                    break;
                case 'D': // left arrow
                    if (cur > 0) {
                        cur--;
                        write(STDOUT_FILENO, "\x1b[D", 3);
                    }
                    break;
                case 'A': // up   arrow — ignore
                case 'B': // down arrow — ignore
                    break;
                default:
                    break;
                }
            } else {
                // Bare ESC (b1 == -1 means no follow-up byte within timeout).
                write(STDOUT_FILENO, "\n", 1);
                ret = -1;
                break;
            }
            continue;
        }

        // ── Enter ────────────────────────────────────────────────────────
        if (c == '\r' || c == '\n') {
            write(STDOUT_FILENO, "\n", 1);
            break;
        }

        // ── Backspace / DEL ──────────────────────────────────────────────
        if (c == 127 || c == 8) {
            if (cur == 0) {
                continue;
            }
            memmove(data + cur - 1, data + cur, len - cur);
            len--;
            cur--;

            // Move back, reprint tail, erase last char, reposition cursor.
            write(STDOUT_FILENO, "\b", 1);
            write(STDOUT_FILENO, data + cur, len - cur);
            write(STDOUT_FILENO, " ", 1);
            for (usize i = cur; i <= len; i++) {
                write(STDOUT_FILENO, "\b", 1);
            }
            continue;
        }

        // ── Printable character ──────────────────────────────────────────
        if (c >= 0x20 && c < 0x7f && len < cap - 1) {
            memmove(data + cur + 1, data + cur, len - cur);
            data[cur] = (char)c;
            len++;

            // Print from cursor to end, then rewind to new cursor position.
            write(STDOUT_FILENO, data + cur, len - cur);
            cur++;
            for (usize i = cur; i < len; i++) {
                write(STDOUT_FILENO, "\b", 1);
            }
        }
    }

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &old);

    if (ret == 0) {
        data[len] = '\0';
        memcpy(buf, data, len + 1);
    }

    return ret;
}
