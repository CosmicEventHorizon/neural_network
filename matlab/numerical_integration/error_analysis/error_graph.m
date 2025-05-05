clear

%plot estimated error against step-size
%p
f = @(x) sqrt(pi)*exp(-2*x^2);
a = 0;
b=3;
q=2; %order of convergence of method
htest=1; %h test to be used for richardson

%algorithm
estimated_error = [];
i=1;
h(1) = htest;

%dont forget to change (b-a)/2h(i) for simpson and b-a/h(i) for trapezoid
for k=1:8
    result_test = trapezoid_comp(f,a,b,(b-a)/h(i));
    i=i+1;
    h(i) = 10^(-k); %h(i-1)/2
    m=(b-a)/h(i);
    result = trapezoid_comp(f,a,b,m);
    estimated_error(k) = abs((result-result_test)/((h(i-1)/h(i))^q-1));
end

loglog(h(2:end),estimated_error,'o');
grid