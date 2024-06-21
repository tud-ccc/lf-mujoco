#include "common.hpp"
#include "common_vector.hpp"

Vector sanity_check_coordinates(Vector coord) {
  
  if ( coord.X_ < 100 ){
    coord.X_ = 100;
  }
  if ( coord.X_ > 500 ){
    coord.X_ = 500;
  }
    if ( coord.Y_ < -300 ){
    coord.Y_ = -300;
  }
    if ( coord.Y_ > 300 ){
    coord.Y_ = 300;
  }
      if ( coord.Z_ < 200 ){
    coord.Z_ = 200;
  }
      if ( coord.Z_ > 300 ){
    coord.Z_ = 300;
  } 
  return coord;
}
