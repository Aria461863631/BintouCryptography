
# This file was *autogenerated* from the file file.sage
from sage.all_cmdline import *   # import sage library

_sage_const_2 = Integer(2); _sage_const_1 = Integer(1); _sage_const_18319922375531859171613379181 = Integer(18319922375531859171613379181); _sage_const_93450983094850938450983409623 = Integer(93450983094850938450983409623); _sage_const_46725491547425469225491704811 = Integer(46725491547425469225491704811); _sage_const_24975361709386136697692789531 = Integer(24975361709386136697692789531); _sage_const_45416776270485369791375944998 = Integer(45416776270485369791375944998); _sage_const_1097384855040028890092027323 = Integer(1097384855040028890092027323); _sage_const_10845983827920419787643668763 = Integer(10845983827920419787643668763); _sage_const_15048074151770884271824225393 = Integer(15048074151770884271824225393)
p = _sage_const_93450983094850938450983409623 
ell = _sage_const_46725491547425469225491704811 
log2 = _sage_const_1097384855040028890092027323 
log3 = _sage_const_24975361709386136697692789531 
target = _sage_const_45416776270485369791375944998 
logtarget = _sage_const_10845983827920419787643668763 
logg = _sage_const_1097384855040028890092027323 

redlog=ZZ(Integers(ell)(logtarget/log2))
print "redlog", redlog

qlog = _sage_const_18319922375531859171613379181 

#x = CRT( 1,2,qlog,ell ) 
x = crt( [ _sage_const_1 , qlog ], [ _sage_const_2 , ell ] )
#y = (ell + qlog * 2 * pow( 2, ell-2, ell )) 
print "crt",x
#print y
key = pow(_sage_const_15048074151770884271824225393 ,x,p)
print key

