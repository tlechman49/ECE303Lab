clear all; close all; clc;

load('energizerBattery.mat');

Imeas = -Imeas(2 : end);
t = t(2 : end);
t = t - t(1);
Vmeas = Vmeas(2 : end);

power = Vmeas.*Imeas;
energy = cumtrapz(t, power);

figure(1)
plot(t, Vmeas)
xlabel('Time (Seconds)')
ylabel('Discharge Voltage (V)')
title('Battery Discharge Voltage Over Time')
grid on

figure(2)
plot(t, Imeas)
xlabel('Time (Seconds)')
ylabel('Discharge Current (A)')
title('Battery Discharge Current Over Time')
grid on


figure(3)
plot(t, power)
xlabel('Time (Seconds)')
ylabel('Instantaneous Power (W)')
title('Instantaneous Battery Power Over Time')
grid on

figure(4)
plot(t, energy)
xlabel('Time (Seconds)')
ylabel('Cumulative Energy Delivered (J)')
title('Cumulative Energy Delivered Over Time')
grid on

for i = 1:4
    saveas(figure(i), sprintf('energizer_%d.jpg',i));
end