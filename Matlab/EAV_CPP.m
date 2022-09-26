function [] = EAV_CPP(data_path,N_vertices,N_samples)
%EAV_CPP Call Enumerating All Vertices C++ solver.
%   EAV_CPP(data_path,N_vertices,N_samples) call the c++ 
%   executable file in ../build/ directory to solve the data stored in
%   data_path directory with enumerating all vertices.
%   N_vertices gives the number of vertices of testing samples.
%   N_samples gives the nubmer of testing samples.
arguments
    data_path
    N_vertices (1,1) {mustBeInteger}
    N_samples (1,1) {mustBeInteger}
end
command_EAV  = ["../build/EAV",data_path,num2str(N_vertices),num2str(N_samples)];
status_EAV   = system(join(command_EAV,' '));
assert(status_EAV==0, "EAV Failed");
end