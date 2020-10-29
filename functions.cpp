#include <vector>
#include <iostream>
#include <cmath>
#include <memory>
#include <utility>
#include "functions.hpp"


////////////////////
// Cell functions //
////////////////////

void Cell::initialize(int init_value) {
  // Initialize cell. If init_value is 1-9, cell value is set.
  // If init_value is 0, cell value is unknown and all values are possible.

  value = init_value-1; // Subtract 1 to account for the 1-up/0-up issue
  //error checking would probably be a good idea
  if(init_value != 0) {
    done = true;
    for(int i = 0;i<SUDOKU_SIZE;i++){
      possible.push_back(false);
    }
    possible[value] = true;    
  }
  else {
    done = false;
    for(int i = 0;i<SUDOKU_SIZE;i++) {
      possible.push_back(true);
    }
  }
}

bool Cell::is_done() {
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
  value = new_value;
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

// Remove specified value from possible values.
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

// If only one possibility, updates cell value to it
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

void Cell::set_id(std::pair<int, int> new_id) {
  id = new_id;
}

std::pair<int,int> Cell::get_id() {
  return(id);
}

/////////////////////
// Group functions //
/////////////////////
void Group::add_cell(const std::shared_ptr<Cell> &cell_ptr) {
  cell_ptrs.push_back(cell_ptr);
}

// returns true if any changes were made
// For each cell, if value is known, remove that value
// from every other cell in group
bool Group::remove_known() {
  bool change = false;
  int known_value;
  
  for(int i=0;i<cell_ptrs.size();i++) {
    if(!cell_ptrs[i]->is_done()) {
      continue;
    }
    known_value = cell_ptrs[i]->get_value();
    for(int j=0;j<cell_ptrs.size();j++) {
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
// Returns true if change was made, false if not
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

// Checks whether a specified value is known in group
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

// Run the various tests
bool Group::update() {
  bool change = false;

  if(remove_known()) {
    change = true;
  }

  if(check_one_poss()) {
    change = true;
  }

  return(change);
}

bool Group::check_set(set_struct set) {
  bool change = false;
  bool found;

  // check if all of the cell ids in the set are in the group
  for(int i=0;i<set.cell_ids.size();i++) {
    found = false;
    std::pair<int,int> id = set.cell_ids[i];

    for(int j=0;j<cell_ptrs.size();j++) {
      if(id == cell_ptrs[j]->get_id()) {
	found = true;
	continue;
      }
    }
    if(!found) {
      break;
    }
  }
  if(!found) {
    change = false;
  }

  else {
    /*std::cout << "Value " << set.value << ", " << "Found group that entirely contains set ";
    for(int i=0;i<set.cell_ids.size();i++) {
      std::cout << set.cell_ids[i].first << "," << set.cell_ids[i].second << "; ";
    }
    std::cout << "\n";*/
    // remove the value from all the other cells
    for(int i=0;i<cell_ptrs.size();i++) {
      found = false;
      for(int j=0;j<set.cell_ids.size();j++) {
	if(cell_ptrs[i]->get_id() == set.cell_ids[j]) {
	  found = true;
	  break;
	}
      }
      if(!found) {
	if(cell_ptrs[i]->remove_value(set.value)) {
	  change = true;
	}
      }
    }
      
  }
  /*if(change){
    std::cout << "Made a change\n";
    }*/
  return(change);
}

bool Group::check_cell_poss(int value, int cell_index) {
  return(cell_ptrs[cell_index]->get_possible()[value]);
}

std::pair<int,int> Group::get_cell_id(int cell_index) {
  return(cell_ptrs[cell_index]->get_id());
}

//////////////////////
// Sudoku functions //
//////////////////////


// Read in sudoku data and set up cells and groups
// Values are entered as string of space-separated digits
// Unknown values should be entered as 0
void Sudoku::load_sudoku(std::vector<int> data){
  if(sqrt(data.size()) != SUDOKU_SIZE) {
    std::cout << "Size mismatch\n";
  }
  for(int i=0;i<data.size();i++) {
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
    int y_orig = floor(i/SQUARE_SIZE)*SQUARE_SIZE;
    
    for(int x_mod=0;x_mod<SQUARE_SIZE;x_mod++) {
      for(int y_mod=0;y_mod<SQUARE_SIZE;y_mod++) {
	std::pair<int,int> cell_id(x_orig+x_mod, y_orig+y_mod);
	groups[i+SUDOKU_SIZE*2].add_cell(get_cell(cell_id));
      }
    }
  }
}


// As long as there are changes, update all groups
void Sudoku::solve() {
  bool change = true;
  while(change) {
    change = false;
    for(int i=0;i<groups.size();i++) {
      if(groups[i].update()) {
	change = true;
      }
    }
    make_sets();
    if(check_sets()) {
      change = true;
    }
    //print_sudoku();
  }
}

// Sets are all the cells within a group that can possibly be a certain value 
void Sudoku::make_sets() {
  sets.clear();
  
  //for each value

  for(int i=0;i<SUDOKU_SIZE;i++) {
    
    //for each group
    for(int j=0;j<groups.size();j++) {
      // if value is not done in group
      if(!groups[j].known_val(i)) {
	
	//find set of cells that can contain that value
	set_struct set;
	set.value = i;

	for(int k=0;k<SUDOKU_SIZE;k++) {
	  if(groups[j].check_cell_poss(i,k)) { //checks if i is a possible value for cell k
	    set.cell_ids.push_back(groups[j].get_cell_id(k)); //gets cell id of cell k
	  }
	}
	sets.push_back(set);
      }
    }
  }
}

// For each group, check if any sets are entirely contained within the group.
// If so, all other cells in the group cannot have the set's value.
// Returns true if any changes were made, false if none.
bool Sudoku::check_sets() {
  bool change = false;

  //for each set
  for(int i=0;i<sets.size();i++) {

  //for each group
    for(int j=0;j<groups.size();j++) {

  //check the group for the set
      if(groups[j].check_set(sets[i])){
	change = true;
      }
    }
  }

  return(change);
}
