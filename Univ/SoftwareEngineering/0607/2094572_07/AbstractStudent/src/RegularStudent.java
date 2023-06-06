
public class RegularStudent extends Student{
	
	public RegularStudent()
	{
		super();
	}
	
	public RegularStudent(String id, String name, int credit)
	{
		super(id, name, credit);
	}
	
	
	
	@Override
	public void explain()
	{
		System.out.println("私" + getName() + "は、正規の学生です。");
	}
	
}
