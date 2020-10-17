#include <iostream>
#include <vector>

class Cell{
  std::vector<int> values;
  int id;
  int x_coord;
  int y_coord;
  bool done;

public:
  
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
