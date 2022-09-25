
if ~exist('N','var')
    N = 6;
end
N_samples = 10000
indx = 2;
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

CWC_CPP(N,N_samples);
CWC_Lin_CPP(N,N_samples,N_edges);
CWC_Lin_CPP(N,N_samples,N_edges_2);
EAV_CPP(N,N_samples);
LCL_CPP(N,N_samples);
BestLine_CPP(N,N_samples,N_grid_Opt);
BestLine_CPP(N,N_samples,N_grid);

%% 
CompareResult;
