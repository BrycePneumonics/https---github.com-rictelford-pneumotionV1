global s
vec=[];
xvec=[];
yvec=[];
zvec=[];
while(1)
    vec=[vec char(send_serial('x',1,s))];
    idxx=find(vec=='x');
    idxy=find(vec=='y');
    idxz=find(vec=='z');
    for k=1:length(idxx)-1
       xvec=[xvec str2num(vec(idxx(k)+2:idxx(k)+7))]; 
    end
    for k=1:length(idxy)-1
       yvec=[yvec str2num(vec(idxy(k)+2:idxy(k)+7))]; 
    end
     for k=1:length(idxz)-1
       zvec=[zvec str2num(vec(idxz(k)+2:idxz(k)+7))]; 
    end
    vec=vec(min([idxx(end) idxy(end) idxz(end)]):end);
    if length(xvec)>600;
        xveco=xvec(end-floor(600):end);
    else
        xveco=xvec;
    end
    if length(yvec)>600;
        yveco=yvec(end-floor(600):end);
    else
        yveco=yvec;
    end
    if length(zvec)>600;
        zveco=zvec(end-floor(600):end);
    else
        zveco=zvec;
    end
    l=min([length(xveco) length(yveco) length(zveco)]);
    mat=[xveco(1:l); yveco(1:l); zveco(1:l)];
    bv=mean(mat,2);
    for k=1:length(zveco)
%         anglevec(k)=sum(mat(:,k).*bv.*[1 -1 -1]')/(sqrt(sum(mat(:,k).^2))*sqrt(sum(bv.^2)));
    end
    figure(1)
    subplot(3,1,1)
    hold off
    plot(xveco)
    hold on
    subplot(3,1,2)
    plot(yveco,'g')
    subplot(3,1,3)
    plot(zveco,'r')
    h=axis;
%     axis([h(1:2) -34100 34100])
    drawnow
%     subplot(2,1,2)
%     plot(asind(anglevec));
    drawnow
end