global s
vec=[];
pvec=[]
envec=[];
while(1)
    vec=[vec char(send_serial('x',1,0))];
    idx=find(vec=='a');
    idx2=find((vec(idx(1):end))=='B');
    for k=1:length(idx)-1
%        pvec=[pvec str2num(vec(idx(k)+2:idx(k)+7))*2^16+str2num(vec(idx(1)+idx2(k)+1:idx(1)+idx2(k)+6))]; 
        pvec=[pvec str2num(vec(idx(k)+2:idx(k)+7))];
    end
    for k=1:length(idx2)-1;
        envec=[envec str2num(vec(idx2(k)+2:idx2(k)+7))];
    end
    vec=vec(idx(end):end);
    if length(pvec)>600;
        pveco=pvec(end-floor(600):end);
    else
        pveco=pvec;
    end
    if length(envec)>600
        enveco=envec(end-floor(600):end);
    else
        enveco=envec;
    end
    subplot(2,1,1)
    plot(pveco)
    subplot(2,1,2)
    plot(enveco)
    drawnow
    pause(0.05)
end