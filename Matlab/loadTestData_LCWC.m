function [Data] = loadTestData_LCWC(filename_max, filename_min, CWC)
%loadTestData load the Linear CWC data
%   Detailed explanation goes here
file_CWCLin_max=readmatrix(filename_max);     % id,OptVal,computeTime,feasibility
file_CWCLin_min=readmatrix(filename_min);     % id,OptVal,computeTime,feasibility


file_CWCLin_max(abs(file_CWCLin_max(:,2))<= 1e-6,2) = 0;
file_CWCLin_min(abs(file_CWCLin_min(:,2))<= 1e-6,2) = 0;

error_max_abs = abs(file_CWCLin_max(:,2) - CWC.max);
error_min_abs = abs(file_CWCLin_min(:,2) - CWC.min);
error_whole_abs = (error_max_abs + error_min_abs);

error_max = 100*abs(error_max_abs./CWC.max);
error_min = 100*abs(error_min_abs./CWC.min);
error_whole = 100 - 100 * ( file_CWCLin_max(:,2) - file_CWCLin_min(:,2) ) ./ ( CWC.max - CWC.min );
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

inCorrect = find(CWC.minFeas~=file_CWCLin_min(:,4));
error_max(inCorrect) = NaN;
error_min(inCorrect) = NaN;
error_whole(inCorrect) = NaN;
error_max_abs(inCorrect) = NaN;
error_min_abs(inCorrect) = NaN;
error_whole_abs(inCorrect) = NaN;

error_whole_NoNan = error_whole(  ~isnan(error_whole)  );
errorIsOk = all(error_whole_NoNan >= -1e-3) && all(error_whole_NoNan <= 100+1e-3);
% assert(errorIsOk, "Error Rate Not In Possible Range");
error_whole_NoNan = bound(error_whole_NoNan,0,100);

err_whole_abs_NoNan = error_whole_abs( ~isnan(error_whole_abs) );


Data.max = file_CWCLin_max(:,2);
Data.min = file_CWCLin_min(:,2);
Data.maxTime = mean(file_CWCLin_max(2:end,3));
Data.minTime = mean(file_CWCLin_min(2:end,3));
Data.err_max = error_max;
Data.err_min = error_min;
Data.err_whole = error_whole;
Data.err_whole_NoNan = error_whole_NoNan;
Data.err_mean = mean( error_whole_NoNan ) ;

Data.err_max_abs = error_max_abs;
Data.err_min_abs = error_min_abs;
Data.err_whole_abs = error_whole_abs;
Data.err_mean_abs = mean( err_whole_abs_NoNan );
Data.err_whole_abs_NoNan = err_whole_abs_NoNan;


% disp("Error");
% disp( Data.err_mean );
% disp("Maximization Time");
% disp( Data.maxTime );
% disp("Minimization Time");
% disp( Data.minTime );
% disp("Mis Matching");
% disp( length(inCorrect) );

end

function y = bound(x,lb,ub)
  % return bounded value clipped between bl and bu
  y=min(max(x,lb),ub);
end