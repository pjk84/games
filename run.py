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
            self.player_symbol = None
            self.game_over = False
            self.empty = '┈'
            self.matrix = self.make_empty_matrix()
            self.setup()

        def start(self):
            if not self.setup_complete:
                self.setup()
            self.matrix = self.make_empty_matrix()
            if self.player_symbol == 'x':
                self.player_turn = True
            else:
                self.player_turn = False
            while not self.game_over:
                self.make_move()
        
        def setup(self):
            print("######### -- let's play tic-tac-toe -- #########")
            while not self.player_symbol:
                symbol = input("choose symbol (x or o only) ")
                if not symbol in ['x', 'o']:
                    print("symbol not allowed")
                    continue
                self.player_symbol = symbol
            s = ['x', 'o']
            s.remove(self.player_symbol)
            self.opponent_symbol = s[0]
            self.setup_complete = True

        @staticmethod
        def make_empty_matrix():
            return [["┈", "┈", "┈"] for i in range(3)]
        
        
        def print_board(self):
            print("   1  2  3")
            print("  ┌-------┐")
            for i in range(3):
                row = ["A", "B", "C"]
                print(f"{row[i]} │{self.matrix[i][0]}  {self.matrix[i][1]}  {self.matrix[i][2]}│")
            print("  └-------┘")

        def make_move(self):
            if self.player_turn:
                move = input("make a move: ").upper()
                if move.strip() in ['EXIT', 'QUIT']:
                    self.game_over = True
                    return
                valid_move = False
                while not valid_move:
                    try:
                        coords = self.parse_move(move)
                        address = self.matrix[coords[1]][coords[0]]
                        if not address == '┈':
                            raise Exception
                        valid_move = True
                    except:
                        print(f'❗ --- illegal move')
                        move = input("make a move: ")
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
                if address == "┈":
                    self.matrix[row][col] = self.opponent_symbol
                    made_move = True
            self.test_win()
            self.player_turn = True
        
        def end_game(self, draw=False):
            if draw:
                print('⚖️ --- the game ended in a draw')
            else:
                print(f'👑 --- game over: {"player" if self.player_turn else "computer"} has won')
            if input('play again? ').lower() in ['yes', 'y']:
                return self.start()
            self.game_over = True
            return

        def test_win(self):
            self.print_board()
            m = self.matrix
            if [n.count(self.empty) for n in m].count(0) == 3:
                return self.end_game(draw=True)
            for i in range(3):
                if m[i][0] == m[i][1] == m[i][2]:
                    if not m[i][0] == "┈":
                        return self.end_game()
            t = m[0]+m[1]+m[2]
            if t[1] == t[4] == t[7]:
                if not t[1] == "┈":
                    return self.end_game()
            if t[0] == t[4] == t[8]:
                if not t[0] == "┈":
                    return self.end_game()
            if t[2] == t[4] == t[6]:
                if not t[2] == "┈":
                    return self.end_game()
            if t[0] == t[3] == t[6]:
                if not t[0] == "┈":
                    return self.end_game()
            if t[2] == t[5] == t[8]:
                if not t[2] == "┈":
                    return self.end_game()
            return

        @staticmethod
        def parse_move(move):
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