#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
struct nstate {
    uint8_t rega; /* register a */
    uint8_t regb; /* register b */
    uint8_t *cur; /* data ptr */
    unsigned int iloc; /* location of last 'i' */
    bool jbk; /* jump back */
    bool jfwd; /* jump forward */
};

bool debug = false;
bool quiet = false;
int instr = 0;
struct nstate ts;
char istrs[5121] = {0};

void nein(char c) {
start:    if (ts.jbk) c = istrs[++instr];
          else istrs[++instr] = c;
          if (ts.jfwd && c != 'n') {
              if (debug) printf("[%c] instruction ignored: jumping forward\n", c);
              return;
          }
          if (c == '9') *(ts.cur) += 9;
          if (c == 'N') {
              if (ts.cur == &ts.rega) ts.cur = &ts.regb;
              else ts.cur = &ts.rega;
          }
          if (c == 'e') {
              *ts.cur = *ts.cur - ts.regb;
          }
          if (c == 'i') {
              ts.iloc = instr;
              if (ts.rega == 0) ts.jfwd = true;
          }
          if (c == 'n') {
              if (ts.rega != 0) {
                  instr = ts.iloc;
                  ts.jbk = true;
                  goto start;
              }
              ts.jbk = false;
              ts.jfwd = false;
          }
          if (c == '!') {
              if (!quiet) printf("output: %c (%d)\n", (char) *(ts.cur), *(ts.cur));
              else putchar(*(ts.cur));
          }
          if (c == '6') *ts.cur -= 1;
          if (debug) {
              printf("%6d: %s [cmd %c] cur %c; a %d; b %d;\n", instr, (ts.jbk ? "(looping)" : ts.jfwd ? "(jumping forward)" : "normal"), c, (ts.cur == &ts.rega ? 'a' : 'b'), ts.rega, ts.regb);
          }
          if (instr == 5120) {
              memset(istrs, 0, 5121);
              instr = 0;
          }
          if (ts.jbk) goto start;
};
int main(int argc, char *argv[]) {
    ts.rega = 0;
    ts.regb = 0;
    ts.iloc = 0;
    ts.jfwd = false;
    ts.jbk = false;
    ts.cur = &ts.rega;
    if (argc != 1 && argv[1][0] == 'd') debug = true;
    if (argc != 1 && argv[1][0] == 'm') quiet = true;
    if (!quiet) {
        printf("nein, version 1\n");
        printf("eta's little esolang (%s h for help)\n\n", argv[0]);
    }
    if (argc != 1 && argv[1][0] == 'h') {
        printf("nein has two 8-bit registers (A & B) and a data pointer, default pointing at A\n");
        printf("it also has an instruction pointer and space for 5120 (5KiB) instructions\n");
        printf("there are 7 commands (anything else is ignored):\n\n");
        printf("9: increment pointed-to register by 9\n");
        printf("N: move pointer to other register\n");
        printf("e: decrement pointed-to register by the value of register B\n");
        printf("i: jump forward to 'n' if register A == 0\n");
        printf("n: move instruction pointer to last 'i' if register A != 0\n");
        printf("!: output current register value\n");
        printf("6: (nine upside-down!) decrement current register by 1\n");
        printf("\n\nexample (ungolfed) hello world code (prints 'Hello'):\n");
        printf("96i6N9NnN!eN9i6N9NnN9996666666!eN9i6N9NnN999!!9N9666i6N6NnN!eN\n\n");
        printf("invoke with `%s d` for debugger and `%s m` for quiet mode (prints no output except that generated by your program)\nenjoy!\n\n", argv[0], argv[0]);}
    if (debug) {
        printf("ready (debugger enabled)\n");
    }
    else if (!quiet) printf("ready\n");
    char c;
    while (c = getchar()) {
        if (c == EOF) break;
        if (c == '\n') continue;
        nein(c);
    }
    if (!quiet) printf("printout: %s\n", (&istrs[0] + 1));
}
