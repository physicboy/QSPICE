d = csvread('noise.csv');
hist(d(2:1000002,2),25,'b');
hold on
hist(d(2:1000002,3),25,'r');
legend('mean = 0, stdev = 1','mean = 10, stdev = 1','location','southoutside','orientation','horizontal')
xlim([-5, 20])
