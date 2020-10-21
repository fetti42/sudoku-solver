#include <vector>
#include <iostream>
#include <cmath>
#include <memory>
#include <utility>
#include "functions.hpp"

const int SUDOKU_SIZE = 4;
const int SQUARE_SIZE = sqrt(SUDOKU_SIZE);

//////////////////
//Cell functions//
//////////////////

void Cell::initialize(int init_value) {

  //error checking would probably be a good idea
  if(init_value != 0) {
    done = true;
    value = init_value;
    for(int i = 0;i<SUDOKU_SIZE;i++){
      possible.push_back(false);
    }
    possible[init_value-1] = true;    
  }
  else {
    done = false;
    for(int i = 0;i<SUDOKU_SIZE;i++) {
      possible.push_back(true);
    }
  }
}

bool Cell::is_done() {
  //should I call update here?
  return(done);
}

std::vector<bool> Cell::get_possible() {
  return(possible);
}

void Cell::print_possible() {
  for(int i=0;i<SUDOKU_SIZE;i++) {
    std::cout << possible[i] << " ";
  }
  std::cout << "\n";
}

void Cell::set_value(int new_value) {
  value = new_value;
  done = true;
  for(int i=0;i<SUDOKU_SIZE;i++) {
    if(i!= new_value-1) {
      possible[i] = false;
    }
    else {
      possible[i] = true;
    }
  }
}

int Cell::get_value() {
  if(!done) {
    return(0);
  }
  else {
    return(value);
  }
}

// Returns true if it made a change, false if not
bool Cell::remove_value(int bad_value) {
  if(possible[bad_value-1]) {
    possible[bad_value-1] = false;
    update();
    return(true);
  }
  else {
    return(false);
  }
}

void Cell::update() {
  int num_poss = 0;
  int last_true = 0;

  for(int i=0;i<SUDOKU_SIZE;i++) {
    if(possible[i]) {
      num_poss++;
      last_true = i+1;
    }
  }
  if(num_poss == 1) {
    done = true;
    value = last_true;
    std::cout << "Cell " << id.first << "," << id.second << " value is " << value << "\n";
  }
  
}

void Cell::set_id(std::pair<int, int> new_id) {
  id = new_id;
}

std::pair<int,int> Cell::get_id() {
  return(id);
}

///////////////////
//Group functions//
///////////////////
void Group::add_cell(const std::shared_ptr<Cell> &cell_ptr) {
  cell_ptrs.push_back(cell_ptr);
}

// returns true if any changes were made
bool Group::remove_known() {
  bool change = false;
  int known_value;
  //bool cell_change;
  
  for(int i=0;i<cell_ptrs.size();i++) {
    if(!cell_ptrs[i]->is_done()) {
      continue;
    }
    known_value = cell_ptrs[i]->get_value();
    for(int j=0;j<cell_ptrs.size();j++) {
      //cell_change = false;
      if(i==j) {
	continue;
      }
      if(cell_ptrs[j]->remove_value(known_value)) { 
	change = true;	
      }
    }
  }

  return(change);

}

void Group::set_cell_value(int cell_index, int new_value) {
  cell_ptrs[cell_index]->set_value(new_value);
}
  
int Group::get_cell_value(int cell_index) {
  return(cell_ptrs[cell_index]->get_value());
}

bool Group::update() {
  //call all the remove_knowns from here?
  bool change;

  change = remove_known();

  return(change);
}

void Group::set_test_value(int new_value) {
  test_value = new_value;
}

int Group::get_test_value() {
  return(test_value);
}

////////////////////
//Sudoku functions//
////////////////////

void Sudoku::load_sudoku(std::vector<int> data){
  if(sqrt(data.size()) != SUDOKU_SIZE) {
    std::cout << "Size mismatch\n";
  }
  for(int i=0;i<data.size();i++) {
    // Cell foo;
    std::shared_ptr<Cell> p(new Cell());
    std::pair<int, int> temp_id(floor(i/SUDOKU_SIZE),i%SUDOKU_SIZE); 
    p->initialize(data[i]);
    p->set_id(temp_id);
    cell_ptrs.push_back(p);
  }
  make_groups();
  
}


std::shared_ptr<Cell>& Sudoku::get_cell(std::pair<int,int> cell_id) {
  return(cell_ptrs[cell_id.first*SUDOKU_SIZE+cell_id.second]);
}

void Sudoku::print_sudoku(){
  for(int i=0;i<cell_ptrs.size();i++) {
    std::cout << cell_ptrs[i]->get_value() << " ";
    if(i%SUDOKU_SIZE == SUDOKU_SIZE-1) {
      std::cout << "\n";
    }
  }
  std::cout << "\n";
}

//make standard sudoku groups
void Sudoku::make_groups() {
  int x_coord, y_coord;
  
  //make empty groups
  for(int i=0;i<SUDOKU_SIZE*3;i++) {
    Group foo;
    groups.push_back(foo);
  }

  //add the row groups
  for(int i=0;i<SUDOKU_SIZE;i++) {
    x_coord = i;
    for(int j=0;j<SUDOKU_SIZE;j++) {
      y_coord = j;
      std::pair<int,int> cell_id(x_coord, y_coord);
      groups[i].add_cell(get_cell(cell_id));
    }
  }

  //add the column groups
  for(int i=0;i<SUDOKU_SIZE;i++) {
    y_coord = i;
    for(int j=0;j<SUDOKU_SIZE;j++) {
      x_coord = j;
      std::pair<int,int> cell_id(x_coord, y_coord);
      groups[i+SUDOKU_SIZE].add_cell(get_cell(cell_id));
    }
  }

  //add the square groups
  for(int i=0;i<SUDOKU_SIZE;i++) {
    int x_orig = i*SQUARE_SIZE%SUDOKU_SIZE;
    int y_orig = floor(i/SQUARE_SIZE);
    
    for(int x_mod=0;x_mod<SQUARE_SIZE;x_mod++) {
      for(int y_mod=0;y_mod<SQUARE_SIZE;y_mod++) {
	std::pair<int,int> foo(x_orig+x_mod, y_orig+y_mod);
	groups[i+SUDOKU_SIZE*2].add_cell(get_cell(foo));
      }
    }
  }
}

void Sudoku::solve() {
  bool change = true;
  while(change) {
    change = false;
    for(int i=0;i<groups.size();i++) {
      if(groups[i].update()) {
	change = true;
      }
    }
  }
}
