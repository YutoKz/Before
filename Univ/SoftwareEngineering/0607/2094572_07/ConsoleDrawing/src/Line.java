
public class Line extends Shape{
	
	private int length;
	
	public Line(int length)
	{
		this.setLength(length);
	}
	
	
	
	@Override
	public void draw()
	{
		for(int i = 0; i < length; i++)
		{
			System.out.print(pen);
		}
		System.out.println();
	}

	public void setLength(int length)
	{
		this.length = length;
	}
	
	@Override
	public String toString()
	{
		return String.format("直線(%d)", length);
	}
	
	
	
}
