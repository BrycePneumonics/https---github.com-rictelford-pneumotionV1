function val=send_serial(string,expect,s)

if expect==-1;
  val=char(srl_read(s,10000));
      if length(val)==0
          val=[];
      end
    
elseif expect
    srl_write(s,[string char([10 13])]);
        pause(0.001);
        val=char(srl_read(s,10000));
        if length(val)==0
        val=[];
        end

else
    srl_write(s,[string char([10 13])]);
    val=[];
end
