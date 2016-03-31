package advex4;
import java.lang.Thread;
public class Reader extends Thread {
	//////////fields/////////////////////
	private LimitedQueue Q;
	 private Boolean run=true;
	 
	//////////constructor///////////////////
	public Reader( String s,LimitedQueue q){
		super(s);
		 Q=q;
	}
	
	
	@Override
	public void run() {
		
		
		Logger loger=new Logger(getName());
		 
			while(run){////the thread will running until he get "stop message" 
				
				Message m=Q.get(getName());
				loger.log("START "+m.printM());
				
				if(m.getContent().equals("stop")){
					loger.log("STOPING "+m.printM());
					run=false;
				}
				
				else if(!getName().equals(m.getDestenation())){
				   m.setSource(getName());
				   Q.put(m);
				   loger.log("PUTING BACK BY "+m.printM());
				}
			
			}
			
		}
			
			
		}

	


