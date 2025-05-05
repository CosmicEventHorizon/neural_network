clear

%parameters to modify
f= @(x) log(x); %function to integrate
a = 1; %integrate from a to b
b= 2;
%h=(b-a)/m
h=0.25; %if given h instead of m
m=(b-a)/h; %if given m instead of h


%optional parameters for richardson error
htest= 0.5;
q = 2; %used for richardson error
mtest = (b-a)/htest; %used for richardson error
h = (b-a)/m; %used for richardson error

%algorithm
%trapezoid_comp
%simpson_comp
answer = trapezoid_comp(f,a,b,m);
fprintf("The answer is %.12f\n",answer);
answertest = trapezoid_comp(f,a,b,mtest);
richardson = abs((answer-answertest)/((htest/h)^q-1));
fprintf("The estomated error is %.12f\n",richardson);
