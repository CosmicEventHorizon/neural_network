function answer = trapezoid_comp(f,a,b,m)
h=(b-a)/m;
sigma = 0;
xi=a;
for i=1:1:m-1
    xi = xi + h;
    sigma = sigma+f(xi);
end 
answer = double(h/2 * (f(a)+f(b)+2*sigma));
end