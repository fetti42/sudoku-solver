#include <iostream>
#include "functions.hpp"

int main() {
  //Cell cell1;
  //Cell cell2;
  /*
  std::shared_ptr<Cell> cell_ptr1(new Cell());
  std::shared_ptr<Cell> cell_ptr2(new Cell());
  
  cell_ptr1->initialize(0);
  cell_ptr2->initialize(0);


  Group group1;
  Group group2;

  group1.add_cell(cell_ptr1);
  group1.add_cell(cell_ptr2);
  group2.add_cell(cell_ptr1);
  group2.add_cell(cell_ptr2);

  group1.set_cell_value(0,1);

  std::cout << group1.get_cell_value(0) << "\n";
  
  std::cout << group2.get_cell_value(0) << "\n";

  group1.set_test_value(5);

  std::cout << group1.get_test_value() << "\n";

  
  */

  
  //Define the sudoku
  Sudoku foobar;
  foobar.load_sudoku({1,3,4,0, 2,4,1,0, 3,1,2,0, 4,2,3,0});
  foobar.print_sudoku();

  //solve the sudoku
  foobar.solve();
  
  //output solution
  foobar.print_sudoku();
}
