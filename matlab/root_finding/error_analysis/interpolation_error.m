clear
syms x
%calculate maximum abs(f(x)-P(x))

%parameters to modify
xp=[0,pi/6,2*pi/6,pi/2]; 
f=sin(x);
xt=1; %estimate the error at his point

%algorithm
m=length(xp);
mult = 1;
for i=1:m
    mult = mult*(xt-xp(i));
    f=diff(f,x);
end
disp(f);
i=1;
ca=xp(1):0.01:xp(m);
for c=xp(1):0.01:xp(m)
    f_m_c(i) = subs(f,c);
    i=i+1;
end
f_m_c = abs(f_m_c);
[f_m_c_max, index] = max(f_m_c);
fprintf("The maximum f_m_c is %.5f at c=%.5f\n",f_m_c_max,ca(index));
max_error = mult*f_m_c_max/factorial(m);
fprintf("The maximum error is %.5f\n",max_error);
