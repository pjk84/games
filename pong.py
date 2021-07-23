
from pynput.keyboard import Key, Listener
import time
import sys
import curses




stdscr = curses.initscr()
curses.noecho()
stdscr.keypad(1)
curses.cbreak()
stdscr.timeout(10)

class Pong:

    def __init__(self, side):
        pass
        self.empty = 0
        self.game_over = False
        self.side = side

    def setup(self):
        curses.curs_set(0)
        self.header_size = 5
        self.max_height = int(stdscr.getmaxyx()[0]) 
        self.max_width = int(stdscr.getmaxyx()[1])
        self.set_field_size()
        self.offset = int((self.max_width - self.width)/2)
        self.pedal_height = int(self.height/4) +1
        self.start_pos = int(self.header_size + ((self.height/2) - (self.pedal_height/2)))
        self.pos_a = self.start_pos 
        self.step_size = 2
        self.pos_b = self.start_pos 
        self.ball_size = 2
        self.pos_ball = self.get_center()
        self.dirX = 1
        

    def set_field_size(self):
        self.height = self.max_height - self.header_size
        # make height even
        if not self.height%2 == 0:
            self.height -= 1
        self.width = int(self.height * 3) - 1 
    
    def get_center(self):
        return [int((self.width-1)/2), int(self.header_size + self.height / 2 )]
    
    def start(self):
        self.setup()
        self.run() 

                #       if i in range(self.pos_a, self.pos_a + self.pedal_height):
                #     field = field.replace('#', '┃')
                # if i in range(self.pos_b, self.pos_b + self.pedal_height):
                #     field = field.replace('&', '┃')
                # field = field.replace('&', ' ').replace('#', ' ')

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
            for i in range(self.max_height-1):
                draw_ball = False
                if self.pos_ball and  i == self.pos_ball[1]:
                    draw_ball = True
                if i == self.header_size -1 :
                    stdscr.addstr(i, self.offset, f'height:{i}, self_width:{self.width} pos_x:{str(self.pos_ball)}')
                    continue
                if i < self.header_size - 1:
                    stdscr.addstr(i, self.offset, ''.join([' ' for n in range(self.width)]))
                    continue
                f = field
                if i in range(self.pos_a, self.pos_a + self.pedal_height):
                    f = f.replace('#', '┃')
                if i in range(self.pos_b, self.pos_b + self.pedal_height):
                    f = f.replace('$', '┃')
                f = f.replace('#', ' ').replace('$', ' ')
                stdscr.addstr(i, self.offset, f"{f if not draw_ball else self.draw_ball(f) } \n")
            
    
    def draw_ball(self, field):
        return field[:self.pos_ball[0]] + '█' + field[self.pos_ball[0] + 1:] 
    
    def move_ball(self):
        if self.pos_ball[0] in [0, self.width]:
            # new round
            self.pos_ball = self.get_center()
        if self.pos_ball[0] in [2, self.width-2]:
            self.dirX *= -1
            # return
        # horizontal
        self.pos_ball[0] += self.dirX


        
    def run(self):
        n = 0
        while not self.game_over:
            stdscr.clear()
            n +=1
            self_pos = 'pos_a' if self.side == 'A' else 'pos_b'

            self.move_ball()
            self.print_board()
            stdscr.refresh()    
            # refresh rate
            time.sleep(0.05)
            # listen for input
            key = stdscr.getch()
            if key in [258, 259]:
                step_size = self.step_size
                while not step_size < 1:
                    if key == 259:
                        if not getattr(self, self_pos) - step_size < self.header_size:
                            setattr(self, self_pos, getattr(self, self_pos) - step_size) 
                            break   
                    if key == 258:
                        if not getattr(self, self_pos) + self.pedal_height + step_size >= self.max_height:
                            setattr(self, self_pos, getattr(self, self_pos) + step_size) 
                            break
                    step_size -= 1

# keys: 259:up, 258:down▇


def main():
    try:
        side = sys.argv[1].upper()
    except:
        side = None
    if not side in ['A', 'B']:
        print('side has to be A or B')
        return
    j = Pong(side)
    j.start()


if __name__ == "__main__":
    main()



    