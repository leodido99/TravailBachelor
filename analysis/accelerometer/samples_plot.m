%Plot accelerometer samples


T = readtable('samples.xlsx','ReadRowNames',true);





time = T.time;
x_accel = T.x;
y_accel = T.y;
z_accel = T.z;

figure1 = figure;
axes1 = axes('Parent',figure1)
hold(axes1,'all');

a1 = plot(time, x_accel,'Color',[1,0,0]); M1 = 'Axe x';
xlabel('temps[ms]')
grid on
hold on
a2 = plot(time, y_accel,'Color',[0,1,0]); M2 = 'Axe y';
hold on
a3 = plot(time, z_accel,'Color',[0,0,1]); M3 = 'Axe z';
hold off
title('Analyse des echantillons');
legend([a1;a2;a3], [M1; M2; M3]);

saveas(figure1,'/Users/leonard.bise/gitrepo/TravailBachelor/analysis/accelerometer/accelerometer_samples.png') 