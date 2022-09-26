
#include "helper.hpp"
#include "YawMoment_Geo.hpp"

vector<vector<Point>> sp_list;
double** force_table;
int* id_list;
int num_samples;
int num_vertices;
int num_grid;
double d_alpha;

double* computeTime;
Point ZMP(0,0);

vector<YawMomentResult> result;


#include "loadsave.hpp"



int main(int argc, char const *argv[]) {
  string data_path(argv[1]);
  if ('/' != data_path.back())   data_path.append("/");
  
  num_vertices = atoi(argv[2]); 
  num_samples = atoi(argv[3]);
  num_grid = atoi(argv[4]);
  d_alpha = pi / (double)num_grid;
  computeTime = new double[num_samples];
  loadSamples(data_path);
  
  Inclusion inclusion_status = INSIDE;
  int inclusion_index = 0;
  
  result.reserve(num_samples);
  
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
        double* nmin_list = new double[num_grid];
        double* nmax_list = new double[num_grid];
        
        Point point_max_a(0,0), point_max_b(0,0), point_test(0,0);
        // Find all vertices
        for (size_t i_grid = 0; i_grid < num_grid; i_grid++) {      
          point_test.x = cos(d_alpha*i_grid);
          point_test.y = sin(d_alpha*i_grid);
          vector<Point> max_line_points = Intersect_close(sp, Line(ZMP, point_test));
          if (max_line_points.size() == 2 || max_line_points.size() == 3){
            point_max_a = max_line_points[0];
            point_max_b = max_line_points[1];
          } else {
            // cout << "Warning! Not expected num of intersection points!" << endl;
            cout << "max_line_points.size : " << max_line_points.size() << endl;
            for (size_t i = 0; i < max_line_points.size(); i++) {
              cout << max_line_points[i].x << ", " << max_line_points[i].y << endl;
            }
            #ifdef DEBUG
            print(sp);
            #endif // DEBUG
            cout << "Finding line: O to " << endl;
            cout << point_test.x << ", " << point_test.y << endl << endl;
            
            throw runtime_error("Not expected num of intersection points!");
          }
          YawMomentResult result_temp = YawMoment_Geo(point_max_a, point_max_b, 
                  force2D(force_table[i_sample][0], force_table[i_sample][1]),
                  force_table[i_sample][2], 1.0);
          nmin_list[i_grid] = result_temp.nmin;
          nmax_list[i_grid] = result_temp.nmax;
        }
        
        // Find min & max
        double max_yaw(0.0), min_yaw(0.0);
        for (size_t i_grid = 0; i_grid < num_grid; i_grid++) {
          if (max_yaw < nmax_list[i_grid]){ max_yaw = nmax_list[i_grid]; }
          if (min_yaw > nmin_list[i_grid]){ min_yaw = nmin_list[i_grid]; }
        }
        
        result.push_back( YawMomentResult(true, min_yaw, max_yaw) );
      
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
  
  saveResult(data_path + "Tau_Grid_" + to_string(num_grid) + ".txt");
  
  
  return 0;
}
