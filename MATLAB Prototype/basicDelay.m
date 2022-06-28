%% A basic delay on a sine wave

%% Creation of the sine wave
Fs = 48000; 
Ts = 1/Fs; 
f = 2; 
t = [0:Ts:1].'; 
in = sin(2*pi*f*t);

%% Delay
N = .25;
delayt = t-N; 
delay = sin(2*pi*f*delayt);

%% Plotting Results
figure(1);
plot(t,in, t, delay, '--', 'LineWidth',5); axis([0 1 -1.1 1.1]); set(gca,'FontSize',20);
xlabel('Time (sec)', 'FontSize', 24); ylabel('Amplitude', 'FontSize', 24);
title('Basic Delay', 'FontSize', 24); legend('Signal', 'Delayed Signal', 'FontSize', 12);