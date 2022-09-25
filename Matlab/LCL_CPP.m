function [] = LCL_CPP(N_vertices,N_samples)
%UNTITLED8 Summary of this function goes here
%   Detailed explanation goes here
arguments
    N_vertices (1,1) {mustBeInteger}
    N_samples (1,1) {mustBeInteger}
end
command_LCL  = ["../GeoCWC/LCL",num2str(N_vertices),num2str(N_samples)];
status_LCL      = system(join(command_LCL,' '));
assert(status_LCL==0, "LCL Failed");
end