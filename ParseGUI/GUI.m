


data = fopen("U:\Documents\Parseputty.txt");
ndata = {'0','0','0','0'};
while(~feof(data))
    line = fgets(data);
    x = strsplit(line,',');
    ndata = [ndata;x]
    i = i + 1;
end
ndata = str2double(ndata);

clf("reset");    
for i = 1:length(ndata)-1
    theta = ndata(i,2) * pi/180;
    rhol = ndata(i,3);
    x = rhol * cos(theta);
    y = rhol * sin(theta);
    r= ndata(i,4) / 2;
    circle(x,y,r)
end
k = k + k;







