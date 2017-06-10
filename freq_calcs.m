%Timer script

fCK_PSC=84e6;
%prescaler=100;
%CK_CNT=fCK_PSC/(prescaler+1)
CK_CNT=10e5;

PWM2_period_Hz=20;

%prescaler=(fCK_PSC/CK_CNT)-1

CK_CNT/PWM2_period_Hz


%% Timer interupt calc

timer_freq=84e6;
tim_prescaler=20;
interupt_freq=1000;

cnt_freq=timer_freq/(tim_prescaler+1)

tim_period=round(cnt_freq/interupt_freq)

%% - Stepper calcs
clear all
wheel_d=6.5e-2; %wheel diameter
wheel_r=wheel_d/2;
step_size=1.8;  %1 full step in degrees
step_res=1/16;

%%%wanted max tangential velocity
v_tangential_max=0.1; %1 m/s

w_rad=v_tangential_max/wheel_r;
w_deg=w_rad*(180/pi);   %degrees per sec

PWM_freq_max=w_deg/(step_size*step_res)




