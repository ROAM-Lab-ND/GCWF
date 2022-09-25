function [Data] = loadTestData_Propose(filename, CWC)
%loadTestData load the testing data
%   Detailed explanation goes here
data = readmatrix(filename);  % id,maxval,minval,computeTime,feasibility

assert(all( CWC.minFeas == data(:,5) ));
% I_mis_min = CWC.minFeas ~= data(:,5);
% I_mis_max = CWC.maxFeas ~= data(:,5);
% assert( all(abs(CWC.min(I_mis_min) - data(I_mis_min,3)) < 1e-6 ) );
% assert( all(abs(CWC.max(I_mis_max) - data(I_mis_max,2)) < 1e-6 ) );

data(abs(data(:,2))<= 1e-6,2) = 0;
data(abs(data(:,3))<= 1e-6,3) = 0;

error_max_abs = abs(data(:,2) - CWC.max);
error_min_abs = abs(data(:,3) - CWC.min);
error_whole_abs = (error_max_abs + error_min_abs);

error_max = 100*abs(error_max_abs./CWC.max);
error_min = 100*abs(error_min_abs./CWC.min);
error_whole = 100 - 100 * ( data(:,2) - data(:,3) ) ./ ( CWC.max - CWC.min );
error_max(isnan(error_max)) = 0;
error_min(isnan(error_min)) = 0;
error_whole(isnan(error_whole)) = 0;

inFeasible = find(CWC.minFeas==0);
error_max(inFeasible) = 0;
error_min(inFeasible) = 0;
error_whole(inFeasible) = 0;
error_max_abs(inFeasible) = 0;
error_min_abs(inFeasible) = 0;
error_whole_abs(inFeasible) = 0;

errorIsOk = all(error_whole >= -1e-1) && all(error_whole <= 100+1e-1);
assert(errorIsOk, "Error Rate Not In Possible Range");
error_whole = bound(error_whole,0,100);

Data.data = data;
Data.max = data(:,2);
Data.min = data(:,3);
Data.time = mean(data(2:end,4));
Data.err_max = error_max;
Data.err_min = error_min;
Data.err_whole = error_whole;
Data.err_mean = mean(error_whole);

Data.err_max_abs = error_max_abs;
Data.err_min_abs = error_min_abs;
Data.err_whole_abs = error_whole_abs;
Data.err_mean_abs = mean(error_whole_abs);

% disp("Error");
% disp(Data.err_mean);
% disp("Time");
% disp(Data.time);

end

function y = bound(x,lb,ub)
  % return bounded value clipped between bl and bu
  y=min(max(x,lb),ub);
end