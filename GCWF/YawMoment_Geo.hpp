
#ifndef ZMP_YAWMOMENT_GET
#define ZMP_YAWMOMENT_GET

#include "helper.hpp"


#ifdef DEBUG
int YawSolve_count = 0;
#endif //DEBUG


inline double mod (double x, double y)
{
  double result = fmod(x, y);
  return result >= 0 ? result : result + y;
}

struct YawMomentResult {
  bool isOK;
  double nmin, nmax;
  YawMomentResult(bool isOKin, double nminin, double nmaxin) : 
          isOK(isOKin), nmin(nminin), nmax(nmaxin) {}
};

double moment(Point p_a, Point p_b, force2D f_h, force2D f_a);
double moment(Point p_a, Point p_b, force2D f_h, force2D f_a, force2D f_b);





bool radIn(const double theta_min_in, const double theta_max_in, 
           const bool Bmin, const bool Bmax, const double theta_in);


double moment(Point p_a, Point p_b, force2D f_h, force2D f_a){
  return crossVec2D(p_a)*f_a + crossVec2D(p_b)*(f_h-f_a);
}
double moment(Point p_a, Point p_b, force2D f_h, force2D f_a, force2D f_b){
  return crossVec2D(p_a)*(f_h-f_b) + crossVec2D(p_b)*f_b;
}


bool radIn(const double theta_min_in, const double theta_max_in, 
           const bool Bmin, const bool Bmax, const double theta_in){
  bool isInside;
  double theta_min = mod(theta_min_in,2*pi);
  double theta_max = mod(theta_max_in,2*pi);
  double theta = mod(theta_in,2*pi);
  if (theta_min > theta_max){
    isInside  = ( Bmin && (theta == theta_min) ) ||
                ( (theta > theta_min) && (theta <= 2*pi) ) ||
                ( (theta >= 0) && (theta < theta_max) ) ||
                ( Bmax && (theta == theta_max) );
  }else{
    isInside  = ( Bmin && (theta == theta_min) ) ||
                ( (theta > theta_min) && (theta < theta_max) ) ||
                ( Bmax && (theta == theta_max) );
  }
  return isInside;
}



YawMomentResult YawMoment_Geo(Point p_a, Point p_b, force2D f_h, double f_z, double mu){

  /* This part is comment out for faster computation
     It is the programmer's responsbility to ensure the input positions are co-linear.
  // Check the feasibility of supporting line
  if ( p_a*p_b != norm(p_a)*norm(p_b) ) { // p_a and p_b are not co-linear
    result.isOK = false;
    result.nmin = NAN;
    result.nmax = NAN;
    return result;
  }
  */
  
  #ifdef DEBUG
  ++YawSolve_count;
  #endif //DEBUG
  YawMomentResult result(true, 0.0, 0.0);
  Vec2D f_a(0,0);
  Vec2D f_b(0,0);
  
  /* Old version for quadratic constaint parameter with vector norm */
  // // Spacial cases for p_a or p_b is origin
  // if ( p_a == Vec2D(0,0) ){
  //   return result;
  // }else if ( p_b == Vec2D(0,0) ){
  //   return result;
  // }
  // 
  // // Compute the quadratic constaint parameter
  // double p_a_norm = norm(p_a);
  // double p_b_norm = norm(p_b);
  // double r_a = (p_b_norm/(p_a_norm + p_b_norm)) * mu* f_z;
  // double r_b = (p_a_norm/(p_a_norm + p_b_norm)) * mu* f_z;
  /* END                                                            */
  
  /* New version for quadratic constaint parameter without vector norm 
     It has same times of equality operations but 2 less norm operations
      We believe it improves the speed of approach 2 (checking all vertices) 
      by 50 ns for 10 vertices problem */
  // Compute the quadratic constaint parameter
  double r_a, r_b;
  /* The last condition (p_b.x * p_a.x < 0, p_b.y * p_a.y < 0) 
       is always true for reasonable input (namely, p_a and p_b pointing to oppsite direction)
     The last condition should be added for better stability of the algorithm. */
  if ( p_a.x != 0.0 && p_b.x != 0.0 ){ // && p_b.x * p_a.x < 0 
    r_a = (p_b.x/(p_b.x - p_a.x)) * mu* f_z;
    r_b = (p_a.x/(p_a.x - p_b.x)) * mu* f_z;
  }else if ( p_a.y != 0.0 && p_b.y != 0.0 ){ // && p_b.y * p_a.y < 0 
    r_a = (p_b.y/(p_b.y - p_a.y)) * mu* f_z;
    r_b = (p_a.y/(p_a.y - p_b.y)) * mu* f_z;
  }else{
    // Spacial case for p_a or p_b is origin
    if (f_h_norm > mu*f_z){ // Outside of friction cone
        result.isOK = false;
        result.nmin = NAN;
        result.nmax = NAN;
        return result;
    }else{ // Inside of friction cone
      return result;
    }
    
  }
  
  // Compute some frequently used vars
  double f_h_norm = norm(f_h);
  Vec2D p_a_T_hat = crossVec2D(p_a) * (1 / norm(p_a)); // \hat [\p_a]^\top
  
  // Spacial cases about f_h
  if (f_h_norm > mu*f_z){ // Outside of the friction cone
      result.isOK = false;
      result.nmin = NAN;
      result.nmax = NAN;
      return result;
  }else if (f_h_norm == mu*f_z){ // On the friction cone (no yaw moment can be produced)
      return result;
  }else if (f_h_norm + r_a <= r_b){ // Intersection of two constraint circles is C_a
      f_a =   p_a_T_hat * r_a;
      result.nmax = moment(p_a, p_b, f_h, f_a);
      f_a = p_a_T_hat * -r_a;
      result.nmin = moment(p_a, p_b, f_h, f_a);
      return result;
  }else if (f_h_norm + r_b <= r_a){ // Intersection of two constraint circles is C_b
      f_b = p_a_T_hat * -r_b;
      result.nmax = moment(p_a, p_b, f_h, f_a, f_b);
      f_b =   p_a_T_hat * r_b;
      result.nmin = moment(p_a, p_b, f_h, f_a, f_b);
      return result;
  }
  
  // ELSE: Intersection of two constraint circles is two points D and E
  
  double alpha = atan2(crossVec2D(p_a)*f_h, p_a*f_h); // Angle between tangential force and supporting line
  double beta   = atan2(p_a.y, p_a.x); // Angle between supporting line and x-axis
  // \phi in paper is equal to (alpha + beta) here
  
  double alpha_a = acos( (pow(f_h_norm,2) + pow(r_a,2) - pow(r_b,2)) / (2*f_h_norm*r_a) );
  double alpha_b = acos( (pow(f_h_norm,2) + pow(r_b,2) - pow(r_a,2)) / (2*f_h_norm*r_b) );
  Vec2D  p_e = Vec2D(cos(alpha + beta + alpha_a), sin(alpha + beta + alpha_a)) * r_a;
  Vec2D  p_d = Vec2D(cos(alpha + beta - alpha_a), sin(alpha + beta - alpha_a)) * r_a;
  
  // Yaw moment upper bound  (maximization)
  if ( radIn(3*pi/2+alpha_b, pi/2-alpha_a, false, true, alpha) ){ // case (14a)
      f_a = p_e;
      result.nmax = moment(p_a, p_b, f_h, f_a);
  }else if ( radIn(pi/2-alpha_a, pi/2+alpha_a, false, true, alpha) ){ // case (14b)
      f_a = p_a_T_hat * r_a;
      result.nmax = moment(p_a, p_b, f_h, f_a);
  }else if ( radIn(pi/2+alpha_a, 3*pi/2-alpha_b, false, true, alpha) ){ // case (14c)
      f_a = p_d;
      result.nmax = moment(p_a, p_b, f_h, f_a);
  }else if ( radIn(3*pi/2-alpha_b, 3*pi/2+alpha_b, false, true, alpha) ){ // case (14d)
      f_b = p_a_T_hat * -r_b;
      result.nmax = moment(p_a, p_b, f_h, f_a, f_b);
  }
  
  // Yaw moment lower bound  (minimization)
  if ( radIn(3*pi/2+alpha_a, pi/2-alpha_b, false, true, alpha) ){
      f_a = p_d;
      result.nmin = moment(p_a, p_b, f_h, f_a);
  }else if ( radIn(pi/2-alpha_b, pi/2+alpha_b, false, true, alpha) ){
      f_b = p_a_T_hat * r_b;
      result.nmin = moment(p_a, p_b, f_h, f_a, f_b);
  }else if ( radIn(pi/2+alpha_b, 3*pi/2-alpha_a, false, true, alpha) ){
      f_a = p_e;
      result.nmin = moment(p_a, p_b, f_h, f_a);
  }else if ( radIn(3*pi/2-alpha_a, 3*pi/2+alpha_a, false, true, alpha) ){
      f_a = p_a_T_hat * -r_a;
      result.nmin = moment(p_a, p_b, f_h, f_a);
  }
  
  return result;
}




#endif
