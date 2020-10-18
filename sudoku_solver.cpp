#include <iostream>
#include "functions.hpp"

int main() {
  //Define the sudoku
  Sudoku foobar;
  foobar.load_sudoku({1,2,3,0,2,3,4,0,3,4,1,0,4,1,2,0});
  foobar.print_sudoku();

  //solve the sudoku
  foobar.solve();
  
  //output solution
  foobar.print_sudoku();
}
