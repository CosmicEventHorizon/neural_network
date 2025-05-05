clear

%PA=LU is better because:
%is free of truncation errors, 
%can always be found (as long as the solution of the system exists), 
%is in general more accurate than if computed by A=LU method (assuming A=LU can find a solution)


%parameters to modify
A=[2.2220, 16.710, 9.6120; 3.3330, 15920, -10.333; 1.5611, 5.1791, 1.6852];
b=[28.544;15913;8.4254];
A_copy = A;
b_copy = b;

%algorithm variables
[n, m] = size(A);
x = sym('x', [1 n]);
d = sym('d', [1 n]);
L=eye(n);
P=eye(n);


[U_after, P_after] = start_pivot(A,P,n,m);
U=U_after;
P=P_after;


%gaussian algorithm
for i=1:n
    %at the start of modifying every row call the pivoting function to reorganize the equations
    [U_after, P_after] = start_pivot(U,P,n,m);
    U=U_after;
    P=P_after;
    for j=i+1:n
        c=U(j,i)/U(i,i);
        L(j,i) = c;
        for k=1:n
            U(j,k) = U(j,k) - c*U(i,k);
        end
    end
end

fprintf("The L U P Matrixes are: \n");
disp(L);
disp(U);
disp(P);

%forward substitution
%di = (bi-sigma(j=1,i-1)(L(i,j)*d(j)))/cii i=2,3,4...n
b=P*b;
d(1) = vpa(b(1)/L(1,1));
for i=2:n
    sum=0;
    for j=1:i-1
        sum = sum + L(i,j)*d(j);
    end
    d(i) = vpa((b(i) - sum)/L(i,i));
end


%backward substitution
%xi = (bi-sigma(j=i+1,n)(A(i,j)*x(j)))/cii  i=n-1,n-2...1
x(n) = vpa(d(n)/U(m,n));
for i=n-1:-1:1
    sum=0;
    for j=i+1:n
        sum = sum + U(i,j)*x(j);
    end
    x(i) = vpa((d(i) - sum)/U(i,i));
end


for i=1:n
    fprintf("x%d = %e\n", i, double(x(i)));
end



%pivoting function
function [U_after, P_after] = start_pivot(Uin,Pin,n,m)
    U=Uin;
    P=Pin;
    for i=1:n
        max = abs(U(i,i));
        max_j = i;
        for j=i+1:n
            if(max<U(j,i))
                max=U(j,i);
                max_j = j;
            end
        end
        if(max_j>i)
            for k=1:m
                %Switch U rows
                temp=U(i,k);
                U(i,k) = U(max_j,k);
                U(max_j,k) = temp;

                %Switch P rows
                tempP=P(i,k);
                P(i,k) = P(max_j,k);
                P(max_j,k) = tempP;
            end
        end
    end
    U_after = U;
    P_after = P;
end


