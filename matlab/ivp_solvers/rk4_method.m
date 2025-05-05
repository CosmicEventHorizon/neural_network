function result = rk4_method(f, x0, y0, xn, h, output)
    n=round((xn-x0)/h);
    m=length(y0);
    if(output==true)
        fprintf("ti\t\t\tk1\t\t\tk2\t\t\tk3\t\t\tk4\t\t\twi\n");
        fprintf("%.6f\t\t\t\t\t\t\t\t\t\t\t\t",x0);
    end
    result = y0;
    if(output==true)
        for i=1:1:m
            fprintf("\t%.12f",result(i));
        end
    end
    fprintf("\n");
    for j=1:1:n
        %algorithm parameters
        k1 = f(x0,result);
        k2 = f(x0+h/2,result+k1*h/2);
        k3 = f(x0+h/2,result+k2*h/2);
        k4 = f(x0+h,result+h*k3);
        result = result + (h/6)*(k1+2*k2+2*k3+k4);
        x0 = x0+h;
        if(output==true)
            fprintf("%.6f",x0);
            for i=1:1:m
                fprintf("\t%.6f",k1(i));
            end 
            for i=1:1:m
                fprintf("\t%.6f",k2(i));
            end
            for i=1:1:m
                fprintf("\t%.6f",k3(i));
            end 
            for i=1:1:m
                fprintf("\t%.6f",k4(i));
            end
            for i=1:1:m
                fprintf("\t%.12f",result(i));
            end
            fprintf("\n");
        end
    end
end