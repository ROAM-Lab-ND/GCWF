function [] = SaveForce(file,f_zmp,index)
%UNTITLED11 Summary of this function goes here
%   Detailed explanation goes here

arguments
    file
    f_zmp (3,1) {mustBeNumeric}
    index (1,1) {mustBeInteger}
end
assert(f_zmp(3) == 1);
fprintf(file,'%d;%16.12f,%16.12f,%16.12f\n',index,f_zmp);
end