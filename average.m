x=0:25:300;
y=230:10:330;
figure;
plot([25,50,75,100,125,150,175,200,225,250,275],[243.031,244.098,246.103,...
    244.113,249.155,248.164,249.205,251.234,253.267,252.302,253.318],'-p','Color','r'); hold on;
plot([25,50,75,100,125,150,175,200,225,250,275],[254.197,257.225,256.206,...
    259.185,257.215,259.247,262.284,261.509,264.232,264.235,265.148],'-h','Color','b'); hold on;
plot([25,50,75,100,125,150,175,200,225,250,275],[264.489,264.162,265.101,...
    264.085,269.062,267.073,271.131,271.144,272.166,273.167,274.173],'-^','Color','g'); hold on;
plot([25,50,75,100,125,150,175,200,225,250,275],[272.578,276.971,274.561,...
    276.919,278.618,276.781,278.817,282.837,279.856,282.885,282.859],'-v','Color','k'); hold on;
plot([25,50,75,100,125,150,175,200,225,250,275],[282.907,285.863,287.842,...
    286.859,289.011,291.049,290.081,290.097,289.114,293.135,290.151],'-d','Color','c'); hold on;
plot([25,50,75,100,125,150,175,200,225,250,275],[293.971,296.143,297.957,...
    299.943,297.015,300.051,302.082,301.124,299.149,302.161,302.127],'-*','Color','m'); hold off;
xlabel('Density(N)')
ylabel('Message Authentication Delay(ms)')
legend('S=15m/s','S=16m/s','S=17m/s','S=18m/s','S=19m/s','S=20m/s');
%%
width = 1500;     % Width in inches
% height = 4;    % Height in inches
alw = 1.5;    % AxesLineWidth
fsz = 12;      % Fontsize
lw = 2;      % LineWidth
msz = 9;       % MarkerSize
figHandles = get(0,'Children');
for i=1:size(figHandles, 1)
    gcf = figure(i);
    pos = get(gcf, 'Position');
    set(gca, 'FontSize', fsz, 'LineWidth', alw, 'XMinorTick', 'off',...
        'YMinorTick', 'on', 'Box', 'off'); %<- Set properties set(gca, 'xdir','reverse');
    dp = findobj(get(gca, 'Children'), 'Type', 'Line');
    dp.set('LineWidth', lw);
    dp.set('MarkerSize', msz);
    dp = findobj(dp, 'Marker', '.');
    dp.set('MarkerSize', msz*2);
    grid('on');
    lh = legend(gca, 'show');
    lh.set('Orientation', 'Horizontal', 'Location', 'north');
    lh.set('Position',... %[0.213392863103321 0.940873016866427 0.608928559507643 0.0440476180542083]);
        [0.235714289945151 0.861111112281917 0.560714277252555 0.0559523796041806]);
end
grid minor