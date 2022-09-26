function [] = BestLine_CPP(N_vertices,N_samples,N_grid)
%UNTITLED8 Summary of this function goes here
%   Detailed explanation goes here
arguments
    N_vertices (1,1) {mustBeInteger}
    N_samples (1,1) {mustBeInteger}
    N_grid (1,1) {mustBeInteger}
end
command_GD  = ["../build/Grid",num2str(N_vertices),num2str(N_samples),num2str(N_grid)];
status_GD      = system(join(command_GD,' '));
assert(status_GD==0, "GD Failed");
end
