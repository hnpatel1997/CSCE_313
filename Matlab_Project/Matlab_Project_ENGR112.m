% By submitting this assignment, I agree to the following:
% “Aggies do not lie, cheat, or steal, or tolerate those who do”
% “I have not given or received any unauthorized aid on this assignment”
%
% Name: Hard Patel
% Class: ENGR 112
% Section: 521
% Team: 5
% Assignment: Matlab Project
% Date: 04/24/17

clear;
clc;
username = [];
output_filename = [];
q = [];
project1 = true;
while project1 % runs while project1 is true
    project = menu('What do you want to do?','Set username','Load data file','Clear data from memory','Set output filename','Plot histogram','Plot histogram fit','Plot probability plots','Regression of y on x','Find probability given x or z','Find x or z given probability','Exit');
    % makes the menu
    clc;
    
    switch project % each case is a tab from the menu
        case 1
            if isempty(username) == 1 % If username doesn't already exists, one can set it.
                username = input('Set Username:','s');
                clc;
                fprintf('Username: %s\n\n',username);
            else
                fprintf('Do you want to change user name?\nEnter\na: Yes\nb: No\n\n');
                question = input('Enter a or b:','s');
                clc;
                if question == 'a' % If username already exists, one can choose to keep it or change it.
                    username = input('Set Username:','s');
                    clc;
                    fprintf('Username: %s\n\n',username);
                elseif question == 'b'
                    fprintf('Username remains %s.\n\n',username);
                end
            end
            fprintf('Press any key to go back to the menu.\n'); % takes you back to the menu.
            pause;
        
        case 2
            if isempty(q) == 1
                filename = input('Enter filename:','s');
                q = importdata(filename); % imports the file that is asked for.
                fprintf('Press any key to go back to the menu.\n'); % takes you back to the menu.
                pause;
            else
                clc;
                fprintf('An input file already exists, would you like to upload and replace it?\na: Yes\nb: No\n\n');
                t = input('Enter a or b: ','s');
                f = sprintf('%s',filename);
                if t == 'a'
                    clc;
                    filename = input('Enter filename:','s');
                    q = importdata(filename); % imports the file that is asked for.
%                     fprintf('Please go back to the menu and clear input file from memory by selecting the ''Clear data from memory''\n');
                    fprintf('Press any key to go back to the menu.\n'); % takes you back to the menu.
                    pause;
                elseif t == 'b'
                    clc;
                    fprintf('The input file remains %s\n\n',t);
                    fprintf('Press any key to go back to the menu.\n'); % takes you back to the menu.
                    pause;
                else
                    clc;
                    fprintf('Input invalid, enter\na:Yes\nb:No\n')
                end
            end
        
        case 3
            clc;
            fprintf('Are you sure you want to clear all memory?\n\n');
            fprintf('a: Yes\nb: No\n\n');
            x = input('Enter a or b:','s');
            if x == 'a'
                clc;
                clear;
                username = [];
                output_filename = [];
                q = [];
                project1 = true;
                fprintf('All memory is now cleared.\n\n')
                fprintf('Press any key to go back to the menu.\n'); % takes you back to the menu.
                pause;
            elseif x == 'b'
                clc;
                fprintf('No memory was cleared.\n\n');
                fprintf('Press any key to go back to the menu.\n'); % takes you back to the menu.
                pause;
            end
            
        case 4
            if isempty(q) == 0
                x1 = q(:,1); % measures the number of rows.
                    s = size(x1);
                    Mean = mean(x1);
                    Median = median(x1);
                    Mode = mode(x1);
                    Var = var(x1);
                    Stdev = std(x1);
                    Min = min(x1);
                    Max = max(x1);
                    Count = size(x1);
                x2 = q(:,2); % measures the number of rows.
                    s1 = size(x2);
                    Mean1 = mean(x2);
                    Median1 = median(x2);
                    Mode1 = mode(x2);
                    Var1 = var(x2);
                    Stdev1 = std(x2);
                    Min1 = min(x2);
                    Max1 = max(x2);
                    Count1 = size(x2);
                    if isempty(output_filename) == 1
                        output_filename = input('Enter output filename:','s');
                        out_file = fopen(output_filename,'w');
                        %fprintf(out_file,'Date\t: %s \nUsername\t: %s',date, username);
                        f = sprintf('%s',filename);
                        fprintf(out_file,'Date\t\t: %s \nUsername\t: %s \nFilename\t: %s \n\nStatistics\nMean\t = %8.2f\t%8.2f \nMedian\t = %8.2f\t%8.2f \nMode\t = %8.2f\t%8.2f \nVar\t\t = %8.2f\t%8.2f \nStdev\t = %8.2f\t%8.2f \nMin\t\t = %8.2f\t%8.2f \nMax\t\t = %8.2f\t%8.2f \nCount\t = \t\t%.0f\t\t%.0f\n',date,username,f,Mean,Mean1,Median,Median1,Mode,Mode1,Var,Var1,Stdev,Stdev1,Min,Min1,Max,Max1,Count,Count1);
                        fclose(out_file);
                        l = sprintf('%s',output_filename);
                        fprintf('The data is now stored in the file ''%s''\n\n',l);
                        fprintf('Press any key to go back to the menu.\n'); % takes you back to the menu.
                        pause;
                    else
                        clc;
                        fprintf('Error - An data already exists in the output file.\nIf you would like to input another file please clear current input file from memory.\n/n');
                        fprintf('Press any key to go back to the menu.\n'); % takes you back to the menu.
                        pause;
                    end
            else
                clc;
                fprintf('No input file could be found.\nPlease insert an input file by selecting ''Load data file'' from the menu\n');
                fprintf('Press any key to go back to the menu.\n'); % takes you back to the menu.
                pause;
            end
            
        case 5
            if isempty(q) == 0
                s1 = size(q(1,:),2); % measures the number of colomns.
                for i = 1:s1
                    x1 = q(:,i); % measures the number of rows.
                    s = size(x1);
                    dataset=x1(:,1);
                    subplot(1,s1,i);
                    histogram(dataset,10); % graphs the histogram.
                end
                clc;
                fprintf('Press any key to go back to the menu.\n'); % takes you back to the menu.
                pause;
            else
                clc;
                fprintf('No input file could be found.\nPlease insert an input file by selecting ''Load data file'' from the menu\n');
                fprintf('Press any key to go back to the menu.\n'); % takes you back to the menu.
                pause;
            end
            
        case 6
            if isempty(q) == 0
                s1 = size(q(1,:),2); % measures the number of colomns.
                for i = 1:s1
                    x1 = q(:,i); % measures the number of rows.
                    s = size(x1);
                    dataset=x1(:,1);
                    subplot(1,s1,i);
                    histfit(dataset,10);
                end
                clc;
                fprintf('Press any key to go back to the menu.\n'); % takes you back to the menu.
                pause;
            else
                clc;
                fprintf('No input file could be found.\nPlease insert an input file by selecting ''Load data file'' from the menu\n');
                fprintf('Press any key to go back to the menu.\n'); % takes you back to the menu.
                pause;
            end
            
        case 7
            if isempty(q) == 0
                s1 = size(q(1,:),2); % measures the number of colomns.
                for i = 1:s1
                    x1 = q(:,i); % measures the number of rows.
                    s = size(x1);
                    dataset=x1(:,1);
                    subplot(1,s1,i);
                    normplot(dataset);
                end
                clc;
                fprintf('Press any key to go back to the menu.\n'); % takes you back to the menu.
                pause;
            else
                clc;
                fprintf('No input file could be found.\nPlease insert an input file by selecting ''Load data file'' from the menu\n');
                fprintf('Press any key to go back to the menu.\n'); % takes you back to the menu.
                pause;
            end

        case 8
            if isempty(q) == 0
                s = size(q(1,:),2);
                if s == 3
                    x = q(:,1);
                    y = q(:,3);
                    clc;
                    fprintf('What order of regression would you like to use?\n\na:Linear\nb:Quadratic\nc:Cubic\n\n');
                    l = input('Select a,b or c:','s');
                    if l == 'a'
                        m = 1;
                    elseif l == 'b'
                        m = 2;
                    elseif l == 'c'
                        m = 3;
                    end
                    t = polyfit(x,y,m);

                    if l == 'a'
                        f = t(1)*x + t(2);
                        plot(x,y,'rd',x,f,'k--');
                    elseif l == 'b'
                        f = t(1)*x.^2 + t(2)*x + t(3);
                        plot(x,y,'rd',x,f,'k--');
                    elseif l == 'c'
                        f = t(1)*x.^3 + t(2)*x.^2 + t(3)*x + t(4);
                        plot(x,y,'rd',x,f,'k--');
                    end
                    clc;
                    fprintf('Press any key to go back to the menu.\n'); % takes you back to the menu.
                    pause;
                else
                    clc;
                    fprintf('Invalid number of columns.\nPlease input a file with two columns.\n')
                    fprintf('Press any key to go back to the menu.\n'); % takes you back to the menu.
                    pause;
                end
            else
                clc;
                fprintf('No input file found.\nPlease enter input a file by selecting the ''Load data file'' from the menu.\n');
                fprintf('Press any key to go back to the menu.\n'); % takes you back to the menu.
                pause;
            end
            
        case 9
            if isempty(q) == 0
                clc;
                fprintf('What value are you inputing?\n\na: x-value\nb: z-value\n\n');
                t = input('Enter a or b:','s');
                if t == 'a'
                    z = input('Enter z-value:');
                    if ~isnumeric(z)
                        clc;
                        fprintf('z-value invalid\n');
                        z = input('Enter z-value:');
                    end
                    clc;
                    p = normcdf(z,0,1);
                    fprintf('Probablity for the given z-value is %.2f\n\n',p);
                    fprintf('Press any key to go back to the menu.\n'); % takes you back to the menu.
                    pause;
                elseif t == 'b'
                    clc;
                    x = input('Enter x-value:');
                    if ~isnumeric(x)
                        fprintf('x-value invalid\n');
                        x = input('Enter x-value:');
                    end
                    clc;
                    fprintf('Which column would you like to use for this process?\n Enter the column number that you would like to find the probability of.');
                    t = input('Enter the column:');
                    if isnumeric(t) == 0
                        fprintf('Please enter a numeric value');
                        t = input('Enter the column:');
                    end
                    l = q(:,t);
                    M = mean(l);
                    S = stdev(l);
                    p = normcdf(x,M,S);
                    fprintf('Probablity for the given x-value is %.2f\n\n',p);
                    fprintf('Press any key to go back to the menu.\n'); % takes you back to the menu.
                    pause;
                end
            else
                clc;
                fprintf('No input file found.\nPlease enter input value by selecting the ''Load data file'' from the menu.\n');
                fprintf('Press any key to go back to the menu.\n'); % takes you back to the menu.
                pause;
            end

        case 10
            if isempty(q) == 0
                clc;
                i = input('Enter your desired probability:');
                clc;
                fprintf('Is the value entered a decimal value or a percent value?\n\na: decimal value \nb: percent value\n\n');
                j = input('Enter a or b:','s');
                if j == 'b'
                    i = i/100;
                end
                if or(i>1,i<0)
                    clc;
                    fprintf('Probablility must be between 0 and 1 for decimal value or between 0 and 100 for percent value.\n');
                end
                clc;
                fprintf('What are you looking for?\n\na: x-value\nb: z-value\n\n');
                t = input('Enter a or b:','s');
                if t == 'a'
                    clc;
                    fprintf('Which column would you like to use for this process?\n Enter the column number that you would like to find the x or z value of.');
                    t = input('Enter the column:');
                    if isnumeric(t) == 0
                        fprintf('Please enter a numeric value');
                        t = input('Enter the column:');
                    end
                    l = q(:,t);
                    M = mean(l);
                    S = stdev(l);
                    x = norminv(i,M,S);
                    fprintf('The x-value that can give the probability %.2f is %.2f\n\n',i,x);
                    fprintf('Press any key to go back to the menu.\n'); % takes you back to the menu.
                    pause;
                elseif t =='b'
                    clc;
                    z = norminv(i,0,1);
                    fprintf('The z-value that can give the probability %.2f is %.2f\n\n',i,z);
                    fprintf('Press any key to go back to the menu.\n'); % takes you back to the menu.
                    pause;
                end
            else
                clc;
                fprintf('No input file found.\nPlease enter input value by selecting the ''Load data file'' from the menu.\n');
                fprintf('Press any key to go back to the menu.\n'); % takes you back to the menu.
                pause;
            end
        case 11
            project1 = false; % if this case is selected, the while loop will end and the menu won't show.
            fprintf('Goodbye!\n');
        otherwise % if uses closes the menu this will take user back to the menu.
            project1 = false; % if this case is selected, the while loop will end and the menu won't show.
            fprintf('Goodbye!\n');
    end
end
clear;