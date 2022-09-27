
#include "helper.hpp"
#include "YawMoment_Geo.hpp"

vector<vector<Point>> sp_list;
double** force_table;
int* id_list;
int num_samples;
int num_vertices;

double* computeTime;
Point ZMP(0,0);

vector<YawMomentResult> result;

#include "loadsave.hpp"

int main(int argc, char const *argv[]) {
  string data_path(argv[1]);
  if ('/' != data_path.back())   data_path.append("/");
  
  num_vertices = atoi(argv[2]); 
  num_samples = atoi(argv[3]);
  computeTime = new double[num_samples];
  loadSamples(data_path);
  
  result.reserve(num_samples);
  bool isInside_prev;
  bool isInside;
  vector<Point> inner_vertices;
  vector<Point> inter_vertices;
  
  Inclusion inclusion_status = INSIDE;
  int inclusion_index = 0;
  
  auto start = high_resolution_clock::now();
  auto end   = high_resolution_clock::now();
  auto duration = duration_cast<nanoseconds>(end - start);
  
  
  auto start_total = high_resolution_clock::now();
  
  for (size_t i_sample = 0; i_sample < num_samples; i_sample++) {
    
    start = high_resolution_clock::now();
    vector<Point> sp = sp_list[i_sample];
    
    inside(sp, ZMP, inclusion_status, inclusion_index);
    #ifdef DEBUG
    disp(inclusion_status);
    #endif //DEBUG
    switch (inclusion_status) {
      case OUTSIDE:
        result.push_back( YawMomentResult(false, 0.0, 0.0) );
        break;
      case VERTEX:
        result.push_back( YawMomentResult(true, 0.0, 0.0) );
        break;
      case EDGE:
        result.push_back( YawMoment_Geo(sp[inclusion_index-1], sp[inclusion_index],
                force2D(force_table[i_sample][0], force_table[i_sample][1]),
                force_table[i_sample][2], mu_fric) );
        break;
      case INSIDE:
        inner_vertices.clear();
        inter_vertices.clear();
        
        
        // Find all vertices
        isInside_prev = inside(sp, -sp[0]);
        for (size_t i_vertex = 1; i_vertex < sp.size(); i_vertex++) {
          isInside = inside(sp, -sp[i_vertex]);
          if (isInside){ inner_vertices.push_back(sp[i_vertex]); }
          if (!(isInside && isInside_prev)) {
            vector<Point> inter_vertices_temp = 
              Intersect_close(sp, LineWithSeg(-sp[i_vertex-1], -sp[i_vertex]), i_vertex);
            inter_vertices.insert(inter_vertices.end(),inter_vertices_temp.begin(),inter_vertices_temp.end());
          }
          isInside_prev = isInside;
        }
        
        if( !((inner_vertices.size() + inter_vertices.size()) >= 2) ){
          throw runtime_error("Not enough vertices found! Bad Bug!");
        }
        
        // Find longest
        double max_inner(0.0), max_inter(0.0);
        Point point_max_inner(0,0), point_max_inter(0,0);
        Point point_max_a(0,0), point_max_b(0,0);
        double temp;
        for (size_t i_inner = 0; i_inner < inner_vertices.size(); i_inner++) {
          temp = norm(inner_vertices[i_inner]);
          if (max_inner < temp){ max_inner = temp; point_max_inner = inner_vertices[i_inner]; }
        }
        for (size_t i_inter = 0; i_inter < inter_vertices.size(); i_inter++) {
          temp = norm(inter_vertices[i_inter]);
          if (max_inter < temp){ max_inter = temp; point_max_inter = inter_vertices[i_inter]; }
        }
        
        // Extend
        if (max_inner > max_inter){
          vector<Point> max_line_points = Intersect_close(sp, HalfLine(ZMP, -point_max_inner));
          if (max_line_points.size() == 1 || max_line_points.size() == 2){
            point_max_a = max_line_points[0];
            point_max_b = point_max_inner;
          } else {
            cout << "max_line_points.size : " << max_line_points.size() << endl;
            for (size_t i = 0; i < max_line_points.size(); i++) {
              cout << max_line_points[i].x << ", " << max_line_points[i].y << endl;
            }
            #ifdef DEBUG
            print(sp);
            #endif // DEBUG
            cout << "Finding line: O to " << endl;
            cout << -point_max_inner.x << ", " << -point_max_inner.y << endl << endl;
            
            throw runtime_error("Not expected num of intersection points!");
          }
          
        }else{
          point_max_a = point_max_inter;
          point_max_b = -point_max_inter;
        }
        
        // Compute nmin, nmax
        result.push_back(YawMoment_Geo(point_max_a, point_max_b, 
                force2D(force_table[i_sample][0], force_table[i_sample][1]),
                force_table[i_sample][2], 1.0) );
      
        break;
    
    }
    
    end   = high_resolution_clock::now();
    
    duration = duration_cast<nanoseconds>(end - start); // in microseconds
    
    computeTime[i_sample] = double(duration.count()) / 1000;
  }
  
  auto end_total   = high_resolution_clock::now();
  auto duration_total = duration_cast<microseconds>(end_total - start_total);
  double duration_total_time = double(duration_total.count());
  
  #ifdef DEBUG
  dispN(computeTime,num_samples);
  #endif //DEBUG
  
  double sum_of_time = arraySum(computeTime,num_samples);
  disp(sum_of_time);
  disp(duration_total_time);
  
  double average_time = (sum_of_time - computeTime[0]) / (num_samples-1);
  disp(average_time);
  
  #ifdef DEBUG
  disp(YawSolve_count);
  #endif //DEBUG
  
  saveResult(data_path + "Tau_LCL.txt");
  
  
  
  return 0;
}
