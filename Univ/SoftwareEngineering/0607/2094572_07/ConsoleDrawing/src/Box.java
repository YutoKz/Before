
public class Box extends Shape{

	private int width;
	private int height;
	
	
	
	public Box(int width, int height)
	{
		this.setWidth(width);
		this.setHeight(height);
	}

	
	
	@Override
	public void draw() {
		for (int i = 0; i < width; i++) {
			System.out.print(pen);
		}
		System.out.println();
		
		for(int j = 1; j < (height - 1); j++)
		{
			System.out.print(pen);
			for(int i = 1; i < (width - 1); i++)
			{
				System.out.print("  ");
			}
			System.out.println(pen);
		}
		
		for (int i = 0; i < width; i++) {
			System.out.print(pen);
		}
		
		System.out.println();
	}
	
	
	
	public void setWidth(int width)
	{
		this.width = width;
	}
	
	public void setHeight(int height)
	{
		this.height = height;
	}

	
	
	@Override
	public String toString() {
		return String.format("四角形(%d, %d)", width, height);
	}
}
