clc
close all
clearvars  -except N_vertices
if ~exist('N_vertices','var')
    N_vertices = 6;
end
datapath = "Data/";
N_samples = 1000
% indx = 1;
RandomTestSampleGenerator;
if isDone ~= 1
    disp("Generation Failure");
    return;
end

%% 
N_grid = 81;
N_grid_Opt = 5000;
N_edges = 4;
N_edges_2 = 8;

save(datapath+"num_GD.mat","N_grid_Opt","N_grid");

CWC_CPP(datapath,N_vertices,N_samples);
CWC_Lin_CPP(datapath,N_vertices,N_samples,N_edges);
CWC_Lin_CPP(datapath,N_vertices,N_samples,N_edges_2);
EAV_CPP(datapath,N_vertices,N_samples);
LCL_CPP(datapath,N_vertices,N_samples);
BestLine_CPP(datapath,N_vertices,N_samples,N_grid_Opt);
BestLine_CPP(datapath,N_vertices,N_samples,N_grid);

%% 
CompareResult;
