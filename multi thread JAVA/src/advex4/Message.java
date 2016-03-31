package advex4;

public class Message {
	////////////fields//////////////////
	private String source;
	private String destenation;
	private String content;
	////////constructor and copy constructor////////////////
	public  Message(String S,String D,String C){
		source=S;
		destenation=D;
		content=C;
	}
	public Message(Message other){
		source=other.source;
		destenation=other.destenation;
		content=other.content;
	}
	////////////////////////////methods//////////////////////////
	public String getSource(){return source;}
	public String getDestenation(){return destenation;}
	public String getContent(){return content;}
	public void setSource(String s){ source=s;}
	public String printM() {
		return(source+" --->"+destenation+" {{"+content+"}}");
	}
	

}
