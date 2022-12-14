
if ~exist('NewResult','var') || ~NewResult
    % If directly run this script, let the user choose the data set
    datapath = uigetdir();
    datapath = datapath+"/";
else
    % Call this script from SampleAndCompute, clean flag for next time.
    NewResult = false;
end

% Load results

load(datapath+"num_GD.mat");

disp("CWC");
CWC = loadTrueData(datapath+"max_tau.txt", datapath+"min_tau.txt");

disp("CWC Lin 4");
CWCLin_4 = loadTestData_LCWC(datapath+"max_tau_lin_4.txt", datapath+"min_tau_lin_4.txt", CWC);
disp("CWC Lin 8");
CWCLin_8 = loadTestData_LCWC(datapath+"max_tau_lin_8.txt", datapath+"min_tau_lin_8.txt", CWC);

disp("LCL (approach 1)"); % Longest Centered Line
LCL = loadTestData_Propose(datapath+"Tau_LCL.txt", CWC);

disp("EAV (approach 2)"); % Enumerate All Vertices
EAV = loadTestData_Propose(datapath+"Tau_EAV.txt", CWC);

disp("Grid Search "+num2str(N_grid_Opt)); % Optimal line found by brute force searching
GD5K = loadTestData_Propose(datapath+"Tau_Grid_"+num2str(N_grid_Opt)+".txt", CWC);

disp("Grid Search "+num2str(N_grid)); % Grid search
GD = loadTestData_Propose(datapath+"Tau_Grid_"+num2str(N_grid)+".txt", CWC);


Method = ["2nd CWC","1st CWC-4","1st CWC-8","Approach 1","Approach 2","GD-"+num2str(N_grid),"Optimal Line"]';
ErrorRate = [0,CWCLin_4.err_mean,CWCLin_8.err_mean,LCL.err_mean,EAV.err_mean,GD.err_mean,GD5K.err_mean]';
ComputationTime = [CWC.maxTime,CWCLin_4.maxTime,CWCLin_8.maxTime,LCL.time,EAV.time,GD.time,GD5K.time]';
StandardDeviation = [0,std(CWCLin_4.err_whole_NoNan),std(CWCLin_8.err_whole_NoNan),std(LCL.err_whole),...
    std(EAV.err_whole),std(GD.err_whole),std(GD5K.err_whole)]';

T1 = table(Method,ErrorRate,StandardDeviation,ComputationTime);

disp(T1);

%% 
close all
legend_strs = [];
f = figure(1);
hold on;
grid on;
legend_strs = plotHist(LCL.err_whole,'^-',legend_strs,"Approach 1 - Longest Centered Line");
legend_strs = plotHist(EAV.err_whole,'o-',legend_strs,"Approach 2 - Vertices Enumeration");
legend_strs = plotHist(GD.err_whole,'x-',legend_strs,"Grid Search with "+num2str(N_grid)+" Grids");
legend_strs = plotHist(CWCLin_4.err_whole,'s-',legend_strs,"$1^{\rm st}$-CWC with 4 edges");
legend_strs = plotHist(CWCLin_8.err_whole,'d-',legend_strs,"$1^{\rm st}$-CWC with 8 edges");
legend(legend_strs,'Location','best','Interpreter','latex');
xlabel("Moment Error $e$ (\%)");
ylabel("Cumulative Possibility");

figure_FontSize=14;

f.InnerPosition = [440   378   560   340];
xlim([0 110]);
ylim([0 1]);

box on;
ax = gca;
set(ax,'Color','none');
ax.FontName = 'Times New Roman';
ax.FontSize = figure_FontSize-2;
set(get(ax,'XLabel'),'FontSize',figure_FontSize,'Vertical','top','Interpreter','latex');
set(get(ax,'YLabel'),'FontSize',figure_FontSize,'Interpreter','latex');
set(findobj('FontSize',figure_FontSize),'FontSize',figure_FontSize);
set(f, 'renderer','painters');



%% 

function legend_strs = plotHist(error,style,legend_strs,legend_name)
[n,e]=histcounts(error,linspace(0,100,30+1),'Normalization','cdf');
plot(e(1:end-1),n,style,'LineWidth',2,'MarkerSize',10);
legend_strs = [legend_strs, legend_name];
end

