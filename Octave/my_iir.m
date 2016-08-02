function [outval, b_val, a_val]=my_iir(invec, b_val, a_val, b_filt, a_filt, order)
%aaccum=0;
%for k=1:order
%%  a_val(k)=a_val(k+1);
%  aaccum=aaccum+a_filt(k+1)*a_val(order-k+1);
%  b_val(k)=b_val(k+1);
%end
%b_val(order+1)=in_val;
%baccum=0;
%for k=1:order+1
%  baccum=baccum+b_filt(k)*b_val(order-k+2);
%end

%out_val=(-aaccum+baccum)*a_filt(1);
for n=1:length(invec)
  b_val(1:order)=b_val(2:order+1);
  b_val(order+1)=invec(n);
  accum=0;
  for k=1:order+1
    accum=accum+b_filt(k)*b_val(order+2-k);
  end

  for k=1:order
    accum=accum-a_filt(k+1)*a_val(order+2-k);
  end
  a_val(1:order)=a_val(2:order+1);
  a_val(order+1)=accum;
  outval(n)=accum;
%  for k=1:order;
%    a_val(k)=a_val(k+1);
%  end
end