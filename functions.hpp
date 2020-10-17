#include <iostream>
#include <vector>

int SUDOKU_SIZE = 9;

class Cell{
  std::vector<bool> possible;
  int value;
  int id;
  int x_coord;
  int y_coord;
  bool done;

public:
  void initialize(int init_value);
  bool is_done();
  std::vector<bool> get_possible();
  int get_value();
  void remove_value(int bad_value);
  void update();
  
};

class Group{
  std::vector<Cell> cells;

public:
  void add_cell(Cell &cell);

  void update();
  
};

class Sudoku{
  std::vector<Cell> cells;
  std::vector<Group> groups;

public:
  void load_sudoku(std::vector<int> data);

  void print_sudoku();
}
