function p_hull = RandomPolygon(N)
%UNTITLED3 Summary of this function goes here
%   Detailed explanation goes here
arguments
    N (1,1) {mustBeInteger}
end
notFeasible = true;
while notFeasible
    count = fprintf("Sampling Polygon");
    
    p_hull = 2*rand(2,3*N) - 1;
    [K,~] = convhull( p_hull(1,:), p_hull(2,:) );
    p_hull = p_hull(1:2,K);
    while size(p_hull,2) < N+1
        p_hull = [p_hull,2*rand(2,2*N) - 1];
        [K,~] = convhull( p_hull(1,:), p_hull(2,:) );
        p_hull = p_hull(1:2,K);
    end
    if size(p_hull,2) > N+1
        p_hull = [p_hull(:,1:N), p_hull(:,1)];
    end

    assert(size(p_hull,2) == N+1);
    notFeasible =  ~inpolygon(0,0,p_hull(1,:),p_hull(2,:));
    if notFeasible
        fprintf(" -- Not Feasible\n");
    else
        fprintf(repmat('\b',1,count));
    end
        
end
end