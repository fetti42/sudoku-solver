#include <vector>
#include <iostream>
#include <cmath>
#include "functions.hpp"

//Cell functions

void Cell::initialize(int init_value) {

  //error checking would probably be a good idea
  if(init_value != 0) {
    done = true;
    value = init_value;
    for(int i = 0;i<SUDOKU_SIZE;i++){
      possible.push_back(false);
    }
    possible[init_value] = true;    
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

std::vector<int> Cell::get_possible() {
  return(possible);
}

int Cell::get_value() {
  if(!done) {
    return(0);
  }
  else {
    return(value);
  }
}

void Cell::remove_value(int bad_value) {
  possible[bad_value] = false;
  update();
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
  }
  
}

//Group functions


//Sudoku functions

void Sudoku::load_sudoku(std::vector<int> data){
  if(sqrt(data.size) != SUDOKU_SIZE) {
    std::cout << "Size mismatch\n";
  }
  for(int i=0;i<data.size();i++) {
    Cell foo;
    foo.initialize(data[i]);
    cells.push_back(foo);
  }
  
}

void Sudoku::print_sudoku(){
  for(int i=0;i<cells.size();i++) {
    std::cout << cells[i].get_value() << " ";
    if(i%SUDOKU_SIZE == SUDOKU_SIZE-1) {
      std::cout << "\n";
    }
}
