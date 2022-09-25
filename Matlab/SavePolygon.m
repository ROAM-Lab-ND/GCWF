function [] = SavePolygon(file, p_hull, index)
%UNTITLED5 Summary of this function goes here
%   Detailed explanation goes here
arguments
    file
    p_hull (2,:) {mustBeNumeric}
    index (1,1) {mustBeInteger}
end
assert(all(p_hull(:,1) == p_hull(:,end)),"Not a hull!")
p_vertices = p_hull(:,1:end-1);
    
    % Output the vertices
    fprintf(file,'%d',index);
    for j = 1:size(p_vertices,2)
        fprintf(file,';%16.12f,%16.12f ',p_vertices(:,j));
    end
    fprintf(file,'\n');
end