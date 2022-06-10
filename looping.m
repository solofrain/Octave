more off;
puts "geting timeout..\n";
cagetTimeout()
puts "geting retry count..\n";
cagetRetryCount()

a=zeros(10,10);
caclrmonitor(PVname);
casetmonitor(PVname);

for i=[1:10];
	puts "waiting..\n";
	while(canewmonval(PVname)==0);
	endwhile;
	a(i,:)=caget(PVname);
	a(i,:)
endfor;
caclrmonitor(PVname);
a
more on
