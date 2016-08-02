global s
time_scale=5;
vec=[];
pveco=[];
thfilt=[];
theta1=[];
envec=[];
xvec=[];
yvec=[];
zvec=[];
adcrossu=[1];
adcrossd=[1];
envdcrossu=[];
envdcrossd=[];
au_st_offset=7;
au_ed_offset=3;
NF_pct=0.95;
accel_dcross_flag=0;
au_thresh_num=15;
au_thresh=zeros(1,au_thresh_num);
c_thresh=1;
lenval=500;
b1=[0.015466   0.015466];
a1=[1 -0.96907];
b1vec=[0 0];
a1vec=[0 0];
disnum=100;
 adtcrossu=[];
tempthresh=zeros(1,1);
while(1)
    vec=[vec char(send_serial('x',1,s))];
    idx=find(vec=='a');
    idx2=find((vec(idx(1):end))=='B');
    idxx=find(vec(idx(1):end)=='x');
    idxy=find(vec(idx(1):end)=='y');
    idxz=find(vec(idx(1):end)=='z');
    for k=1:length(idxx)-1        %Convert all of the data into numbers for the buffered data
        xt=[str2num(vec(idxx(k)+2:idxx(k)+7))];
    end
    for k=1:length(idxy)-1
        yt=[ str2num(vec(idxy(k)+2:idxy(k)+7))];
    end
    for k=1:length(idxz)-1
        zt=[ str2num(vec(idxz(k)+2:idxz(k)+7))];
    end
    for k=1:length(idx2)-1;
        ent=[ str2num(vec(idx(k)+2:idx(k)+7))];
    end
    vec=vec(idx(end):end);
    for k=1:length(zt)
        tmat=[xt(k) yt(k) zt(k)];
        thetat(k)=-acosd(sum(tmat.*[0 0.7071 0.7071])./(norm(tmat)));
%        theta2(k)=acosd(sum(tmat.*[0.7071 0 -0.7071])./(norm(tmat)));
%        theta3(k)=acosd(sum(tmat.*[-0.7071 0 0.7071])./(norm(tmat)));
    end
  [thfilltt b1vec a1vec]=my_iir(thetat,b1vec,a1vec,b1,a1,1);  
  
  
  
    pveco=[pveco ent];
    sp=max([1 length(pveco)-disnum]);
    if length(theta1)>0     % Find crossings of motion threshold
      adtcrossu=find_crossings([theta1(end) thetat],[thfilt(end) thfilltt],0)+length(pveco)-length(ent);
      adtcrossd=find_crossings([theta1(end) thetat],[thfilt(end) thfilltt],1)+length(pveco)-length(ent);  
      adcrossu=[adcrossu adtcrossu]-sp+1;
      adcrossd=[adcrossd adtcrossd]-sp+1;
      idt=find(adcrossu>0);
      adcrossu=adcrossu(idt);
      idt=find(adcrossd>0);
      adcrossd=adcrossd(idt);
    end

   
    pveco=pveco(sp:end);
    theta1=[theta1 thetat];
    theta1=theta1(sp:end);
    thfilt=[thfilt thfilltt];
    thfilt=thfilt(sp:end);
    
    %    au_st_offset=7;
%au_ed_offset=3
    if length(adtcrossu)   %update the noise floor
      accel_dcross_flag=1;      
    end
     adtcrossu=[];
    if accel_dcross_flag
%      accel_dcross_flag=accel_dcross_flag+1;
      if length(pveco)-adcrossu(end)>=au_ed_offset
        accel_dcross_flag=0;
        if length(adcrossu)>1
          au_thresh(1:end-1)=au_thresh(2:end);
          dmean=mean(pveco(adcrossu(end-1)+au_ed_offset:max([ adcrossu(end-1)+au_ed_offset adcrossd(end)-au_st_offset])));
          umean=mean(pveco(max([1 adcrossd(end)-au_st_offset]):adcrossu(end)+au_ed_offset));
          au_thresh(end)=NF_pct*dmean+(1-NF_pct)*umean;
          idx=find(au_thresh>0);
        c_thresh=mean(au_thresh(idx));
        end
      end
    end
    if length(pveco)>0     % Find crossings of audio threshold
      envdtcrossu=find_crossings([pveco(max([1 end-length(ent)]):end)],ones(1,length(ent)+1)*c_thresh,0)+length(pveco)-length(ent);
      envdtcrossd=find_crossings([pveco(max([1 end-length(ent)]):end)],ones(1,length(ent)+1)*c_thresh,1)+length(pveco)-length(ent);  
      envdcrossu=[envdcrossu envdtcrossu]-sp+1;
      envdcrossd=[envdcrossd envdtcrossd]-sp+1;
      idt=find(envdcrossu>0);
      envdcrossu=envdcrossu(idt);
      idt=find(envdcrossd>0);
      envdcrossd=envdcrossd(idt);
    end
    tempthresh=ones(1,length(pveco))*c_thresh;
    
    
    subplot(2,1,1)
    hold off
    plot((1:length(pveco))/time_scale,pveco)
    hold on
    plot((1:length(tempthresh))/time_scale,tempthresh,'r')
    plot(envdcrossd/time_scale,pveco(envdcrossd),'gx')
    plot(envdcrossu/time_scale,pveco(envdcrossu),'rx')
%     plot(adcrossd/time_scale,pveco(adcrossd),'kx')
%    plot(adcrossu/time_scale,pveco(adcrossu),'yx')
    ylabel('Audio Amplitude')
    subplot(2,1,2)
    hold off
    plot((1:length(theta1))/time_scale,theta1)
    hold on
    plot((1:length(thfilt))/time_scale,thfilt,'r')
    plot(adcrossd/time_scale,theta1(adcrossd),'gx')
    plot(adcrossu/time_scale,theta1(adcrossu),'rx')
    xlabel('Time in seconds')
    ylabel('Motion angle in degrees')
    
%     subplot(4,1,3)
%     plot(theta2-mean(theta2))
%     subplot(4,1,4)
%     plot(theta3-mean(theta3))
    drawnow
    %  subplot(2,1,1)
    %     plot(pveco)
    %     subplot(2,1,2)
    %     plot(theta)
    
    pause(0.01)
end