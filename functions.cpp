#include <vector>
#include <iostream>
#include <cmath>
#include "functions.hpp"

int SUDOKU_SIZE = 4;

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
  }
  
}

void Cell::set_id(int new_id) {
  id = new_id;
}

int Cell::get_id() {
  return(id);
}

///////////////////
//Group functions//
///////////////////
void Group::add_cell(Cell &cell) {
  cells.push_back(cell);
}

// returns true if any changes were made
bool Group::remove_known() {
  bool change = false;
  int known_value;
  //bool cell_change;
  
  for(int i=0;i<cells.size();i++) {
    if(!cells[i].is_done()) {
      continue;
    }
    known_value = cells[i].get_value();
    for(int j=0;j<cells.size();j++) {
      //cell_change = false;
      if(i==j) {
	continue;
      }
      if(cells[j].remove_value(known_value)) { 
	change = true;	
      }
    }
  }

  return(change);

}

void Group::set_cell_value(int cell_index, int new_value) {
  cells[cell_index].set_value(new_value);
}
  
int Group::get_cell_value(int cell_index) {
  return(cells[cell_index].get_value());
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
    Cell foo;
    foo.initialize(data[i]);
    foo.set_id(i);
    cells.push_back(foo);
  }
  make_groups();
  
}

void Sudoku::print_sudoku(){
  for(int i=0;i<cells.size();i++) {
    std::cout << cells[i].get_value() << " ";
    if(i%SUDOKU_SIZE == SUDOKU_SIZE-1) {
      std::cout << "\n";
    }
  }
  std::cout << "\n";
}

//make standard sudoku groups
void Sudoku::make_groups() {
  //make empty groups
  for(int i=0;i<SUDOKU_SIZE*3;i++) {
    Group foo;
    groups.push_back(foo);
  }
  //add cells to groups
  for(int i=0;i<cells.size();i++) {
    //add to the row group
    int row_num = floor(i/SUDOKU_SIZE);
    groups[row_num].add_cell(cells[i]);

    //add to the column group
    int col_num = i%SUDOKU_SIZE;
    groups[col_num+SUDOKU_SIZE].add_cell(cells[i]);

    //add to the block group
    int block_num = 0;
    //there has got to be a better way to do this
    //4x4
    if(i==0||i==1||i==4||i==5) {
      block_num = 0;
    }
    if(i==2||i==3||i==6||i==7) {
      block_num = 1;
    }
    if(i==8||i==9||i==12||i==13) {
      block_num = 2;
    }
    if(i==10||i==11||i==14||i==15) {
      block_num = 3;
    }
    //9x9
    /*if(i==0||i==1||i==2||i==9||i==10||i==11||i==18||i==19||i==20) {
      block_num = 0;
    }
    if(i==3||i==4||i==5||i==12||i==13||i==14||i==21||i==22||i==23) {
      block_num = 1;
    }
    if(i==6||i==7||i==8||i==15||i==16||i==17||i==24||i==25||i==26) {
      block_num = 2;
    }
    if(i==27||i==28||i==29||i==36||i==37||i==38||i==45||i==46||i==47) {
      block_num = 3;
    }
    if(i==30||i==31||i==32||i==39||i==40||i==41||i==48||i==49||i==50) {
      block_num = 4;
    }
    if(i==33||i==34||i==35||i==42||i==43||i==44||i==51||i==52||i==53) {
      block_num = 5;
    }
    if(i==54||i==55||i==56||i==63||i==64||i==65||i==72||i==73||i==74) {
      block_num = 6;
    }
    if(i==57||i==58||i==59||i==66||i==67||i==68||i==75||i==76||i==77) {
      block_num = 7;
    }
    if(i==60||i==61||i==62||i==69||i==70||i==71||i==78||i==79||i==80) {
      block_num = 8;
      }*/
    groups[block_num+SUDOKU_SIZE*2].add_cell(cells[i]);
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
