clear all, close all, clc

g = 9.81;
b=0.001;
k=0.001;

n=4; %system states
p=1; %inputs

Ts=0.001;


%% Wheel calcs
m_wheel=30e-3;
r_wheel=104e-3;
I_wheel=0.5*m_wheel*r_wheel^2  %I=0.5*M*r^2

tao_m_max=0.45;     %In Nm
F_input_max= 2*(tao_m_max/r_wheel);

%% mass calcs

%%%%%% Cart %%%%%%%%
stepper_m=360e-3;
stepper_holder=60e-3;   %2 holders + plate
M = 2*stepper_m+stepper_holder+2*m_wheel;

%%%%%% "Rod" %%%%%%%%
Battery_ycord=18e-2;
Battery_mass=285e-3;    %%including holders and breadboard
Nucelo_ycord=6e-2;
Nucleo_mass=70e-3;      %%Including cables and motordrivers
rod_ycord=10.5e-2;
rod_mass=35e-3;
middleplate_ycord=11e-2;
middleplate_mass=20e-3;

m=Battery_mass+Nucleo_mass+rod_mass+middleplate_mass;
l = (Battery_ycord*Battery_mass + Nucelo_ycord*Nucleo_mass + rod_ycord*rod_mass + middleplate_ycord*middleplate_mass)/m;

I_frame=0.26*l^2;

%% Non-lin simulation open loop

angle_init=10;
theta_rad_init=angle_init*(pi/180);
%states: [theta; d_theta; x; dx];
tspan = 0:Ts:10;
u=0;
z0 = [theta_rad_init; 0; 0; 0];
[t,z] = ode45(@(t,z)nonlin_sys(z,m,M,l,g,b,k,u),tspan,z0);

figure(1)
clf
plot(t,z(:,1));
hold on
plot(t,z(:,2));
legend('theta','d_theta');

figure(2)
clf
plot(t,z(:,3));
hold on
plot(t,z(:,4));
legend('x','dx');

p=ws2struct(); %for simulink simulation
%% Linear system open loop
linsys=lin_sys(m,M,l,g,b,k);
linsys_init=z0;

%% Complementary filter calcs
tao=1.5e-1;



%% Controller design

ctrl_matrix=ctrb(linsys.A,linsys.B);
rank(ctrl_matrix);

obs_matrix=obsv(linsys.A,linsys.C);
rank(obs_matrix);

R=10;    %cheap input -> low R
Q=[10 0 0 0;
   0 10 0 0;
   0 0 0.01 0;
   0 0 0 1];    %More penalization on theta and most on d_theta

%Continous
K=lqr(linsys.A,linsys.B,Q,R);

%Discrete
sysd = c2d(linsys,Ts);
K_d = dlqr(sysd.A,sysd.B,Q,R)

sprintf('{%.02f ,%.02f, %.02f,%.02f}', K_d(1), K_d(2), K_d(3), K_d(4))

%% Simulation closed loop
N=length(tspan);    %simulation time in samples

x0=z0;%[0; 0; 0; 0];
u0=0;

x=zeros(N,4);
y=zeros(N,4);
%u=zeros(1,N);

x(1,:)=x0;

for k=1:N-1
    
    x(k+1,:)=x(k,:)*(sysd.A-(sysd.B*K_d));  %x(n+1)=x(n)*(A-BK) feedback law

end

figure (3)
clf
plot(tspan,x(:,1));
hold on
plot(tspan,x(:,2));
legend('theta','d_theta');

figure (4)
clf
plot(tspan,x(:,3));
hold on
plot(tspan,x(:,4));
legend('x','d_x');

