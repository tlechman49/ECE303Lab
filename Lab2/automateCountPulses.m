close all;clear all;clc

s = serial('COM5', 'BaudRate', 9600);
set(s,'Terminator','CR/LF');
fopen(s);

readData = fscanf(s); %% hello world?
fprintf(readData);
while (1)
    
    fprintf(s, 'Hello'); %% request data
    pause(1);
    readData = fscanf(s); %% clear buffer
    readData = fscanf(s); %% clear buffer
    readData = fscanf(s); %% receive pulse count
    fprintf(readData);
    
end

fclose(s);
delete(s);