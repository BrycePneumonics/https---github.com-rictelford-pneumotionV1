global s
vec=[];
pvec=[]
envec=[];
xvec=[];
yvec=[];
zvec=[];
lenval=500;
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
    if length(xvec)>lenval;
        xveco=xvec(end-floor(lenval):end);
    else
        xveco=xvec;
    end
    if length(yvec)>lenval;
        yveco=yvec(end-floor(lenval):end);
    else
        yveco=yvec;
    end
    if length(zvec)>lenval;
        zveco=zvec(end-floor(lenval):end);
    else
        zveco=zvec;
    end
    %      meanval=[mean(xveco) mean(yveco) mean(zveco)];
    for k=1:length(idx)-1
        %        pvec=[pvec str2num(vec(idx(k)+2:idx(k)+7))*2^16+str2num(vec(idx(1)+idx2(k)+1:idx(1)+idx2(k)+6))];
        pvec=[pvec str2num(vec(idx(k)+2:idx(k)+7))];
    end
    %     theta=return2planar(xveco',yveco',zveco');
    
    for k=1:length(idx2)-1;
        envec=[envec str2num(vec(idx2(k)+2:idx2(k)+7))];
    end
    vec=vec(idx(end):end);
    if length(pvec)>lenval;
        pveco=pvec(end-floor(lenval):end);
    else
        pveco=pvec;
    end
    if length(envec)>lenval
        enveco=envec(end-floor(lenval):end);
    else
        enveco=envec;
    end
    for k=1:length(zveco)
        tmat=[xveco(k) yveco(k) zveco(k)];
        theta1(k)=-acosd(sum(tmat.*[0 0.7071 0.7071])./(norm(tmat)));
%        theta2(k)=acosd(sum(tmat.*[0.7071 0 -0.7071])./(norm(tmat)));
%        theta3(k)=acosd(sum(tmat.*[-0.7071 0 0.7071])./(norm(tmat)));
    end
    thfilt=filter([0.015466   0.015466],[1 -0.96907],theta1);
    subplot(2,1,1)
    plot(pveco(100:end))
    subplot(2,1,2)
    hold off
    plot(theta1(100:end))
    hold on
    plot(thfilt(100:end),'r')
    
    
    
%     subplot(4,1,3)
%     plot(theta2-mean(theta2))
%     subplot(4,1,4)
%     plot(theta3-mean(theta3))
    drawnow
    %  subplot(2,1,1)
    %     plot(pveco)
    %     subplot(2,1,2)
    %     plot(theta)
    
    drawnow
    pause(0.01)
end