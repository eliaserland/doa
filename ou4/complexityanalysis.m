% ------------------------------------------------------------------------
% DOA LP4 2020 - OU4 
% Author: Elias Olofsson (tfy17eon@cs.umu.se)
%
% Experimental analysis of complexity for various table implementations. 
%
% Choose test nr to plot graphs for:
%   1: Insertion of n elements.
%   2: Random removal of n elements.
%   3: Random unsuccessful lookup speed of n elements.
%   4: Random successful lookup speed of n elements.
%   5: Skewed successful lookup speed of n elements.
% ------------------------------------------------------------------------
clear all; close all; clc;

test_nr = 5; %Test nr 1-5 is valid.

% Load data
DATA_table2 = load('time_table2.txt');
DATA_mtf = load('time_mtf.txt');
DATA_array = load('time_array.txt');
DATA_hash = load('time_hash.txt');


% Calculate avgerages
avg_table2 = find_average(DATA_table2, test_nr);
avg_mtf = find_average(DATA_mtf, test_nr);
avg_array = find_average(DATA_array, test_nr);
avg_hash = find_average(DATA_hash, test_nr);

% Merge into matrix 
avg_matrix = [avg_table2 , avg_mtf(:,2), avg_array(:,2), avg_hash(:,2)]

%-------------------------------------------
% Plot timed results
for i = 2:5
   plot(avg_matrix(:,1), avg_matrix(:,i));
   hold on
end

legend('table.c', 'mtftable.c', 'arraytable.c', 'hashtable.c','Location', 'northwest')
xlabel('n - Number of table entries')
ylabel('Time (ms)')
axis([0 21000 -max(avg_matrix(end,2:5))*0.1 max(avg_matrix(end,2:5))*1.1 ])

switch test_nr
    case 1
        title('Insertion speed test')
    case 2
        title('Removal speed test')
    case 3
        title('Random unsuccessful lookup speed')
    case 4
        title('Random successful lookup speed')
    case 5
        title('Skewed successful lookup speed')
end

offset = 500;
scale = 0.75;
set(gcf, 'Position', [500 + offset, 100 + offset, scale*500, scale*400])


%-------------------------------------------
% Execution speed per function call of table interface
% rel = avg / n 
rel_avg_matrix = [avg_matrix(:,1), avg_matrix(:,2:5)./avg_matrix(:,1)];

figure
for i = 2:5
   plot(rel_avg_matrix(:,1), rel_avg_matrix(:,i));
   hold on
end

legend('table.c', 'mtftable.c', 'arraytable.c', 'hashtable.c','Location', 'northwest')
xlabel('n - Number of table entries')
ylabel('Time/n (ms)')
axis([0 21000 -max(rel_avg_matrix(end,2:5))*0.1 max(rel_avg_matrix(end,2:5))*1.1 ])

switch test_nr
    case 1
        title('Insertion speed test')
    case 2
        title('Removal speed test')
    case 3
        title('Random unsuccessful lookup speed')
    case 4
        title('Random successful lookup speed')
    case 5
        title('Skewed successful lookup speed')
end

offset = 500;
scale = 0.75;
set(gcf, 'Position', [1000 + offset, 100 + offset, scale*500, scale*400])





% Relative complexity plots

const_func = @(x) 1;
linear_func = @(x) x;
nlogn_func = @(x) x.*log2(x);
quad_func = @(x) x.^2;









% Calculate averages for specific test nr.
function avg_vec = find_average(data_matrix, test_nr)
    %function contents --
    data = zeros(size(data_matrix,1)/5, 2);

    % single out specific test data 
    index = 1;
    for i = 1:size(data_matrix,1)
        if data_matrix(i,1) == test_nr
            data(index,:) = data_matrix(i,2:3);
            index = index + 1;
        end
    end

    % calculate average
    index = 1; 
    i = 1;
    tmp = length(unique(data(:,1)));
    avg_vec = zeros(tmp,2);
    
    while (index <= size(data,1)) 
        value = data(index, 1);
        start_ind = index;
        end_ind = index;

        while (end_ind ~= size(data,1) && value == data(end_ind+1, 1))
            end_ind = end_ind + 1; 
        end

        avg_vec(i,1) = value;
        avg_vec(i,2) = mean(data(start_ind:end_ind,2));
        i = i + 1;
        index = end_ind + 1;
    end
end
