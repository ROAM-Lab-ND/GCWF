function [Data] = loadTrueData(filename_max, filename_min)
%loadTrueData load the true CWC data
%   Detailed explanation goes here
file_CWC_max = readmatrix(filename_max);% id,OptVal,computeTime,feasibility
file_CWC_min = readmatrix(filename_min);% id,OptVal,computeTime,feasibility
assert(all( file_CWC_min(:,4) == file_CWC_max(:,4) ));


Data.max = file_CWC_max(:,2);
Data.min = file_CWC_min(:,2);
Data.max(abs(Data.max)<= 1e-6) = 0;
Data.min(abs(Data.min)<= 1e-6) = 0;
Data.maxFeas = file_CWC_max(:,4);
Data.minFeas = file_CWC_min(:,4);
Data.maxTime = mean(file_CWC_max(2:end,3));
Data.minTime = mean(file_CWC_min(2:end,3));

% disp("Maximization Time");
% disp( mean(file_CWC_max(2:end,3)) );
% disp("Minimization Time");
% disp( mean(file_CWC_min(2:end,3)) );
end