x=0:25:300;
y=230:10:330;
figure;
plot([25,50,75,100,125,150,175,200,225,250,275],[246.031,247.098,249.103,248.113,251.155,...
    253.164,251.205,253.234,255.267,253.302,256.318],'-p','Color','r'); hold on;
plot([25,50,75,100,125,150,175,200,225,250,275],[256.197,259.225,258.206,261.185,259.215,...
    262.247,263.284,264.509,265.232,267.235,265.148],'-h','Color','b'); hold on;
plot([25,50,75,100,125,150,175,200,225,250,275],[266.489,267.162,268.101,267.085,271.062,...
    269.073,273.131,273.144,275.166,274.167,277.173],'-^','Color','g'); hold on;
plot([25,50,75,100,125,150,175,200,225,250,275],[274.578,277.971,276.561,278.919,280.618,...
    282.781,280.817,284.837,281.856,284.885,285.859],'-v','Color','k'); hold on;
plot([25,50,75,100,125,150,175,200,225,250,275],[284.907,285.863,289.842,288.859,290.011,...
    291.049,291.081,292.097,292.114,295.135,293.151],'-d','Color','c'); hold on;
plot([25,50,75,100,125,150,175,200,225,250,275],[298.143,296.971,298.957,301.943,300.015,...
    302.051,300.082,303.124,301.149,304.161,305.127],'-*','Color','m'); hold off;
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
end;
grid minor

