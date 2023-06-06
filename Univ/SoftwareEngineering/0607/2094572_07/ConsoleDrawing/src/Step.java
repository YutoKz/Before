
public class Step extends Shape{
	
	private int height;
	
	public Step(int height)
	{
		this.setHeight(height);
	}
	
	
	
	@Override
	public void draw()
	{
		for(int i = 1; i <= height; i++)
		{
			for(int j = 0; j < i; j++)
			{
				System.out.print(pen);
			}
			System.out.println();
		}
		System.out.println();
	}

	
	
	public void setHeight(int height)
	{
		this.height = height;
	}
	
	@Override
	public String toString()
	{
		return String.format("階段(%d)", height);
	}
}
