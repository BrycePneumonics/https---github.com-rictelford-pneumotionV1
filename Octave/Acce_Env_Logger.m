global s
vec=[];
pvec=[]
envec=[];
xvec=[];
yvec=[];
zvec=[];
while(1)
    vec=[vec char(send_serial('x',1,s))];
    idx=find(vec=='a');
    idx2=find((vec(idx(1):end))=='B');
     idxx=find(vec(idx(1):end)=='x');
    idxy=find(vec(idx(1):end)=='y');
    idxz=find(vec(idx(1):end)=='z');
    for k=1:length(idxx)-1
       xvec=[xvec str2num(vec(idxx(k)+2:idxx(k)+7))]; 
    end
    for k=1:length(idxy)-1
       yvec=[yvec str2num(vec(idxy(k)+2:idxy(k)+7))]; 
    end
     for k=1:length(idxz)-1
       zvec=[zvec str2num(vec(idxz(k)+2:idxz(k)+7))]; 
    end
%     vec=vec(min([idxx(end) idxy(end) idxz(end)]):end);
    if length(xvec)>600;
%         xveco=xvec(end-floor(600):end);
    else
        xveco=xvec;
    end
    if length(yvec)>600;
%         yveco=yvec(end-floor(600):end);
    else
        yveco=yvec;
    end
    if length(zvec)>600;
%         zveco=zvec(end-floor(600):end);
    else
        zveco=zvec;
    end
    for k=1:length(idx)-1
%        pvec=[pvec str2num(vec(idx(k)+2:idx(k)+7))*2^16+str2num(vec(idx(1)+idx2(k)+1:idx(1)+idx2(k)+6))]; 
        pvec=[pvec str2num(vec(idx(k)+2:idx(k)+7))];
    end
    for k=1:length(idx2)-1;
        envec=[envec str2num(vec(idx2(k)+2:idx2(k)+7))];
    end
    vec=vec(idx(end):end);
    if length(pvec)>600;
%         pveco=pvec(end-floor(600):end);
    else
        pveco=pvec;
    end
    if length(envec)>600
        enveco=envec(end-floor(600):end);
    else
        enveco=envec;
    end
%     subplot(4,1,1)
%     plot(pveco)
%     subplot(4,1,2)
%     plot(xveco)
%      subplot(4,1,3)
%     plot(yveco)
%      subplot(4,1,4)
%     plot(zveco)
%     drawnow
    pause(0.01)
end