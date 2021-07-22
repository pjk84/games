import sys
import random


def dice():

    dice_count = input("how many dice? ")
    try:
        dice_count = int(dice_count)
    except:
        print("not a valid int")
        return

    rolls = []
    total = 0
    for n in range(dice_count):
        roll = random.randint(1, 6)
        total += roll
        rolls.append(roll)

    list.sort(rolls)

    states = {
        1: ['│       │', '│   o   │', '│       │'],
        2: ['│ o     │', '│       │', '│     o │'],
        3: ['│ o     │', '│   o   │', '│     o │'],
        4: ['│ o   o │', '│       │', '│ o   o │'],
        5: ['│ o   o │', '│   o   │', '│ o   o │'],
        6: ['│ o   o │', '│ o   o │', '│ o   o │']
    }

    print(''.join(["┌-------┐" for roll in rolls]))
    for i in range(3):
        print(''.join([states[roll][i] for roll in rolls]))
    print(''.join(["└-------┘" for roll in rolls]))
    print(f"🎲 total: {total}")

def tictactoe():


    class Game:
        def __init__(self):
            self.games = 0 
            self.game_over = False
            self.empty = ' '
            self.matrix = self.make_empty_matrix()

        def start(self):
            self.setup()
            self.games += 1
            self.matrix = self.make_empty_matrix()
            while not self.game_over:
                self.make_move()
        
        def setup(self):
            self.player_turn = False
            s = ['o', 'x']
            r = random.randint(0,1)
            self.player_symbol = s[r]
            self.opponent_symbol = s[(r-1)*-1]
            if r:
                self.player_turn = True
            print(f"---- you are playing: {self.player_symbol}")
            if self.games == 0:
                print("---- type 'exit', 'quit or 'q' to exit the program")

            return

        def make_empty_matrix(self):
            return [[self.empty for i in range(3)] for i in range(3)]
        
        
        def print_board(self):
            print("    1   2   3")
            print("  ┌───┬───┬───┐")
            for i in range(3):
                row = ["A", "B", "C"]
                print(f"{row[i]} │ {self.matrix[i][0]} │ {self.matrix[i][1]} │ {self.matrix[i][2]} │")
                if i < 2:
                    print("  ├───┼───┼───┤")
                else:
                    print("  └───┴───┴───┘")

        def check_if_exit(self, inp):
            if inp.strip().lower() in ['exit', 'quit', 'q']:
                self.game_over = True
                return 1

        def make_move(self):
            if self.player_turn:
                self.print_board()
                valid_move = False
                while not valid_move:
                    try:
                        move = input("make a move: ")
                        if self.check_if_exit(move):
                            return
                        coords = self.parse_move(move)
                        address = self.matrix[coords[1]][coords[0]]
                        if not len(address.strip()) == 0:
                            raise Exception
                        valid_move = True
                    except Exception as e:
                        print(f'❗ --- illegal move')
                        continue
                        
                self.matrix[coords[1]][coords[0]] = self.player_symbol
                self.test_win()
                self.player_turn = False
                return
            self.ai_make_move()

        def check_opportunities(self, opportunity):
            focus = self.opponent_symbol if opportunity == 'win' else self.player_symbol

            # evaluate player victory opportunity. block is found
            m = self.matrix
            for i, l in enumerate(m):
                # by row
                if l.count(focus) == 2 and l.count(self.empty) == 1:
                    return [i, l.index(self.empty)]
                # by col
                col = [m[0][i], m[1][i], m[2][i]]
                if col.count(focus) == 2 and col.count(self.empty) == 1:
                    return [col.index(self.empty), i]
            # by diagonal
            d1 = [m[0][0], m[1][1], m[2][2]]
            if d1.count(focus) == 2 and d1.count(self.empty) == 1:
                return [d1.index(self.empty), d1.index(self.empty)]
            d1 = [m[2][0], m[1][1], m[0][2]]
            if d1.count(focus) == 2 and d1.count(self.empty) == 1:
                if d1.index(self.empty) == 1:
                    return [1, 1]
                if d1.index(self.empty) == 2:
                    return [0, 2]
                return [2, 0]
            return

        def ai_make_move(self):
            made_move = False
            if opp := self.check_opportunities('win'):
                # test for move that wins the game
                self.matrix[opp[0]][opp[1]] = self.opponent_symbol
                made_move = True
            if not made_move:
                if opp := self.check_opportunities('block'):
                    # test if player has opportunity to win. If found, block
                    self.matrix[opp[0]][opp[1]] = self.opponent_symbol
                    made_move = True
            # random
            while not made_move:
                row = random.randint(0, 2)
                col = random.randint(0, 2)
                address = self.matrix[row][col]
                if address == self.empty:
                    self.matrix[row][col] = self.opponent_symbol
                    made_move = True
            self.test_win()
            self.player_turn = True
        
        def end_game(self, draw=False):
            if draw:
                print('⚖️ --- the game ended in a draw')
            else:
                print(f'👑 --- game over: {"player" if self.player_turn else "computer"} has won')
            self.print_board()
            if input('play again? ').lower() in ['yes', 'y']:
                return self.start()
            self.game_over = True
            return

        def test_win(self):
            def validate_win(symbol):
                if not symbol == self.empty:
                    return self.end_game()
            m = self.matrix
            for i in range(3):
                # rows
                if m[i][0] == m[i][1] == m[i][2]:
                    validate_win(m[i][0])
                # cols
                if m[0][i] == m[1][i] == m[2][i]:
                    validate_win(m[0][i])
            t = m[0]+m[1]+m[2]
            # diagonal
            if t[0] == t[4] == t[8]:
                validate_win(t[0])
            if t[2] == t[4] == t[6]:
                validate_win(t[2])
            if [n.count(self.empty) for n in m].count(0) == 3:
                return self.end_game(draw=True)
            return

        @staticmethod
        def parse_move(move):
            move = move.upper()
            rows = {
                'A': 0, 
                'B': 1,
                "C": 2
            }
            return [int(move[1])-1, rows[move[0]]]


    g = Game()
    g.start()


def main():
    game = sys.argv[1]
    if game == 'dice':
        return dice()
    if game == 'tictactoe':
        return tictactoe()
    

if __name__ == "__main__":
    main()