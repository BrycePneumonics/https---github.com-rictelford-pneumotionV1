global s
cport='COM3';
baud=115200;
pkg load instrument-control
if exist('s')
    try
        serial_off
    end
%    s=serial('COM8',19200,0.1);
    s=serial(cport,baud,0.1);
%    fopen(s);
%    set(s,'Timeout',.1);
%    s.ReadAsyncMode='continuous';
else
     s=serial(cport,baud,0.1);
%    fopen(s);
%    set(s,'Timeout',.1);
%    s.ReadAsyncMode='continuous';
    global s
end