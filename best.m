x=0:25:300;
y=230:10:320;
figure;
plot([25,50,75,100,125,150,175,200,225,250,275],[239.031,241.098,242.103,242.113,244.155,...
    245.164,244.205,247.234,248.267,247.302,249.318],'-p','Color','r'); hold on;
plot([25,50,75,100,125,150,175,200,225,250,275],[250.197,252.225,252.206,254.185,253.215,...
    255.247,257.284,257.509,259.232,260.235,259.148],'-h','Color','b'); hold on;
plot([25,50,75,100,125,150,175,200,225,250,275],[259.489,260.162,261.101,261.085,264.062,...
    263.073,266.131,267.144,268.166,268.167,270.173],'-^','Color','g'); hold on;
plot([25,50,75,100,125,150,175,200,225,250,275],[268.578,270.971,270.561,271.919,274.618,...
    273.781,274.817,277.837,275.856,277.885,278.859],'-v','Color','k'); hold on;
plot([25,50,75,100,125,150,175,200,225,250,275],[277.907,279.863,283.842,282.859,283.011,...
    285.049,285.081,286.097,285.114,288.135,286.151],'-d','Color','c'); hold on;
plot([25,50,75,100,125,150,175,200,225,250,275],[289.971,291.143,292.957,294.943,293.015,...
    295.051,294.082,296.124,295.149,297.161,298.127],'-*','Color','m'); hold off;
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

