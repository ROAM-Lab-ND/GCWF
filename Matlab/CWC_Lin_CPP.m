function [] = CWC_Lin_CPP(data_path,N_vertices,N_samples,N_edges)
%CWC_Lin_CPP Call Linear-CWC C++ solver.
%   CWC_Lin_CPP(data_path,N_vertices,N_samples,N_edges) call the c++ 
%   executable file in ../build/ directory to solve the data stored in
%   data_path directory with linear approximated cones. 
%   N_vertices gives the number of vertices of testing samples.
%   N_samples gives the nubmer of testing samples.
%   N_edges gives the number of linear approximation constraints.
arguments
    data_path
    N_vertices (1,1) {mustBeInteger}
    N_samples (1,1) {mustBeInteger}
    N_edges (1,1) {mustBeInteger}
end
command_CWCLin = ["../build/CWC_Lin",data_path,...
    num2str(N_vertices),num2str(N_samples),num2str(N_edges)];
status_CWCLin_max = system(join(command_CWCLin,' ') + ' max');
assert(status_CWCLin_max==0, "CWCLin Failed");
status_CWCLin_min = system(join(command_CWCLin,' ') + ' min');
assert(status_CWCLin_min==0, "CWCLin Failed");
end