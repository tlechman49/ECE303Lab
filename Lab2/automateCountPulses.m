close all;clear all;clc;

volts_per_division=2;
time_range='3E-3';

% Create equipment connections and objects
awg=visa('agilent', 'USB0::0x0957::0x0407::MY44043475::0::INSTR');
scope=visa('agilent', 'USB0::0x0957::0x1799::MY58100823::0::INSTR');

scope.InputBufferSize=2^17;
scope.OutputBufferSize=2^17;

% Open instruments
fopen(awg);
fopen(scope);

fprintf(awg,'OUTP:LOAD INF'); % Place waveform generator into high-Z

range_str=[':CHANNEL1:RANGE ' num2str(8*volts_per_division) '\n'];
fprintf(scope,range_str);
fprintf(scope,[':TIMEBASE:MODE NORMAL; RANGE ' time_range]);
tv=query(scope,':CHANNEL1:RANGE?;');
offset=query(scope,':CHANNEL1:OFFSET?');

operationComplete = str2double(query(scope,'*OPC?'));

fprintf(scope,'WAV:SOUR CHAN1;');
fprintf(scope,'WAV:FORM WORD;');
fprintf(scope,'WAV:POINTS:MODE RAW');
fprintf(scope,'WAV:POIN 2000');
fprintf(scope,':WAVEFORM:BYTEORDER LSBFirst');

% Conduct experiment by sweeping waveform generator voltage
V=[0:0.1:5, 4.9:-0.1:0, 0.1:0.1:5, 4.9:-0.1:0];
N_volts = numel(V);
countsPerSecond=zeros(1,N_volts);
f=zeros(1,N_volts);

s = serial('COM56', 'BaudRate', 9600);
set(s,'Terminator','CR/LF');
fopen(s);

readData = fscanf(s); %% hello world?
fprintf(readData);

for K=1:N_volts
    str1=['APPL:DC DEF,DEF,' num2str(V(K))];
    fprintf(awg,str1); % Apply DC Voltage Output from AWG
    pause(5)
    
    fprintf(s, 'Hello'); %% request data
    readData = fscanf(s); %% clear buffer
    readData = fscanf(s); %% clear buffer
    readData = fscanf(s); %% receive pulse count
    fprintf(readData);
    countsPerSecond(K) = str2num(readData);
    
    fprintf(scope,[':AUTOSCALE;']);
    fprintf(scope,'DIG CHAN1;');
    operationComplete = str2double(query(scope,'*OPC?'));
    while ~operationComplete
        operationComplete = str2double(query(scope,'*OPC?'));
    end
    query(scope,':WAVEFORM:PREAMBLE?');
    fprintf(scope,'WAV:DATA?;');
    binblockread(scope,'uint16'); fread(scope,1);
    f(K)=str2double(query(scope,':MEAS:FREQUENCY?'));
    disp(f(K));
end

fclose(s);
delete(s);

% Close and delete instrument connections and objects
fclose(awg);
delete(awg);
fclose(scope);
delete(scope);