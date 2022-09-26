clc
clearvars  -except N_vertices N_samples indx complexity N_list datapath datapath_CPP
close all force

isDone = 0;

mu = 1;
fz = 1;

if ~exist('datapath','var')
    datapath = "Data/";
end
system("rm -rf "+datapath+"*");

if ~exist('datapath_CPP','var')
    datapath_CPP = "../Matlab/"+datapath;
end

if ~exist('N_vertices','var')
    N_vertices = 10;
end

if ~exist('N_samples','var')
    N_samples = 10000;
end


if ~exist('indx','var')
    sp_list = {'Random','N-gon'};
    [indx,tf] = listdlg('PromptString','Choose your supporting polygon.','ListString',sp_list,'SelectionMode','single');
    if (tf==0)
        return
    end
end



file_vertices = fopen(datapath + "vertices.txt",'w');
file_force = fopen(datapath + "force.txt",'w');

close all force
wb = waitbar(0,"Generating");

count = 0;
Regen = false;
for i = 1:N_samples
    switch indx
        case 1
            p_hull = RandomPolygon(N_vertices);
        case 2
            p_hull = N_Gon(N_vertices);
    end
%     disp(p_hull');
 
    f = RandomForce();
%     disp(f');

    SavePolygon(file_vertices, p_hull, i);
    SaveForce(file_force,f,i);

    waitbar(i/N_samples,wb,"Generating");

end
fprintf('\n');
waitbar(1,wb,"Done");
close(wb);

fclose(file_vertices);
fclose(file_force);

isDone = 1;
