close all;clear all;clc

s = serial('COM5', 'BaudRate', 9600);
fopen(s);

readData = fscanf(s); %% hello world?
fprintf(readData);

fwrite(s, 1); %% request data

readData = fscanf(s); %% receive pulse count
