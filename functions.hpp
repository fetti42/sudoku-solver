#include <iostream>
#include <vector>

//int SUDOKU_SIZE = 4;

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
  void print_possible();
  int get_value();
  void set_value(int new_value);
  bool remove_value(int bad_value);
  void update();
  void set_id(int new_id);
  int get_id();
  
};

class Group{
  std::vector<Cell> cells;
  
  int test_value = 0;
  
public:
  void add_cell(Cell &cell);
  //bool check_for_singles();
  bool remove_known();
  void set_cell_value(int cell_index, int new_value);
  int get_cell_value(int cell_index);
  bool update();

  void set_test_value(int new_value);
  int get_test_value();
  
};

class Sudoku{
  std::vector<Cell> cells;
  std::vector<Group> groups;

public:
  void load_sudoku(std::vector<int> data);
  void print_sudoku();
  void make_groups();
  void solve();
};
