#ifndef LOADSAVE_HPP
#define LOADSAVE_HPP


void loadSamples(string data_path){
  
  ///////// Load File //////////
  ifstream file_vertices(data_path + "vertices.txt", ios::in);
  ifstream file_force(data_path + "force.txt", ios::in);
  
  string linestr_vertices, linestr_force;
  vector<string> line_split;
  vector<string> vector_split;
  int id_vertices, id_force;
  vector<Point> point_list;
  force_table  = new double*[num_samples];
  id_list = new int[num_samples];
  
  for (int i_sample = 0; i_sample < num_samples; ++i_sample){
    point_list.clear();
    line_split.clear();
    getline(file_vertices,linestr_vertices);
    SplitString(linestr_vertices,line_split,";");
    id_vertices = (int)atoi(line_split[0].c_str());
    if (num_vertices >= line_split.size()) { throw runtime_error("Not Enough Vertices!"); }
    for (int i_vertices = 0; i_vertices < num_vertices; ++i_vertices){
      vector_split.clear();
      SplitString(line_split[i_vertices+1],vector_split,",");
      point_list.push_back(Point( atof(vector_split[0].c_str()),atof(vector_split[1].c_str()) ));
      
      #ifdef DEBUG
      cout << point_list.back().x << ", " 
          << point_list.back().y << endl;
      #endif //DEBUG
      
    }
    point_list.push_back(point_list[0]);
    sp_list.push_back(point_list);
    
    #ifdef DEBUG
    cout << point_list.back().x << ", " 
        << point_list.back().y << endl << endl;
    #endif //DEBUG
    
    
    
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
    #endif //DEBUG
    
    
    if( !(id_force == id_vertices) ){
      throw runtime_error("IDs for force data and vertices data are different! Check your data.");
    }
    id_list[i_sample] = id_force;
  }
  
  
  file_vertices.close();
  file_force.close();
}


void saveResult(string file_path){
  cout<<setiosflags(ios::fixed); 
  ofstream csvLog;
  csvLog.open(file_path);
  int id, minval, maxval, feasibility;
  csvout(csvLog,id,true);
  csvout(csvLog,maxval,true);
  csvout(csvLog,minval,true);
  csvout(csvLog,computeTime,true);
  csvout(csvLog,feasibility,true);
  csvLog << "\n";
  for (size_t i_sample = 0; i_sample < num_samples; i_sample++) {
    csvout(csvLog,id_list[i_sample],false);
    csvout(csvLog,result[i_sample].nmax,false);
    csvout(csvLog,result[i_sample].nmin,false);
    csvout(csvLog,computeTime[i_sample],false);
    csvout(csvLog,result[i_sample].isOK,false);
    csvLog << "\n";
  }
  csvLog.close();

}

#endif //LOADSAVE_HPP
