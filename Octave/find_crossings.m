function idx=find_crossings(x1,x2,rorf)
idx=[];


if rorf
  idx1=find(x1(1:end-1)>x2(1:end-1));
  idx2=find(x1(idx1+1)<=x2(idx1+1));
  idx=idx1(idx2);
else
  idx1=find(x1(1:end-1)<x2(1:end-1));
  idx2=find(x1(idx1+1)>=x2(idx1+1));
  idx=idx1(idx2);
end