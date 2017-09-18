function dz = nonlin_sys(z,m,M,l,g,b,k,u)

c1=cos(z(1));
s1=sin(z(1));

dz(1,1) = z(2);

dz(2,1) = ((u*c1)-(b*z(3)*c1)-((M+m)*g*s1)+(m*l*c1*s1*(z(2)^2))+((M+m)*((k*z(2))/(l*m))) )/(m*l*(c1^2)-((M+m)*l));
%dy(4,1) = (1/D)*((m+M)*m*g*L*Sy - m*L*Cy*(m*L*y(4)^2*Sy - d*y(2))) - m*L*Cy*(1/D)*u +.01*randn;

dz(3,1) = z(4);

dz(4,1) = (u-(b*z(3))+(m*l*s1*(z(2)^2))-(m*g*c1*s1)+(c1*(k/l)*z(2)))/((M+m)-(m*(c1^2)));
%dy(2,1) = (1/D)*(-m^2*L^2*g*Cy*Sy + m*L^2*(m*L*y(4)^2*Sy - d*y(2))) + m*L*L*(1/D)*u;


end