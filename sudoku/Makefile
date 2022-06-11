sudoku_ncurses: sudoku_ncurses.c ../dadactyl/keymaps/default/sudoku.c
	gcc -I../dadactyl/keymaps/default -g -o sudoku_ncurses sudoku_ncurses.c -lncurses

clean:
	rm -f sudoku_ncurses log.txt sudoku_seeds

sudoku_seeds: sudoku_seeds.c
	gcc -O2 -o sudoku_seeds sudoku_seeds.c

seeds: sudoku_seeds
	./sudoku_seeds
