function [] = CWC_Lin_CPP(N_vertices,N_samples,N_edges)
%UNTITLED7 Summary of this function goes here
%   Detailed explanation goes here
arguments
    N_vertices (1,1) {mustBeInteger}
    N_samples (1,1) {mustBeInteger}
    N_edges (1,1) {mustBeInteger}
end
command_CWCLin = ["/.../mosek/9.3/tools/examples/fusion/cxx/CWC_Lin",...
    num2str(N_vertices),num2str(N_samples),num2str(N_edges)];
status_CWCLin_max = system(join(command_CWCLin,' ') + ' max');
assert(status_CWCLin_max==0, "CWCLin Failed");
status_CWCLin_min = system(join(command_CWCLin,' ') + ' min');
assert(status_CWCLin_min==0, "CWCLin Failed");
end