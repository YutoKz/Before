
public class Pikachu extends PixelArt{
	
	private String name;
	
	
	public Pikachu()
	{
		this("野生のピカチュウ");
	}
	
	public Pikachu(String name)
	{
		this.name = name;
	}
	
	@Override
	public void draw()
	{
		System.out.println("■■■　　　■■　　　■■■");
		System.out.println("■■　■　　■■　　■　■■");
		System.out.println("　■　　■■■■■■　　■");
		System.out.println("　　■　　　　　　　　■");
		System.out.println("　　■　●　　　　●　■");
		System.out.println("　　■　　　■■　　　■");
		System.out.println("　■　○　■■■■　○　■");
		System.out.println("　■　　　　　　　　　　■");
		System.out.println("　■　　■　　　　■　　■");
		System.out.println("　■　■■　　　　■■　■");
		System.out.println("　■　　　　　　　　　　■");
		System.out.println("　　■　　■■■■　　■");
		System.out.println("　　　■■　　　　■■");
	}
	
	@Override
	public String toString()
	{
		return "ピカチュウ (" + name + ")";
	}

}
