package advex4;

public class Logger {
	private String name;

	public  Logger(String S)  {name=S;}
	public synchronized void log(String s)  {System.out.println(s);}
	
		
		
}
