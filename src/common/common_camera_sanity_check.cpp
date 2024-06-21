#include "common_camera_sanity_check.hpp"
#include "common_vector.hpp"

Vector sanity_check_coordinates(Vector coord) {
  
  if ( coord.X_ < 250 ){
    coord.X_ = 250;
  }
  if ( coord.X_ > 600 ){
    coord.X_ = 600;
  }
    if ( coord.Y_ < -400 ){
    coord.Y_ = -400;
  }
    if ( coord.Y_ > 400 ){
    coord.Y_ = 400;
  }
      if ( coord.Z_ < 150 ){
    coord.Z_ = 150;
  }
      if ( coord.Z_ > 500 ){
    coord.Z_ = 500;
  } 
  return coord;
}
  