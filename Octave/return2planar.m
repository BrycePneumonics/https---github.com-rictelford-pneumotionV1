function theta=return2planar(x,y,z)
% x y and z are column vectors of the same length
%  if only x it is a nx3 matrix
if nargin>1
    N=[x y z];
else
    N=x;
end
[a idx]=max(abs(N'));
a=[];
a(1)=length(find(idx==1));
a(2)=length(find(idx==2));
a(3)=length(find(idx==3));
[a bd]=max(a);
for k=1:length(x)-10;
     G(k,1:3)=cross(N(k,:),N(k+10,:));
     G(k,1:3)=G(k,1:3)*sign(G(k,bd));
end

GM=mean(G,1);
GM=GM/sqrt(sum(GM.^2));
R=fcn_RotationFromTwoVectors(GM',[ 0 0 1]');
M=R*N';


theta=angle(M(1,:)+M(2,:)*j)*180/pi;
theta=theta-mean(theta);




%  and

function R=fcn_RotationFromTwoVectors(v1, v2)
% R*v1=v2
% v1 and v2 should be column vectors and 3x1

% 1. rotation vector
w=cross(v1,v2);
w=w/norm(w);
w_hat=fcn_GetSkew(w);
% 2. rotation angle
cos_tht=v1'*v2/norm(v1)/norm(v2);
tht=acos(cos_tht);
% 3. rotation matrix, using Rodrigues' formula
R=eye(size(v1,1))+w_hat*sin(tht)+w_hat^2*(1-cos(tht));

function x_skew=fcn_GetSkew(x)
x_skew=[0 -x(3) x(2);
 x(3) 0 -x(1);
 -x(2) x(1) 0];