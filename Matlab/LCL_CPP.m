function [] = LCL_CPP(data_path,N_vertices,N_samples)
%LCL_CPP Call Longest Centered Line C++ solver.
%   LCL_CPP(data_path,N_vertices,N_samples) call the c++ 
%   executable file in ../build/ directory to solve the data stored in
%   data_path directory with longest centered line. 
%   N_vertices gives the number of vertices of testing samples.
%   N_samples gives the nubmer of testing samples.
arguments
    data_path
    N_vertices (1,1) {mustBeInteger}
    N_samples (1,1) {mustBeInteger}
end
command_LCL  = ["../build/LCL",data_path,num2str(N_vertices),num2str(N_samples)];
status_LCL   = system(join(command_LCL,' '));
assert(status_LCL==0, "LCL Failed");
end