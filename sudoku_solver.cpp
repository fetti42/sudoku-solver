#include <iostream>
#include "functions.hpp"

int main() {
  
  //Define the sudoku
  Sudoku foobar;
  //testing an easy 9x9 sudoku
  foobar.load_sudoku({4,0,5, 0,0,0, 0,0,0,
		      0,0,3, 2,0,9, 1,0,0,
		      0,2,0, 0,5,1, 0,7,4,
		      6,7,0, 8,9,0, 5,3,0,
		      8,3,1, 0,2,0, 0,0,9,
		      0,0,0, 0,0,0, 8,0,0,
		      1,5,0, 3,0,2, 4,0,6,
		      0,9,0, 4,0,5, 0,0,0,
		      2,8,0, 9,1,0, 7,5,0});
	
	
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
