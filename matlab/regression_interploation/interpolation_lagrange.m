clear

%parameters to modify
global X;
global Y; 
X = [1,2,4]; %points xi
Y = [2,-1,5]; %points yi

%algorithm parameters
M = length(X);
solution=[];

for i=1:1:M
    coefficient = 1;
    for j=1:1:M
        if i~=j
            fprintf("(%d-%d)", X(i), X(j))
            coefficient = coefficient*(X(i)-X(j));
        end
    end
    fprintf("=%d",coefficient);
    solution(i)=(1/coefficient)*Y(i);
    fprintf("\n");
end
format rational 
disp(solution);
            
