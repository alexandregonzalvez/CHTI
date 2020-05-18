clear;
clear all;
close all;
% Temps fenêtre d'analyse
T = 0.01;

% nb points fft
M = 3200;

% Période d'échantillonnage
Te = T/M;

% Temps de simulation
Tsim = T-Te;

% Fréquence de la sinusoïde (hz)
Fsin = 120000;

SimOut = sim('model_simulink');% Link to Simulink

subplot(2,2,1);
plot(SimOut.Tps_Cont, SimOut.Sin_Cont); % Plot Sin_Cont en fonction du temps
title('Sinusoïde continue');
xlabel('Temps (s)');
ylabel('Amplitude');

subplot(2,2,2);
plot(SimOut.Tps_Ech, SimOut.Sin_Ech,"."); 
title('Sinusoïde discrète');
xlabel('Temps (s)');
ylabel('Amplitude');

subplot(2,2,3);
fourier = fft(SimOut.Sin_Ech);
f = (0:length(fourier) - 1)*M/length(fourier);
f=(0:M-1)/T
plot(f, abs(fourier),"*");
title('Transformée de fourier');
xlabel('Fréquence (Hz)');
ylabel('Amplitude');