#!/bin/bash

# import library, please check path
#source /usr/lib/simple_curses.sh
#source /usr/local/lib/simple_curses.sh
source ../../bashsimplecurses/simple_curses.sh

# You must create a "main" function:
main () {
    # Your code here, here we add some windows and text
    window "title" "color"
    append "Text..."
    endwin
}

# Then, execute the loop every second ( -t 1 => 1s)
main_loop -t 1

