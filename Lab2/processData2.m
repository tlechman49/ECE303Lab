f = f*60;
countsPerSecond = countsPerSecond*60;

for i = 1:length(f)
    if f(i) > 500*60
        f(i) = 0;
    end
end

figure(1)
hold on
plot(V(1:51), f(1:51), 'red')
plot(V(52:101), f(52:101), 'blue')
plot(V(102:151), f(102:151), 'green')
xlabel('Voltage')
ylabel('RPM')
title('RPM vs. Voltage from Oscilloscope')
legend('0 to 5V From Off', '5 to 0V', '0 to 5V Running')
grid on

figure(2)
hold on
plot(V(1:51), countsPerSecond(1:51), 'red')
plot(V(52:101), countsPerSecond(52:101), 'blue')
plot(V(102:151), countsPerSecond(102:151), 'green')
xlabel('Voltage')
ylabel('RPM')
title('RPM vs. Voltage from Arduino')
legend('0 to 5V From Off', '5 to 0V', '0 to 5V Running')
grid on