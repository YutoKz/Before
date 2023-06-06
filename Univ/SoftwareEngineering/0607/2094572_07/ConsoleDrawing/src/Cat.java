
public class Cat extends AsciiArt{
	
	private String text;
	
	public Cat()
	{
		this("ショボーン");
	}
	public Cat(String text)
	{
		this.setText(text);
	}
	
	
	@Override
	public void draw()
	{
		System.out.println("　 ∧＿∧　　　／￣￣￣￣");
		System.out.println("　（ ´･ω･)　＜　" + text);
		System.out.println("　（ つ旦と）　　＼＿＿＿＿");
		System.out.println("　と＿）＿）");
		System.out.println();
	}
	
	
	
	
	
	public void setText(String text)
	{
		this.text = text;
	}

	
	@Override
	public String toString()
	{
		return "Cat (" + text + ")";
	}
}
