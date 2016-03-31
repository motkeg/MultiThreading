package advex4;

public class Main {

	public static void main(String[] args) throws InterruptedException {
	
		
		
		Logger log = new Logger("main");
		LimitedQueue q = new LimitedQueue(5);
		// ... send hello messages
		for (int i = 0; i < 5; i++) {
			synchronized (q) {
			
				q.put(new Message("main", "t"+i, "hello"));
				//System.out.println(q.size());
			}
		}
		Thread[] ts = new Thread[10];
		// ... create Reader threads t0 ... t9
          for (  int i = 0; i < ts.length; i++) {
			ts[i]=new Reader("t"+i, q);
			ts[i].start();
			
		}
		
		
		// ... sleep for 0.5s
		
			Thread.sleep(3000);
		
		// ... send stop messages
		for (int i = 0; i < ts.length; i++) {
			synchronized (q) {
				
				q.put(new Message("main", "t"+i, "stop"));
			}
		}
		// ... wait for threads to stop
		for (int i = 0; i < ts.length; i++) {
			ts[i].join();
		}

		log.log("done with queue size " + q.size());
	}

     
	
} 
