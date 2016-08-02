function outmat=find_nearest_vec(x,y,z);

mx=mean(x);
my=mean(y);
mz=mean(z);
mmat=[mx my mz]/(sqrt(mx^2+my^2+mz^2));
mymat=[x;y;z]';
compmat=[0 0 1; 0 1 0; 0 1 1; 1 0 0; 1 0 1; 1 1 0 ; 1 1 1 ...
    ; 0 0 -1; 0 1 -1; 1 0 -1 ; 1 1 -1 ...
    ; 0 -1 0; 0 -1 1; 1 0 0; 1 -1 0 ; 1 -1 1 ...
    ; -1 0 0; -1 0 1; -1 1 0 ; -1 1 1 ];
[a b]=size(compmat)
for k=1:a;
    compmat(k,:)=compmat(k,:)/sqrt(sum(compmat(k,:).^2));
end
close all
figure(1)
hold on
cvec='bgryckm'
for k=1:a
    for n=1:length(x)
        theta(n)=acosd(sum(mymat(n,:).*compmat(k,:))./(norm(mymat(n,:))*norm(compmat(k,:))));
    end
    rem(k-1,6)+1
    plot(theta-mean(theta),cvec(rem(k-1,7)+1))
    outvec(k)=max(theta)-min(theta)
end
[a b]=max(outvec);
outmat=compmat(b,:);