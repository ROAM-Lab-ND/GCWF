#ifndef ZMP_HELPER
#define ZMP_HELPER

#include <iostream>
#include <cstdio>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <iomanip>
#include <stdexcept>
#include <cmath>

#include <chrono>

// #define DEBUG

using namespace std;
using namespace chrono;

#define disp(x) cout<<#x<<"="<<endl<<x<<endl
#define dispN(x,n) cout<<#x<<"="<<endl;for(int i=0;i<n;++i){cout<<x[i]<<endl;}

#define csvout(outport,x,flag) if(flag){outport << #x <<",";}else{outport<<setprecision(12)<<x<<",";}
#define csvoutN(outport,x,n,flag)                                     \
                    if(flag){                                         \
                      for(int i=0;i<n;++i){                           \
                        outport << #x << "{" << i <<"}/" << n << ","; \
                      }                                               \
                    }else{                                            \
                      for(int j=0;j<n;++j){outport<<x[j]<<",";}       \
                    }
                    
#define Intersect_marco(TypeA, TypeB)                                                   \
        inline Point Intersect(TypeA line1, TypeB line2){                               \
          Point p(0,0);                                                                 \
          if (line1.line.A == line2.line.A && line1.line.B == line2.line.B && line1.line.C == line2.line.C) \
            {cout << "Warning! Same Line for intersection!" << endl; p.feasible = false;} \
          double det = (line1.line.A * line2.line.B) - (line1.line.B * line2.line.A);   \
          if (det == 0){                                                                \
            p.feasible = false;                                                         \
          }else{                                                                        \
            p.x = ( (line1.line.B*line2.line.C) - (line2.line.B*line1.line.C) ) / det;  \
            p.y = ( (line2.line.A*line1.line.C) - (line1.line.A*line2.line.C) ) / det;  \
            if ( !(line1.contain(p) && line2.contain(p)) ){                             \
              p.feasible = false;                                                       \
            }                                                                           \
          }                                                                             \
          return p;                                                                     \
        }


#define pi 3.14159265358979323846
#define tol_relax 1e-10
#define mu_fric 1.0

#define force2D Vec2D
#define Point Vec2D

enum Inclusion { INSIDE, VERTEX, EDGE, OUTSIDE };



void SplitString(const string& s, vector<string>& v, const string& c)
{
  std::string::size_type pos1, pos2;
  pos2 = s.find(c);
  pos1 = 0;
  while(std::string::npos != pos2)
  {
    v.push_back(s.substr(pos1, pos2-pos1));
   
    pos1 = pos2 + c.size();
    pos2 = s.find(c, pos1);
  }
  if(pos1 != s.length())
    v.push_back(s.substr(pos1));
}


struct force {
  double fx, fy, fz;
  force(double fxin, double fyin, double fzin) : fx(fxin), fy(fyin), fz(fzin) {}
};

struct Vec2D {
  double x, y;
  bool feasible;
  inline Vec2D(double xin, double yin): x(xin), y(yin), feasible(true) {}
  inline double operator*(Vec2D b){ return ((this->x * b.x) + (this->y * b.y)); }
  inline Vec2D operator-(Vec2D b){ return Vec2D((this->x - b.x), (this->y - b.y)); }
  inline Vec2D operator-(){ return Vec2D(-this->x, -this->y); }
  inline Vec2D operator*(double b){ return Vec2D(this->x*b, this->y*b); }
  inline bool operator==(Vec2D b){ return (this->x==b.x) && (this->y==b.y); }
};

inline Vec2D operator*(double b, Vec2D a){ return Vec2D(a.x*b, a.y*b); }


inline double norm(Vec2D a){
  return sqrt(pow(a.x,2) + pow(a.y,2));
}

inline Vec2D crossVec2D(Vec2D r){
  return Vec2D(-r.y, r.x);
}

struct Range {
  double lb, ub;
  Range(double lbin, double ubin) : lb(lbin), ub(ubin) { };
  inline bool contain(double x) {
    return (lb <= x) && (x <= ub);
  };
  inline bool contain_open(double x) {
    return (lb - tol_relax < x) && (x < ub + tol_relax);
  };
  inline bool contain_right_open(double x) {
    return (lb <= x) && (x < ub + tol_relax);
  };
  inline bool contain_left_open(double x) {
    return (lb - tol_relax < x) && (x <= ub);
  };
};

struct LineBase {
  double A, B, C;
  LineBase(Point Ain, Point Bin){
    A = Ain.y - Bin.y;
    B = Bin.x - Ain.x;
    C =  (Ain.x * Bin.y) - (Bin.x * Ain.y);
    if (A==0 && B==0){
      throw runtime_error("Two points to construct a line cannot be the same point!");
    }
  };
  LineBase(double Ain, double Bin, double Cin): A(Ain), B(Bin), C(Cin) { 
    if (A==0 && B==0){ throw runtime_error("Infeasible line!"); }
  };
  inline bool contain(Point Xin){ return true; };
};

struct Line {
  LineBase line;
  Line(Point Ain, Point Bin) : line(Ain, Bin) { };

  inline bool contain(Point Xin){ return true; };
};

struct LineWithSeg {
  LineBase line;
  Range r_y, r_x;
  LineWithSeg(Point Ain, Point Bin) : line(Ain, Bin), r_y(0,0), r_x(0,0) {
      if (line.A > 0){
        r_y = Range(Bin.y, Ain.y);
      }else if (line.A < 0){
        r_y = Range(Ain.y, Bin.y);
      }else{
        r_y = Range(-INFINITY, INFINITY);
      }
      if (line.B > 0){
        r_x = Range(Ain.x, Bin.x);
      }else if (line.B < 0){
        r_x = Range(Bin.x, Ain.x);
      }else{
        r_x = Range(-INFINITY, INFINITY);
      }
  };

  inline bool contain(Point Xin){
    return r_y.contain(Xin.y) && r_x.contain(Xin.x);
  };
};

struct LineWithSeg_HalfOpen {
  LineBase line;
  Range r_y, r_x;
  bool r_y_right, r_x_right; //open side
  LineWithSeg_HalfOpen(Point Ain, Point Bin) : line(Ain, Bin), r_y(0,0), r_x(0,0) {
    // Point A is the open side.
      if (line.A > 0){
        r_y = Range(Bin.y, Ain.y);
        r_y_right = true;
      }else if (line.A < 0){
        r_y = Range(Ain.y, Bin.y);
        r_y_right = false;
      }else{
        r_y = Range(-INFINITY, INFINITY);
        r_y_right = true;
      }
      if (line.B > 0){
        r_x = Range(Ain.x, Bin.x);
        r_x_right = false;
      }else if (line.B < 0){
        r_x = Range(Bin.x, Ain.x);
        r_x_right = true;
      }else{
        r_x = Range(-INFINITY, INFINITY);
        r_x_right = true;
      }
  };

  inline bool contain(Point Xin){
    return ( r_y_right ? r_y.contain_right_open(Xin.y) : r_y.contain_left_open(Xin.y) )
        && ( r_x_right ? r_x.contain_right_open(Xin.x) : r_x.contain_left_open(Xin.x) );
  };
};

struct LineWithSeg_Open {
  LineBase line;
  Range r_y, r_x;
  LineWithSeg_Open(Point Ain, Point Bin) : line(Ain, Bin), r_y(0,0), r_x(0,0) {
      if (line.A > 0){
        r_y = Range(Bin.y, Ain.y);
      }else if (line.A < 0){
        r_y = Range(Ain.y, Bin.y);
      }else{
        r_y = Range(-INFINITY, INFINITY);
      }
      if (line.B > 0){
        r_x = Range(Ain.x, Bin.x);
      }else if (line.B < 0){
        r_x = Range(Bin.x, Ain.x);
      }else{
        r_x = Range(-INFINITY, INFINITY);
      }
  };

  inline bool contain(Point Xin){
    return r_y.contain_open(Xin.y) && r_x.contain_open(Xin.x);
  };
};

struct HalfLine {
  LineBase line;
  Range r_y, r_x;
  // bool r_y_right, r_x_right;
  HalfLine(Point Ain, Point Bin) : line(Ain, Bin), r_y(0,0), r_x(0,0) {
    // Infinity along B direction
      if (line.A > 0){
        r_y = Range(-INFINITY, Ain.y);
      }else if (line.A < 0){
        r_y = Range(Ain.y, INFINITY);
      }else{
        r_y = Range(-INFINITY, INFINITY);
      }
      if (line.B > 0){
        r_x = Range(Ain.x, INFINITY);
      }else if (line.B < 0){
        r_x = Range(-INFINITY, Ain.x);
      }else{
        r_x = Range(-INFINITY, INFINITY);
      }
  };

  inline bool contain(Point Xin){
    return r_y.contain(Xin.y) && r_x.contain(Xin.x);
  };
};

Intersect_marco(Line, Line);
Intersect_marco(LineWithSeg, LineWithSeg);
Intersect_marco(LineWithSeg_HalfOpen, LineWithSeg_HalfOpen);
Intersect_marco(LineWithSeg_Open, LineWithSeg_Open);
Intersect_marco(HalfLine, HalfLine);

Intersect_marco(Line, LineWithSeg);
Intersect_marco(Line, LineWithSeg_HalfOpen);
Intersect_marco(Line, LineWithSeg_Open);
Intersect_marco(Line, HalfLine);

Intersect_marco(LineWithSeg, LineWithSeg_HalfOpen);
Intersect_marco(LineWithSeg, LineWithSeg_Open);
Intersect_marco(LineWithSeg, HalfLine);

Intersect_marco(LineWithSeg_HalfOpen, LineWithSeg_Open);
Intersect_marco(LineWithSeg_HalfOpen, HalfLine);

Intersect_marco(LineWithSeg_Open, HalfLine);


vector<Point> Intersect_close(vector<Point> P, Line line){
  vector<Point> inter_points;
  for (size_t i_edge = 1; i_edge < P.size(); i_edge++) {
    Point inter_point = Intersect( line, LineWithSeg_HalfOpen(P[i_edge-1], P[i_edge]) );
    if (inter_point.feasible){ inter_points.push_back(inter_point); }
  }
  return inter_points;
}
vector<Point> Intersect_close(vector<Point> P, HalfLine line){
  vector<Point> inter_points;
  for (size_t i_edge = 1; i_edge < P.size(); i_edge++) {
    Point inter_point = Intersect( LineWithSeg_HalfOpen(P[i_edge-1], P[i_edge]), line );
    if (inter_point.feasible){ inter_points.push_back(inter_point); }
  }
  return inter_points;
}
vector<Point> Intersect_close(vector<Point> P, LineWithSeg line, int i_skip){
  vector<Point> inter_points;
  for (size_t i_edge = 1; i_edge < P.size(); i_edge++) {
    if (i_skip != i_edge){
      Point inter_point = Intersect(line, LineWithSeg_HalfOpen(P[i_edge-1], P[i_edge]));
      if (inter_point.feasible){ inter_points.push_back(inter_point); }
    }
  }
  return inter_points;
}

vector<Point> Intersect(vector<Point> P, Line line){
  vector<Point> inter_points;
  for (size_t i_edge = 1; i_edge < P.size(); i_edge++) {
    Point inter_point = Intersect( line, LineWithSeg(P[i_edge-1], P[i_edge]) );
    if (inter_point.feasible){ inter_points.push_back(inter_point); }
  }
  return inter_points;
}
vector<Point> Intersect(vector<Point> P, HalfLine line){
  vector<Point> inter_points;
  for (size_t i_edge = 1; i_edge < P.size(); i_edge++) {
    Point inter_point = Intersect( LineWithSeg(P[i_edge-1], P[i_edge]), line );
    if (inter_point.feasible){ inter_points.push_back(inter_point); }
  }
  return inter_points;
}
vector<Point> Intersect(vector<Point> P, LineWithSeg line, int i_skip){
  vector<Point> inter_points;
  for (size_t i_edge = 1; i_edge < P.size(); i_edge++) {
    if (i_skip != i_edge){
      Point inter_point = Intersect(line, LineWithSeg(P[i_edge-1], P[i_edge]));
      if (inter_point.feasible){ inter_points.push_back(inter_point); }
    }
  }
  return inter_points;
}

inline double ccw(const Point& a, const Point& b, const Point& c) {
	return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

inline bool inside(vector<Point> SP, Point P, Inclusion &state, int &index){
  state = INSIDE;
  for (size_t i = 1; i < SP.size(); i++) {
    double distance = ccw(SP[i-1], SP[i], P);
    if (distance < -1e-6){ state = OUTSIDE; return false; }
    else if (-1e-6 <= distance && distance <= 0.0){
      if (SP[i-1] == P || SP[i] == P) {state = VERTEX;}
      else {index = i; state = EDGE;}
    }
  }
  return true;
}

inline bool inside(vector<Point> SP, Point P){
  for (size_t i = 1; i < SP.size(); i++) {
    if (ccw(SP[i-1], SP[i], P) < 0.0){ return false; }
  }
  return true;
}


inline double arraySum(double const * array, int const len){
  double sum = 0.0;
  for (size_t i = 0; i < len; i++) { sum += array[i]; }
  return sum;
  
}

#ifdef DEBUG
void print(vector<Point> P){
  cout << "Polygon" << endl;
  for (size_t i = 0; i < P.size(); i++) {
    cout << P[i].x << ", " << P[i].y << endl;
  }
  cout << endl;
}
#endif //DEBUG


#endif
