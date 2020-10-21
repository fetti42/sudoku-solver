#include <vector>
#include <iostream>
#include <cmath>
#include <memory>
#include <utility>
#include "functions.hpp"

const int SUDOKU_SIZE = 9;
const int SQUARE_SIZE = sqrt(SUDOKU_SIZE);

//////////////////
//Cell functions//
//////////////////

void Cell::initialize(int init_value) {

  value = init_value-1; // Fix the 1-up/0-up issue
  //error checking would probably be a good idea
  if(init_value != 0) {
    done = true;
    //value = init_value-1;
    for(int i = 0;i<SUDOKU_SIZE;i++){
      possible.push_back(false);
    }
    possible[value] = true;    
  }
  else {
    //value = -1;
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
  if(done) {
    //this should be real error checking
    std::cout << "Cell already has a value " << get_value() << ", can't set it to " << new_value << "\n";
  }
  value = new_value;//account for off by one error
  done = true;
  for(int i=0;i<SUDOKU_SIZE;i++) {
    if(i!= new_value) {
      possible[i] = false;
    }
    else {
      possible[i] = true;
    }
  }
}

int Cell::get_value() {
  return(value);
}

// Returns true if it made a change, false if not
bool Cell::remove_value(int bad_value) {
  if(possible[bad_value]) {
    possible[bad_value] = false;
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
      last_true = i;
    }
  }
  if(num_poss == 1) {
    done = true;
    value = last_true;
    //std::cout << "Cell " << id.first << "," << id.second << " value is " << value << "\n";
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

//Checks if there is only one possibility in the group for each value
bool Group::check_one_poss() {
  bool change = false;
  int num_poss = 0;
  int last_seen;
  
  for(int i=0;i<SUDOKU_SIZE;i++) {
    num_poss = 0;
    last_seen = 0;
    if(known_val(i)) {
      continue;
    }
    for(int j=0;j<cell_ptrs.size();j++) {
      if(cell_ptrs[j]->get_possible()[i]) {
	num_poss++;
	last_seen = j;
      }
    }
    if(num_poss == 1) {
      cell_ptrs[last_seen]->set_value(i);
      change = true;
      //std::cout << "Setting cell " << cell_ptrs[last_seen]->get_id().first << "," << cell_ptrs[last_seen]->get_id().second << " to " << i+1 << "\n";
    }
    
  } 
  return(change);
}

bool Group::known_val(int number) {
  bool known = false;
  for(int i=0;i<cell_ptrs.size();i++) {
    if(cell_ptrs[i]->is_done() && cell_ptrs[i]->get_value() == number) {
      known = true;
    }
  }
  return(known);
}

void Group::set_cell_value(int cell_index, int new_value) {
  cell_ptrs[cell_index]->set_value(new_value);
}
  
int Group::get_cell_value(int cell_index) {
  return(cell_ptrs[cell_index]->get_value());
}

bool Group::update() {
  //call all the remove_knowns from here?
  bool change = false;

  if(remove_known()) {
    change = true;
  }

  if(check_one_poss()) {
    change = true;
  }

  return(change);
}

void Group::set_test_value(int new_value) {
  test_value = new_value;
}

int Group::get_test_value() {
  return(test_value);
}

bool Group::val_done(int value) {
  bool done = false;

  for(int i=0;i<cell_ptrs.size();i++) {
    if(cell_ptrs[i]->is_done() && cell_ptrs[i]->get_value() == value) {
      done = true;
    }
  }
  return(done);
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
    std::cout << cell_ptrs[i]->get_value()+1 << " ";
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

  //std::cout << "Row groups\n";
  //add the row groups
  for(int i=0;i<SUDOKU_SIZE;i++) {
    x_coord = i;
    //std::cout << "Group " << i << "\n";
    for(int j=0;j<SUDOKU_SIZE;j++) {
      y_coord = j;
      std::pair<int,int> cell_id(x_coord, y_coord);
      groups[i].add_cell(get_cell(cell_id));
      //std::cout << cell_id.first << "," << cell_id.second << " ";
    }
    //std::cout << "\n";
  }
  //std::cout << "\n  Column groups\n";

  //add the column groups
  for(int i=0;i<SUDOKU_SIZE;i++) {
    y_coord = i;
    //std::cout << "Group " << i << "\n";
    for(int j=0;j<SUDOKU_SIZE;j++) {
      x_coord = j;
      std::pair<int,int> cell_id(x_coord, y_coord);
      groups[i+SUDOKU_SIZE].add_cell(get_cell(cell_id));
      //std::cout << cell_id.first << "," << cell_id.second << " ";
    }
    //std::cout << "\n";
  }

  //std::cout << "\n  Square groups \n";

  //add the square groups
  for(int i=0;i<SUDOKU_SIZE;i++) {
    //std::cout << "Group " << i << "\n";
    int x_orig = i*SQUARE_SIZE%SUDOKU_SIZE;
    int y_orig = floor(i/SQUARE_SIZE)*SQUARE_SIZE;
    
    for(int x_mod=0;x_mod<SQUARE_SIZE;x_mod++) {
      for(int y_mod=0;y_mod<SQUARE_SIZE;y_mod++) {
	std::pair<int,int> cell_id(x_orig+x_mod, y_orig+y_mod);
	groups[i+SUDOKU_SIZE*2].add_cell(get_cell(cell_id));
	//std::cout << cell_id.first << "," << cell_id.second << " ";
      }
    }
    //std::cout << "\n";
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
    //print_sudoku();
  }
}


void Sudoku::make_sets() {
  //for each value
  for(int i=0;i<SUDOKU_SIZE;i++) {
    
    //for each group
    for(int j=0;j<groups.size();j++) {
      // if value is not done in group

      //find set of cells that can contain that value
    }
  }
     
  
}

bool Sudoku::check_sets() {
  bool change = false;

  //for each set

  //for each group

  //if set is entirely contained in group

  //remove value from other cells in group

  return(change);
}
