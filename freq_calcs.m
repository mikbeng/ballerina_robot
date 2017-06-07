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
tim_prescaler=2000;
interupt_freq=10;

cnt_freq=timer_freq/(tim_prescaler+1);

tim_period=round(cnt_freq/interupt_freq)