clear

%parameters to modify
f= @(x) 1/sqrt(x^2+4); %function to integrate
a = 0; %integrate from a to b
b= 2*sqrt(3);
q = 2; %initial truncation order
otarget=8; %target of order of truncation error 

%algorithm
m=1;
no_rows =  otarget/q;
I = [];
q_i = [];
for i=1:1:no_rows
    answer = trapezoid_comp(f,a,b,m);
    I(i) = answer;
    q_i(i) = q*i;
    m=m*2;
end

rij=0;
for i=1:1:no_rows
    fprintf("%.9f\t",I(i));
    for j=2:1:i
        rij = recursive(q_i,I,i,j);
        fprintf("%.9f\t",rij);
    end
    fprintf("\n");
end

fprintf("The answer is %.12f\n",rij);

function result = recursive(q,I,i,j)
    if j == 1
        result=I(i);
        return;
    end
    rij_1 = recursive(q,I,i,j-1);
    ri_1j_1 = recursive(q,I,i-1,j-1);
    result = ((2^q(j-1))*rij_1-ri_1j_1)/((2^q(j-1))-1); 
end