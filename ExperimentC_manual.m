pulse_freq = 1400.0;
dutycycle = [0.2:0.05:0.8,0.75:-0.05:0.2];
on_time = (1.0 / pulse_freq) * dutycycle;

figure
plot(on_time,dutycycle,'o','MarkerFaceColor','b')
xlabel('On Time')
ylabel('Duty Cycle')
title('On Time vs. Duty Cycle')
grid on
legend('On Time vs. Duty Cycle')
