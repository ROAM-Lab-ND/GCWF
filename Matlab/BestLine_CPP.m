function [] = BestLine_CPP(data_path,N_vertices,N_samples,N_grid)
%BestLine_CPP Call Optimal Supporting Line C++ solver.
%   BestLine_CPP(data_path,N_vertices,N_samples,N_grid) call the c++ 
%   executable file in ../build/ directory to solve the data stored in
%   data_path directory with grid searching the best supporting line.
%   N_vertices gives the number of vertices of testing samples.
%   N_samples gives the nubmer of testing samples.
%   N_grid gives the number of searching grid.
arguments
    data_path
    N_vertices (1,1) {mustBeInteger}
    N_samples (1,1) {mustBeInteger}
    N_grid (1,1) {mustBeInteger}
end
command_GD  = ["../build/Grid",data_path,num2str(N_vertices),num2str(N_samples),num2str(N_grid)];
status_GD   = system(join(command_GD,' '));
assert(status_GD==0, "GD Failed");
end
