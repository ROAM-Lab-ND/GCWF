

#include <iostream>
#include <cstdio>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <iomanip>
#include <cmath>
#include "fusion.h"

#include <chrono>

using namespace std;
using namespace mosek::fusion;
using namespace monty;
using namespace chrono;

// #define DEBUG

#define pi 3.14159265358979323846

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

int num_vertices;

double*** vertices_table;
double** force_table;
int* id_list;
int num_samples;
int num_edges;

double* OptVal;
bool* Feasibility;
double* computeTime;
bool isMaximize;


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

void loadSamples(string data_path){
  #ifdef DEBUG
  cout << "Loading samples" << endl;
  #endif // DEBUG
  
  ///////// Load File //////////
  ifstream file_vertices(data_path + "vertices.txt", ios::in);
  ifstream file_force(data_path + "force.txt", ios::in);
  
  string linestr_vertices, linestr_force;
  vector<string> line_split;
  vector<string> vector_split;
  int id_vertices, id_force;
  vertices_table  = new double**[num_samples];
  force_table  = new double*[num_samples];
  id_list = new int[num_samples];
  
  for (int i_sample = 0; i_sample < num_samples; ++i_sample){
    vertices_table[i_sample] = new double*[num_vertices];
    line_split.clear();
    getline(file_vertices,linestr_vertices);
    SplitString(linestr_vertices,line_split,";");
    id_vertices = (int)atoi(line_split[0].c_str());
    if (num_vertices >= line_split.size()) { throw runtime_error("Not Enough Vertices!"); }
    for (int i_vertices = 0; i_vertices < num_vertices; ++i_vertices){
      vector_split.clear();
      SplitString(line_split[i_vertices+1],vector_split,",");
      vertices_table[i_sample][i_vertices] = new double[2];
      vertices_table[i_sample][i_vertices][0] = atof(vector_split[0].c_str());
      vertices_table[i_sample][i_vertices][1] = atof(vector_split[1].c_str());
      #ifdef DEBUG
      cout << vertices_table[i_sample][i_vertices][0] << ", " 
          << vertices_table[i_sample][i_vertices][1] << endl;
      #endif // DEBUG
    }
    #ifdef DEBUG
    cout << endl;
    #endif // DEBUG
    
    force_table[i_sample] = new double[3];
    line_split.clear();
    getline(file_force,linestr_force);
    SplitString(linestr_force,line_split,";");
    id_force = (int)atoi(line_split[0].c_str());
    vector_split.clear();
    SplitString(line_split[1],vector_split,",");
    force_table[i_sample][0] = atof(vector_split[0].c_str());
    force_table[i_sample][1] = atof(vector_split[1].c_str());
    force_table[i_sample][2] = atof(vector_split[2].c_str());
    #ifdef DEBUG
    cout << "Force: " << force_table[i_sample][0] << ", " 
              << force_table[i_sample][1]  << ", " 
              << force_table[i_sample][2] << endl;
    
    
    cout << endl << endl;
    #endif // DEBUG
    
    if( !(id_force == id_vertices) ){
      throw runtime_error("IDs for force data and vertices data are different! Check your data.");
    }
    id_list[i_sample] = id_force;
  }
  
  
  file_vertices.close();
  file_force.close();
}

double arraySum(double const * array, int const len){
  double sum = 0.0;
  for (size_t i = 0; i < len; i++) { sum += array[i]; }
  return sum;
  
}

void saveResult(string data_path){
  cout<<setiosflags(ios::fixed); 
  ofstream csvLog;
  if (isMaximize){
    csvLog.open(data_path + "max_tau_lin_" + to_string(num_edges) + ".txt");
  }else{
    csvLog.open(data_path + "min_tau_lin_" + to_string(num_edges) + ".txt");
  }
  
  int id;
  csvout(csvLog,id,true);
  csvout(csvLog,OptVal,true);
  csvout(csvLog,computeTime,true);
  csvout(csvLog,Feasibility,true);
  csvLog << "\n";
  for (size_t i_sample = 0; i_sample < num_samples; i_sample++) {
    csvout(csvLog,id_list[i_sample],false);
    csvout(csvLog,OptVal[i_sample],false);
    csvout(csvLog,computeTime[i_sample],false);
    csvout(csvLog,Feasibility[i_sample],false);
    csvLog << "\n";
  }
  csvLog.close();
  
}


int main(int argc, char ** argv)
{  
  string data_path(argv[1]);
  if ('/' != data_path.back())   data_path.append("/");
  
  num_vertices = atoi(argv[2]);
  num_samples = atoi(argv[3]);
  num_edges = atoi(argv[4]);
  string objective_str = argv[5];
  
  #ifdef DEBUG
  disp(data_path);
  disp(num_vertices);
  disp(num_samples);
  disp(num_edges);
  #endif //DEBUG
  
  double mu = 1;
  double muc = mu * cos(pi/num_edges);
  double alpha;
  vector<double> Mat;
  for (size_t i_edge = 0; i_edge < num_edges; i_edge++) {
    alpha = i_edge * 2 * pi / num_edges;
    Mat.push_back(-muc);
    Mat.push_back(cos(alpha));
    Mat.push_back(sin(alpha));
  }
  auto LCMat =  Matrix::dense(num_edges,3,new_array_ptr(Mat));
  
  if ( objective_str.compare("max") == 0 ){
    cout << "Maxing" << endl;
    isMaximize = true;
  }else if ( objective_str.compare("min") == 0 ){
    cout << "Mining" << endl;
    isMaximize = false;
  }else{
    cout << "Wrong input No. 4" << endl;
    return 1;
  }
  
  OptVal = new double[num_samples];
  Feasibility = new bool[num_samples];
  computeTime = new double[num_samples];
  loadSamples(data_path);
  
  //////////////// Construct Model ////////////////////////
  Model::t M = new Model("cwc_lin"); auto _M = finally([&]() { M->dispose(); });
  
  // x = [x1 x2 x3] = [fz fx fy]
  
  vector<Variable::t> x_list;
  
  string state_name, constraint_name;
  
  for (size_t i_state = 0; i_state < num_vertices; i_state++) {
    state_name = "x" + to_string(i_state);
    auto x = M->variable(state_name, 3, Domain::unbounded());
    constraint_name = "linearcone" + to_string(i_state);
    M->constraint(constraint_name, Expr::mul(LCMat, x), Domain::lessThan(0.0));
    x_list.push_back( x );
  }
  
  auto x_stack = Var::vstack( new_array_ptr(x_list) );
  
  // x_zmp = [x_zmp_1 x_zmp_2 x_zmp_3] = [f_zmp_z f_zmp_x f_zmp_y]
  Parameter::t x_zmp = M->parameter("x_zmp", 3);
  
  // Create the constraint
  M->constraint("linearforce", 
    Expr::sub(Expr::add( new_array_ptr(x_list) ), x_zmp), 
    Domain::equalsTo(0.0));
  
  Parameter::t SP_x = M->parameter("sp_x", new_array_ptr<int,1>({2,3*num_vertices}));
  M->constraint("moment", Expr::mul(SP_x, x_stack), Domain::equalsTo(0.0));
  
  Parameter::t SP_x_3 = M->parameter("sp_x_3", new_array_ptr<int,1>({1,3*num_vertices}));
  
  // Set the objective function 
  if (isMaximize){
    M->objective("obj", ObjectiveSense::Maximize, Expr::mul(SP_x_3, x_stack));
  }else{
    M->objective("obj", ObjectiveSense::Minimize, Expr::mul(SP_x_3, x_stack));
  }
  
  
  vector<vector<double>> SP_x_row_12;
  vector<double> SP_x_row_1;
  vector<double> SP_x_row_2;
  vector<double> SP_x_row_3;
  double p_x, p_y, p_z;
  p_z = 0.0;
  
  
  auto start = high_resolution_clock::now();
  auto end   = high_resolution_clock::now();
  auto duration = duration_cast<nanoseconds>(end - start);
  
  
  auto start_total = high_resolution_clock::now();
  
  for (size_t i_sample = 0; i_sample < num_samples; i_sample++) {
    #ifdef DEBUG
    cout << "Sample num: " << i_sample + 1 << endl;
    #endif //DEBUG
    
    start = high_resolution_clock::now();
    
    // cout << endl << endl << "Sample " << i_sample << endl;
    
    // Update the parameters
    SP_x_row_12.clear();
    SP_x_row_1.clear();
    SP_x_row_2.clear();
    SP_x_row_3.clear();
    for (size_t i_vertices = 0; i_vertices < num_vertices; i_vertices++) {
      p_x = vertices_table[i_sample][i_vertices][0];
      p_y = vertices_table[i_sample][i_vertices][1];
      
      SP_x_row_1.push_back(p_y);
      SP_x_row_1.push_back(0.0);
      SP_x_row_1.push_back(-p_z);
      
      SP_x_row_2.push_back(-p_x);
      SP_x_row_2.push_back(p_z);
      SP_x_row_2.push_back(0.0);
      
      SP_x_row_3.push_back(0.0);
      SP_x_row_3.push_back(-p_y);
      SP_x_row_3.push_back(p_x);
    }
    SP_x_row_12.push_back(SP_x_row_1);
    SP_x_row_12.push_back(SP_x_row_2);
    
    // dispN(SP_x_row_1,3*num_vertices);
    // dispN(SP_x_row_2,3*num_vertices);
    // dispN(SP_x_row_3,3*num_vertices);
    
    SP_x->setValue(new_array_ptr<double>(SP_x_row_12) );
    SP_x_3->setValue(new_array_ptr<double>(SP_x_row_3) );
    x_zmp->setValue(new_array_ptr<double,1>({ force_table[i_sample][2], force_table[i_sample][0], force_table[i_sample][1] }));
    
    
    // Solve the problem
    M->solve();
    
    
    // Get the linear solution values
    if (M->getPrimalSolutionStatus() == SolutionStatus::Optimal || M->getPrimalSolutionStatus() == SolutionStatus::Feasible){
      OptVal[i_sample] = M->primalObjValue();
      Feasibility[i_sample] = true;
    }else{
      OptVal[i_sample] = 0.0;
      Feasibility[i_sample] = false;
    }
    
    
    
    end   = high_resolution_clock::now();
    
    duration = duration_cast<nanoseconds>(end - start); // in microseconds
    
    computeTime[i_sample] = double(duration.count()) / 1000;
    
         
  }
  
  auto end_total   = high_resolution_clock::now();
  auto duration_total = duration_cast<microseconds>(end_total - start_total);
  double duration_total_time = double(duration_total.count());
  
  #ifdef DEBUG
  dispN(OptVal,num_samples);
  dispN(computeTime,num_samples);
  #endif // DEBUG
  
  double sum_of_time = arraySum(computeTime,num_samples);
  disp(sum_of_time);
  disp(duration_total_time);
  
  double average_time = (sum_of_time - computeTime[0]) / (num_samples-1);
  disp(average_time);
  
  saveResult(data_path);
  
  
  // M->writeTask("dump.opf");
  
  
  
}
