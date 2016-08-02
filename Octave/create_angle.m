for k=1:length(zvec)
        tmat=[xvec(k) yvec(k) zvec(k)];
        theta1(k)=-acosd(sum(tmat.*[0 0.7071 0.7071])./(norm(tmat)));
%        theta2(k)=acosd(sum(tmat.*[0.7071 0 -0.7071])./(norm(tmat)));
%        theta3(k)=acosd(sum(tmat.*[-0.7071 0 0.7071])./(norm(tmat)));
    end
    [b a]=butter(2,0.01);
    thfilt=filter(b,a,theta1);
    rangest=7000;
    rangeed=length(theta1);
    idx1=find(thfilt(rangest:rangeed-1)>theta1(rangest:rangeed-1));
    idx2=find(thfilt(rangest+idx1)<=theta1(rangest+idx1));
    idx3=find(thfilt(rangest:rangeed-1)<theta1(rangest:rangeed-1));
    idx4=find(thfilt(rangest+idx3)>=theta1(rangest+idx3));
    bthresh=30;
    counter=1;
    moffset=0;
    if idx2(idx1(1))>idx4(idx3(1))
      moffset=1
      idx4t(1)=idx4(1);
    end
    for k=1:length(idx2)-moffset-1   %Check for minimum time threshold
      if abs(idx1(idx2(k+1))-idx1(idx2(k)))>bthresh
        idx2t(counter)=idx2(k);
        idx4t(counter+moffset)=idx4(k+moffset);
        counter=counter+1;        
      end
    end
    idx2=idx2t;
    idx4=idx4t;
     moffset=0;
    if idx2(idx1(1))>idx4(idx3(1))
      moffset=1
      idx4t(1)=idx4(1);
    end
    counter=1;
    athresh=0.1;
    for k=1:length(idx2)-moffset-1      %Check or minimum movement threshold
      if (max(theta1(idx1(idx2(k)):idx3(idx4(k+moffset))))-min(idx1(idx2(k)):idx3(idx4(k+moffset))))>athresh
        idx2t(counter)=idx2(k);
        idx4t(counter+moffset)=idx4(k+moffset);
        counter=counter+1;        
      end
    end
    idx2=idx2t;
    idx4=idx4t;
    moffset=0;
    d_offset=3;
    a_offset=7;
    if idx2(idx1(1))<idx4(idx3(1))
      moffset=1
    end
    flen=20;
    NFpct=0.95;
    tvecm=ones(1,flen);
 
    for k=1:length(idx4)-moffset-1
      dir_sig=1;
      if((idx3(idx4(k))+rangest+d_offset)>(idx1(idx2(k+moffset))+rangest-a_offset))
          dir_sig=-1;
      end
      kvec(k)=mean(envec((idx1(idx2(k+moffset))+rangest-a_offset):(idx3(idx4(k+1))+rangest+d_offset)));
      mvec(k)=mean(envec((idx3(idx4(k))+rangest+d_offset):dir_sig:(idx1(idx2(k+moffset))+rangest-a_offset)));
      
      if k==1
        tvecm=tvecm*mvec(k)*NFpct+(1-NFpct)*kvec(k);
           tvec(1:idx1(idx2(1))+rangest)=tvecm(1);
      end
      tvecm(1:flen-1)=tvecm(2:flen);
      tvecm(flen)=mvec(k)*NFpct+(1-NFpct)*kvec(k);
      tvec(idx1(idx2(k))+rangest:idx1(idx2(k+1))+rangest)=mean(tvecm);
    end
    tvec(idx1(idx2(k+1))+rangest:length(envec))=mean(tvecm);
    idxa1=find(tvec(rangest:rangeed-1)>envec(rangest:rangeed-1));
    idxa2=find(tvec(rangest+idxa1)<=envec(rangest+idxa1));
    idxa3=find(tvec(rangest:rangeed-1)<envec(rangest:rangeed-1));
    idxa4=find(tvec(rangest+idxa3)>=envec(rangest+idxa3));
     moffset=0;
     counter=1;
    if idxa2(idxa1(1))>idxa4(idxa3(1))
      moffset=1
      idxa4t(1)=idxa4(1);
    end
     for k=1:length(idxa2)-moffset-1   %Check for minimum time threshold
      if abs(idxa1(idxa2(k+1))-idxa1(idxa2(k)))>bthresh/2
        idx2t(counter)=idxa2(k);
        idx4t(counter+moffset)=idxa4(k+moffset);
        counter=counter+1;        
      end
    end
    idxa2=idx2t;
    idxa4=idx4t;
    
    close all
    figure
    
    subplot(2,1,1)
    plot(rangest:rangeed,envec(rangest:rangeed));
    hold on
    h=axis;
    
    plot(idxa3(idxa4)+rangest,envec(idxa3(idxa4)+rangest),'gx')
    plot(idxa1(idxa2)+rangest,envec(idxa1(idxa2)+rangest),'kx')
%    plot(idxa3(idxa4)+rangest+d_offset,tvec(idxa3(idxa4)+rangest+d_offset),'gx')
%    plot(idxa1(idxa2)+rangest-a_offset,tvec(idxa1(idxa2)+rangest-a_offset),'kx')
    
%    plot((1:length(tvec))+rangest,tvec,'r')
    plot(idx1(idx2)+rangest-a_offset,tvec(idx1(idx2)+rangest-a_offset),'r')
    axis(h)
    subplot(2,1,2)
    plot(rangest:rangeed,theta1(rangest:rangeed));
    hold on
    plot(rangest:rangeed,thfilt(rangest:rangeed),'r');
    h=axis;
    plot(idx3(idx4)+rangest,theta1(idx3(idx4)+rangest),'gx')
  plot(idx1(idx2)+rangest,theta1(idx1(idx2)+rangest),'kx')
    
    