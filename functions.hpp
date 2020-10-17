#include <iostream>
#include <vector>

class Cell{
  std::vector<int> values;
  int id;
  int x_coord;
  int y_coord;
  bool done;

public:
  void set_values(int init_value);
  bool is_done();
  std::vector<int> return_possible();
  int return_value();
  void remove_value(int bad_value);
  
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
