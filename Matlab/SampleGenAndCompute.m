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


wb = waitbar(0,"Computing...");
waitbar(1/7,wb,"Computing 2nd-CWC: 1 / 7");
CWC_CPP(datapath,N_vertices,N_samples);
waitbar(2/7,wb,"Computing 1st-CWC with "+num2str(N_edges)+" edges: 2 / 7");
CWC_Lin_CPP(datapath,N_vertices,N_samples,N_edges);
waitbar(3/7,wb,"Computing 1st-CWC with "+num2str(N_edges_2)+" edges: 3 / 7");
CWC_Lin_CPP(datapath,N_vertices,N_samples,N_edges_2);
waitbar(4/7,wb,"Computing Enumerate All Vertices: 4 / 7");
EAV_CPP(datapath,N_vertices,N_samples);
waitbar(5/7,wb,"Computing Longest Centered Line: 5 / 7");
LCL_CPP(datapath,N_vertices,N_samples);
waitbar(6/7,wb,"Computing Optimal Line with "+num2str(N_grid_Opt)+" grids: 6 / 7");
BestLine_CPP(datapath,N_vertices,N_samples,N_grid_Opt);
waitbar(7/7,wb,"Computing Grid Search with "+num2str(N_grid)+" grids: 7 / 7");
BestLine_CPP(datapath,N_vertices,N_samples,N_grid);


waitbar(1,wb,"Done. Ploting Result.");

%% 
CompareResult;

close(wb);
