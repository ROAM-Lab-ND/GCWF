clc
clearvars  -except N N_samples indx complexity N_list
system("rm -rf Data/*");
close all force

isDone = 0;

mu = 1;
fz = 1;
if ~exist('N','var')
    N = 10;
end
p_zmp = [0;0];
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



file_vertices = fopen('Data/vertices.txt','w');
file_force = fopen('Data/force.txt','w');

close all force
wb = waitbar(0,"Generating");

count = 0;
Regen = false;
for i = 1:N_samples
    switch indx
        case 1
            p_hull = RandomPolygon(N);
        case 2
            p_hull = N_Gon(N);
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
