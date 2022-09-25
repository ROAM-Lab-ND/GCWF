function [] = CWC_CPP(N_vertices,N_samples)
%UNTITLED6 Summary of this function goes here
%   Detailed explanation goes here
arguments
    N_vertices (1,1) {mustBeInteger}
    N_samples (1,1) {mustBeInteger}
end
command_CWC = ["/.../mosek/9.3/tools/examples/fusion/cxx/CWC",...
    num2str(N_vertices),num2str(N_samples)];
status_CWC_max = system(join(command_CWC,' ') + ' max');
assert(status_CWC_max==0, "CWC Failed");
status_CWC_min = system(join(command_CWC,' ') + ' min');
assert(status_CWC_min==0, "CWC Failed");
end