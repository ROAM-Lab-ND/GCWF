function p_hull = N_Gon(N)
%UNTITLED3 Summary of this function goes here
%   Detailed explanation goes here
arguments
    N (1,1) {mustBeInteger}
end
notFeasible = true;
angle_step = linspace(0,2*pi,N+1);
while notFeasible
    count = fprintf("Sampling Polygon");
    
    angle_shift = rand(1)*pi;
    pos_shift = 2*rand(2,1) - 1;
    p_hull = [sin(angle_step+angle_shift);cos(angle_step+angle_shift)];
    p_hull = p_hull(:,1:end-1);
    [K,~] = convhull( p_hull(1,:), p_hull(2,:) );
    p_hull = p_hull(1:2,K) + pos_shift;

    assert(size(p_hull,2) == N+1);
    notFeasible =  ~inpolygon(0,0,p_hull(1,:),p_hull(2,:));
    if notFeasible
        fprintf(" -- Not Feasible, reGen\n");
    else
        fprintf(repmat('\b',1,count));
    end
        
end
end