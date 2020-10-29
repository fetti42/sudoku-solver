#ifndef SUDOKU_LIBRARY
#define SUDOKU_LIBRARY

#include <iostream>
#include <vector>
#include <memory>
#include <utility>
#include <cmath>

const int SUDOKU_SIZE = 9;
const int SQUARE_SIZE = sqrt(SUDOKU_SIZE);


struct set_struct {
  int value;
  std::vector<std::pair<int,int>> cell_ids;
}; 


class Cell{
  std::vector<bool> possible;
  int value;
  std::pair<int, int> id;
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
  void set_id(std::pair<int,int> new_id);
  std::pair<int,int> get_id();
  
};

class Group{
  std::vector<std::shared_ptr<Cell>> cell_ptrs;
  
  int test_value = 0;
  
public:
  void add_cell(const std::shared_ptr<Cell> &cell_ptr);
  //bool check_for_singles();
  bool remove_known();
  bool check_one_poss();
  bool known_val(int number);
  void set_cell_value(int cell_index, int new_value);
  int get_cell_value(int cell_index);
  bool update();
  bool check_set(set_struct set);

  /*  void set_test_value(int new_value);
  int get_test_value();
  bool val_done(int value);*/
  
};

class Sudoku{
  std::vector<std::shared_ptr<Cell>> cell_ptrs;
  std::vector<Group> groups;
  //list of sets; set is pair of value and list of cell ids
  //std::vector<std::pair<int,std::vector<std::pair<int,int>>>> sets;
  std::vector<set_struct> sets;

public:
  void load_sudoku(std::vector<int> data);
  std::shared_ptr<Cell>& get_cell(std::pair<int,int> cell_id);
  void print_sudoku();
  void make_groups();
  void solve();
  void make_sets();
  bool check_sets();
};


#endif
