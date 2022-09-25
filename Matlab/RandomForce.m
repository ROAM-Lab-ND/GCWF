function [f] = RandomForce(theta,f_h_norm,N)
%UNTITLED12 Summary of this function goes here
%   Detailed explanation goes here
if nargin < 3 || isempty(N)
    N = 1;
end
if nargin < 2 || isempty(f_h_norm)
    f_h_norm = rand(1,N);
end

if nargin < 1 || isempty(theta)
    theta = 2 * pi * rand(1,N);
end
N = length(theta);

f_h = f_h_norm .* [cos(theta); sin(theta)];
f = [f_h;ones(1,N)];

assert(all(size(f) == [3,N]));
end