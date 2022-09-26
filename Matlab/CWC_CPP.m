function [] = CWC_CPP(data_path,N_vertices,N_samples)
%CWC_CPP Call Exact-CWC C++ code.
%   CWC_CPP(data_path,N_vertices,N_samples) call the c++ 
%   executable file in ../build/ directory to exact solve the data stored 
%   in data_path directory with second-order-cones. 
%   N_vertices gives the number of vertices of testing samples.
%   N_samples gives the nubmer of testing samples.
%   N_edges gives the number of linear approximation constraints.
arguments
    data_path
    N_vertices (1,1) {mustBeInteger}
    N_samples (1,1) {mustBeInteger}
end
command_CWC = ["../build/CWC",data_path,...
    num2str(N_vertices),num2str(N_samples)];
status_CWC_max = system(join(command_CWC,' ') + ' max');
assert(status_CWC_max==0, "CWC Failed");
status_CWC_min = system(join(command_CWC,' ') + ' min');
assert(status_CWC_min==0, "CWC Failed");
end