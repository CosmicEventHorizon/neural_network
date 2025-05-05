%algorithm
function answer = simpson_comp(f,a,b,m)
    h=(b-a)/(2*m);
    sigma = 0;
    for i=1:1:m
        x2i_1 = a + h*(2*i-1);
        sigma = sigma+f(x2i_1);
    end 
    sigma2 = 0;
    for i=1:1:m-1
        x2i = a + h*(2*i);
        sigma2 = sigma2+f(x2i);
    end
    answer = double(h/3 * (f(a)+f(b)+4*sigma+2*sigma2)); 
end

