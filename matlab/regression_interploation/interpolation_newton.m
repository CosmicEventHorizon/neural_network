clear

%parameters to modify
global X;
global Y; 
X = [1,2,3,4]; %points xi
Y = [3.4,-4.6,-10.2,-66.8]; %points yi

%algorithm parameters
M = length(X);
L = []; %newton level
solution=[];



for i=1:1:M
    fprintf("The level is %d\n", i);
    L(i) = i;
    solution(i) = newton(L);
end

disp(solution);

function level_value = newton(P)
    global Y;
    global X;
    if length(P)==1
        level_value = Y(P(1));
    else
        %construct the new P's
        P2=[];
        P1=[];
        for i=1:1:length(P)-1
            P1(i) = P(i);
            P2(i) = P(length(P)+1-i);
        end
        level_value = (newton(P2)-newton(P1))/(X(P(length(P)))-X(P(1)));
    end
end

