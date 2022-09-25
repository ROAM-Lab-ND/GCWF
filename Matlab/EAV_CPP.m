function [] = EAV_CPP(N_vertices,N_samples)
%UNTITLED8 Summary of this function goes here
%   Detailed explanation goes here
arguments
    N_vertices (1,1) {mustBeInteger}
    N_samples (1,1) {mustBeInteger}
end
command_EAV  = ["../GeoCWC/EAV",num2str(N_vertices),num2str(N_samples)];
status_EAV      = system(join(command_EAV,' '));
assert(status_EAV==0, "EAV Failed");
end