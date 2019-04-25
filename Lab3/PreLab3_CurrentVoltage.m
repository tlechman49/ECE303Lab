close all;clear all;clc

% Create equipment connections and objects
DMM=visa('agilent', 'USB0::0x2A8D::0xB318::MY58160068::0::INSTR');

% Open instruments
fopen(DMM);

fprintf(awg,'OUTP:LOAD INF'); % Place waveform generator into high-Z

% Conduct experiment by sweeping waveform generator voltage
Imeas = [];
Vmeas = [];
t = [];
tic();
while Vmeas(-1) > 4.8
    pause(1)
    voltage = str2num(query(DMM, 'MEAS:VOLT:DC?'));
    current = str2num(query(DMM, 'MEAS:CURR:DC?'));
    Vmeas = [Vmeas ; voltage];
    Imeas = [Imeas ; -current];
    t = [t ; toc()];
end

% Close and delete instrument connections and objects
fclose(DMM);
delete(DMM);

