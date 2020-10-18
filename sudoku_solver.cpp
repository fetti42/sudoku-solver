#include <iostream>
#include "functions.hpp"

int main() {
  Cell cell1;
  Cell cell2;

  cell1.initialize(0);
  cell2.initialize(0);


  Group group1;
  Group group2;

  group1.add_cell(cell1);
  group1.add_cell(cell2);
  group2.add_cell(cell1);
  group2.add_cell(cell2);

  


  
  //Define the sudoku
  Sudoku foobar;
  foobar.load_sudoku({1,3,4,0, 2,4,1,0, 3,1,2,0, 4,2,3,0});
  foobar.print_sudoku();

  //solve the sudoku
  foobar.solve();
  
  //output solution
  foobar.print_sudoku();
}
