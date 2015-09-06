# nein: a little esolang

a tiny esoteric language, inspired by [BF](https://en.wikipedia.org/wiki/Brainfuck) and [this thing](https://www.youtube.com/watch?v=GVC_g68nUpo)

compile:

    $ gcc nein.c -o nein
    
run:

    $ ./nein h
    
nein takes one argument: it can be 'h' for help, 'm' for quiet mode (just process stdin and writes output to stdout, no other messages) or 'd' to use a debugger.


## language specification

nein has two 8-bit registers (A & B) and a data pointer, default pointing at A.
it also has an instruction pointer and space for 5120 (5KiB) instructions. Registers are
**unsigned** and wrap around after 255.

there are 7 commands (anything else is ignored):

| Command | Definition |
| ------- | ---------- |
| 9       | increase pointed-to register by 9 |
| N       | move pointer to other register |
| e       | decrement pointed-to register by the value of register B
| i       | jump forward to 'n' if register A == 0 |
| n       | move instruction pointer to last 'i' if register A != 0 |
| !       | output pointed-to register's value |
| 6       | (nine upside-down) decrement pointed-to register by 1 |

## hello world!

The following nein code prints the word "Hello":

    96i6N9NnN!eN9i6N9NnN9996666666!eN9i6N9NnN999!!9N9666i6N6NnN!eN

The first part of this code is analysed below:

    9 # add 9 to register A
    6 # regA = 8 now (-1)
    i # begin loop
    6 # decrement current register, as a loop counter (register A)
    N # switch registers (to B, the one we want to increment)
    9 # add 9 to B
    N # switch back
    n # loop
    N # switch to register B
    ! # will print 'H' (ascii 72).
    
enjoy!