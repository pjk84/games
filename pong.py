
import time
import sys
import curses
import random



stdscr = curses.initscr()
curses.noecho()
stdscr.keypad(1)
curses.cbreak()
stdscr.timeout(20)

class Pong:

    def __init__(self, difficulty):
        pass
        self.empty = 0
        self.quit = False
        self.game_over = False
        self.difficulty = difficulty

    def setup(self):
        curses.curs_set(0)
        self.header_size = 5
        self.max_height = int(stdscr.getmaxyx()[0]) 
        self.max_width = int(stdscr.getmaxyx()[1])
        self.set_field_size()
        self.offset = int((self.max_width - self.width)/2)
        self.paddle_height = int(self.height/4) +1
        self.start_pos = int(self.header_size + ((self.height/2) - (self.paddle_height/2)))
        self.pos_a = self.start_pos 
        self.pos_b = self.start_pos 
        self.player_step_size = 2
        self.step_sizes = [2, 2, 3, 3, 4]
        self.pos_ball = self.get_center()
        self.y_buffer = 0
        self.velocity = 1
        self.score_a = 0
        self.score_b = 0
        self.y_buffercount = 0
        self.count_down = 50
        self.ai_tick = 0

    def set_field_size(self):
        self.height = self.max_height - self.header_size
        if not self.height%2 == 0:
            self.height -= 1
        self.width = int(self.height * 3) - 1 
    
    def get_center(self):
        self.delta_x = [1, -1][random.randint(0,1)]
        self.y_buffer = 0
        self.set_delta_y()
        return [int((self.width-1)/2), int(self.header_size + self.height / 2 )]
    
    def set_delta_y(self):
        self.dir_y = [-1, 1][random.randint(0, 1)]
        angle = [0, 0.25, 0.5]
        self.delta_y = angle[random.randint(0,2)] * self.dir_y

    def start(self):
        self.setup()
        self.run() 

    def print_board(self):
            field = ''
            for i in range(self.width):
                if i in [0, 1]:
                    field += '#'
                    continue
                if i in [self.width -1, self.width -2]:
                    field += '$'
                    continue
                if i == ((self.width-2)/2) + 0.5:
                    field += '╎'
                    continue
                field += " "
            stdscr.clear()
            for i in range(self.max_height-1):
                draw_ball = False
                if self.pos_ball and  i == self.pos_ball[1]:
                    draw_ball = True
                if i == 0 :
                    # display stats here
                    if self.pos_ball:
                        stdscr.addstr(i, self.offset, f'difficulty: {self.game_over}')
                    continue
                if i == self.header_size - 2: 
                    # score goes here
                    score = [' ' for n in range(self.width)]
                    score[int((self.width - 1) / 2) - 5] = str(self.score_a)
                    score[int((self.width - 1) / 2) + 5] = str(self.score_b)
                    stdscr.addstr(i, self.offset, ''.join(score))
                    continue
                if i == self.header_size - 1: 
                    # score goes here
                    stdscr.addstr(i, self.offset, ''.join(['┈' for n in range(self.width)]))
                    continue
                if i < self.header_size - 1:
                    # top margin
                    stdscr.addstr(i, self.offset, ''.join([' ' for n in range(self.width)]))
                    continue
                f = field
                if i in range(self.pos_a, self.pos_a + self.paddle_height):
                    f = f.replace('#', '┃')
                if i in range(self.pos_b, self.pos_b + self.paddle_height):
                    f = f.replace('$', '┃')
                f = f.replace('#', ' ').replace('$', ' ')
                # game over message
                if self.game_over and i == self.header_size + (self.height / 2):
                    message = f'{"you win" if self.score_b > 10 else "game over"}. continue? y/n'
                    chunk = int(((self.width - 1) / 2) - (len(message)) / 2)
                    stdscr.addstr(i, self.offset, f[:chunk] + message + f[(chunk + len(message)):])
                    continue
                stdscr.addstr(i, self.offset, f"{f if not draw_ball else self.draw_ball(f) } \n")
            stdscr.addstr(i + 1, self.offset, ''.join(['┈' for n in range(self.width)]))
            stdscr.refresh() 
    
    def draw_ball(self, field):
        return field[:self.pos_ball[0]] + '▇' + field[self.pos_ball[0] + 1:] 
    
    def move_ball(self):

        # on paddles, angle of return is random
        # left paddle hit
        if self.pos_ball[0] == 2:
            if self.pos_ball[1] >= self.pos_a and self.pos_ball[1] <= self.pos_a + self.paddle_height:
                self.y_buffer = 0
                self.delta_x = self.velocity
                self.set_delta_y()
        # right paddle hit
        if self.pos_ball[0] == self.width - 3:
           if self.pos_ball[1] >= self.pos_b and self.pos_ball[1] <= self.pos_b + self.paddle_height:
                self.y_buffer = 0
                self.delta_x = -self.velocity
                self.set_delta_y()
            
        # on walls, angle of return is equal to angle of approach
        # hit bottom wall
        if self.pos_ball[1] >= self.max_height - 2:
                self.pos_ball[1] -= 1
                self.delta_y *= -1
        # hit top wall
        if self.pos_ball[1] == self.header_size:
                self.pos_ball[1] += 1
                self.delta_y *= -1
        if self.pos_ball[0] < 0:
            # B scores. wait 50 ticks before starting next round
            self.score_b += 1
            self.count_down = 50
            self.pos_ball = None
            if self.score_b > 10:
                self.game_over = True
            return
        if self.pos_ball[0] > self.width:
            # A scores
            self.score_a += 1
            self.count_down = 50
            self.pos_ball = None
            if self.score_a > 10 :
                self.game_over = True
            return
        # horizontal
        self.y_buffer += self.delta_y
        self.pos_ball[0] += self.delta_x
        if self.y_buffer in [1, -1]:
            self.pos_ball[1] += self.y_buffer
            self.y_buffer = 0


    def ai_move(self):
        if not self.pos_ball:
            return
        def test_border(n, c):
            if c == -1:
                return self.pos_a + n == self.header_size
            return self.pos_a + self.paddle_height + n == self.max_height - 1
        paddle_center = self.pos_a + ((self.paddle_height - 1) / 2)
        c = 1 if paddle_center < self.pos_ball[1] else -1
        if c == 1 and self.pos_a + self.paddle_height == self.max_height:
            # want to move down but already on lower wall
            return
        if c == -1 and self.pos_a == self.header_size:
            # want to move up but already on upper wall
            return
        max_step_size = self.step_sizes[random.randint(0,4)]
        step_size = 0
        if paddle_center == self.pos_ball[1]:
            return
        if self.difficulty == 'hard':
            # center on ball in jumps. Results in faster ai movement.
            while not paddle_center + (step_size * c) == self.pos_ball[1]:
                if test_border(step_size * c, c):
                    break
                step_size += 1
            n = random.randint(0,10)
            # make ai less perfect
            if n < 2:
                step_size = 2
            self.pos_a += step_size * c
            return
        while step_size < max_step_size:
            if test_border(c * step_size, c):
                break
            step_size += 1
        self.pos_a += step_size * c

    def run(self):
        while not self.quit:
            if not self.game_over:
                if self.count_down > 0:
                    if not self.pos_ball:
                        self.pos_ball = self.get_center()
                    self.count_down -= 1
                else:
                    self.move_ball()
                self.ai_tick += 1
                if self.ai_tick == 10:
                    self.ai_tick = 0
                    self.ai_move()
                self.print_board()   
            key = stdscr.getch()
            if key in [121, 110] and self.game_over:
                if key == 121:
                    self.score_a = self.score_b = 0
                    self.game_over = False
                if key == 110:
                    self.quit = True
            if key in [258, 259]:
                step_size = self.player_step_size
                while not step_size < 1:
                    if key == 259:
                        # move up
                        if not self.pos_b - step_size < self.header_size:
                            self.pos_b = self.pos_b - step_size
                            break   
                    if key == 258:
                        # move down
                        if not self.pos_b + self.paddle_height + step_size >= self.max_height:
                            self.pos_b = self.pos_b + step_size
                            break
                    step_size -= 1

# keys: 259:up, 258:down▇


def main():
    try:
        difficulty = sys.argv[1]
    except:
        difficulty = None
    if not difficulty in ['hard', 'normal']:
        return
    j = Pong(difficulty)
    j.start()


if __name__ == "__main__":
    main()



    


# 121 y
# 110 n