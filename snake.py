
import time
import sys
import curses
import random





stdscr = curses.initscr()


def cleanup():
    curses.echo()
    stdscr.keypad(0)
    curses.nocbreak()
    curses.endwin()

class Snake:

    def __init__(self):
        pass
        self.quit = False

    def setup(self):
        curses.noecho()
        stdscr.keypad(1)
        curses.cbreak()
        stdscr.timeout(20)
        curses.curs_set(0)
        self.game_over = False
        self.pos_x = 0
        self.pox_y = 0
        self.food_pos = None
        self.width = 40
        self.height = 20
        self.offset = 10
        self.all_x = [int(self.width/2)] 
        self.all_y = [int(self.height/2)]
        self.dir_y = 0
        self.dir_x = 1
        self.test = None

    def get_food(self):
        self.food_pos = None
        while not self.food_pos:
            food_pos = [random.randint(0, self.width-1), random.randint(1, self.height-1)]
            if not food_pos[0] in self.all_x and not food_pos[1] in self.all_y:
                self.food_pos = food_pos

    def move(self):
        x = self.all_x.copy()
        y = self.all_y.copy()
        self.all_x[0] += self.dir_x
        self.all_y[0] += self.dir_y
        for i in range(len(x)):
            if i == 0:
                continue
            self.all_x[i] = x[i-1]
            self.all_y[i] = y[i-1]
        if self.food_pos:
            if self.all_x[0] == self.food_pos[0] and self.all_y[0] == self.food_pos[1]:
                self.get_food()
                self.all_x.append(x[len(x) - 1])
                self.all_y.append(y[len(y) - 1])
        self.test_collision()

    def turn(self, key):
        """
            left = 260
            right = 261
            up = 259
            down = 258
        """
        if key == 258:
            if self.dir_y == -1:
                return
            self.dir_x = 0
            self.dir_y = 1
        if key == 259:
            if self.dir_y == 1:
                return
            self.dir_x = 0
            self.dir_y = -1
        if key == 261:
            if self.dir_x == -1:
                return
            self.dir_y = 0
            self.dir_x = 1
        if key == 260:
            if self.dir_x == 1:
                return
            self.dir_y = 0
            self.dir_x = -1

    def test_collision(self):
        # snake eats self
        if len(set(list(zip(*[self.all_y, self.all_x])))) < len(self.all_x):
            self.game_over = True
        # hit wall
        if self.all_x[0] < 0 or self.all_x[0] >= self.width:
            self.game_over = True
        if self.all_y[0] < 1 or self.all_y[0] == self.height:
            self.game_over = True
            
    def start(self):
        self.setup()
        self.run() 

    def print_board(self):
        stdscr.clear()
        len_str = f'snake length: {len(self.all_x)}'
        for i in range(self.height):
            if i == 0:
                stdscr.addstr(i, self.offset+1, f'_{len_str}{"".join(["_"] * (self.width - len(len_str)-1))}')
                continue
            row = [f'{"_" if i == self.height - 1 else " "}'] * (self.width)
            if self.game_over:
                game_over_str = 'game over. Play again? y/n'
                if i == self.height / 2:
                    row = row[0:int((self.width/2) - (len(game_over_str)/2))]  + [game_over_str] + row[int((self.width/2) - (len(game_over_str)/2)) + len(game_over_str):]
            else:
                for n, pos_y in enumerate(self.all_y):
                    if pos_y == i:
                        row[self.all_x[n]] = '▇'
                if i == self.food_pos[1]:
                    row[self.food_pos[0]] = '𓅞'
            row = "".join(row)
            stdscr.addstr(i, 10, f'┊{row}┊')
        stdscr.refresh()

    def run(self):
        while not self.quit:
            if not self.game_over:
                self.move()
                if not self.food_pos:
                    self.get_food()
                self.print_board()   
            time.sleep(0.1)
            key = stdscr.getch()
            if key in range(258,262):
                self.turn(key)
            if key in [121, 110] and self.game_over:
                if key == 121:
                    self.setup()
                if key == 110:
                    cleanup()
                    self.quit = True

def main():
    j = Snake()
    j.start()
    cleanup()

if __name__ == "__main__":
    main()
