package advex4;

import java.util.ArrayList;

public class LimitedQueue {
	///////////////////////fields//////////////////
	private ArrayList<Message> quene;
	int capcity;
	private  Logger loger=new Logger("Quene");

////////constructor and copy constructor////////////////
	public LimitedQueue(LimitedQueue other){
		quene=new ArrayList<Message>();
		capcity=other.capcity;
		for (int i = 0; i < other.capcity; i++) {
			quene.add(other.quene.get(i));
		}
	}
	public LimitedQueue(int num){
		quene=new ArrayList<Message>();
		capcity=num;
	}
	///////////////////methods//////////////////
	public synchronized void put(Message m) {
		while(size()==capcity){/////if the array is full
			try {
				wait();
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
			quene.add(m);
			loger.log("new messege added by "+m.getSource()+" to--> "+m.getDestenation());
		    notifyAll();
			
	}
    public synchronized Message get(String name) {
    	while(size()==0){/////if the array is empty
    		try {
				wait();
    			
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
    	}
    	Message temp= new Message(quene.get(size()-1));
		quene.remove(size()-1);
		loger.log("messege removed by--> "+name);
		
		notifyAll();
    	return temp;
    	
    }
	public int size()
	{
		return quene.size();
	}
	
}
