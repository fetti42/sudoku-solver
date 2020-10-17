#include <vector>
#include <iostream>
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

std::vector<int> Cell::return_possible() {
  return(possible);
}

int Cell::return_value() {
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
}

void Sudoku::print_sudoku(){
}
