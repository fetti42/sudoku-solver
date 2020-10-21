#include <iostream>
#include "functions.hpp"

int main() {
  
  //Define the sudoku
  Sudoku foobar;
  //testing an easy 9x9 sudoku
  foobar.load_sudoku({8,0,0, 0,1,0, 0,7,0,
		      0,0,4, 0,5,0, 0,0,2,
		      0,0,9, 0,0,4, 0,3,0,
		      0,5,0, 2,0,0, 0,0,0,
		      0,0,0, 0,0,0, 6,8,0,
		      9,0,0, 0,6,3, 0,0,0,
		      0,0,0, 0,0,0, 3,4,0,
		      7,0,0, 5,2,0, 0,0,1,
		      0,8,0, 0,0,0, 0,0,0});
	
	
  /*foobar.load_sudoku({1,0,4,0,
		      2,4,1,0,
		      3,1,0,0,
		      0,2,3,0});*/
  foobar.print_sudoku();

  //solve the sudoku
  foobar.solve();
  
  //output solution
  foobar.print_sudoku();
}
