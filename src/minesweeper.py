import itertools
import random


class Minesweeper:

    def __init__(self, size=10, num_mines=10):
        self.size = size
        self.num_mines = num_mines
        self.board = [[" " for _ in range(size)] for _ in range(size)]
        self.visible = [[False for _ in range(size)] for _ in range(size)]
        self.flags = set()  # Tracks coordinates marked as mines
        self.mines = set()

    def place_mines(self, start_row, start_col):
        safe_cells = set(
            itertools.product(
                range(start_row - 1, start_row + 2),
                range(start_col - 1, start_col + 2),
            )
        )
        possible_mines = [
            (r, c)
            for r in range(self.size)
            for c in range(self.size)
            if (r, c) not in safe_cells
        ]
        self.mines = set(random.sample(possible_mines, self.num_mines))

    def count_neighbors(self, row, col):
        count = 0
        for r in range(row - 1, row + 2):
            for c in range(col - 1, col + 2):
                if (r, c) in self.mines:
                    count += 1
        return count

    def reveal(self, row, col):
        if not (0 <= row < self.size and 0 <= col < self.size):
            return True
        if self.visible[row][col] or (row, col) in self.flags:
            return True  # Cannot reveal flagged tiles

        self.visible[row][col] = True

        if (row, col) in self.mines:
            return False

        count = self.count_neighbors(row, col)
        if count > 0:
            self.board[row][col] = str(count)
        else:
            self.board[row][col] = "0"
            for r in range(row - 1, row + 2):
                for c in range(col - 1, col + 2):
                    self.reveal(r, c)
        return True

    def toggle_flag(self, row, col):
        if self.visible[row][col]:
            print("Cannot flag a revealed tile.")
            return
        if (row, col) in self.flags:
            self.flags.remove((row, col))
            print(f"Removed flag from ({row}, {col}).")
        else:
            self.flags.add((row, col))
            print(f"Flagged ({row}, {col}) as a mine.")

    def check_win(self):
        for r in range(self.size):
            for c in range(self.size):
                if not self.visible[r][c] and (r, c) not in self.mines:
                    return False
        return True

    def print_board(self, reveal_all=False):
        print("\n   " + " ".join([f"{i:2d}" for i in range(self.size)]))
        print("  " + "-" * (3 * self.size + 1))
        for r in range(self.size):
            row_str = []
            for c in range(self.size):
                if reveal_all and (r, c) in self.mines:
                    row_str.append("X")  # Show exploded mines at game over
                elif (r, c) in self.flags:
                    row_str.append("*")  # Show flagged locations
                elif self.visible[r][c]:
                    row_str.append(self.board[r][c])
                else:
                    row_str.append("+")
            print(f"{r:2d}| " + "  ".join(row_str) + " |")
        print("  " + "-" * (3 * self.size + 1))
        print(f"Flags placed: {len(self.flags)} / Total Mines: {self.num_mines}")

    def play(self):
        first_click = True

        while True:
            self.print_board()

            # One-line command: action + row + col
            cmd = input("Enter move (e.g., d 3 5 or f 3 5): ").strip().lower().split()

            if len(cmd) != 3:
                print("Invalid input. Format: d 3 5 or f 3 5")
                continue

            action, r_str, c_str = cmd

            if action not in ["d", "f"]:
                print("Invalid action. Use 'd' to dig or 'f' to flag.")
                continue

            try:
                r, c = int(r_str), int(c_str)
            except ValueError:
                print("Row and column must be numbers.")
                continue

            if not (0 <= r < self.size and 0 <= c < self.size):
                print("Coordinates out of bounds.")
                continue

            # Process action
            if action == "f":
                self.toggle_flag(r, c)
            else:  # action == "d"
                if (r, c) in self.flags:
                    print("That location is flagged! Unflag it first to dig.")
                    continue

                if first_click:
                    self.place_mines(r, c)
                    first_click = False

                safe = self.reveal(r, c)
                if not safe:
                    self.print_board(reveal_all=True)
                    print("\n💥 Game Over! You hit a mine. 💥")
                    break

            if self.check_win():
                self.print_board(reveal_all=True)
                print("\n🏆 Congratulations! You won! 🏆")
                break


if __name__ == "__main__":
    game = Minesweeper()
    game.play()
